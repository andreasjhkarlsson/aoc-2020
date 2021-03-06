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

all: day1$(EXT) day2$(EXT) day3$(EXT) day4$(EXT) day5$(EXT) day6$(EXT) day7$(EXT) day8$(EXT) day9$(EXT) day10$(EXT) day11$(EXT) day12$(EXT) day13$(EXT) day14$(EXT) day15$(EXT) day16$(EXT) day17$(EXT) day18$(EXT) day19$(EXT)

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

day11$(EXT): obj/src/day11.c.o obj/src/util.c.o
	$(CC) obj/src/day11.c.o obj/src/util.c.o -o $@ $(LDFLAGS)	

day12$(EXT): obj/src/day12.c.o obj/src/util.c.o obj/src/hashmap.c.o
	$(CC) obj/src/day12.c.o obj/src/util.c.o obj/src/hashmap.c.o -o $@ $(LDFLAGS)			

day13$(EXT): obj/src/day13.c.o obj/src/util.c.o
	$(CC) obj/src/day13.c.o obj/src/util.c.o -o $@ $(LDFLAGS)	

day14$(EXT): obj/src/day14.c.o obj/src/util.c.o obj/src/bitarray.c.o
	$(CC) obj/src/day14.c.o obj/src/util.c.o obj/src/bitarray.c.o -o $@ $(LDFLAGS)	

day15$(EXT): obj/src/day15.c.o obj/src/util.c.o obj/src/hashmap.c.o
	$(CC) obj/src/day15.c.o obj/src/util.c.o obj/src/hashmap.c.o -o $@ $(LDFLAGS)					

day16$(EXT): obj/src/day16.c.o obj/src/util.c.o
	$(CC) obj/src/day16.c.o obj/src/util.c.o -o $@ $(LDFLAGS)	

day17$(EXT): obj/src/day17.c.o obj/src/util.c.o obj/src/hashmap.c.o
	$(CC) obj/src/day17.c.o obj/src/util.c.o obj/src/hashmap.c.o -o $@ $(LDFLAGS)		

day18$(EXT): obj/src/day18.c.o obj/src/util.c.o obj/src/parse.c.o
	$(CC) obj/src/day18.c.o obj/src/util.c.o obj/src/parse.c.o -o $@ $(LDFLAGS)	

day19$(EXT): obj/src/day19.c.o obj/src/util.c.o  obj/src/parse.c.o
	$(CC) obj/src/day19.c.o obj/src/util.c.o  obj/src/parse.c.o -o $@ $(LDFLAGS)		

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
