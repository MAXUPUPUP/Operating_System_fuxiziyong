/* Lab A: pthread 基础 —— 亲眼验证"共享全局、私有栈"
 *
 * 对应第四课:同进程内多线程【共享】全局变量,但各有【私有】的栈。
 * 看输出:所有线程打印的 g_shared 地址【相同】(共享同一份),
 *        而各自局部变量 local 的地址【各不相同】(各自独立的栈)。
 */
#include <stdio.h>
#include <pthread.h>

#define NT 4

int g_shared = 100;        /* 全局变量,放数据段,所有线程共享 */

void *worker(void *arg) {
    long id = (long)arg;
    int local = (int)id;   /* 局部变量,在【本线程自己的栈】上 */

    printf("线程 %ld: 共享 g_shared=%d @ %p  |  我的 local @ %p\n",
           id, g_shared, (void *)&g_shared, (void *)&local);
    return NULL;
}

int main(void) {
    pthread_t t[NT];
    printf("主线程: g_shared @ %p\n", (void *)&g_shared);
    printf("------ 创建 %d 个线程 ------\n", NT);

    for (long i = 0; i < NT; i++)
        pthread_create(&t[i], NULL, worker, (void *)i);
    for (int i = 0; i < NT; i++)
        pthread_join(t[i], NULL);   /* 等所有线程结束 */

    printf("------ 结论 ------\n");
    printf("g_shared 地址:各线程【相同】 => 共享同一份内存\n");
    printf("local    地址:各线程【不同】 => 各有各的栈\n");
    return 0;
}