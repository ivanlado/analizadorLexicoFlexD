%{
#include <string.h>
#include "definiciones.h"
%}

%option noyywrap header-file="lex.yy.h" noinput nounput

DIGITO [0-9]
ID [a-zA-Z][a-zA-Z0-9]*
IGNORAR [\n ]

%%

cast|double|enforce|foreach|import|int|return|void|while {return 600;}

{ID} {return 600;}
{IGNORAR} /* ignorar*/
"."|";" {return (int) *yytext;}

%%