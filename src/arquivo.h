#include "ls_adj.h"
#include "hash.h"

#define ARQUIVO "palavras.txt"

typedef struct {
    ListaAdjacencia *ls_adj;
    Hash *hash;
    char (*palavras)[TAM_PALAVRA];
    uint tamanho;
} GrafoPalavras;

/* arquivo.c */
GrafoPalavras *cria_grafo_txt(char *arquivo);
void escreve_DOT(Grafo *g, Hash *hash);
