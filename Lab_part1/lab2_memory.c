/* Lab 2: 父子进程的内存是相互独立的
 *
 * 对应第二/三课:进程隔离 + 写时复制的"结果"。
 * 父子从同一个 counter 出发,各自修改,互不影响。
 * 还会打印变量地址 —— 你会看到地址相同(虚拟地址)
 * 但值不同,因为它们落在各自独立的物理内存上。
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int counter = 100;   /* 全局变量,放在数据段 */

int main(void) {
    printf("[fork 前] counter=%d, &counter=%p\n", counter, (void *)&counter);

    pid_t pid = fork();

    if (pid == 0) {
        counter += 1;                /* 子进程把它 +1 */
        printf("  [子进程] 我把 counter 改成 %d, 地址还是 %p\n",
               counter, (void *)&counter);
    } else {
        wait(NULL);                  /* 先让子进程跑完 */
        counter += 1000;             /* 父进程把它 +1000 */
        printf("[父进程] 我看到的 counter=%d (没被子进程的 +1 影响)\n", counter);
        printf("[父进程] 结论: 同一个虚拟地址 %p, 父子各看各的值\n",
               (void *)&counter);
    }
    return 0;
}