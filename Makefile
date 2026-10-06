CC = gcc

CFLAGS = -Wall -Wextra -std=c99
LDLIBS = -lm

SRC = src

BUILD = compiler

ifeq ($(OS),Windows_NT)
TARGET = $(BUILD)/sheylang.exe
else
TARGET = $(BUILD)/sheylang
endif

FILES = \
  $(SRC)/main.c \
  $(SRC)/lexer.c \
  $(SRC)/parser.c \
  $(SRC)/sheylang.c


.PHONY: all clean rebuild

all: $(TARGET)

$(TARGET): $(FILES) | $(BUILD)
	$(CC) $(CFLAGS) $(FILES) -o $(TARGET) $(LDLIBS)

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -f $(BUILD)/sheylang $(BUILD)/sheylang.exe


rebuild: clean all
