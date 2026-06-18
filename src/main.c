#include "arquivo.h"

#ifndef ARQUIVO
#error "Arquivo não definido."
#endif

int main() {
    Hash *hash;
    ListaAdjacencia *grafo = cria_grafo_txt(ARQUIVO, &hash);

    // rever toda essa parte
    if(!grafo) {
        printf("Saindo...\n");
        return -2;
    }
    printf("Lista de Adjacências criada a partir de \"%s\"\n", ARQUIVO);

    int opt = -1;
    do {
        menu();
        if(scanf("%d", &opt) != 1)
            opt = -1;

        switch(opt) {
        case 0:
            printf("Saindo...\n");
            break;
        case 1:
            printf("Grau máximo: %u\n", grau_maximo(grafo));
            break;
        case 2:
            printf("Grau mínimo: %u\n", grau_minimo(grafo));
            break;
        case 3: {
            uint *valores = is_multigrafo(grafo);
            if(!valores) {
                printf("Erro na alocação de memória.\n");
                free_grafo(grafo);
                return -3;
            }

            if(!valores[0])
                printf("O grafo analisado é simples.\n");
            else
                printf("O grafo analisado é um multigrafo.\n"
                       "Possui %u laços e %u arestas múltiplas.\n",
                       valores[1], valores[2]);
            free(valores);
            break;
        }
        case 4: {
            PRINT_BUSCA
            InfoComponentes *info = componentes_conexos(grafo);

            if(!info) {
                printf("Erro na alocação de memória.\n");
                return -4;
            }

            printf("Número de componentes conexos: %u\n", info->num_componentes);
            
            if(info->num_componentes == 1) 
                printf("O grafo é conexo.\n");
            else 
                printf("O grafo é desconexo.\n");

            printf("\n Componente | Tamanho\n");
            printf(" --------------------\n");

            for(uint i = 0; i < info->num_componentes; i++) 
                printf("      %-5u | %-8u\n", i + 1, info->tamanhos[i]);
            
            free_componentes(info);

            break;
        }
        case 5:
            printf("Quantidade de vértices: %u\n", grafo->count - 1);  
            break;
        default:
            printf("Opção inválida.\n");
        }
    } while(opt != 0);

    free_grafo(grafo);

    return 0;
}


void menu() {
    printf("Escolha:\n"
           "0 - Sair\n"
           "1 - Determinar grau máximo do grafo\n"
           "2 - Determinar grau mínimo do grafo\n"
           "3 - Determinar se o grafo é simples ou multigrafo\n"
           "4 - Determinar quantos componentes conexos existem no grafo\n"
           "5 - Determinar quantidade de vértices no grafo\n"
          );
}
