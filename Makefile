CC      ?= cc
CFLAGS  ?= -std=c11 -Wall -Wextra -Wpedantic -g -O0 -Isrc
SAN      = -fsanitize=address,undefined

SRCS = src/ll.c tests/test_ll.c
BIN  = build/test_ll

.PHONY: all test clean

all: $(BIN)

$(BIN): $(SRCS) src/ll.h | build
	$(CC) $(CFLAGS) $(SAN) $(SRCS) -o $@

build:
	mkdir -p build

test: $(BIN)
	./$(BIN)

clean:
	rm -rf build
