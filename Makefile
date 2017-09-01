main:
	flex lexico.lex
	gcc lex.yy.c -ll
	./a.out < tests/test_int.mmg

clean:
		rm a.out
