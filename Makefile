all: main.c roots.c code.o
	$(CC) -mno-sse -m32 -mfpmath=387 main.c roots.c integral.c code.o -o iugerum

code.o: compiler
	./compiler
	nasm -felf32 -o code.o -w-number-overflow code.asm 

compiler: codegen.o parser.o lexer.o compiler.c differentiator.o
	$(CC) -DSPEC_FILE="\"$(SPEC_FILE)\"" compiler.c differentiator.o codegen.o parser.o lexer.o -o compiler
	chmod +x compiler

differentiator.o: differentiator.c
	$(CC) differentiator.c -c -o differentiator.o

codegen.o: codegen.c
	$(CC) codegen.c -c -o codegen.o

parser.o: parser.c
	$(CC) parser.c -std=c99 -c -o parser.o

lexer-test: lexer.o
	$(CC) lexer-test.c lexer.o -o lex-test
	chmod +x lex-test
	./lex-test <lexer-test.txt

lexer.o: lexer.c
	$(CC) lexer.c -c -o lexer.o
	
clean:
	rm -f lexer.o parser.o codegen.o lex-test compiler code.asm differentiator.o code.asm code.o iugerum
