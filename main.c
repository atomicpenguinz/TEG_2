#include "headers.h"

#ifndef ARQUIVO
#error "Arquivo não definido."
#endif

int main() {
    ListaAdjacencia *grafo = cria_grafo_csv(ARQUIVO);
    if(!grafo) {
        printf("Saindo...\n");
        return -2;
    }
    printf("Lista de Adjacências criada a partir de \"%s\"\n", ARQUIVO);

    int opt = -1;
    do {
        menu();
        scanf("%d", &opt);

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
            InfoComponentes *info = componentes_conexos(grafo);

            if(!info) {
                printf("Erro na alocação de memória.\n");
                break;
            }

            printf("Número de componentes conexos: %u\n", info->num_componentes);
            
            if(info->num_componentes == 1) 
                printf("O grafo é conexo.\n");
            else 
                printf("O grafo é desconexo.\n");

            printf("\nComponente | Tamanho\n");
            printf("--------------------\n");

            for(uint i = 0; i < info->num_componentes; i++) 
                printf("%u | %u\n", i + 1, info->tamanhos[i]);
            

            if(info->num_componentes > 0) {

                uint maior = info->tamanhos[0];
                uint menor = info->tamanhos[0];

                for(uint i = 1; i < info->num_componentes; i++){

                    if(info->tamanhos[i] > maior)
                        maior = info->tamanhos[i];

                    if(info->tamanhos[i] < menor)
                        menor = info->tamanhos[i];
                }

                printf("\nMaior componente: %u\n", maior);
                printf("\nMenor componente: %u\n", menor);
            }

            free_componentes(info);

            break;
        }
            
        default:
            printf("Opção inválida.\n");
        }
    } while(opt != 0);

    free_grafo(grafo);

    return 1;
}


void menu() {
    printf("Escolha:\n"
           "0 - Sair\n"
           "1 - Determinar grau máximo do grafo\n"
           "2 - Determinar grau mínimo do grafo\n"
           "3 - Determinar se o grafo é simples ou multigrafo\n"
           "4 - Determinar quantos componentes conexos existem no grafo\n"
          );
}
