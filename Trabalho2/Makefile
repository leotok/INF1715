testsyntax:
	lex lexico.lex
	yacc -d grammar.y
	cc lex.yy.c y.tab.c -o out
	cat teste1 | ./out

clean:
	rm out
	rm lex.yy.c
	rm y.tab.c
	rm y.tab.h
