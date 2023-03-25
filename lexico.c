#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexico.h"
#include "sistemaEntrada.h"
#include "definiciones.h"
#include "ts.h"

// Constante para las dos formas de aceptar un lexema
#define ACEPTACIONPORSIGCHAR 1
#define ACEPTACIONULTIMOCHAR 0

// Componente lexico asociado a un comentario, se usa para saber cuando una estructura del tipo
// compLexico es un comentario
#define COMENTARIO -500


/*
 * Funcion llamada por el analizador sintactico para que le devuelva el siguiente componente lexico
 */
compLexico siguienteComponente();

/*
 * Funcion que devuelve 1 si el char c es un token y 0 si no
 * Un char se considera token si esta en la seccion de token en la documentacion oficial de d
 */
int istoken(char c);

/**
 * Funcion que acepta un lexema
 * @param aceptacionPorSigChar para indicar si se acepta el lexema por siguiente char o por el actual
 * @param componenteLexico tipo de componente lexico a aceptar, p.ej, COMPLEXICO_FLOAT, COMPLEXICO_INT
 * @return
 */
compLexico _aceptarLexema(int aceptacionPorSigChar, int componenteLexico);

/*
 * Automata que reconoce identficadores y palabras reservadas
 */
compLexico _erIdentificadorPalabraReservada();

/*
 * Automata que roconoce numeros binarios
 */
compLexico _erNumBinarios();

/*
 * Automata que reconoce numeros, tanto enteros como decimales
 */
compLexico _erNumero(char charAux);

/*
 * Automata que reconoce comentario de una sola linea
 */
compLexico _erComentariosTipoLinea();

/*
 * Automata que reconoce comentarios de tipo bloque
 */
compLexico _erComentariosTipoBlock();

/*
 * Funcion que se invoca de manera recursiva para reconocer los comentarios anidados.
 */
void _erNestComment();

/*
 * Automata que reconoce comentarios de bloque anidados
 */
compLexico _erComentariosTipoNestBlock();

/**
 * Funcion que procesa los tokens. Recibe el primer token leido. A partir de el, se lee
 * el siguiente char, y en funcion de este ultimo podemos estar ante un numero decimal, un
 * comentario o un token doble
 * @param primerToken
 * @return
 */
compLexico _erToken(char primerToken);

/**
 * Funcion que reconoce strings
 * @param delimitadorString caracter que marca el comienzo y fin de la misma, " o '
 * @return
 */
compLexico _erString(char delimitadorString);


/*
 *
 *
 *
 *
 * IMPLEMENTACION DE LAS FUNCIONES
 *
 *
 *
 *
 */

compLexico siguienteComponente() {
    compLexico comp;
    char c;
    while (1) {
        // Se obtiene siguiente caracter por sistema de entrada
        c = siguienteChar();
        //Caso 1: Identificadores y palabras reservadas
        if (isalpha(c) || c == '_') return _erIdentificadorPalabraReservada();
        // Caso 2: Numeros (enteros y decimales)
        if (isdigit(c)) return _erNumero(c);
        // Caso 3: Tokens
        if (istoken(c)) {
            comp = _erToken(c);
            // Si es un comentario, se ignora, y se siguen leyendo caracteres
            if (comp.comp == COMENTARIO) {
                // Se avisa al sistema de e/s de que empieza un nuevo lexema
                finLexema();
                continue;
            }
            return comp;
        }
        // Caso 4: Strings
        if (c == '"' || c == '\'') return _erString(c);
        // Caso 5: Espacio en blanco, \n y \t los saltamos
        if (isblank(c) || c == '\n' || c == '\t') {
            // Se avisa al sistema de e/s de que empieza un nuevo lexema
            finLexema();
            continue;
        }
        // Caso 6: Componente léxico EOF
        if (c == EOF) {
            comp.comp = EOF;
            return comp;
        }
    }

}


int istoken(char c) {
    return ispunct(c) && c != '\\' && c != '"' && c != '\'';
}

