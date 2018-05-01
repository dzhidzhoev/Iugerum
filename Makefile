

compiler: codegen.o parser.o lexer.o compiler.c
	gcc -DSPEC_FILE="\"$(SPEC_FILE)\"" compiler.c codegen.o parser.o lexer.o -o compiler

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
	rm lexer.o parser.o codegen.o lex-test compiler code.asm