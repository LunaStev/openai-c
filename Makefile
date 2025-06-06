CC = gcc
CFLAGS = -Iinclude
LDFLAGS = -lcurl -lcjson

SRC = src/openai.c
OBJ = build/openai.o
LIB = build/libopenai.a
EXAMPLE = examples/chat.c
TARGET = openai_example

PREFIX ?= /usr/local

all: build

build: $(LIB)

$(LIB): $(OBJ)
	ar rcs $(LIB) $(OBJ)

$(OBJ): $(SRC)
	mkdir -p build
	$(CC) -c $(SRC) -o $(OBJ) $(CFLAGS)

test: build
	$(CC) -o $(TARGET) $(EXAMPLE) -Iinclude -Lbuild -lopenai $(LDFLAGS)

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
