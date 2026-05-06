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

static uint duplicados_lista_old(Nodo *head, uint vertices, uint index, uint *lacos) {
    // Versão antiga da função duplicados_lista, que usa
    // um vetor de frequência para contar duplicados
    // planejo falar sobre isso no relatório
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

static uint grau_vertice_aux(Nodo *head, int acc) {
    return (head == NULL) ?
           acc :
           grau_vertice_aux(head->prox, acc + 1);
}

static uint grau_vertice(Nodo *head) {
    return grau_vertice_aux(head, 0);
}

static int compara_uint(const void *a, const void *b) {
    uint x = *(const int*)a;
    uint y= *(const int*)b;
    return (x > y) - (x < y);
}

static uint duplicados_lista(Nodo *head, uint index, uint *lacos) {
    uint tam = grau_vertice(head);
    uint *array = malloc(sizeof(uint) * tam);
    if(!array) return 0;

    Nodo *aux = head;
    for(uint i = 0; i < tam; i++, aux = aux->prox)
        array[i] = aux->vertice;

    qsort(array, tam, sizeof(uint), &compara_uint);

    uint arestasMultiplas = 0;
    for(int i = 0; i < tam; i++) {
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
    for(int i = 0; i < g->count; i++)
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
    if(g->count == 0) return 0;
    uint maior = 0;
    for(uint i = 1; i < g->count; i++) {
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
    if(g->count == 0) return 0;
    uint menor = 0;
    for(uint i = 1; i < g->count; i++) {
        uint grau = grau_vertice(g->array[i]);
        if(menor > grau)
            menor = grau;
    }
    return menor;
}

//Letra "e" add a BFS para componentes conexos
typedef struct {
    uint *dados;
    uint frente;
    uint cauda;
    uint cap;
} Fila;


//estrutura de fila 
static Fila *cria_fila(uint cap) {
    Fila *F = malloc(sizeof(Fila));
    if(!F) 
     return NULL;

    F->dados = malloc(cap * sizeof(uint));
    if(!F->dados) {
        free(F);
        return NULL;
    }

    F->frente = 0;
    F->cauda = 0;
    F->cap = cap;

    return F;
}

static void insere(Fila *F, uint v) {
    F->dados[F->cauda++] = v;
}

static uint frente(Fila *F) {
    return F->dados[F->frente];
}

static void remover(Fila *F) {
    F->frente++;
}

static int vazia(Fila *F) {
    return F->frente == F->cauda;
}

static void destroi_fila(Fila *F) {
    if(!F) return;

    free(F->dados);
    free(F);
}


static uint BFS(Grafo *g, uint inicio, bool *visitado) {

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

//aqui determina os componentes conexos usando a BFS 
InfoComponentes *componentes_conexos(Grafo *g) {

    bool *visitado = calloc(g->count, sizeof(bool));
    if(!visitado) return NULL;

    uint *tmp = malloc(g->count * sizeof(uint));
    if(!tmp) {
        free(visitado);
        return NULL;
    }

    uint num = 0;

    for(uint i = 1; i < g->count; i++) {

        if(g->array[i] != NULL && !visitado[i]) {

            tmp[num] = BFS(g, i, visitado);
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

