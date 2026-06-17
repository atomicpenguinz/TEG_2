#include "hash.h"
#include "grafo.h"
#include <ctype.h>

uint tamanho_arquivo(FILE *f) {
    char line[32];
    uint linhas = 0;
    while(fgets(line, sizeof(line), f))
        linhas++;

    rewind(f);
    return linhas;
}

static void tolower_string(char *str) {
    for(int i = 0; str[i] != '\0'; i++)
        str[i] = tolower(str[i]);
}

static inline bool divergem_por_um(char *a, char *b, int tam) {
    int diff = 0;
    for(int i = 0; a[i] != '\0' || b[i] != '\0'; i++) {
        if(diff > 1)
            return false;
        if(a[i] != b[i])
            diff++;
    }
    return diff == 1;
}

Grafo *cria_grafo_txt(char *arquivo, Hash **hash) {
    FILE *file = fopen(arquivo, "r");
    if(file == NULL) {
        printf("Arquivo %s não encontrado.\n", arquivo);
        return NULL;
    }

    uint tam_arq = tamanho_arquivo(file);
    Grafo *g = cria_grafo(tam_arq + 1);
    if(!g) {
        printf("Erro na alocação de memória.\n");
        return NULL;
    }

    char line[32];
    char (*vetor)[TAM_PALAVRA] = malloc(tam_arq * sizeof(*vetor));
    *hash = criar_hash(tam_arq +1);
    if(!(*hash) || !vetor) return NULL;

    uint i = 0;
    while(fgets(line, sizeof(line), file)) {
        if(sscanf(line, "%s", vetor[i]) ==  1) {
            tolower_string(vetor[i]);
            adicionar_hash(*hash, vetor[i], i);
            i++;
        }
    }
    uint n = i;

    for(uint i = 0; i < n; i++) { // talvez seja possível otimizar essa parte
        for(uint j = i + 1; j < tam_arq; j++) {
            if(divergem_por_um(vetor[i], vetor[j], TAM_PALAVRA)) {
                add_nodo(g, i, j, 1);
                if(strcmp(vetor[i], vetor[j]) != 0) 
                    add_nodo(g, j, i, 1);
            }
        }
    }
    free(vetor);
    fclose(file);
    return g;
}

void escreve_DOT(Grafo *g, Hash *hash) {
    // WIP
}
