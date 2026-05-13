CC = gcc
LEX = flex
YACC = bison -d
CFLAGS = -Wall -Isrc

OBJS = lex.yy.o parser.tab.o semantic.o codegen.o optimizer.o target.o interpreter.o main.o
TARGET = compiler.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

lex.yy.c: src/lexer.l
	$(LEX) -o lex.yy.c src/lexer.l

parser.tab.c parser.tab.h: src/parser.y
	$(YACC) -o parser.tab.c src/parser.y

lex.yy.o: lex.yy.c parser.tab.h
	$(CC) $(CFLAGS) -c lex.yy.c

parser.tab.o: parser.tab.c src/ast.h
	$(CC) $(CFLAGS) -c parser.tab.c

semantic.o: src/semantic.c src/semantic.h src/ast.h
	$(CC) $(CFLAGS) -c src/semantic.c

codegen.o: src/codegen.c src/codegen.h src/tac.h src/ast.h
	$(CC) $(CFLAGS) -c src/codegen.c

optimizer.o: src/optimizer.c src/optimizer.h src/tac.h
	$(CC) $(CFLAGS) -c src/optimizer.c

target.o: src/target.c src/target.h src/tac.h
	$(CC) $(CFLAGS) -c src/target.c

interpreter.o: src/interpreter.c src/interpreter.h src/tac.h
	$(CC) $(CFLAGS) -c src/interpreter.c

main.o: src/main.c src/ast.h src/semantic.h src/codegen.h src/optimizer.h src/target.h src/interpreter.h
	$(CC) $(CFLAGS) -c src/main.c

clean:
	rm -f *.o lex.yy.c parser.tab.c parser.tab.h $(TARGET)
