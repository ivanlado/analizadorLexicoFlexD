#include <stdio.h>
#include "ts.h"
#include "comun.h"


void analizadorSintactico(){
    compLexico comp;

    do{
        // Se pide un componente lexico al analizador lexico
        comp = siguienteComponenteLexico();
        // Se imprime el resultado
        printf("<%d, %s>\n", comp.comp, comp.lexema);
    } while(comp.comp);
}