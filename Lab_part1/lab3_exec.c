/* Lab 3: fork + exec —— 运行一个全新的程序
 *
 * 对应第三课:父进程 fork 出子进程,子进程 exec 变身成 /bin/ls,
 * 父进程 wait 等它结束。这就是 shell 执行命令的核心骨架。
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    printf("[父进程 pid=%d] 准备运行 ls...\n", getpid());

    pid_t pid = fork();

    if (pid == 0) {
        /* 子进程:把自己整个替换成 ls 程序 */
        printf("  [子进程 pid=%d] 我要 exec 变身成 ls 了\n", getpid());
        execlp("ls", "ls", "-l", (char *)NULL);
        /* 只有 exec 失败才会执行到这里 —— 成功的话代码段已被换掉 */
        perror("exec 失败");
        return 1;
    } else {
        wait(NULL);   /* 等子进程(也就是 ls)跑完 */
        printf("[父进程 pid=%d] ls 跑完了, 注意子进程 pid 跟我不同\n", getpid());
    }
    return 0;
}