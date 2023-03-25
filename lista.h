typedef struct compLexico{
    int comp;
    char * lexema;
} compLexico;

struct celda {
    compLexico elemento;
    struct celda *sig;
};

typedef struct celda *posicion;

struct l {
    posicion inicio;
    unsigned longitud;
    posicion fin;
};

typedef struct l *lista;

void crea(lista *l);

void destruye(lista *l);

posicion primero(lista l);

posicion siguiente(lista l, posicion p);

posicion fin(lista l);

posicion anterior(lista l, posicion p);

unsigned existe(lista l);

unsigned esvacia(lista l);

void recupera(lista l, posicion p, compLexico *e);

unsigned longitud(lista l);

void inserta(lista *l, posicion p, compLexico e);

void suprime(lista *l, posicion p);

void modifica(lista *l, posicion p, compLexico e);

posicion posinser(lista l, compLexico e);
