#!/usr/bin/env bash

# comando padrão para compilar o código.
# Mais instruções no relatório
gcc ./vendor/fort.c ls_adj.c hash.c arquivo.c main.c -O2 -o grafo $1
