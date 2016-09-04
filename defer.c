#include "defer.h"

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static pthread_key_t defer_scope_stack;
static pthread_once_t dss_init_once = PTHREAD_ONCE_INIT;

static void execute_all_deferred(void* dscope /* defer_scope_t * */);

static void final_cleanup(void);

static void init_dss(void) {
    pthread_key_create(&defer_scope_stack, execute_all_deferred);
    pthread_setspecific(defer_scope_stack, 0);
    atexit(final_cleanup);
}

typedef struct defer {
    struct defer* next;
    union {
        deferable_free_like fn;
        deferable_noarg noarg;
    };
    void* data;
    _Bool arg;
} defer_t;

struct defer_scope {
    struct defer_scope* parent;
    defer_t* routines;
};

static inline void execute_deferred(defer_t* d) {
    while (d) {
        if (!d->fn) {
            fprintf(stderr, "Invalid defer encountered, aborting\n");
            abort();
        }
        if (d->arg) {
            d->fn(d->data);
        } else {
            d->noarg();
        }
        defer_t* tmp = d;
        d = d->next;
        free(tmp);
    }
}

defer_scope_t* defer_scope_new(void) {
    defer_scope_t* ds = malloc(sizeof *ds);
    ds->parent = NULL;
    ds->routines = NULL;
    return ds;
}

defer_scope_t* defer_scope_push(defer_scope_t* ds) {
    pthread_once(&dss_init_once, init_dss);
    if (!ds)
        ds = defer_scope_new();
    defer_scope_t* top = pthread_getspecific(defer_scope_stack);
    ds->parent = top;
    if (top == NULL) {
        // Starting from the bottom register thread cleanup on fork
        pthread_atfork(NULL, NULL, final_cleanup);
    }
    pthread_setspecific(defer_scope_stack, ds);
    return ds;
}

void defer_scope_pop(defer_scope_t* ds) {
    defer_scope_t* top = pthread_getspecific(defer_scope_stack);
    assert(top);
    defer_scope_t* until =
        ds ? (ds == (defer_scope_t*)1 ? NULL : ds->parent) : top->parent;
    while (top != until && top != NULL) {
        execute_deferred(top->routines);
        defer_scope_t* tmp = top->parent;
        free(top);
        top = tmp;
    }
    pthread_setspecific(defer_scope_stack, top);
}

defer_scope_t* defer_scope_begin(void) {
    return defer_scope_push(NULL);
}

void defer_scope_end(void) {
    defer_scope_pop(NULL);
}

#ifdef __GNUC__
#define INIT(X) void X(void) __attribute__((constructor))
#else
#warning You must manually create a scope before deferring a value
#endif
INIT(defer_scope_auto_init);

void defer_scope_auto_init(void) {
    defer_scope_begin();
}

static void execute_all_deferred(void* dscope /* defer_scope_t * */) {
    defer_scope_pop(dscope);
}

static void final_cleanup(void) {
    defer_scope_pop((defer_scope_t*)1);
}

static void defer_append(defer_scope_t* s, defer_t* d) {
    d->next = s->routines;
    s->routines = d;
}

void defer_specific(defer_scope_t* ds, deferable_free_like fn, void* p) {
    assert(fn);
    defer_t* nd = malloc(sizeof *nd);
    *nd = (defer_t){.arg = 1, .fn = fn, .data = p};
    defer_append(ds, nd);
}

void defer_specific_noarg(defer_scope_t* ds, deferable_noarg fn) {
    assert(fn);
    defer_t* nd = malloc(sizeof *nd);
    nd->arg = 0;
    nd->noarg = fn;
    nd->data = NULL;
    defer_append(ds, nd);
}

void defer(deferable_free_like fn, void* p) {
    defer_specific(pthread_getspecific(defer_scope_stack), fn, p);
}

void defer_noarg(deferable_noarg fn) {
    defer_specific_noarg(pthread_getspecific(defer_scope_stack), fn);
}
