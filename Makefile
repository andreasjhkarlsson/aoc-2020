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

all: day1$(EXT) day2$(EXT) day3$(EXT) day4$(EXT) day5$(EXT) day6$(EXT) day7$(EXT) day8$(EXT) day9$(EXT) day10$(EXT)

day1$(EXT): obj/src/day1.c.o obj/src/util.c.o obj/src/bitarray.c.o
	$(CC) obj/src/day1.c.o obj/src/util.c.o obj/src/bitarray.c.o -o $@ $(LDFLAGS)

day2$(EXT): obj/src/day2.c.o obj/src/util.c.o
	$(CC) obj/src/day2.c.o obj/src/util.c.o -o $@ $(LDFLAGS)

day3$(EXT): obj/src/day3.c.o obj/src/util.c.o
	$(CC) obj/src/day3.c.o obj/src/util.c.o -o $@ $(LDFLAGS)	

day4$(EXT): obj/src/day4.c.o obj/src/util.c.o
	$(CC) obj/src/day4.c.o obj/src/util.c.o -o $@ $(LDFLAGS)	

day5$(EXT): obj/src/day5.c.o obj/src/util.c.o obj/src/bitarray.c.o
	$(CC) obj/src/day5.c.o obj/src/util.c.o obj/src/bitarray.c.o -o $@ $(LDFLAGS)	

day6$(EXT): obj/src/day6.c.o obj/src/util.c.o
	$(CC) obj/src/day6.c.o obj/src/util.c.o -o $@ $(LDFLAGS)

day7$(EXT): obj/src/day7.c.o obj/src/util.c.o
	$(CC) obj/src/day7.c.o obj/src/util.c.o -o $@ $(LDFLAGS)

day8$(EXT): obj/src/day8.c.o obj/src/util.c.o obj/src/bitarray.c.o
	$(CC) obj/src/day8.c.o obj/src/util.c.o obj/src/bitarray.c.o -o $@ $(LDFLAGS)		

day9$(EXT): obj/src/day9.c.o obj/src/util.c.o obj/src/bitarray.c.o
	$(CC) obj/src/day9.c.o obj/src/util.c.o obj/src/bitarray.c.o -o $@ $(LDFLAGS)		

day10$(EXT): obj/src/day10.c.o obj/src/util.c.o obj/src/bitarray.c.o
	$(CC) obj/src/day10.c.o obj/src/util.c.o obj/src/bitarray.c.o -o $@ $(LDFLAGS)	

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
