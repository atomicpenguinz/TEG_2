#include "headers.h"

Grafo *cria_grafo(uint tam) {
	Grafo *novo = malloc(sizeof(Grafo));
	if(novo == NULL) return NULL;

	novo->array = calloc(tam, sizeof(Nodo*)); // todos começam como NULL
	if(novo->array == NULL) {
		free(novo);
		return NULL;
	}

	novo->count = tam;
	return novo;
}

static void free_lista(Nodo *head) {
	if(!head) return;
	free_lista(head->prox);
	free(head);	
}

void free_grafo(Grafo *g) {
	for(uint i = 0; i < g->count; i++)
		free_lista(g->array[i]);
	free(g->array);
	g->array = NULL;
	g->count = 0;
}

Nodo *cria_nodo(uint novoVertice) {
	Nodo *novo = malloc(sizeof(Nodo));
	if(!novo) return NULL;
	
	novo->vertice = novoVertice;
	novo->prox = NULL;
	return novo;
}

void add_nodo(Grafo *g, uint novo, uint index) {
	Nodo *new = cria_nodo(novo);
	if(!new) return;
	
	new->prox = g->array[index];
	g->array[index] = new;
}

static uint duplicados_lista(Nodo *head, uint vertices, uint index, uint *lacos) {
	uint *freq = calloc(vertices, sizeof(uint));
	if(!freq) return 0;

	Nodo *aux = head;

	uint acc = 0;
	for(; aux != NULL; aux = aux->prox) {
		if(aux->vertice == index)
			(*lacos)++;
		else
			freq[aux->vertice]++;
	}
	
	for(uint i = 0; i < vertices; i++)
		if(freq[i] > 1)
			acc += freq[i] -1;

	free(freq);
	return acc;
}

uint *is_multigrafo(Grafo *g) {
	uint mArestas = 0;
	uint lacos = 0;
	for(int i = 0; i < g->count; i++)
		mArestas += duplicados_lista(g->array[i], g->count, i, &lacos); 
	mArestas /= 2;
	uint *valores = malloc(3 * sizeof(uint));
	if(valores){
		valores[0] = lacos + mArestas;
		valores[1] = lacos;
		valores[2] = mArestas;
	}
	return valores;
}

static uint grau_vertice_aux(Nodo *head, int acc) {
	return (head->prox == NULL) ?
				acc :
				grau_vertice_aux(head->prox, acc + 1);
}

static uint grau_vertice(Nodo *head) {
	return grau_vertice_aux(head, 0);
}

uint grau_maximo(Grafo *g) {
	if(g->count == 0) return 0;
	uint maior = 0;
	for(uint i = 1; i < g->count; i++) {
		uint grau = grau_vertice(g->array[i]);
		if(maior < grau) 
			maior = grau;
	}
	return maior;
}

uint grau_minimo(Grafo *g) {
	if(g->count == 0) return 0;
	uint menor = grau_vertice(g->array[0]);
	for(uint i = 1; i < g->count; i++) {
		uint grau = grau_vertice(g->array[i]);
		if(menor > grau)
			menor = grau;
	}
	return menor;
}

