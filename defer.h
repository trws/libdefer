
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
 * @brief Execute all defers in the stack until the passed scope is popped and
 * executed.
 *
 * @param ds The scope to pop to or NULL for innermost scope
 *
 * This function can be used to handle multi-level cleanup.  If your project
 * uses setjmp/longjmp to implement exception-like behavior, or exceptions for
 * that matter, along with this library, it can be useful to clear all the way
 * back to a known scope rather than just the innermost.
 */
void defer_scope_pop(defer_scope_t* ds);

/**
 * @brief Defer execution of the free-like function fn, with argument p, until
 * the nearest enclosing scope is cleared.
 *
 * @param fn The free-like function to execute on cleanup
 * @param p The user-defined value to pass to the function
 */
void defer(deferable_free_like fn, void* p);

/**
 * @brief Defer execution of the void(void) function fn until the nearest
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
 * @brief Explicitly execute and clear scope. This may be safely used on a
 * scope that has been pushed to execute cleanup early.
 *
 * This should be most useful as either early cleanup, or to attach cleanup of
 * something that had an undefined lifetime to the cleanup routine of a scope
 * after which it should be cleared.  Note that this can safely be passed to
 * `defer`, as in `defer(defer_scope_clear, ds)`, as can `defer_scope_delete`.
 *
 * @param ds The defer scope to clear
 */
void defer_scope_clear(defer_scope_t* ds);

/**
 * @brief Explicitly execute, clear and free scope.  This *must not* be used
 * on a scope that has been pushed onto the stack.
 *
 * @param ds A new but un-registered defer scope
 */
void defer_scope_delete(defer_scope_t* ds);

/**
 * @brief Defer execution of the free-like function fn, with argument p, until
 * the scope ds is cleared, this can be across an un-structured region
 *
 * @param ds scope to append to
 * @param fn function to execute on cleanup
 * @param p argument to be passed to fn
 */
void defer_specific(defer_scope_t* ds, deferable_free_like fn, void* p);

/**
 * @brief Defer execution of the void(void) function fn,  until the scope ds
 * is cleared, this can be across an un-structured region
 *
 * @param ds scope to append to
 * @param fn function to execute on cleanup
 */
void defer_specific_noarg(defer_scope_t* ds, deferable_noarg fn);
