CC      ?= gcc
CFLAGS  ?= -std=c11 -Wall -Wextra -g

SRC_DIRS := src src/core src/entities src/world
CPPFLAGS += $(addprefix -I,$(SRC_DIRS))
SRCS     := $(foreach d,$(SRC_DIRS),$(wildcard $(d)/*.c))
HDRS     := $(foreach d,$(SRC_DIRS),$(wildcard $(d)/*.h))
OBJS     := $(SRCS:%.c=build/%.o)

dungeon: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

build/%.o: %.c $(HDRS)
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build dungeon dungeon.exe

.PHONY: clean
