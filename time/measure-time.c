/*
 * clock_gettime()を使って経過時間を計測するサンプル
 */
#include <stdio.h>
#include <time.h>

void subtractTimespec (struct timespec ts1, struct timespec ts2) {
    struct timespec ts;

    ts.tv_sec = ts1.tv_sec - ts2.tv_sec;
    ts.tv_nsec = ts1.tv_nsec - ts2.tv_nsec;
    if (ts.tv_nsec < 0) {
        ts.tv_sec -= 1;
        ts.tv_nsec += 1000 * 1000 * 1000;
    }

    printf("Actual time:\t %10ld.%09ld\n", ts.tv_sec, ts.tv_nsec);
}

int main(int argc, char **argv){
    struct timespec ts1, ts2;
    struct timespec tsOfSleep;

    clock_getres(CLOCK_REALTIME, &ts1);
    printf("Resolution:\t %10ld.%09ld\n", ts1.tv_sec, ts1.tv_nsec);

    tsOfSleep.tv_sec = 0;
    tsOfSleep.tv_nsec = 500 * 1000 * 1000; // Ref: 1 sec = 1000 * 1000 * 1000 nsec
    clock_gettime(CLOCK_REALTIME, &ts1);

    printf("Exec nanosleep:\t %10ld.%09ld\n", tsOfSleep.tv_sec, tsOfSleep.tv_nsec);
    nanosleep(&tsOfSleep, NULL);

    clock_gettime(CLOCK_REALTIME, &ts2);
    subtractTimespec(ts2, ts1);
    return 0;
}

