# 一键编译: make    单独: make labA    清理: make clean
# 注意: pthread 程序必须加 -pthread (Linux 和 macOS 通用)

CC     = gcc          # macOS 上没有 gcc 也行,clang 会被 cc 调用; 或改成 CC=cc
CFLAGS = -Wall -Wextra -O0 -g -pthread
LABS   = labA labB labC

all: $(LABS)

labA: labA_pthread.c
	$(CC) $(CFLAGS) -o $@ $<
labB: labB_race.c
	$(CC) $(CFLAGS) -o $@ $<
labC: labC_schedule.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(LABS)

.PHONY: all clean
