#include "hash.h"

Hash *criar_hash(uint tam) {
    Hash *hash = malloc(sizeof(Hash));
    if(!hash) return NULL;

    hash->elementos = calloc(tam, sizeof(NoHash*));
    if(!hash->elementos) {
        free(hash);
        return NULL;
    }

    hash->tamanho = tam;
    hash->total = 0;

    return hash;
}

void liberar_hash(Hash *hash) {
    if(!hash) return;

    for(uint i = 0; i < hash->tamanho; i++) {
        NoHash *atual = hash->elementos[i];
        while(atual) {
            NoHash *aux = atual->proximo;
            free(atual->entrada);
            free(atual);
            atual = aux;
        }
    }
    free(hash->elementos);
    free(hash);
}

static uint hashing_djb2(const char *s, uint tam) {
    uint h = 5381;

    while(*s)
        h = ((h << 5) + h) + (unsigned char)*s++;

    return h % tam;
}

void adicionar_hash(Hash *hash, const char *palavra, uint indice_grafo) {
    if(!hash) return;
    if (strlen(palavra) != (TAM_PALAVRA - 1)) return;

    uint indice = hashing_djb2(palavra, hash->tamanho);
    EntradaHash *entrada = malloc(sizeof(EntradaHash));
    if(!entrada) return;

    entrada->indice = indice_grafo;

    strncpy(entrada->valor, palavra, TAM_PALAVRA);

    NoHash *no = malloc(sizeof(NoHash));
    if(!no) {
        free(entrada);
        return;
    }

    no->proximo = hash->elementos[indice];
    no->entrada = entrada;
    hash->elementos[indice] = no;
    hash->total++;

}

bool buscar_hash(Hash *hash, const char *palavra, uint *indice) {
    if (!hash || !palavra || !indice)
        return false;

    uint pos = hashing_djb2(palavra, hash->tamanho);

    for (NoHash *atual = hash->elementos[pos];
            atual;
            atual = atual->proximo)
    {
        if (strcmp(atual->entrada->valor, palavra) == 0) {
            *indice = atual->entrada->indice;
            return true;
        }
    }

    return false;
}
