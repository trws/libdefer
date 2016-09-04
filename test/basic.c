#ifdef NDEBUG
#undef NDEBUG
#endif

#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <defer.h>

int ctr = 0;

void add(void* i) {
    intptr_t n = (intptr_t) i;
    ctr+=n;
}

void add_one() {
    add((void*)1);
}

void check_order(void *n) {
    assert(ctr == (intptr_t)n);
    printf("order observed %ld\n", ctr - (intptr_t)n);
}

DEFER_SCOPED_VOID(t1, ()){
    deferi(check_order, 1);
    defer_noarg(add_one);
    assert(ctr == 0);
}

DEFER_SCOPED_VOID(t2, (int, a)){
    assert(ctr == 1);
    deferi(check_order, 1+a);
    deferi(add, a);
    assert(ctr == 1);
}

int main(int argc, char *argv[])
{
    t1();
    t2(5);
    assert(ctr == 6);
    
    return ctr - 6;
}
