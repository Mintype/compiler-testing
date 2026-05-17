CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/main.c src/lexer.c src/parser.c
OUT = compiler

TEST_DIR = tests
EXPECTED_DIR = expected
BUILD_DIR = build

GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

test: all
	@mkdir -p $(BUILD_DIR)

	@for file in $(TEST_DIR)/*.min; do \
		name=$$(basename $$file .min); \
		expected_file="$(EXPECTED_DIR)/$$name.txt"; \
		expected=$$(cat $$expected_file); \
		asm="$(BUILD_DIR)/$$name.asm"; \
		obj="$(BUILD_DIR)/$$name.o"; \
		bin="$(BUILD_DIR)/$$name"; \
		./$(OUT) $$file > $$asm; \
		nasm -f elf64 $$asm -o $$obj; \
		gcc $$obj -o $$bin; \
		$$bin; actual=$$?; \
		if [ $$actual -eq $$expected ]; then \
			printf "$(GREEN)PASS$(RESET) %s => %s\n" $$name $$actual; \
		else \
			printf "$(RED)FAIL$(RESET) %s: expected %s, got %s\n" $$name $$expected $$actual; \
			exit 1; \
		fi; \
	done

clean:
	rm -f $(OUT)
	rm -rf $(BUILD_DIR)
	find . -type f \( \
		-name "*.asm" -o \
		-name "*.o" -o \
		-name "*.out" -o \
		-name "out" \
	\) -delete