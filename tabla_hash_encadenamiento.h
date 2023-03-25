/* 
 * Tabla hash con encadenamiento
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

#define Tam 10007 /*DEFINIR NUMERO de entradas en la tabla.*/

typedef lista TablaHash[Tam];

void InicializarTablaHash (TablaHash t);
void DestruirTablaHash (TablaHash t);
int Hash (char *cad);
int Busqueda(TablaHash t, char *clavebuscar, int *compLexico);
int MiembroHash (TablaHash t, char *clavebuscar);
void InsertarHash (TablaHash *t, compLexico e);
void BorrarHash (TablaHash *t, char *claveborrar);

