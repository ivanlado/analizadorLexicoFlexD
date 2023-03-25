%{
#include <string.h>
#include "definiciones.h"
%}

%option noyywrap header-file="lex.yy.h" noinput nounput

DIGITO [0-9]
LETRA [a-zA-Z]
ID {LETRA}({LETRA}|{DIGITO}|_)*
IGNORAR [\n ]
STRING (\"([^"]*)\")|(\'([^'']*)\')
TOKENSIMPLE "/"|"."|"&"|"|"|"-"|"+"|"<"|">"|"!"|"("|")"|"["|"]"|"?"|","|";"|":"|"$"|"="|"*"|"%"|"^"|"~"|"@"|"#"
TOKENDOBLE "++"|"+="|"=="

INTDECIMAL 0|[1-9][0-9_]*
BINARIOS 0b[0-1_]*
INT {INTDECIMAL}|{BINARIOS}

DECIMALDIGITSNOSINGLEUS ([0-9][0-9_]*)|([0-9_]*[0-9])
DECIMALNOSTARTINGUS [0-9][0-9_]*

EXP [eE][+-]?{DECIMALDIGITSNOSINGLEUS}
LEADINGDECIMAL {INTDECIMAL}|0{DECIMALDIGITSNOSINGLEUS}

FLOAT1 {LEADINGDECIMAL}?"."{DECIMALNOSTARTINGUS}{EXP}?
FLOAT2 {LEADINGDECIMAL}{EXP}
FLOAT3 {LEADINGDECIMAL}"."
FLOAT {FLOAT1}|{FLOAT2}|{FLOAT3}

COMENTARIOSLINEA "//"[^\n]*
COMENTARIOSBLOQUE "/*"[^*/]*"*/"


%%

cast|double|enforce|foreach|import|int|return|void|while {return 600;}

{IGNORAR} /* ignorar*/

{ID} {return COMPLEXICO_KEYWORD_INDENTIFICADOR;}

{TOKENSIMPLE} {return (int) *yytext;}

{STRING} return COMPLEXICO_STRING;

{INT} return COMPLEXICO_INT;
{FLOAT} return COMPLEXICO_FLOAT;
{COMENTARIOSLINEA} return 88888888;
{COMENTARIOSBLOQUE} return 99999999;
{EXP} return 4444444;
{FLOAT3} return 6666666666;
{LEADINGDECIMAL} return 7666666666;


%%