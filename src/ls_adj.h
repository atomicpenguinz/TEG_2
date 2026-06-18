#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int uint; // aparentemente necessário no Windows/VSCode (?)

#ifdef BFS
    #define busca busca_largura
    #define PRINT_BUSCA printf("Busca em largura:\n");
#else
    #define busca busca_profundidade
    #define PRINT_BUSCA printf("Busca em profundidade:\n");
#endif

typedef struct nodo {
    uint vertice;
    uint peso;
    struct nodo *prox;
} Nodo;

typedef struct {
    Nodo **array;
    uint count;
} Grafo, ListaAdjacencia;


typedef struct {
    uint indice;
    uint grau;
} VerticeEGrau;

typedef struct {
    uint num_componentes;
    uint *tamanhos;
} InfoComponentes;

/* ls_adj.c */
Grafo *cria_grafo(uint tam);
void add_vertice(Grafo *g, uint elemento);
void free_grafo(Grafo *g);
Nodo *cria_nodo(uint novoVertice, uint peso);
void add_nodo(Grafo *g, uint novo, uint index, uint peso);
void free_nodo(Nodo *nodo);
uint *is_multigrafo(Grafo *g);
uint grau_maximo(Grafo *g);
uint grau_minimo(Grafo *g);
InfoComponentes *componentes_conexos(Grafo *g);
void free_componentes(InfoComponentes *info);

/* main.c */
int main(void);
void menu(void);
