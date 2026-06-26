/* Lab 5 (进阶彩蛋): 缓冲 vs 系统调用 —— 一个经典面试陷阱
 *
 * 对应第一课:
 *   - write() 是薄包装,基本一次调用 = 一次 write 系统调用,不带缓冲。
 *   - printf() 走 stdio,在【用户态】维护一个缓冲区,攒够了/遇到换行才真正 write。
 *
 * 陷阱: printf 的内容若还停在用户态缓冲区里没被刷出,
 * 此时 fork(),缓冲区会被【连同地址空间一起复制】给子进程,
 * 于是父子各自退出时都把这份缓冲刷出来 —— 那句话被打印了两次!
 *
 * 怎么看出区别:
 *   ./lab5              (输出到终端: 行缓冲,遇到 \n 就刷,看不太出问题)
 *   ./lab5 > /tmp/a.txt (输出到文件: 全缓冲,不到换行不刷, 陷阱现形)
 *   然后 cat /tmp/a.txt
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(void) {
    /* write: 直接系统调用,立刻送出,不进用户态缓冲 */
    const char *msg = "通过 write 写的(系统调用,不缓冲)\n";
    write(1, msg, strlen(msg));

    /* printf: 注意这里【故意不加 \n】,让它留在用户态缓冲区里 */
    printf("通过 printf 写的(可能还在缓冲区里)");

    fork();   /* 此刻缓冲区被复制一份给子进程 */

    /* 父子进程退出时各自刷新自己那份缓冲区 ->
       输出到文件时,这句没有 \n 的 printf 会出现两次 */
    wait(NULL);
    return 0;
}