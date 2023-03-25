#ifndef TS_H
#define TS_H
#include "tabla_hash_encadenamiento.h"
#include "lexico.h"


void inicializarTS();
int buscarTS(compLexico *compLexico);
void insertarTS(char * lexema, int comp);
void finalizarTS();

#endif
