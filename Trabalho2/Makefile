main:
	lex lexico.lex
	yacc -d -v grammar.y
	cc lex.yy.c y.tab.c -o out

testsyntax: main
	python test.py unittest

saveresult:
	python test.py saveresults

clean:
	rm out
	rm lex.yy.c
	rm y.tab.c
	rm y.tab.h
