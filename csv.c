#include "headers.h"

void SSV_para_CSV(char *arquivo) {
	char comando[100];
	snprintf(comando, sizeof(comando),
				"tr ' ' ',' < %s > file.tmp.csv;"
	       		"mv --force file.tmp.csv %s", arquivo, arquivo);
	system(comando);
}

uint maior_indice(FILE *f) {
	char line[32];
	uint a, b;
	uint maior = 0;
	while(fgets(line, sizeof(line), f)) {
		if(sscanf(line, "%u,%u", &a, &b) != 2)
			continue;
		uint pretendente = a > b ? a : b;
		maior = maior > pretendente ? maior : pretendente;
	}
	return maior;
}

Grafo *cria_grafo_csv(char *arquivo) {
	FILE *file = fopen(arquivo, "r");
	if(file == NULL) {
		printf("Arquivo %s não encontrado.\n", arquivo); // mensagem nao ficará aqui
		return NULL;
	}
	Grafo *g = cria_grafo(maior_indice(file));
	if(!g){
		printf("Erro na alocação de memória.\n");
		return NULL;
	}

	rewind(file);
	char line[50];
	while(fgets(line, sizeof(line), file)) {
		uint a, b;
		sscanf(line, "%u,%u", &a, &b);
		add_nodo(g, a, b);
		if(a != b)
			add_nodo(g, b, a);
	}
	fclose(file);
	return g;
}
