# libdefer: Go-style defer for C

libdefer replicates much of the functionality of go's `defer` in C99.  When linked in, on supporting platforms, a defer context is automatically created before main is called, and cleaned up after exit or return from main.  Scopes are thread-local, and completely under user control, so you can make the scopes as small or large as you like (even unstructured or un-nested!)

## Build and Install

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make install
```

## Example

One of golang's most iconic features is the `defer` keyword.  Lacking scoped destruction, it's one of the best ways to clean up resources in the face of panics and without having to worry about multiple returns.  A canonical example is closing a pair of files in the face of error handling:

```golang
// Example courtesy of https://blog.golang.org/defer-panic-and-recover
func CopyFile(dstName, srcName string) (written int64, err error) {
    src, err := os.Open(srcName)
    if err != nil {
        return
    }
    defer src.Close()

    dst, err := os.Create(dstName)
    if err != nil {
        return
    }
    defer dst.Close()

    return io.Copy(dst, src)
}
```

Doing this in C is normally a matter of replicating cleanup code or use of the dreaded `goto`.  For example, a similar C function might be:

```c
int64_t copy_file(const char *dst_name, const char *src_name) {
    FILE *dst = fopen(dst_name, "w");
    if (!dst) {
        return -1;
    }
    FILE *src = fopen(src_name, "r");
    if (!src) {
        fclose(dst); // cleanup #1
        return -1;
    }
    
    int64_t ret = copy_between_streams(dst, src);
    
    // cleanup #2
    fclose(dst);
    fclose(src);
    return ret;
}
```

The replicated cleanup is small, but it's also quite easy to miss.  If the copy_between_streams code is inlined and adds one or more returns it gets yet more complicated.  This is what you can do with libdefer:

```c
#include <defer.h>
int64_t copy_file(const char *dst_name, const char *src_name) {
    defer_scope_begin();
    FILE *dst = fopen(dst_name, "w");
    if (!dst) {
        DEFER_POP1_RETURN(-1);
    }
    defer(fclose, dst);
    FILE *src = fopen(src_name, "r");
    if (!src) {
        DEFER_POP1_RETURN(-1);
    }
    defer(fclose, src);
    
    DEFER_POP1_RETURN(copy_between_streams(dst, src));
}
```

Now, the close is closer to the open, and handled regardless of exit.  That said, we still have the issue of easy mistakes.  Every return has been replaced with a cleanup macro, this is not really what we're after.  This is why libdefer also provides function definition wrapper macros:

```c
#include <defer.h>
DEFER_SCOPED(int64_t, copy_file, (const char *, dst_name, const char, *src_name)) {
    FILE *dst = fopen(dst_name, "w");
    if (!dst) {
        return -1;
    }
    defer(fclose, dst);
    FILE *src = fopen(src_name, "r");
    if (!src) {
        return -1;
    }
    defer(fclose, src);
    
    return copy_between_streams(dst, src);
}
```

The definition line is a little longer, but the body is now completely surrounded by a guaranteed defer block that actually does a little more work than the one in the previous version.  With a decent compiler, the extra static inline function generated here will optimize away, but either way the function gets its cleanup without visible handling in the body.  Also, the signature of the function *has not changed*, the prototype in headers can remain the same, just the definition needs the macro treatment.

## Limitations

* A scope is not auto-created with new threads, the easiest way to deal with this is to wrap the entry function in a `DEFER_SCOPED` macro
* Only functions with no arguments or a single pointer sized argument can be deferred at this time, if you have a good reason to defer functions that don't look like `free`, feel free to post an issue 
* The library is currently un-optimized, and is not cost free.  Each defer entails a `malloc` and a `free` on cleanup, this will likely be fixed, but be aware
* Use of `setjmp` and `longjmp` for ad-hoc exceptions is actually supported, but does not execute all defers during unwinding like C++ would, rather all of the scope creation functions return a handle for that scope, allowing the code around the setjmp to get a handle that can clean up all scopes below that context.  Using the handles is a good way to protect against unmatched push/pop pairs in inner scopes as well.
