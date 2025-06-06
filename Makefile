CC = gcc
CFLAGS = -Iinclude
LDFLAGS = -lcurl -lcjson

SRC = src/openai.c
OBJ = build/openai.o
LIB = build/libopenai.a
EXAMPLES = $(wildcard examples/*.c)
TARGETS = $(patsubst examples/%.c, build/%, $(EXAMPLES))
TARGET = openai_example

PREFIX ?= /usr/local

all: build

build: $(LIB)

$(LIB): $(OBJ)
	ar rcs $(LIB) $(OBJ)

$(OBJ): $(SRC)
	mkdir -p build
	$(CC) -c $(SRC) -o $(OBJ) $(CFLAGS)

test: build $(TARGETS)

build/%: examples/%.c build
	$(CC) -o $@ $< -Iinclude -Lbuild -lopenai $(LDFLAGS)

install: build
	mkdir -p $(PREFIX)/lib
	cp $(LIB) $(PREFIX)/lib/
	mkdir -p $(PREFIX)/include
	cp include/openai.h $(PREFIX)/include/

uninstall:
	rm -f $(PREFIX)/lib/libopenai.a
	rm -f $(PREFIX)/include/openai.h

clean:
	rm -rf $(TARGET) build
