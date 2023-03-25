#include <stdio.h>
#include "lexico.h"
#include "definiciones.h"


void analizadorSintactico(){
    compLexico comp;

    int kk;
    int fin;
    do{
        // Se pide un componente lexico al analizador lexico
        kk = yylex();
        // Se comprueba si se recibe el componente lexico EOF
        // fin = (comp.comp == EOF);
        // if(!fin){
        //     // Si no hay componente lexico EOF se imprime
        //     printf("<%d, %s>\n", comp.comp, comp.lexema);
        //     // Se libera la memoria reservada del lexema
        //     free(comp.lexema);
        // } else{
        //     // Se recibe el componente lexico
        //     printf("Componente léxico EOF\n");
        // }
        printf("%d \n", kk);
    } while(kk);
}