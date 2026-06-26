# 第一套实验(C, fork/exec)的 Makefile
# 用法: 把本文件放进【第一套】lab 文件夹(含 lab1_fork.c ... lab5_buffer.c)
# 一键编译: make    单独编译: make lab1    清理: make clean

CC      = gcc
CFLAGS  = -Wall -Wextra -O0 -g
LABS    = lab1 lab2 lab3 lab4 lab5

all: $(LABS)

lab1: lab1_fork.c
	$(CC) $(CFLAGS) -o $@ $<
lab2: lab2_memory.c
	$(CC) $(CFLAGS) -o $@ $<
lab3: lab3_exec.c
	$(CC) $(CFLAGS) -o $@ $<
lab4: lab4_redirect.c
	$(CC) $(CFLAGS) -o $@ $<
lab5: lab5_buffer.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(LABS) out.txt

.PHONY: all clean