#include <inttypes.h> /* for intptr_t */
#include <stdio.h>
#include <pthread.h>

struct S {
    int x;
    int y;
};

int sum(int x, int y) {
    return x + y;
}

void *thread_function(void *pv) {
    struct S *ps = pv;
    pthread_exit((void *)(intptr_t)sum(ps->x, ps->y));
}

int main(void) {
    struct S s = {41, 1};
    pthread_t pt;
    pthread_create(&pt, NULL, thread_function, &s);

    void *pv;
    pthread_join(pt, &pv);

    int z = (intptr_t)pv;
    printf("%d + %d = %d\n", s.x, s.y, z);
}
