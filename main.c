#include "sintactico.h"
#include "ts.h"
#include "lex.yy.h"

void inicializacion(char * nombreFichero);
void finalizacion();

int main(int argc, char **argv) {
    // Se lee el nombre del fichero por parametro
    char *nombreFichero;
    if(argc > 1){
        nombreFichero = argv[1];
    } else{
        nombreFichero = "regression.d";
    }
    // Acciones de inicializacion
    inicializacion(nombreFichero);
    // Analisis sintactico
    analizadorSintactico();
    // Acciones de finalizacion
    finalizacion();
    return EXIT_SUCCESS;
}

void inicializacion(char * nombreFichero){
    // Se abre el fichero
    yyin = fopen("regression.d", "r");
    //Se inicializa la ts
    inicializarTS();
}

void finalizacion(){
    // Se cierra el fichero
    fclose(yyin);
    // Se finaliza la ts
    finalizarTS();
}


