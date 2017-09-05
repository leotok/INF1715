main: main.c
	flex lexico.lex
	gcc lex.yy.c -ll
	cc -Wall -std=c99 -o main main.c lex.yy.c

testelex:
	python test.py unittest

clean:
	rm a.out
	rm main
	rm lex.yy.c

saveresult:
	python test.py saveresults