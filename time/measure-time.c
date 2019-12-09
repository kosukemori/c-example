/*
 * clock_gettime()を使って経過時間を計測するサンプル
 */
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv){
    struct timespec ts;
    struct timespec tsOfSleep;

    clock_getres(CLOCK_REALTIME, &ts);
    printf("Time resolution: %10ld.%09ld\n", ts.tv_sec, ts.tv_nsec);

    tsOfSleep.tv_sec = 0;
    tsOfSleep.tv_nsec = 100; // Ref: 1 sec = 1000 * 1000 * 1000 nsec
    clock_gettime(CLOCK_REALTIME, &ts);
    printf("Before: %10ld.%09ld\n", ts.tv_sec, ts.tv_nsec);
    for(int i = 0; i < 1000; i++){
        nanosleep(&tsOfSleep, NULL);
    }
    clock_gettime(CLOCK_REALTIME, &ts);
    printf("After : %10ld.%09ld\n", ts.tv_sec, ts.tv_nsec);
    return 0;
}

