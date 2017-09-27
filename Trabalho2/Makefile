main:
	lex lexico.lex
	yacc -d grammar.y
	cc lex.yy.c y.tab.c -o out

testsyntax:
	python test.py unittest

saveresult:
	python test.py saveresults

clean:
	rm out
	rm lex.yy.c
	rm y.tab.c
	rm y.tab.h
