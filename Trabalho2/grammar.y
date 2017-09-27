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

programa:              lista_definicoes                             {;};

lista_definicoes:      definicao                                    {;}
                   |   definicao lista_definicoes                   {;};

definicao:             def_variavel                                 {;}
                   |   def_funcao                                   {;};

def_variavel:          TK_ID ':' tipo ';'                           {;};

lista_def_var:         def_variavel lista_def_var                   {;}
                   |   /* vazio*/                                   {;};

tipo:                  tipo_primitivo                               {;}
                   |   tipo '[' ']'                                 {;};

tipo_primitivo:         TK_INT                                      {;}
                   |    TK_FLOAT                                    {;}
                   |    TK_CHAR                                     {;}
                   |    TK_VOID                                     {;};

def_funcao:             TK_ID '(' parametros ')' ':' tipo bloco     {;};

parametros:             lista_params                                {;}
                   |    /* vazio*/                                  {;};

lista_params:           parametro                                   {;}
                   |    parametro ',' parametros                    {;};

parametro:              TK_ID ':' tipo                              {;};

bloco:                  '{' lista_def_var lista_comandos '}'                     {;};

lista_comandos:         lista_comandos comando                      {;}
                   |    /*vazio*/                                            {;};

comando:                TK_IF  expressao  bloco                     {;}
                   |    TK_IF  expressao  bloco  TK_ELSE  bloco     {;}
                   |    TK_WHILE    expressao    bloco              {;}
                   |    variavel '=' expressao ';'                  {;}
                   |    cmd_return                                  {;}
                   |    chamada ';'                                 {;}
                   |    '@' expressao ';'                           {;}
                   |    bloco                                       {;};


cmd_return:             TK_RETURN expressao ';'                     {;}
                   |    TK_RETURN ';'                               {;};

variavel:               TK_ID                                       {;}
                   |    expressao '[' expressao ']'                 {;};

chamada:                TK_ID '(' expressoes ')'                    {;};

expressoes:             lista_exp                                   {;}
                   |                                                {;};

lista_exp:              expressao                                   {;}
                   |    expressao ',' lista_exp                     {;};

expressao: expNew{;}
          | expAs {;};

expAs: expressao TK_AS tipo {;};

expNew: TK_NEW tipo '[' expressao ']' {;}
        | expLogica {;};

expLogica: expLogica TK_AND expCmp
          | expOr;

expOr: expLogica TK_OR expCmp {;}
        | expCmp  {;};

opCmp: TK_EQUAL
        | TK_NOTEQUAL {;}
        | TK_LESSEQUAL {;}
        |	TK_GREATEREQUAL {;}
        | '<' {;}
        | '>'  {;};

expCmp: expCmp opCmp expAdd {;}
        | expAdd {;};

addOp: '+' {;}
      | '-' {;};

expAdd: expAdd addOp expMult {;}
        | expMult {;};

multOp: '*' {;}
      | '/' {;};

expMult: expMult multOp expUnaria {;}
        | expUnaria {;};

opUnario: '!'  {;}
          | '-' {;};

expUnaria: expUnaria opUnario expVar {;}
          | expVar {;};

expVar: expVar '[' expressao ']' {;}
      | TK_ID {;}
      | TK_DEC {;}
      | TK_REAL {;}
      | chamada {;};

%%

void yyerror(const char *str) {
  fprintf(stderr,"error: %s\n",str);
}

int yywrap() {
  return 1;
}

int main(void) {
  yyparse();
  return 0;
}
