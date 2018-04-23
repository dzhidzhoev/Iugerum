parser.o: parser.c
	gcc parser.c -c -o parser.o

lexer-test: lexer.o
	gcc lexer-test.c lexer.o -o lex-test
	chmod +x lex-test
	./lex-test <lexer-test.txt

lexer.o: lexer.c
	gcc lexer.c -c -o lexer.o
	
clean:
	rm lexer.o parser.o lex-test