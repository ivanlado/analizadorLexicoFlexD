#include <stdio.h>
#include "ts.h"
#include "comun.h"


void analizadorSintactico(){
    compLexico comp;

    do{
        // Se pide un componente lexico al analizador lexico
        comp = siguienteComponenteLexico();
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
        printf("%d %s\n", comp.comp, comp.lexema);
    } while(comp.comp);
}