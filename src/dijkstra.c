#include "dijkstra.h"

#define INF INT_MAX

static void mostra_caminho(GrafoPalavras *grafo, int *anterior, int origem, int destino) {
    if(destino == origem) {
        printf("%s", grafo->palavras[origem]);
        return;
    }

    if(anterior[destino] == -1) {
        printf("Caminho inexistente");
        return;
    }

    mostra_caminho(grafo, anterior, origem, anterior[destino]);
    printf(" -> %s", grafo->palavras[destino]);
}

static void mostra_arestas(GrafoPalavras *grafo, int *anterior, int origem, int destino) {
    if(destino == origem || anterior[destino] == -1)
        return;

    mostra_arestas(grafo, anterior, origem, anterior[destino]);
    printf("(%s, %s)\n", grafo->palavras[anterior[destino]], grafo->palavras[destino]);
}

void dijkstra(GrafoPalavras *grafo, char *p1, char *p2) {
#ifndef CASE_SENSITIVE
    tolower_string(p1);
    tolower_string(p2);
#endif
    uint origem, destino;

    if(!buscar_hash(grafo->hash, p1, &origem) ||
            !buscar_hash(grafo->hash, p2, &destino)) {
        printf("Uma das palavras nao existe no grafo.\n");
        return;
    }

    int *distancia = malloc(grafo->tamanho * sizeof(int));
    int *anterior = malloc(grafo->tamanho * sizeof(int));
    bool *visit = calloc(grafo->tamanho, sizeof(bool));

    if(!distancia || !anterior || !visit) {
        printf("Erro na alocacao de memoria.\n");
        free(distancia);
        free(anterior);
        free(visit);
        return;
    }

    for(uint i = 0; i < grafo->tamanho; i++) {
        distancia[i] = INF;
        anterior[i] = -1;
    }

    distancia[origem] = 0;

    for(uint i = 0; i < grafo->tamanho; i++) {
        int u = -1;
        int menor = INF;

        for(uint j = 0; j < grafo->tamanho; j++) {
            if(!visit[j] && distancia[j] < menor) {
                menor = distancia[j];
                u = j;
            }
        }

        if(u == -1)
            break;

        if((uint)u == destino)
            break;

        visit[u] = true;

        for(Nodo *atual = grafo->ls_adj->array[u]; atual != NULL; atual = atual->prox) {
            uint v = atual->vertice;
            int peso = atual->peso;

            if(!visit[v] && distancia[u] + peso < distancia[v]) {
                distancia[v] = distancia[u] + peso;
                anterior[v] = u;
            }
        }
    }

    if(distancia[destino] == INF) {
        printf("Nao existe caminho entre %s e %s.\n", p1, p2);
    } else {
        printf("Menor caminho entre %s e %s:\n", p1, p2);
        mostra_caminho(grafo, anterior, origem, destino);

        printf("\n\nQuantidade de arestas: %d\n", distancia[destino]);

        printf("\nSequencia de arestas:\n");
        mostra_arestas(grafo, anterior, origem, destino);
    }

    free(distancia);
    free(anterior);
    free(visit);
}
