# Variables
CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS =

SRCS = src/token.c src/lexer.c src/main.c src/ast.c
OBJS = $(SRCS:.c=.o)
TARGET = runic

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean
