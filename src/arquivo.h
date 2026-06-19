#include "ls_adj.h"
#include "hash.h"

#define ARQUIVO "palavras.txt"

#define ARQUIVO_DOT "../scripts/grafo.dot"
#define CONFIG_DOT { \
    "normalize=true;", \
    "dim=9;", \
    "beautify=true;" \
}

#ifdef CASE_SENSISITIVE
#define ARQUIVO_DOT "../scripts/case_grafo.dot"
#endif

typedef struct {
    ListaAdjacencia *ls_adj;
    Hash *hash;
    char (*palavras)[TAM_PALAVRA];
    uint tamanho;
} GrafoPalavras;

/* arquivo.c */
GrafoPalavras *cria_grafo_txt(char *arquivo);
void escreve_DOT(char *arquivo, GrafoPalavras *gp);
