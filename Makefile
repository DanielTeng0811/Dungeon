CC      ?= gcc
CFLAGS  ?= -std=c11 -Wall -Wextra -g
SRCS    := $(wildcard *.c)
OBJS    := $(SRCS:.c=.o)

dungeon: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c $(wildcard *.h)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) dungeon dungeon.exe

.PHONY: clean
