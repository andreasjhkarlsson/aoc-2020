CC = clang
CFLAGS = -x c -std=c17 -MMD -MP -g -Wall
ifeq ($(OS),Windows_NT)
EXT = .exe
else
EXT = 
endif

BUILD_DIR ?= obj

SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

DEPS := $(OBJS:.o=.d)


all: day1.exe day2.exe

day1$(EXT): obj/src/day1.c.o obj/src/util.c.o obj/src/bitarray.c.o
	$(CC) obj/src/day1.c.o obj/src/util.c.o obj/src/bitarray.c.o -o $@ $(LDFLAGS)

day2$(EXT): obj/src/day2.c.o obj/src/util.c.o
	$(CC) obj/src/day2.c.o obj/src/util.c.o -o $@ $(LDFLAGS)

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
	rmdir /S /Q $(BUILD_DIR)
else
	$(RM) -r $(BUILD_DIR)
endif

-include $(DEPS)
