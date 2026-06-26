/* Lab B (核心): 亲手制造 race condition,再用锁修好
 *
 * 对应第四课结尾埋的"病":多个线程同时改共享变量会出错。
 *
 * 4 个线程,各自把全局 counter 自增 100 万次。
 * 理论上最终应当 = 4 * 1,000,000 = 4,000,000。
 *   - 不加锁: counter++ 的"读-改-写"三步会交错,丢失更新 => 结果偏小!
 *   - 加锁:   把 counter++ 保护成不可打断的整体 => 结果正确。
 *
 * 注意:用 -O0 编译,避免编译器优化把循环"算没了",好让 race 现形。
 */
#include <stdio.h>
#include <pthread.h>

#define NT 4
#define ITERS 100000L

long counter;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int use_lock;                 /* 0=不加锁, 1=加锁 */

void *worker(void *arg) {
    (void)arg;
    for (long i = 0; i < ITERS; i++) {
        if (use_lock) pthread_mutex_lock(&lock);

        /* 把 counter++ 显式拆成三步,并在"读"和"写"之间留一道缝。
         * 单核机器上,这道缝极大概率被调度器切走,从而放大竞态:
         * 两个线程都读到同一个旧值,各自 +1 写回,只生效了一次 -> 丢更新。*/
        long tmp = counter;                         /* 1) 读 */
        for (volatile int k = 0; k < 20; k++) { }   /*    缝(放大窗口) */
        counter = tmp + 1;                          /* 3) 写回 */

        if (use_lock) pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void run(int lk, const char *label) {
    counter = 0;
    use_lock = lk;
    pthread_t t[NT];
    for (int i = 0; i < NT; i++) pthread_create(&t[i], NULL, worker, NULL);
    for (int i = 0; i < NT; i++) pthread_join(t[i], NULL);

    long expected = (long)NT * ITERS;
    printf("%s: counter=%ld  (期望 %ld)  -> %s\n",
           label, counter, expected,
           counter == expected ? "正确" : "出错! 丢了更新");
}

int main(void) {
    printf("%d 个线程, 各自 counter++ %ld 次\n", NT, ITERS);
    run(0, "【不加锁】");
    run(1, "【加 锁 】");
    printf("\n结论:不加锁时 counter++ 的读-改-写被交错,导致丢更新;\n");
    printf("     加锁让它成为不可分割的整体,结果才正确。\n");
    return 0;
}