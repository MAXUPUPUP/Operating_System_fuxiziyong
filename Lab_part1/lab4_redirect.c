/* Lab 4 (压轴): 亲手实现 `ls > out.txt`
 *
 * 对应我们反复讨论的那个机制:
 *   shell 在 fork 出的子进程里、exec 之前,
 *   用 open + dup2 把 fd 1(标准输出)改指向文件,
 *   再 exec 成 ls。ls 浑然不觉地把输出写进了文件。
 *
 * 这个程序不依赖 shell 的 '>',而是我们自己用系统调用做出 '>' 的效果。
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid == 0) {
        /* ===== 子进程:在变身成 ls 之前布置重定向 ===== */

        /* 1) 打开(没有就创建,有就清空)目标文件,拿到一个新 fd */
        int fd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) { perror("open 失败"); return 1; }

        /* 2) 把 fd 1(stdout)改成指向这个文件。
         *    dup2(fd, 1): 让 1 号也指向 fd 指的那个文件,
         *    原来 1 号指向的终端被覆盖。 */
        dup2(fd, 1);

        /* 3) fd 这个号已经没用了(1 号已经接上文件),关掉它 */
        close(fd);

        /* 4) 变身成 ls。它会往 fd 1 写 —— 而 fd 1 现在通向 out.txt */
        execlp("ls", "ls", "-l", (char *)NULL);
        perror("exec 失败");
        return 1;
    } else {
        /* ===== 父进程:等子进程跑完,然后把文件内容读出来给你看 ===== */
        wait(NULL);
        printf("== 子进程(ls)的输出已被写进 out.txt, 内容如下 ==\n");
        fflush(stdout);
        /* 偷个懒,直接 fork+exec 一个 cat 把文件打出来 */
        if (fork() == 0) {
            execlp("cat", "cat", "out.txt", (char *)NULL);
            perror("cat 失败");
            return 1;
        }
        wait(NULL);
    }
    return 0;
}