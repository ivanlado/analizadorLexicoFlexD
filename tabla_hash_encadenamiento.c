#include "tabla_hash_encadenamiento.h"

/* TABLA HASH CON ENCADENAMIENTO */

/* CREAR TABLA VACIA */
void InicializarTablaHash(TablaHash t) {
    int i;
    for (i = 0; i < Tam; i++)
        crea(&t[i]);
}

/* DESTRUIR TABLA */
void DestruirTablaHash(TablaHash t) {
    int i;
    for (i = 0; i < Tam; i++)
        destruye(&t[i]);
}

/******* FUNCIONES HASH *******/

/* FUNCION HASH 1 */


/*
int Hash(char *cad) {
    int valor;
    unsigned char *c;

    for (c = cad, valor = 0; *c; c++)
        valor += (int) *c;

    return (valor % Tam);
}
*/

 

/* FUNCION HASH 2 */
int Hash(char *cad) {
    int i, suma = 0;
    for (i = strlen(cad) - 1; i >= 0; i--) {
        suma = (suma * 256 + cad[i]) % Tam;
    }
    return suma;
}

/* FUNCION HASH 3: Probad al menos dos valores para la constante K */


//  int Hash (char *cad){
//     int i,suma=0;
//     int K=1;
//     for (i=strlen(cad)-1;i>=0;i--){
//         suma=(suma*K+cad[i])%Tam;
//     }
//     return suma;
// }
 

/* BUSCA UN ELEMENTO CON LA CLAVE INDICADA EN LA TABLA HASH, Y LO DEVUELVE, 
 * ADEMAS DE INDICAR CON 1 QUE EXISTE EL ELEMENTO, Y CON 0 QUE NO ESTA EN LA TABLA */
int Busqueda(TablaHash t, char *clavebuscar, int *compLexico) {
    posicion p;
    int enc;
    struct compLexico ele;
    int pos = Hash(clavebuscar);

    p = primero(t[pos]);
    enc = 0;

    while (p != fin(t[pos]) && !enc) {
        recupera(t[pos], p, &ele);
        if (strcmp(ele.lexema, clavebuscar) == 0) {
            enc = 1;
            *compLexico = ele.comp;
        } else{
            p = siguiente(t[pos], p);
        }
    }

    return enc;
}

/* DEVUELVE 1 SI EL ELEMENTO e ESTA EN LA TABLA Y 0 SI NO LO ESTA */
int MiembroHash(TablaHash t, char *clavebuscar) {
    posicion p;
    int enc;
    compLexico elemento;
    int pos = Hash(clavebuscar);
    p = primero(t[pos]);
    enc = 0;
    while (p != fin(t[pos]) && !enc) {
        recupera(t[pos], p, &elemento);
        if (strcmp(clavebuscar, elemento.lexema) == 0)
            enc = 1;
        else
            p = siguiente(t[pos], p);
    }

    return enc;
}

/* INSERTA UN ELEMENTO EN LA TABLA HASH: AL PRINCIPIO DE LA LISTA EN LA 
 * POSICION INDICADA POR LA FUNCION HASH */
void InsertarHash(TablaHash *t, compLexico elemento) {
    int pos;
    pos = Hash(elemento.lexema);
    inserta(&(*t)[pos], primero((*t)[pos]), elemento);

}

/* Elimina de la tabla el elemento con la lexema indicada */
void BorrarHash(TablaHash *t, char *claveborrar) {
    posicion p;
    compLexico elemento;
    int pos = Hash(claveborrar);

    p = primero((*t)[pos]);
    recupera((*t)[pos], p, &elemento);
    while (p != fin((*t)[pos]) && strcmp(claveborrar, elemento.lexema)) {
        p = siguiente((*t)[pos], p);
        recupera((*t)[pos], p, &elemento);
    }
    if (p != fin((*t)[pos]))
        suprime(&(*t)[pos], p);
}



