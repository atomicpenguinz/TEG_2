#include "grafo.h"
#include "hash.h"

/* arquivo.c */
uint tamanho_arquivo(FILE *f);
Grafo *cria_grafo_txt(char *arquivo, Hash **hash);
void escreve_DOT(Grafo *g, Hash *hash);
