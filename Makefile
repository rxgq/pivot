CFLAGS = -Wall -g -Iinclude
OBJS = main.o lexer.o token.o parser.o ast.o
TARGET = main

$(TARGET): $(OBJS)
	gcc $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: src/main.c include/lexer.h
	gcc $(CFLAGS) -c src/main.c

lexer.o: src/lexer.c include/lexer.h
	gcc $(CFLAGS) -c src/lexer.c

token.o: src/token.c include/token.h
	gcc $(CFLAGS) -c src/token.c

parser.o: src/parser.c include/parser.h
	gcc $(CFLAGS) -c src/parser.c

ast.o: src/ast.c include/ast.h
	gcc $(CFLAGS) -c src/ast.c

clean:
	del /f $(OBJS) $(TARGET)