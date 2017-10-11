main:
	flex lexico.lex
	bison -d -v grammar.y
	cc lex.yy.c grammar.tab.c ast.c -o out

testflex: main
	python test.py unittest_diff tests_grammar

testsyntax: main
	python test.py unittest tests_syntax

testast: main
	python test.py unittest_diff tests_ast

saveresult_flex:
	python test.py saveresults tests_grammar

saveresult_syntax:
	python test.py saveresults tests_syntax

saveresult_ast:
	python test.py saveresults tests_ast

clean:
	rm out
	rm lex.yy.c
	rm grammar.tab.c
	rm grammar.tab.h
	rm grammar.output
