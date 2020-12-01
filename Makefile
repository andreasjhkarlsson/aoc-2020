CC = gcc
CFLAGS = -x c -std=c17 -MMD -MP -g
TARGET_EXEC ?= aoc-2020

BUILD_DIR ?= obj

SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

DEPS := $(OBJS:.o=.d)


all: day1

day1: obj/src/day1.c.o obj/src/util.c.o
	$(CC) obj/src/day1.c.o obj/src/util.c.o -o $@ $(LDFLAGS)

# c source
obj/%.c.o: %.c
ifeq ($(OS),Windows_NT)
	if not exist $(subst /,\,$(dir $@)) md $(subst /,\,$(dir $@))
else
	mkdir -p $(dir $@)
endif
	
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
ifeq ($(OS),Windows_NT)
	del $(BUILD_DIR)
else
	$(RM) -r $(BUILD_DIR)
endif

-include $(DEPS)
