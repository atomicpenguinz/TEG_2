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
	// novo->capacity = tam;
	return novo;
}

// void add_vertice(Grafo *g, Nodo *elemento) {
// 	if(g->count >= g->capacity) {
// 		g->capacity = g->capacity == 0 ? 4 : g->capacity * 2;
// 		g->array = realloc(g->array, sizeof(Nodo*) * g->capacity);
// 	}
// 	g->array[g->count++] = elemento;
// }

// void garante_capacidade(Grafo *g, uint elemento) {
// 	uint index = elemento;
// 	if(!g->capacity)
// 		g->capacity = 4;
// 	if(index >= g->capacity) {
// 		uint old_cpct= g->capacity;
// 		while(index >= g->capacity) {
// 			g->capacity *= 2;
// 		}
// 		Nodo **temp = realloc(g->array, sizeof(Nodo*) * g->capacity);
// 		if(!temp) {
// 			free_grafo(g);
// 			return;
// 		}
// 		g->array = temp;
// 		for(uint i = old_cpct; i < g->capacity; i++)
// 			g->array[i] = NULL;
// 	}
// }

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
	g->count =/* g->capacity =*/ 0;
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

// static Nodo *mergesort_list(Nodo *first) {
// 	Nodo *aux = first;
// 	for(Nodo *fim = first; fim != NULL; fim = fim->prox->prox, aux = aux->prox);

// 	Nodo *lista2 = aux->prox;
// 	aux->prox = NULL;
	
// 	returnçNULL;
// ç

// static uiçt duplicados_aux(Nodo *atual, uint acc) { // tail call recursion
// 	if(!atual || !(atual->prox))
// 		return acc;
// 	return (atual->vertice == atual->prox->vertice) ?
// 				duplicados_aux(atual->prox, acc + 1) :
// 				duplicados_aux(atual->prox, acc);
// }


// static Nodo *copia_lista(Nodo *head) {
// 	if(!head)
// 		return NULL;
// 	Nodo *novo = malloc(sizeof(Nodo));
// 	novo->vertice = head->vertice;
// 	novo->prox = copia_lista(head->prox);
// 	return novo;
// }

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

uint is_multigrafo(Grafo *g) {
	uint mArestas = 0;
	uint lacos = 0;
	for(int i = 0; i < g->count; i++)
		mArestas += duplicados_lista(g->array[i], g->count, i, &lacos); 
	return lacos + mArestas / 2;
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

// uint quantos_conexos(Grafo *g) {
// 	int qtd = 0;
// 	for(int i = 0; i < g->count; i++) {
// 		if(g->array[i] != NULL)
// 			qtd++;
// 	}
// 	return qtd;
// }
