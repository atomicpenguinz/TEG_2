#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct nodo {
	int vertice;
	struct nodo *prox;
} Nodo;

typedef struct {
	Nodo **array;
	uint count;
	// uint capacity;
} Grafo, ListaAdjacencia;

// typedef struct {
// 	ArrayList *array;
// 	uint tam;
// } ListaAdjacencia;

/* ls_adj.c */
Grafo *cria_grafo(uint tam);
void add_vertice(Grafo *g, uint elemento);
void free_grafo(Grafo *g);
Nodo *cria_nodo(uint novoVertice);
void add_nodo(Grafo *g, uint novo, uint index);
void free_nodo(Nodo *nodo);
uint is_multigrafo(Grafo *g);
uint grau_maximo(Grafo *g);
uint grau_minimo(Grafo *g);
