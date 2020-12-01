CC = gcc
CFLAGS = -x c -std=c17 -MMD -MP -g
TARGET_EXEC ?= aoc-2020

BUILD_DIR ?= obj

SRCS :=  $(wildcard src/*.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

DEPS := $(OBJS:.o=.d)

$ $(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

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
