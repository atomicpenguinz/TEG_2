#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef unsigned int uint;
#define TAM_PALAVRA 5

typedef struct {
    uint indice;
    char valor[TAM_PALAVRA];
} EntradaHash;

typedef struct no_hash {
    EntradaHash *entrada;
    struct no_hash *proximo;
} NoHash;

typedef struct {
    uint total;
    uint tamanho;
    NoHash** elementos;
} Hash;

/* hash.c */
Hash *criar_hash(uint tam);
void liberar_hash(Hash *hash);
void adicionar_hash(Hash *hash, const char *palavra, uint indice_grafo);
bool buscar_hash(Hash *hash, const char *palavra, uint *indice);
