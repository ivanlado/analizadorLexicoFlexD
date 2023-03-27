
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

IDSTART ({LETRA}|_)
ID {IDSTART}({IDSTART}|{DIGITO})*
IGNORAR [\n ]
STRING1 \"(.|"\"")*\"
STRING2 \'(.|"\'")*\''
STRING {STRING1}|{STRING2} 
TOKENSIMPLE "/"|"."|"&"|"|"|"-"|"+"|"<"|">"|"!"|"("|")"|"["|"]"|"?"|","|";"|":"|"$"|"="|"*"|"%"|"^"|"~"|"@"|"#"|"{"|"}"
TOKENDOBLEMASMAS "++"
TOKENDOBLEMASIGUAL "+="
TOKENDOBLEIGUALIGUAL "=="

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
%x comentarioNestedE3
COMENTARIOSLINEA "//"[^\n]*
INICIOCOMENTARIOBLOQUE "/*"
FINCOMENTARIOBLOQUE "*/"
INICIOCOMENTARIONESTED "/+"
FINCOMENTARIONESTED "+/"



%%



{IGNORAR} /* ignorar*/

{ID} {
        compLexico comp;
        comp.lexema = yytext;
        if(buscarTS(&comp)){
            return comp.comp;
        } else{
            insertarTS(yytext, COMPLEXICO_KEYWORD_INDENTIFICADOR);
            comp.comp = COMPLEXICO_KEYWORD_INDENTIFICADOR;
            return comp.comp;
        }

}

{TOKENSIMPLE} {return (int) *yytext;}
{TOKENDOBLEMASMAS} {return COMPLEXICO_MASMAS;}
{TOKENDOBLEMASIGUAL} {return COMPLEXICO_MASIGUAL;}
{TOKENDOBLEIGUALIGUAL} {return COMPLEXICO_IGUALIGUAL;}

{STRING} return COMPLEXICO_STRING;

{INT} return COMPLEXICO_INT;
{FLOAT} return COMPLEXICO_FLOAT;
{EXP} return 4444444;
{LEADINGDECIMAL} return 76666;
{COMENTARIOSLINEA} /*ignorar*/


{INICIOCOMENTARIOBLOQUE} {BEGIN(comentario);}
<comentario>[^*]* 
<comentario>"*" BEGIN(comentarioE2);
<comentarioE2>"/" {BEGIN(INITIAL);} 
<comentarioE2>"*" 
<comentarioE2>[^*/] BEGIN(comentario);

{INICIOCOMENTARIONESTED} {BEGIN(comentarioNested); nestedCount++;}
<comentarioNested>[^+/]* 
<comentarioNested>"+" BEGIN(comentarioNestedE2);
<comentarioNested>"/" BEGIN(comentarioNestedE3);
<comentarioNestedE2>"/" {nestedCount--; if(!nestedCount){BEGIN(INITIAL);} else{BEGIN(comentarioNested);}} 
<comentarioNestedE2>"+" 
<comentarioNestedE2>[^+/] BEGIN(comentarioNested);
<comentarioNestedE3>"/"
<comentarioNestedE3>[^+/] BEGIN(comentarioNested);
<comentarioNestedE3>"+" nestedCount++; BEGIN(comentarioNested);




%%




compLexico siguienteComponenteLexico(){
    compLexico comp;
    comp.comp = yylex();
    comp.lexema = yytext;
    return comp;
}