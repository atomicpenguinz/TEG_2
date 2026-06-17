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
    Nodo *aux = head->prox;
    free(head);
    free_lista(aux);
}

void free_grafo(Grafo *g) {
    for(uint i = 0; i < g->count; i++)
        free_lista(g->array[i]);
    free(g->array);
    free(g);
}

Nodo *cria_nodo(uint novoVertice, uint peso) {
    Nodo *novo = malloc(sizeof(Nodo));
    if(!novo) return NULL;

    novo->vertice = novoVertice;
    novo->prox = NULL;
    novo->peso = peso;
    return novo;
}

void add_nodo(Grafo *g, uint novo, uint index, uint peso) {
    Nodo *new = cria_nodo(novo, peso);
    if(!new) return;

    new->prox = g->array[index];
    g->array[index] = new;
}

static uint grau_vertice_aux(Nodo *head, int acc) {
    return (head == NULL) ?
           acc :
           grau_vertice_aux(head->prox, acc + 1);
}

static uint grau_vertice(Nodo *head) {
    return grau_vertice_aux(head, 0);
}

static int compara_uint(const void *a, const void *b) {
    uint x = *(const uint*)a;
    uint y= *(const uint*)b;
    return (x > y) - (x < y);
}

static uint duplicados_lista(Nodo *head, uint index, uint *lacos) {
    uint tam = grau_vertice(head);
    if(!tam) return 0;
    uint *array = malloc(sizeof(uint) * tam);
    if(!array) return 0;

    Nodo *aux = head;
    for(uint i = 0; i < tam; i++, aux = aux->prox)
        array[i] = aux->vertice;

    qsort(array, tam, sizeof(uint), &compara_uint);

    uint arestasMultiplas = 0;
    for(uint i = 0; i < tam; i++) {
        if(array[i] == index)
            (*lacos)++;
        else if(i + 1 < tam && array[i] == array[i+1])
            arestasMultiplas++;
    }

    free(array);
    return arestasMultiplas;
}

uint *is_multigrafo(Grafo *g) {
    uint mArestas = 0;
    uint lacos = 0;
    for(uint i = 0; i < g->count; i++)
        mArestas += duplicados_lista(g->array[i], i, &lacos);
    mArestas /= 2;

    uint *valores = malloc(3 * sizeof(uint));
    if(valores) {
        valores[0] = lacos + mArestas;
        valores[1] = lacos;
        valores[2] = mArestas;
    }
    return valores;
}

uint grau_maximo(Grafo *g) {
    if(g->count <= 1) return 0;
    uint maior = grau_vertice(g->array[1]);
    for(uint i = 2; i < g->count; i++) {
        uint grau = grau_vertice(g->array[i]);
#ifdef DEBUG
        if(grau >= 3) printf("vertice %u grau %u\n", i, grau);
#endif
        if(maior < grau)
            maior = grau;
    }
    return maior;
}

uint grau_minimo(Grafo *g) {
    if(g->count <= 1) return 0;
    uint menor = grau_vertice(g->array[1]);
    for(uint i = 2; i < g->count; i++) {
        uint grau = grau_vertice(g->array[i]);
        if(menor > grau)
            menor = grau;
    }
    return menor;
}

static uint busca_profundidade(Grafo *g, uint raiz, bool* visitados) {
    visitados[raiz] = 1;
    Nodo *aux = g->array[raiz];
    uint acc = 1;
    while(aux) {
        if(!visitados[aux->vertice])
            acc += busca_profundidade(g, aux->vertice, visitados);
        aux = aux->prox;
    }
    return acc;
}

static uint busca_largura(Grafo *g, uint inicio, bool *visitado) {

    uint *fila = malloc(g->count * sizeof(uint));
    if(!fila) return 0;

    uint frente = 0;
    uint fim = 0;
    uint tamanho = 0;

    visitado[inicio] = true;
    fila[fim++] = inicio;

    while(frente < fim) {

        uint atual = fila[frente++];
        tamanho++;

        for(Nodo *aux = g->array[atual];
            aux != NULL;
            aux = aux->prox) {

            uint vizinho = aux->vertice;

            if(!visitado[vizinho]) {
                visitado[vizinho] = true;
                fila[fim++] = vizinho;
            }
        }
    }

    free(fila);

    return tamanho;
}

//aqui determina os componentes conexos usando DFS ou BFS
InfoComponentes *componentes_conexos(Grafo *g) {
    bool *visitado = calloc(g->count, sizeof(bool));
    if(!visitado) return NULL;

    uint *tmp = malloc(g->count * sizeof(uint));
    if(!tmp) {
        free(visitado);
        return NULL;
    }

    uint num = 0;

    for(uint i = 0; i < g->count; i++) {
        if(!visitado[i]) {
            tmp[num] = busca(g, i, visitado);
            num++;
        }
    }

    InfoComponentes *info = malloc(sizeof(InfoComponentes));
    if(!info) {
        free(visitado);
        free(tmp);
        return NULL;
    }

    info->num_componentes = num;
    info->tamanhos = malloc(num * sizeof(uint));
    if(!info->tamanhos) {
        free(visitado);
        free(tmp);
        free(info);
        return NULL;
    }

    for(uint i = 0; i < num; i++)
        info->tamanhos[i] = tmp[i];

    free(visitado);
    free(tmp);

    return info;
}

void free_componentes(InfoComponentes *info) {

    free(info->tamanhos);
    free(info);
}

