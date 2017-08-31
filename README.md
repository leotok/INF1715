# INF1715 - Compiladores

## Linguagem Monga
http://www.inf.puc-rio.br/~roberto/comp/lang.html

## Site
http://www.inf.puc-rio.br/~roberto/comp/

## Como usar (MacOS)
- Teste
```
$ flex teste.lex
$ gcc lex.yy.c -ll
$ ./a.out < teste.lex
```
- Output do teste:
```
# of lines = 12, # of chars = 209
```

## Analisador léxico para linguagem Monga
```
$ flex lexico.lex
$ gcc lex.yy.c -ll

$ ./a.out < tests/test_int.mmg
$ ./a.out < tests/test_keywords.mmg
```
