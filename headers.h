#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned int uint;
#define ARQUIVO "teste2.csv"

typedef struct nodo {
    int vertice;
    struct nodo *prox;
} Nodo;

typedef struct {
    Nodo **array;
    uint count;
} Grafo, ListaAdjacencia;

typedef struct {
    uint num_componentes;
    uint *tamanhos;
} InfoComponentes;

/* ls_adj.c */
Grafo *cria_grafo(uint tam);
void add_vertice(Grafo *g, uint elemento);
void free_grafo(Grafo *g);
Nodo *cria_nodo(uint novoVertice);
void add_nodo(Grafo *g, uint novo, uint index);
void free_nodo(Nodo *nodo);
uint *is_multigrafo(Grafo *g);
uint grau_maximo(Grafo *g);
uint grau_minimo(Grafo *g);
InfoComponentes *componentes_conexos(Grafo *g);
void free_componentes(InfoComponentes *info);

/* csv.c */
void SSV_para_CSV(char *arquivo);
uint maior_indice(FILE *f);
Grafo *cria_grafo_csv(char *arquivo);

/* main.c */
int main(void);
void menu(void);
