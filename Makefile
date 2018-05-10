roots.o: roots.c
	gcc roots.c -c -o roots.o

code.o: compiler
	./compiler
	nasm -fmacho32 -o code.o -w-number-overflow code.asm 

compiler: codegen.o parser.o lexer.o compiler.c differentiator.o
	gcc -DSPEC_FILE="\"$(SPEC_FILE)\"" compiler.c differentiator.o codegen.o parser.o lexer.o -o compiler
	chmod +x compiler

differentiator.o: differentiator.c
	gcc differentiator.c -c -o differentiator.o

codegen.o: codegen.c
	gcc codegen.c -c -o codegen.o

parser.o: parser.c
	gcc parser.c -c -o parser.o

lexer-test: lexer.o
	gcc lexer-test.c lexer.o -o lex-test
	chmod +x lex-test
	./lex-test <lexer-test.txt

lexer.o: lexer.c
	gcc lexer.c -c -o lexer.o
	
clean:
	rm lexer.o parser.o codegen.o lex-test compiler code.asm differentiator.o code.asm code.o roots.o