%{
	#include <stdio.h>
	int yylex(void);
	void yyerror(char *);
%}

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
%token	TK_LESSEQUAL
%token	TK_GREATEREQUAL
%token	TK_AND
%token	TK_OR
%token	TK_STRING

%%
               
programa:              lista_definicoes                             {;};
               
lista_definicoes:      definicao                                    {;}
                   |   lista_definicoes definicao                   {;};
               
definicao:             def_variavel                                 {;}
                   |   def_funcao                                   {;};
               
def_variavel:          TK_ID ':' tipo                               {;};

lista_def_var:         lista_def_var def_variavel                   {;}
                   |                                                {;};
               
tipo:                  tipo_primitivo                               {;}
                   |   tipo '[' ']'                                 {;};
               
tipo_primitivo:         TK_INT                                      {;}
                   |    TK_FLOAT                                    {;}
                   |    TK_CHAR                                     {;}
                   |    TK_VOID                                     {;};

def_funcao:             TK_ID '(' parametros ')' ':' tipo bloco     {;};

parametros:             lista_params                                {;}
                   |                                                {;};

lista_params:           parametro                                   {;}
                   |    lista_params ',' parametro                  {;};

parametro:              TK_ID ':' tipo                              {;};

bloco:                  '{' lista_def_var  '}'                      {;};

lista_comandos:         lista_comandos comando                      {;}
                   |                                                {;};

comando:                TK_IF  expressao  bloco                     {;}
                   |    TK_IF  expressao  bloco  TK_ELSE  bloco     {;}
                   |    TK_WHILE    expressao    bloco              {;}
                   |    variavel '=' expressao ';'                  {;}
                   |    cmd_return                                  {;}
                   |    chamada ';'                                 {;}
                   |    '@' expressao ';'                           {;}
                   |    bloco                                       {;};
                   

cmd_return:             TK_RETURN expressao ';'                     {;}
                   |    TK_RETURN                                   {;};

variavel:               TK_ID                                       {;}
                   |    expressao '[' expressao ']'                 {;};

chamada:                TK_ID '(' expressoes ')'                    {;};

expressoes:             lista_exp                                   {;}
                   |                                                {;};

lista_exp:              expressao                                   {;}
                   |    lista_exp ',' expressao                     {;};