compLexico _aceptarLexema(int aceptacionPorSigChar, int componenteLexico) {
    // Variable que va a recibir el analizador sintactico con el componente lexico
    compLexico comp;
    // Si es el componente lexico es un comentario, devolvemos un componente lexico de tipo comentario
    if (componenteLexico == COMENTARIO) {
        compLexico comentario;
        comentario.comp = COMENTARIO;
        return comentario;
    }
    // Lexema a aceptar
    char *lexema = getLexemaCompleto(aceptacionPorSigChar);
    comp.comp = componenteLexico;
    comp.lexema = lexema;
    int lexLen;
    // Si el componente lexico es una palabra reservada o un identificador hay que ir a la TS
    if (componenteLexico == COMPLEXICO_KEYWORD_INDENTIFICADOR) {
        // Se comprueba si el lexema está en la ts
        if (buscarTS(&comp)) {
            return comp;
        } else {
            // Hay que hacer una copia de la string, que es la que se va ir la ts
            lexLen = (strlen(lexema) + 1) * sizeof(char);
            comp.lexema = (char *) malloc(lexLen);
            strncpy(comp.lexema, lexema, lexLen);
            // Si no esta en la ts, se inserta (se inserta la copia, la orginal la va destruir el sintactico)
            insertarTS(comp.lexema, COMPLEXICO_KEYWORD_INDENTIFICADOR);
            // Importante, la referencia que le damos al sintactico, es la dada por el sistema de entrada salida,
            // la copia que acabamos de hacer es para la ts
            comp.lexema = lexema;
            return comp;
        }
    }
    // En caso de que no sea un identificador o una palabra reservada no es necesario acudir a la ts
    return comp;
}


compLexico _erIdentificadorPalabraReservada() {
    char c;
    // Se leen caracteres mientras que no se reciba EOF
    while (1) {
        c = siguienteChar();
        // Si llega EOF se acepta
        if (c == EOF) return _aceptarLexema(ACEPTACIONPORSIGCHAR, COMPLEXICO_KEYWORD_INDENTIFICADOR);
        // El fin del lexema se encuentra cuando el caracter leido
        // no es un char alfanum ni barra baja
        if (!isdigit(c) && !isalpha(c) && c != '_') {
            return _aceptarLexema(ACEPTACIONPORSIGCHAR, COMPLEXICO_KEYWORD_INDENTIFICADOR);
        }
    }
}

compLexico _erNumBinarios() {
    char c;
    while (1) {
        // Se lee el siguiente caracter
        c = siguienteChar();
        // Si se llega al EOF, se acepta el componente lexico
        if (c == EOF) return _aceptarLexema(ACEPTACIONPORSIGCHAR, COMPLEXICO_INT);
        // Si se encuentra \n significa el fin del comentario
        if (c != 0 && c != '1' && c != '_') return _aceptarLexema(ACEPTACIONPORSIGCHAR, COMPLEXICO_INT);
    }
}

