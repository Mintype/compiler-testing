CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/main.c src/lexer.c
OUT = compiler

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT) examples/test.min

clean:
	rm -f $(OUT)