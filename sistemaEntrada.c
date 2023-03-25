#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sistemaEntrada.h"
#include "errores.h"

#define TAMBLOQUE 128

// Fichero que contiene el programa a compilar
FILE *fp;

// Una sola varaible para los dos bloques del centinela
char bloque[TAMBLOQUE * 2];

// Ultimo bloque cargado, 0 el primero, 1 el segundo
int bloqueActual = 1;

// Posiciones del primero y ultima char del lexema actual
int _delantero = -1;
int _primero = 0;

// Variable booleana para indicar cuando se acaba un lexema, y hay que empezar el siguiente
int comienzoLexema = 0;

// Variable para indicar si es necesario seguir comprobando si se ha excedido el tamaño maximo
int seguirComprobandoTamMaximo = 0;

// Variable para indicar si se ha detectado el tamaño maximo
int tamMaximoDetectado = 0;


void inicializarSistemaEntrada(char * nombreFichero);

void cargarSiguienteBloque();

char *getLexemaCompleto(int aceptacionPorSigChar);

int _esFinFichero();

char siguienteChar();

void finLexema();

void finalizarSistemaEntrada();


void inicializarSistemaEntrada(char * fichero) {
    // Se abre el fichero de entrada en modo lectura
    fp = fopen(fichero, "r");
    // Se comprueba si ha habido algún error al abrirlo
    if (fp == NULL) {
        fprintf(stderr, "Error al abrir fichero.\n");
    }
    // EOF de cada bloque para implementarS centinela
    bloque[TAMBLOQUE - 1] = EOF;
    bloque[TAMBLOQUE * 2 - 1] = EOF;
    // Se carga el primer bloque
    cargarSiguienteBloque();
}

void cargarSiguienteBloque() {
    int nBytesLeidos;
    // Se actualiza el bloque actual
    bloqueActual = (bloqueActual + 1) % 2;
    // Punturo a la posicion del bloque en donde se quiere escribir, bloque A o bloque B
    char *bloqueLeer = bloque + (bloqueActual * TAMBLOQUE);
    // Como un char ocupa 1 byte, el nº de bytes leidos coincide con el nº de chars leidos.
    nBytesLeidos = fread(bloqueLeer, sizeof(char), TAMBLOQUE - 1, fp);
    // Se comprueba si se llega al fin del fichero, ya que fread no devuelve EOF
    if (nBytesLeidos < TAMBLOQUE - 1) {
        bloqueLeer[nBytesLeidos] = EOF;
    }
}

int _retroceder(int var) {
    return var == 0 ? 2 * TAMBLOQUE - 1 : var - 1;
}

int _avanzar(int var){
    return (var + 1) % (TAMBLOQUE * 2);
}

