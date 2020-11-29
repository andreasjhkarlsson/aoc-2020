CC = gcc
CFLAGS = -x c -std=c17 -MMD -MP
TARGET_EXEC ?= aoc-2020

BUILD_DIR ?= obj

SRCS :=  src/main.c
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
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)
