/* Lab 1: fork —— "调用一次,返回两次"
 *
 * 对应第三课:验证 fork 后世界上有了两个进程,
 * 父进程拿到子进程 PID(>0),子进程拿到 0。
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    printf("[fork 之前] 我是唯一的进程, pid=%d\n", getpid());

    pid_t pid = fork();          /* <-- 这一行之后,就有两个进程在跑了 */

    if (pid < 0) {
        perror("fork 失败");
        return 1;
    } else if (pid == 0) {
        /* 这段只有子进程会执行 */
        printf("  [子进程] 我的 fork() 返回值 = %d\n", pid);
        printf("  [子进程] 我的 pid=%d, 我爸爸 ppid=%d\n", getpid(), getppid());
    } else {
        /* 这段只有父进程会执行 */
        printf("[父进程] 我的 fork() 返回值 = %d (这是子进程的 pid)\n", pid);
        printf("[父进程] 我的 pid=%d\n", getpid());
        wait(NULL);              /* 等子进程结束,避免输出乱序 */
        printf("[父进程] 子进程已结束,我继续往下走\n");
    }

    /* 注意:下面这行父子都会执行 —— 因为两个进程都从 fork 处往下跑 */
    printf("    >> 同一行代码, 被 pid=%d 执行了\n", getpid());
    return 0;
}