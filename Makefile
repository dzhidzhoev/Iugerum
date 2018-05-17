CFLAGS=-std=c99 -mno-sse -m32 -mfpmath=387

all: main.o roots.o integral.o code.o
	$(CC) $(CFLAGS) main.o roots.o integral.o code.o -o iugerum

code.o: compiler
	./compiler
	nasm -felf32 -o code.o -w-number-overflow code.asm 

compiler: codegen.o parser.o lexer.o compiler.c differentiator.o
	$(CC) $(CFLAGS) -DSPEC_FILE="\"$(SPEC_FILE)\"" compiler.c differentiator.o codegen.o parser.o lexer.o -o compiler
	chmod +x compiler

lexer-test: lexer.o
	$(CC) lexer-test.c lexer.o -o lex-test
	chmod +x lex-test
	./lex-test <lexer-test.txt

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)
	
clean:
	rm -f lexer.o parser.o codegen.o lex-test compiler code.asm differentiator.o code.asm code.o integral.o main.o roots.o iugerum