compLexico _erNumero(char charAux) {
    char c;
    // Estado inicial
    int estado = 3;
    int tipoNumero = COMPLEXICO_INT;
    // En funcion del primer char, se cambia de estado inicial
    if (charAux == '0') estado = 1;
    if (charAux == '_') estado = 2;
    if (charAux == '.') {
        // Este numero es un float
        estado = 4;
        tipoNumero = COMPLEXICO_FLOAT;
    }
    // Se leen caracteres mientras que no se reciba EOF
    while (1) {
        // Se lee el siguiente caracter
        c = siguienteChar();
        // Si se llega al EOF, se acepta el componente lexico
        if (c == EOF) return _aceptarLexema(ACEPTACIONPORSIGCHAR, tipoNumero);
        // Automata
        switch (estado) {
            case 1:
                // El primer char es un 0, veamos si es binario, para eso tiene que llegar una b
                if (!isdigit(c)) {
                    if (c == 'b') return _erNumBinarios();
                    else if (c == '_') estado = 2;
                    else return _aceptarLexema(ACEPTACIONPORSIGCHAR, tipoNumero);
                } else estado = 3;
                break;
            case 2:
                // Ha llegado _, estado intermedio mientras no llegue un digito
                if (isdigit(c)) estado = 3;
                else if (c != '_') return _aceptarLexema(ACEPTACIONPORSIGCHAR, tipoNumero);
                break;
            case 3:
                if (!isdigit(c)) {
                    if (c == '.') {
                        // Si llega un punto, estamos ante un float
                        estado = 4;
                        tipoNumero = COMPLEXICO_FLOAT;
                    } else if (c == 'e' || c == 'E') {
                        estado = 7;
                    } else if (c == '_') estado = 2;
                    else return _aceptarLexema(ACEPTACIONPORSIGCHAR, tipoNumero);
                }
                break;
            case 4:
                if (isdigit(c)) estado = 5;
                else return _aceptarLexema(ACEPTACIONPORSIGCHAR, tipoNumero);
                break;
            case 5:
                if (!isdigit(c)) {
                    if (c == 'e' || c == 'E') estado = 7;
                    else if (c != '_') return _aceptarLexema(ACEPTACIONPORSIGCHAR, tipoNumero);
                }
                break;
            case 7:
                if (isdigit(c))estado = 9;
                else if (c == '+' || c == '-') estado = 8;
                else if (c == '_') estado = 10;
                else return _aceptarLexema(ACEPTACIONPORSIGCHAR, tipoNumero);
                break;
            case 8:
                if (isdigit(c)) estado = 9;
                else if (c == '_') estado = 10;
                else return _aceptarLexema(ACEPTACIONPORSIGCHAR, tipoNumero);
                break;
            case 9:
                if (isdigit(c)) estado = 11;
                else if (c != '_') return _aceptarLexema(ACEPTACIONPORSIGCHAR, tipoNumero);
                break;
            case 10:
                if (isdigit(c)) estado = 12;
                else if (c != '_') return _aceptarLexema(ACEPTACIONPORSIGCHAR, tipoNumero);
                break;
            case 11:
                if (!isdigit(c)) {
                    if (c == '_') estado = 9;
                    else return _aceptarLexema(ACEPTACIONPORSIGCHAR, tipoNumero);
                }
                break;
            case 12:
                if (!isdigit(c)) {
                    if (c == '_') estado = 10;
                    else return _aceptarLexema(ACEPTACIONPORSIGCHAR, tipoNumero);
                }
                break;
        }
    }
}

/*
 * Reconoce comentario de una sola línea que empiezan por // y acaban por \n
 */
compLexico _erComentariosTipoLinea() {
    char c;
    while (1) {
        // Se lee el siguiente caracter
        c = siguienteChar();
        // Si se encuentra \n significa el fin del comentario
        if (c == '\n') {
            return _aceptarLexema(ACEPTACIONPORSIGCHAR, COMENTARIO);
        }
        // Si se llega al fin del fichero se acepta el comentario tambien
        if (c == EOF) return _aceptarLexema(ACEPTACIONPORSIGCHAR, COMENTARIO);
    }
}

/*
 * Reconoce comentario de bloque, que empiezan por barra asterisco y acaban por barra asterisco
 */
compLexico _erComentariosTipoBlock() {
    char c;
    int primerAsterisco = 0;
    while (1) {
        // Se lee el siguiente caracter
        c = siguienteChar();
        // Si se le una barra, y el anterior char fue un asterisco, se acepta el comentario
        if (primerAsterisco && c == '/') {
            return _aceptarLexema(ACEPTACIONULTIMOCHAR, COMENTARIO);
        }
        // Se le un asterisco, entonces, si el siguiente char es una barra se acepta el comentario
        if (c == '*') primerAsterisco = 1;
            // No se ha leido un asterisco, entonces no se puede acabar el comentario
        else primerAsterisco = 0;
        // Si se llega al fin del fichero
        if (c == EOF) return _aceptarLexema(ACEPTACIONPORSIGCHAR, COMENTARIO);
    }
}

