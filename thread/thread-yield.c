/**
 * CPUを意図的に開放するyieldの例
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>

void func_thread();

int main() {
    pthread_t pthread;
    if (pthread_create(&pthread, NULL, (void *)func_thread, NULL) != 0) {
        perror("pthread_create: ");
    }
    for (int i = 0; i < 10000000; i++) {
        printf("B: %10d", i);
        printf("\r");
    }

    if (pthread_join(pthread, NULL) != 0) {
        perror("pthread_join: ");
    }
    return 0;
}

void func_thread() {
    for (int i = 0; i < 10000000; i++) {
        printf("A: %10d", i);
        printf("\r");
        sched_yield();
    }
}
