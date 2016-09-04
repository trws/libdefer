#ifndef __DEFER_H
#define __DEFER_H 1
#include "defer_macros.h"

#include <stdint.h>

typedef struct defer_scope defer_scope_t;

typedef void (*deferable_free_like)(void*);
typedef void (*deferable_noarg)(void);

/**
 * @brief Begin a new defer scope for this thread here
 *
 * @return A handle to the new scope or NULL
 */
defer_scope_t* defer_scope_begin(void);

/**
 * @brief Execute the deferred functions in the current defer scope and pop it
 * off the stack
 */
void defer_scope_end(void);

/** 
 * @brief Create a wrapper function that creates a defer scope around the
 * function being defined, only use for void return functions.
 * 
 * @param NAME function name to define
 * @param ARG_LIST argument list, types and names, all separated by commas,
 * see example
 * 
 * The main advantage to using one of the wrapper macros to define
 * defer-scoped functions is that however you leave the function so wrapped,
 * the defers will run.  No calls or return macros are required inside the
 * function when it is defined this way. The only exception to this is using
 * setjmp/longjmp, that must be handled by use of pop to clean up all scopes
 * back to a known point around the setjmp point. 
 *
 * Use like this:
 * DEFER_SCOPED_VOID(test_fn, (int, a, char *, b)) {
 *     defer(free, b);
 * }
 *
 * int main(int argc, char *argv[]){
 *     test_fn(5, strdup("string"));
 *     return 0;
 * }
 */
#define DEFER_SCOPED_VOID(NAME, ARG_LIST)                         \
    static inline void __defer_inner##NAME __DEFER_ARGS ARG_LIST; \
    void NAME __DEFER_ARGS ARG_LIST {                             \
        defer_scope_t* ds = defer_scope_begin();                  \
        __defer_inner##NAME __DEFER_CALL ARG_LIST;                \
        defer_scope_pop(ds);                                      \
    }                                                             \
    static inline void __defer_inner##NAME __DEFER_ARGS ARG_LIST

/** 
 * @brief Create a wrapper function that creates a defer scope around the
 * function being defined, only use for non-void return functions.
 * 
 * @param RET return type of the function to be defined
 * @param NAME function name to define
 * @param ARG_LIST argument list, types and names, all separated by commas,
 * see example
 * 
 */
#define DEFER_SCOPED(RET, NAME, ARG_LIST)                        \
    static inline RET __defer_inner##NAME __DEFER_ARGS ARG_LIST; \
    RET NAME __DEFER_ARGS ARG_LIST {                             \
        defer_scope_t* ds = defer_scope_begin();                 \
        RET r = __defer_inner##NAME __DEFER_CALL ARG_LIST;       \
        defer_scope_pop(ds);                                     \
        return r;                                                \
    }                                                            \
    static inline RET __defer_inner##NAME __DEFER_ARGS ARG_LIST

/**
 * @brief Convenience macro to pop the scope and return
 *
 * @param DS defer scope to pop to
 * @param RET expression to return from the current function
 *
 * @return
 */
#define DEFER_POP_RETURN(DS, RET) \
    do {                          \
        defer_scope_pop(DS);      \
        return (RET);             \
    } while (0)

/**
 * @brief Convenience macro to end the current scope and return
 *
 * @param RET expression to return from the current function
 *
 * @return
 */
#define DEFER_POP1_RETURN(RET) \
    do {                       \
        defer_scope_end();     \
        return RET;            \
    } while (0)

/**
 * @brief Push a new scope onto the stack
 *
 * @param ds An initialized scope from defer_scope_new or NULL to create a new
 * scope
 *
 * @return The scope added to the top of the stack
 */
defer_scope_t* defer_scope_push(defer_scope_t* ds);

/**
 * @brief Execute all defers in the stack until the passed scope is
 * popped and
 * executed.
 *
 * @param ds The scope to pop to or NULL for innermost scope
 *
 * This function can be used to handle multi-level cleanup.  If your
 * project
 * uses setjmp/longjmp to implement exception-like behavior, or
 * exceptions for
 * that matter, along with this library, it can be useful to clear
 * all the way
 * back to a known scope rather than just the innermost.
 */
void defer_scope_pop(defer_scope_t* ds);

/**
 * @brief Defer execution of the free-like function fn, with
 * argument p, until
 * the nearest enclosing scope is cleared.
 *
 * @param fn The free-like function to execute on cleanup
 * @param p The user-defined value to pass to the function
 */
void defer(deferable_free_like fn, void* p);

/**
 * @brief Defer execution of the free-like function fn, with intptr_t argument
 * p, until
 * the nearest enclosing scope is cleared.
 *
 * @param fn The free-like function to execute on cleanup
 * @param p The user-defined value to pass to the function
 */
static inline void deferi(deferable_free_like fn, intptr_t p){
    defer(fn, (void*)p);
}

/**
 * @brief Defer execution of the void(void) function fn until the
 * nearest
 * enclosing scope is cleared
 *
 * @param fn The function to execute when the scope ends
 */
void defer_noarg(deferable_noarg fn);

/**
 * @brief Explicitly create a new scope that *is not on the stack*
 *
 * @return A new but un-registered defer scope
 */
defer_scope_t* defer_scope_new(void);

/**
 * @brief Explicitly execute and clear scope. This may be safely
 * used on a
 * scope that has been pushed to execute cleanup early.
 *
 * This should be most useful as either early cleanup, or to attach
 * cleanup of
 * something that had an undefined lifetime to the cleanup routine
 * of a scope
 * after which it should be cleared.  Note that this can safely be
 * passed to
 * `defer`, as in `defer(defer_scope_clear, ds)`, as can
 * `defer_scope_delete`.
 *
 * @param ds The defer scope to clear
 */
void defer_scope_clear(defer_scope_t* ds);

/**
 * @brief Explicitly execute, clear and free scope.  This *must not*
 * be used
 * on a scope that has been pushed onto the stack.
 *
 * @param ds A new but un-registered defer scope
 */
void defer_scope_delete(defer_scope_t* ds);

/**
 * @brief Defer execution of the free-like function fn, with
 * argument p, until
 * the scope ds is cleared, this can be across an un-structured
 * region
 *
 * @param ds scope to append to
 * @param fn function to execute on cleanup
 * @param p argument to be passed to fn
 */
void defer_specific(defer_scope_t* ds, deferable_free_like fn, void* p);

/**
 * @brief Defer execution of the void(void) function fn,  until the
 * scope ds
 * is cleared, this can be across an un-structured region
 *
 * @param ds scope to append to
 * @param fn function to execute on cleanup
 */
void defer_specific_noarg(defer_scope_t* ds, deferable_noarg fn);

/* HELPER MACROS, IGNORE THESE... */

#define __DEFER_ARGS_INNER(T1, V1, ...) T1 V1 FOR_PAIRS(AS_ARGS, __VA_ARGS__)

#define __DEFER_ARGS(...) (IFNEMPTY(__VA_ARGS__)(__DEFER_ARGS_INNER(__VA_ARGS__)))

#define __DEFER_CALL_INNER(T1, V1, ...) V1 FOR_PAIRS(AS_ARG_NAMES, __VA_ARGS__)

#define __DEFER_CALL(...)                                                 \
        (IFNEMPTY(__VA_ARGS__)(__DEFER_CALL_INNER(__VA_ARGS__)))

#endif
