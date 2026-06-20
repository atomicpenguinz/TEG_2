#include "ls_adj.h"

ListaAdjacencia *cria_grafo(uint tam) {
    ListaAdjacencia *novo = malloc(sizeof(ListaAdjacencia));
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

void free_grafo(ListaAdjacencia *g) {
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

void add_nodo(ListaAdjacencia *g, uint novo, uint index, uint peso) {
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

uint *is_multigrafo(ListaAdjacencia *g) {
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

uint grau_maximo(ListaAdjacencia *g) {
    if(g->count <= 1) return 0;
    uint maior = grau_vertice(g->array[0]);
    for(uint i = 1; i < g->count; i++) {
        uint grau = grau_vertice(g->array[i]);
        if(maior < grau)
            maior = grau;
    }
    return maior;
}

Vertices_e_Grau *vertices_grau_maximo(ListaAdjacencia *g) {
    if(g->count <= 1) return NULL;
    Vertices_e_Grau *maiores = malloc(sizeof(Vertices_e_Grau));
    if(!maiores) return NULL;
    maiores->indices = NULL;
    maiores->grau = grau_maximo(g);

    maiores->qtd = 0;
    for(uint i = 0; i < g->count; i++) {
        if(grau_vertice(g->array[i]) == maiores->grau) {
            Nodo *novo = malloc(sizeof(Nodo));
            if(!novo) return NULL;
            novo->vertice = i;
            novo->prox = maiores->indices;
            maiores->indices = novo;
            maiores->qtd++;
        }
    }
    return maiores;
}

uint grau_minimo(ListaAdjacencia *g) {
    if(g->count <= 1) return 0;
    uint menor = grau_vertice(g->array[0]);
    for(uint i = 1; i < g->count; i++) {
        uint grau = grau_vertice(g->array[i]);
        if(menor > grau)
            menor = grau;
    }
    return menor;
}

Vertices_e_Grau *vertices_grau_minimo(ListaAdjacencia *g) {
    if(g->count <= 1) return NULL;
    Vertices_e_Grau *menores = malloc(sizeof(Vertices_e_Grau));
    if(!menores) return NULL;
    menores->indices = NULL;
    menores->grau = grau_minimo(g);

    menores->qtd = 0;
    for(uint i = 0; i < g->count; i++) {
        if(grau_vertice(g->array[i]) == menores->grau) {
            Nodo *novo = malloc(sizeof(Nodo));
            if(!novo) return NULL;
            novo->vertice = i;
            novo->prox = menores->indices;
            menores->indices = novo;
            menores->qtd++;
        }
    }
    return menores;
}

static uint busca_profundidade(ListaAdjacencia *g, uint raiz, bool* visitados) {
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

// DFS
static void busca_stats(ListaAdjacencia *g, uint raiz, bool *visitados, EstComponentes *comp) {
    visitados[raiz] = 1;
    comp->tamanho++;

    uint grau = grau_vertice(g->array[raiz]);
    if(grau > comp->grau_maior) {
        comp->grau_maior = grau;
        comp->vertice_maior = raiz;
    }

    if(grau < comp->grau_menor) {
        comp->grau_menor = grau;
        comp->vertice_menor = raiz;
    }

    Nodo *aux = g->array[raiz];
    while(aux) {
        if(!visitados[aux->vertice])
            busca_stats(g, aux->vertice, visitados, comp);
        aux = aux->prox;
    }
}

static uint busca_largura(ListaAdjacencia *g, uint inicio, bool *visitado) {

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
InfoComponentes *componentes_conexos(ListaAdjacencia *g) {
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

EstComponentes *estatisticas_componentes(ListaAdjacencia *g, uint *num_comp) {
    bool *visitados = calloc(g->count, sizeof(bool));
    if(!visitados) return NULL;

    EstComponentes *tmp = malloc(g->count * sizeof(EstComponentes));
    if(!tmp) {
        free(visitados);
        return NULL;
    }

    uint num = 0;
    for(uint i = 0; i < g->count; i++) {
        if(!visitados[i]) {
            EstComponentes *comp = malloc(sizeof(EstComponentes));
            if(!comp) {
                free(visitados);
                free(tmp);
                return NULL;
            }
            comp->tamanho = 0;
            comp->grau_maior = 0;
            comp->vertice_maior = i;
            comp->grau_menor = UINT_MAX;
            comp->vertice_menor = i;
            busca_stats(g, i, visitados, comp);
            tmp[num] = *comp;
            num++;
            free(comp);
        }
    }
    EstComponentes *ret = malloc(num * sizeof(EstComponentes));
    if(!ret) {
        free(tmp);
        free(visitados);
        return NULL;
    }

    for(uint i = 0; i < num; i++)
        ret[i] = tmp[i];

    free(tmp);
    free(visitados);
    *num_comp = num;
    return ret;
}
