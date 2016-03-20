all:
	flex lexical.l
	gcc lex.yy.c -lfl -o scanner
	git add -A --ignore-errors
clean:
	rm scanner
