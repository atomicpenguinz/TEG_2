#include "arquivo.h"
#include "vendor/fort.h"
#include "dijkstra.h"

void menu();
int informacoes_gerais(GrafoPalavras *grafo);
void vertices_maximos(GrafoPalavras *grafo);
void vertices_minimos(GrafoPalavras *grafo);
#ifndef ARQUIVO
#error "Arquivo não definido."
#endif

int main() {
    GrafoPalavras *grafo = cria_grafo_txt(ARQUIVO);
    if(!grafo) {
        printf("Erro na criação do grafo.\n");
        return -1;
    }
    // rever toda essa parte

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
        case 1: {
            int err = informacoes_gerais(grafo);
            if(!err) return -2;
            
            break;
        }
        case 2:
            vertices_minimos(grafo);
            break;
        case 3: {
            vertices_maximos(grafo);
            break;
        }
        case 4: {
            PRINT_BUSCA
            InfoComponentes *info = componentes_conexos(grafo->ls_adj);

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
            printf("Quantidade de vértices: %u\n", grafo->tamanho);  
            break;
        
        case 6:{
            char p1[5], p2[5];

            printf("Digite a primeira palavra: ");
            scanf("%s", p1);

            printf("Digite a segunda palavra: ");
            scanf("%s", p2);

            dijkstra(grafo, p1, p2);
            break;
        }
        default:
            printf("Opção inválida.\n");
        }
    } while(opt != 0);

    free_grafo(grafo->ls_adj);

    return 0;
}

void menu() {
    printf("Escolha:\n"
           "0 - Sair\n"
           "1 - Informações gerais sobre o grafo\n"
           "2 - Determinar os vértices de grau mínimo do grafo\n"
           "3 - Determinar os vértices de grau máximo do grafo\n"
           "4 - Mostrar componentes conexos\n"
           "5 - Escrever arquivo .DOT (para criação da imagem)\n"
           "6 - Utilizar o algoritmo de Dijkstra\n"
          );
}

int informacoes_gerais(GrafoPalavras *grafo) {
    ft_table_t *table = ft_create_table();
    ft_set_border_style(table, FT_PLAIN_STYLE);

    char buffer[128];
    uint *valores = is_multigrafo(grafo->ls_adj);
    if(!valores) {
        printf("Erro na alocação de memória.\n");
        ft_destroy_table(table);                                                      
        free_grafo(grafo->ls_adj);
        return 0;
    }
    snprintf(buffer, sizeof(buffer), "%u", grafo->tamanho);
    ft_write_ln(table, "Quantidade de vértices", buffer);
    snprintf(buffer, sizeof(buffer), "%u", grau_maximo(grafo->ls_adj));

    ft_add_separator(table);
    ft_write_ln(table, "Grau máximo", buffer);
    snprintf(buffer, sizeof(buffer), "%u", grau_minimo(grafo->ls_adj));
    ft_write_ln(table, "Grau mínimo", buffer);
    ft_add_separator(table);


    ft_add_separator(table);
    if(!valores[0])
        ft_write_ln(table, "É multigrafo?", "Não");
    else
        ft_write_ln(table, "É multigrafo?", "Sim");

    snprintf(buffer, sizeof(buffer), "%u", valores[1]);
    ft_write_ln(table, "Qtd. Laços", buffer);
    snprintf(buffer, sizeof(buffer), "%u", valores[2]);
    ft_write_ln(table, "Qtd. Arestas múltiplas", buffer);
    ft_add_separator(table);
    free(valores);

    InfoComponentes *info = componentes_conexos(grafo->ls_adj);

    if(!info) {
        ft_destroy_table(table);                                                      
        printf("Erro na alocação de memória.\n");
        return 0;
    }
    snprintf(buffer, sizeof(buffer), "%u", info->num_componentes);
    ft_write_ln(table, "Qtd. Componentes conexos", buffer);
    free_componentes(info);

    printf("%s\n", ft_to_string(table));                                          
    ft_destroy_table(table);                                                      

    return 1;
}

void vertices_maximos(GrafoPalavras *grafo) {
    Vertices_e_Grau *vg = vertices_grau_maximo(grafo->ls_adj);
    if(!vg) return;

    printf("Maior grau: %u\n", vg->grau);
    if(vg->qtd == 1)
        printf("Palavra de grau máximo:\n%s\n", grafo->palavras[vg->indices->vertice]);
    else {
        printf("Palavras com o grau mínimo:\n");
        for(Nodo *aux = vg->indices; aux; aux = aux->prox)
            if(aux->prox == NULL)
                printf("%s\n", grafo->palavras[aux->vertice]);
            else
                printf("%s, ", grafo->palavras[aux->vertice]);
    }
}


void vertices_minimos(GrafoPalavras *grafo) {
    Vertices_e_Grau *vg = vertices_grau_minimo(grafo->ls_adj);
    if(!vg) return;

    printf("Menor grau: %u\n", vg->grau);
    if(vg->qtd == 1)
        printf("Palavra de grau mínimo:\n%s\n", grafo->palavras[vg->indices->vertice]);
    else {
        printf("Palavras com o grau mínimo:\n");
        for(Nodo *aux = vg->indices; aux; aux = aux->prox)
            if(aux->prox == NULL)
                printf("%s\n", grafo->palavras[aux->vertice]);
            else
                printf("%s, ", grafo->palavras[aux->vertice]);
    }
}
