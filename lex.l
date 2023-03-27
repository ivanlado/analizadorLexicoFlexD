
%{
#include <string.h>
#include <stdio.h>
#include "definiciones.h"
#include "comun.h"
#include "ts.h"
int nestedCount = 0;
%}

%array
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

%x comentario
%x comentarioE2
%x comentarioNested
%x comentarioNestedE2
COMENTARIOSLINEA "//"[^\n]*
INICIOCOMENTARIOBLOQUE "/*"
FINCOMENTARIOBLOQUE "*/"
INICIOCOMENTARIONESTED "/+"
FINCOMENTARIONESTED "+/"



%%


cast|double|enforce|foreach|import|int|return|void|while {return 600;}

{IGNORAR} /* ignorar*/

{ID} {return COMPLEXICO_KEYWORD_INDENTIFICADOR;}

{TOKENSIMPLE} {return (int) *yytext;}

{STRING} return COMPLEXICO_STRING;

{INT} return COMPLEXICO_INT;
{FLOAT} return COMPLEXICO_FLOAT;
{EXP} return 4444444;
{LEADINGDECIMAL} return 76666;
{COMENTARIOSLINEA} return 99999;


{INICIOCOMENTARIOBLOQUE} {BEGIN(comentario);}
<comentario>[^*]* 
<comentario>"*" BEGIN(comentarioE2);
<comentarioE2>"/" {BEGIN(INITIAL); return 8888;} 
<comentarioE2>"*" 
<comentarioE2>[^*/] BEGIN(comentario);

{INICIOCOMENTARIONESTED} {BEGIN(comentarioNested);}
<comentarioNested>[^+]* 
<comentarioNested>"+" BEGIN(comentarioNestedE2);
<comentarioNestedE2>"/" {BEGIN(INITIAL); return 7777;} 
<comentarioNestedE2>"+" 
<comentarioNestedE2>[^+/] BEGIN(comentarioNested);




%%


compLexico siguienteComponenteLexico(){
    compLexico comp;
    comp.comp = yylex();
    comp.lexema = "kk";
    return comp;
}