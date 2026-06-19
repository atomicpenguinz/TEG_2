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
} ListaAdjacencia;


typedef struct {
    Nodo *indices;
    uint qtd;
    uint grau;
} Vertices_e_Grau;

typedef struct {
    uint num_componentes;
    uint *tamanhos;
} InfoComponentes;

/* ls_adj.c */
ListaAdjacencia *cria_grafo(uint tam);
void free_grafo(ListaAdjacencia *g);
Nodo *cria_nodo(uint novoVertice, uint peso);
void add_nodo(ListaAdjacencia *g, uint novo, uint index, uint peso);
uint *is_multigrafo(ListaAdjacencia *g);
uint grau_maximo(ListaAdjacencia *g);
Vertices_e_Grau *vertices_grau_maximo(ListaAdjacencia *g);
uint grau_minimo(ListaAdjacencia *g);
Vertices_e_Grau *vertices_grau_minimo(ListaAdjacencia *g);
InfoComponentes *componentes_conexos(ListaAdjacencia *g);
void free_componentes(InfoComponentes *info);
