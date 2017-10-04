%{
#include <stdio.h>
#include <string.h>

int yylex();
void yyerror(const char *str);
int yywrap();

%}

%union{
  int i;
  char *s;
  float f;
}

%token	TK_INT
%token	TK_FLOAT
%token	TK_CHAR
%token	TK_VOID
%token	TK_NEW
%token	TK_IF
%token	TK_ELSE
%token	TK_WHILE
%token	TK_AS
%token	TK_RETURN
%token	TK_DEC
%token	TK_REAL
%token	TK_ID
%token	TK_EQUAL
%token  TK_NOTEQUAL
%token	TK_LESSEQUAL
%token	TK_GREATEREQUAL
%token	TK_AND
%token	TK_OR
%token	TK_STRING

%%

programa:              lista_definicoes                             ;

lista_definicoes:      definicao
                   |   definicao lista_definicoes                   ;

definicao:             def_variavel
                   |   def_funcao                                   ;

def_variavel:       TK_ID ':' tipo ';'
                    ;

lista_def_var:         /* vazio*/
                  |    def_variavel lista_def_var
                  ;

tipo:                  tipo_primitivo
                   |   tipo '[' ']'                                 ;

tipo_primitivo:         TK_INT
                   |    TK_FLOAT
                   |    TK_CHAR
                   |    TK_VOID                                     ;

def_funcao:             TK_ID '(' parametros ')' ':' tipo bloco     ;

parametros:             lista_params
                   |    /* vazio*/                                  ;

lista_params:           parametro
                   |    parametro ',' parametros                    ;

parametro:              TK_ID ':' tipo                              ;

bloco:                  '{' lista_def_var lista_comandos '}'        ;

lista_comandos:     lista_comandos2;
                   |    /*vazio*/                                   ;

lista_comandos2:   comando lista_comandos
                  |
                   ;

comando:                bloco
                   |    '@' expressao ';'
                   |    TK_RETURN expressao ';'
                   |    TK_RETURN ';'
                   |    variavel '=' expressao ';'
                   |    TK_IF  expressao  bloco
                   |    TK_IF  expressao  bloco  TK_ELSE  bloco
                   |    TK_WHILE    expressao    bloco              ;

variavel:               TK_ID
                   |    variavel '[' expressao ']'            ;

chamada:                TK_ID '(' expressoes ')'                    ;

expressoes:             lista_exp
                   |     /* vazio*/                                 ;

lista_exp:              expressao
                   |    expressao ',' lista_exp                     ;


expressao: expOr
          ;

expOr:      expOr TK_OR expAnd
                | expAnd                                            ;

expAnd:         expCmp
                | expAnd TK_AND expCmp                              ;

expCmp:   expCmp TK_EQUAL expAdd
        | expCmp TK_NOTEQUAL expAdd
        | expCmp TK_LESSEQUAL expAdd
        |	expCmp TK_GREATEREQUAL expAdd
        | expCmp'<' expAdd
        | expCmp '>' expAdd
        | expAdd;

expAdd: expAdd '+' expMult
        | expAdd '-' expMult
        | expMult                                                   ;

expMult: expMult '*' expUnaria
        | expMult '/' expUnaria
        | expUnaria                                                 ;


expUnaria:              expressao_base
                | '-'   expressao_base
                | '!'   expressao_base                              ;

expressao_base:     TK_DEC
                |   TK_REAL
                |   TK_STRING
                |   variavel
                |   '(' expressao ')'
                |   chamada
                |   expressao TK_AS tipo
                |   TK_NEW tipo '[' expressao ']'
                ;

%%

void yyerror(const char *str) {
  printf("error: %s\n",str);
}

int yywrap() {
  return 1;
}

int main(void) {
  yyparse();
  return 0;
}
