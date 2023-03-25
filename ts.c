#include "ts.h"
#include "definiciones.h"

// Variable de la tabla de simbolos
TablaHash ts;

/*
 * Funcion para inicializar la tabla de simbolos
 */
void inicializarTS() {
    InicializarTablaHash(ts);
    compLexico keywords[] = {
            {CAST, "cast"},
            { DOUBLE, "double" },
            { ENFORCE, "enforce" },
            { FOREACH, "foreach" },
            { IMPORT, "import" },
            { INT, "int" },
            { RETURN, "return" },
            { VOID, "void" },
            { WHILE, "while" }
    };

    // Se añaden en la tabla de simbolos, reservando memoria para cada palabra dinamicamente
    for (int i = 0; i < sizeof(keywords) / sizeof(compLexico); i++) {
        char * keyword = keywords[i].lexema;
        int lexLen = (strlen(keyword)+1)*sizeof(char);
        // Se reserva memoria
        char * lexema = (char *) malloc(lexLen);
        // Se copia de la variable estatica a la dinamica
        strncpy(lexema, keyword, lexLen);
        insertarTS(lexema, keywords[i].comp);
    }

}

/*
 * Funcion para buscar obtener el componente lexico a partir del lexema en la tabla de simbolos
 */
int buscarTS(compLexico *comp) {
    return Busqueda(ts, comp->lexema, &(comp->comp));
}

/*
 * Funcion para inserta en tabla de simbolos un lexema con un componente lexico asociado
 */
void insertarTS(char *lexema, int comp) {
    compLexico elem;
    elem.lexema = lexema;
    elem.comp = comp;
    InsertarHash(&ts, elem);
}

/*
 * Funcion para finalizar la tabla de simbolos
 */
void finalizarTS() {
    DestruirTablaHash(ts);
}

