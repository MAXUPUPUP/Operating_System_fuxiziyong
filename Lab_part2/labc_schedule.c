/* Lab C: 观察调度器怎么交替分配 CPU
 *
 * 对应第五课:多个就绪线程,谁先跑、怎么交替,由【调度器】决定,
 * 程序自己说了不算。3 个线程各打印若干步,看输出的交错顺序。
 * 多跑几次,顺序往往不同 —— 因为调度时机和核心分配每次都可能变。
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NT 3
#define STEPS 5

void *worker(void *arg) {
    long id = (long)arg;
    for (int s = 0; s < STEPS; s++) {
        printf("    线程 %ld: 第 %d 步\n", id, s);
        fflush(stdout);     /* 立刻刷出,别让缓冲掩盖真实交替 */
        usleep(1000);       /* 主动让出一下,制造更多切换机会 */
    }
    return NULL;
}

int main(void) {
    pthread_t t[NT];
    printf("启动 %d 个线程, 各打印 %d 步, 观察交替:\n", NT, STEPS);
    for (long i = 0; i < NT; i++)
        pthread_create(&t[i], NULL, worker, (void *)i);
    for (int i = 0; i < NT; i++)
        pthread_join(t[i], NULL);
    printf("注意:输出顺序由调度器决定,不固定;多跑几次会变。\n");
    return 0;
}