main: main.c lex.yy.c
	flex lexico.lex
	gcc lex.yy.c -ll
	cc -Wall -std=c99 -o main main.c lex.yy.c

testelex:
	sh testelex.sh

clean:
		rm a.out
		rm main