char *getLexemaCompleto(int aceptacionPorSigChar) {
    if(tamMaximoDetectado){
        if(!seguirComprobandoTamMaximo || (seguirComprobandoTamMaximo && !aceptacionPorSigChar)){
            errorTamanoLexema();
        }
    }
    char *lexema;
    int caso = 0;
    int tamPrimerBloque;
    // Variable necesaria en caso de que haya que retroceder delantero, y se encuentre un EOF
    int avanzarDelantero = 0;
    // Si aceptamos por siguiente caracter hay que retroceder una posicion
    if (aceptacionPorSigChar) {
        _delantero = _retroceder(_delantero);
        // Si al retroceder hay un EOF, hay que retroceder otra vez,
        // pero una vez acabada esta funcion, hay que sumarle uno a delantero,
        // para que vuelva a apuntar a EOF, si no, se cargara un nuevo bloque
        // (sobreescribiendo que uno que no se ha leido completamente)
        // Para eso se usara la variable avanzarDelantero
        if (bloque[_delantero] == EOF) {
            _delantero = (_delantero - 1) % (TAMBLOQUE * 2);
            avanzarDelantero = 1;
        }
    }
    if(tamMaximoDetectado){
        if(_delantero > TAMBLOQUE-1) _primero = _delantero-TAMBLOQUE+1;
        else if(_delantero < TAMBLOQUE -2 )_primero = (_delantero + 2*TAMBLOQUE)- TAMBLOQUE + 1;
        else _primero = 0;
    }
    // Tamaño del lexema si primero y delantero está en el mismo bloque
    int tamLexema = _delantero - _primero + 2;
    // Tamaño del lexema si primero y delantero están en bloques distintos
    if (_primero < TAMBLOQUE && _delantero >= TAMBLOQUE) {
        caso = 1;
        tamLexema = _delantero - _primero + 1;
    }
    if (_delantero < TAMBLOQUE && _primero >= TAMBLOQUE) {
        caso = 2;
        tamLexema = 2 * TAMBLOQUE - _primero;
        tamLexema += _delantero + 1;
    }
    // Se reserva memoria
    lexema = (char *) malloc(tamLexema * sizeof(char));
    // Se comprueba que la reserva de memoria haya sido correcta
    if (lexema == NULL) {
        fprintf(stderr, "Error al reservar memoria con malloc.\n");
    }

    // Para recuperar el lexema reconocido, se actua en funcion de los 3 posibles casos
    switch (caso) {
        case 0:
            // Caso 0: principio y delantero en el mismo bloque
            strncpy(lexema, &bloque[_primero], tamLexema - 1);
            break;
        case 1:
            // Caso 1: principio en el primer bloque y delantero en el segundo
            tamPrimerBloque = TAMBLOQUE - 1 - _primero;
            strncpy(lexema, &bloque[_primero], tamPrimerBloque);
            strncpy(lexema + tamPrimerBloque, &bloque[TAMBLOQUE], _delantero - (TAMBLOQUE - 1));
            break;
        case 2:
            // Caso 2: principio en el segundo bloque y delantero en el primero
            tamPrimerBloque = 2 * TAMBLOQUE - 1 - _primero;
            strncpy(lexema, &bloque[_primero], tamPrimerBloque);
            strncpy(lexema + tamPrimerBloque, bloque, _delantero + 1);
            break;
    }
    // Se pone caracter nulo al lexema
    lexema[tamLexema - 1] = 0;
    // Como se ha acabado un lexema, ahora puede comenzar un lexema nuevo
    finLexema();
    // Si al retrasar delantero habia un EOF, se volvia a retrasar. Entonces ahora
    // hay que deshace el ultimo retraso
    _delantero += avanzarDelantero;
    return lexema;
}

// Función para saber que devuelve 1 si delantero esta en una posicion en la que, en caso de leer
// un EOF, significaria el fin del fichero a compilar, y no los EOF del centinela
int _esFinFichero() {
    return (_delantero % TAMBLOQUE) < TAMBLOQUE - 1;
}

void comprobarTamanoMaximo(){
    if(_delantero + (seguirComprobandoTamMaximo - 1) * 2 * TAMBLOQUE - _primero > TAMBLOQUE - 1){
        if(tamMaximoDetectado) seguirComprobandoTamMaximo = 0;
        tamMaximoDetectado = 1;
    }
}

// Funcion que devuelve el siguiente caracter
char siguienteChar() {
    // Se avanza delantero
    _delantero = _avanzar(_delantero);
    if(seguirComprobandoTamMaximo) comprobarTamanoMaximo();
    // Se lee el caracter en la posicion delantero
    char sigChar = bloque[_delantero];
    // Si se lee EOF, puede ser del centinela, o porque se haya alcanzado el fin de fichero a compilar
    if (sigChar == EOF) {
        // Si se ha alcanzado el fin de fichero a compilar, se devuelve EOF
        if (_esFinFichero()) {
            return EOF;
        } else {
            seguirComprobandoTamMaximo = (tamMaximoDetectado == 0) * (bloqueActual + 1);
            // Si se ha alcanzado un EOF del centinela, se carga el siguiente bloque y se avanza delantero
            cargarSiguienteBloque();
            _delantero = _avanzar(_delantero);
            // Se lee el caracter en el nuevo bloque
            sigChar = bloque[_delantero];
            // Se mira si el primer char del nuevo bloque es EOF, entonces seria el final del fichero
            if (sigChar == EOF) return EOF;
            if(seguirComprobandoTamMaximo) comprobarTamanoMaximo();
        }
    }
    // Si comiezoLexema es True, significa que el lexema anterior se acabo, por lo que estamos empezando uno nuevo
    if (comienzoLexema) {
        _primero = _delantero;
        comienzoLexema = 0;
        seguirComprobandoTamMaximo = 0;
        tamMaximoDetectado = 0;
    }
    return sigChar;
}

// Funcion para finalizar el sistema de entrada, se cierra el fichero
void finalizarSistemaEntrada() {
    fclose(fp);
}


void finLexema() {
    seguirComprobandoTamMaximo = 0;
    tamMaximoDetectado = 0;
    comienzoLexema = 1;
}