void _erNestComment() {
    char c;
    // Variable auxiliar para detectar fin de comentario
    int primerMas = 0;
    // Variable auxiliar para detectar caracteres escapados
    int primerBarra = 0;
    while (1) {
        // Se lee el siguiente caracter
        c = siguienteChar();
        // Si el char previo era un +, y este una barra, se acepta el comentario
        if (primerMas && c == '/') {
            break;
        }
        // Si el char previo era una barra y el actual un +, estamos ante un comentario anidado
        if (primerBarra && c == '+') {
            _erNestComment();
        }
        // Se ha leido un +, por lo que puede acabar el comentario
        primerMas = (c == '+');
        // Se comprueba si el char actual es una barra
        primerBarra = (c == '/');
        // Si se lleva al fin del fichero
        if (c == EOF) break;
    }
}

/*
 * Reconoce comentarios de bloque anidados
 */
compLexico _erComentariosTipoNestBlock() {
    char c;
    // Variable auxiliar para detectar fin de comentario
    int primerMas = 0;
    // Variable auxiliar para detectar caracteres escapados
    int primerBarra = 0;
    while (1) {
        // Se lee el siguiente caracter
        c = siguienteChar();
        // Si el char previo era un +, y este una barra, se acepta el comentario
        if (primerMas && c == '/') {
            return _aceptarLexema(ACEPTACIONULTIMOCHAR, COMENTARIO);
        }
        // Si el char previo era una barra y el actual un +, estamos ante un comentario anidado
        if (primerBarra && c == '+') {
            _erNestComment();
        }
        // Se ha leido un +, por lo que puede acabar el comentario
        primerMas = (c == '+');
        // Se comprueba si el char actual es una barra
        primerBarra = (c == '/');
        // Si se alcanza el fin del archivo
        if (c == EOF) return _aceptarLexema(ACEPTACIONPORSIGCHAR, COMENTARIO);
    }
}

compLexico _erToken(char primerToken) {
    char c;
    while (1) {
        // Se lee el siguiente caracter
        c = siguienteChar();
        // Numero decimal que empieza por punto
        if (primerToken == '.' && isdigit(c)) {
            return _erNumero('.');
        }
        // Tokens dobles
        if (primerToken == '+' && c == '=') return _aceptarLexema(ACEPTACIONULTIMOCHAR, COMPLEXICO_MASIGUAL);
        if (primerToken == '+' && c == '+') return _aceptarLexema(ACEPTACIONULTIMOCHAR, COMPLEXICO_MASMAS);
        if (primerToken == '=' && c == '=') return _aceptarLexema(ACEPTACIONULTIMOCHAR, COMPLEXICO_IGUALIGUAL);
        // Comentarios
        if (primerToken == '/') {
            if (c == '*') return _erComentariosTipoBlock();
            if (c == '/') return _erComentariosTipoLinea();
            if (c == '+') return _erComentariosTipoNestBlock();
        }
        // Si no es nada de lo anterior se acepta el token
        return _aceptarLexema(ACEPTACIONPORSIGCHAR, primerToken);
    }
}

compLexico _erString(char delimitadorString) {
    char c;
    int estado = 0;
    while (1) {
        // Se lee el siguiente caracter
        c = siguienteChar();
        // Hay 2 estados en funcion de si se ha leido una barra para escapar caracteres
        switch (estado) {
            case 0:
                // Se comprueba si se leen las comillas que cierran la string
                if (c == delimitadorString) return _aceptarLexema(ACEPTACIONULTIMOCHAR, COMPLEXICO_STRING);
                // Si se lee la barra para escapar caracteres se cambia de estado
                estado = (c == '\\');
                break;
            case 1:
                // Se vuelve al estado anterior, ya que la barra del char anterior ha escapado el actual
                estado = 0;
                break;
        }
        // Si llega el final del fichero
        if (c == EOF) _aceptarLexema(ACEPTACIONPORSIGCHAR, COMPLEXICO_STRING);
    }
}



