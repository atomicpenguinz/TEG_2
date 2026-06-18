#include "arquivo.h"
#include <ctype.h>

static uint tamanho_arquivo(FILE *f) {
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

static GrafoPalavras *cria_grafo_palavra(uint tam) {
    GrafoPalavras *gp = malloc(sizeof(GrafoPalavras));
    if(!gp) return NULL;
    gp->palavras = NULL;
    gp->hash = NULL;
    gp->ls_adj = NULL;
    gp->tamanho = tam;
    return gp;
}

static void free_grafo_palavra(GrafoPalavras *gp) {
    free(gp->palavras);
    liberar_hash(gp->hash);    
    free_grafo(gp->ls_adj);
    free(gp);
}

GrafoPalavras *cria_grafo_txt(char *arquivo) {
    FILE *file = fopen(arquivo, "r");
    if(file == NULL) {
        printf("Arquivo %s não encontrado.\n", arquivo);
        return NULL;
    }

    uint tam_arq = tamanho_arquivo(file);
    GrafoPalavras *gp = cria_grafo_palavra(tam_arq);
    if(!gp) {
        printf("Erro na alocação de memória.\n");
        fclose(file);
        return NULL;
    }
    gp->ls_adj = cria_grafo(tam_arq + 1);
    if(!gp->ls_adj) {
        printf("Erro na alocação de memória.\n");
        free_grafo_palavra(gp);
        fclose(file);
        return NULL;
    }

    char line[32];
    gp->palavras = malloc(tam_arq * sizeof(*gp->palavras));
    if(!gp->palavras) {
        fclose(file);
        free_grafo(gp->ls_adj);
        free_grafo_palavra(gp);
        return NULL;
    }

    gp->hash = criar_hash(tam_arq +1);
    if(!gp->hash) {
        fclose(file);
        free(gp->palavras);
        free_grafo(gp->ls_adj);
        return NULL;
    }

    uint i = 0;
    while(fgets(line, sizeof(line), file)) {
        if(sscanf(line, "%s", (gp->palavras)[i]) ==  1) {
            tolower_string((gp->palavras)[i]);
            adicionar_hash(gp->hash, (gp->palavras)[i], i);
            i++;
        }
    }
    uint n = i;

    for(uint i = 0; i < n; i++) { // talvez seja possível otimizar essa parte
        for(uint j = i + 1; j < n; j++) {
            if(divergem_por_um((gp->palavras)[i], (gp->palavras)[j], TAM_PALAVRA)) {
                add_nodo(gp->ls_adj, i, j, 1);
                if(strcmp((gp->palavras)[i], (gp->palavras)[j]) != 0) 
                    add_nodo(gp->ls_adj, j, i, 1);
            }
        }
    }
    fclose(file);
    return gp;
}

void escreve_DOT(Grafo *g, Hash *hash) {
    // WIP
}
