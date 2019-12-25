/**
 * シグナルの制御サンプル。Ctrl+CのSIGINTをハンドリングする
 * Tips: バグでCtrl+Cが効かなくなったらCtrl+\によるSIGQUITを試すと良い
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sigintHandler() {
    printf("Handling SIGINT\n");
    exit(0);
}

int main () {
    struct sigaction act;
    act.sa_handler = sigintHandler;
    // act.sa_flagsを何も設定していないので、ハンドラは一度だけ有効
    sigaction(SIGINT, &act, NULL);

    while(1) {
        printf("Infinite loop...\n");
        sleep(1);
    }
    return 0;
}
