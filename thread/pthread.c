/**
 * pthreadの使用例
 * - スレッドの使い方
 * - CPUを意図的に開放するyield
 * - mutexの使い方
 * - などなど
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>

// mutexについて
// - 初期化はpthread_mutex_init()かPTHREAD_MUTEX_INITIALIZERで行う
// - 破棄はpthread_mutex_destroy()で行う
// - 変数への書き込みはロック必須
// - 一般的な変数の読み込みはアトミック操作なのでロック不要だが、複雑なデータ構造はそうでない可能性があるので注意
//   - 例: int型なら特に問題はないはずだが、これが配列になると怪しい
pthread_mutex_t mutex;
int x = 0;

void funcThreadA() {
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        x = 1;
        pthread_mutex_unlock(&mutex);
        printf("A: %10d\n", i);
        sched_yield();

        // yield関数の種類について
        // - sched_yield(); // 基本的にはこれを使うのが良い
        // - pthread_yield(); // LinuxにあるがMacに存在しない。Linux実装では内部でsched_yield()を呼ぶ
        // - pthread_yield_np(); // MacにあるがLinuxに存在しない。npはNo Posixの意味
        //
        // またMacはLinuxと比べると実行されるスレッドが偏る傾向にある
    }
}

void funcThreadB() {
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        x = 2;
        pthread_mutex_unlock(&mutex);
        printf("B: %10d\n", i);
        sched_yield();
    }
}

int main() {
    pthread_t pthreadA, pthreadB;
    pthread_mutex_init(&mutex, NULL);

    if (pthread_create(&pthreadA, NULL, (void *)funcThreadA, NULL) != 0) {
        perror("pthread_create A: ");
    }
    if (pthread_create(&pthreadB, NULL, (void *)funcThreadB, NULL) != 0) {
        perror("pthread_create B: ");
    }

    if (pthread_join(pthreadA, NULL) != 0) {
        perror("pthread_join: ");
    }
    if (pthread_join(pthreadB, NULL) != 0) {
        perror("pthread_join: ");
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}
