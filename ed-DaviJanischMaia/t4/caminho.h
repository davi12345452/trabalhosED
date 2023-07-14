#ifndef CAMINHO_H_
#define CAMINHO_H_

#include <stdio.h>
#include "grafo.h"
#include "local.h"

// Função que calcula a distância entre vértices, atribuindo como peso da aresta
double calcula_distancia(double latitude1, double longitude1, double latitude2, double longitude2);

// Função que recebe o arquivo com as arestas, criando um grafo, usando os índices atribuídos em local
Grafo caminho_cria(char *arquivo, Local locais);

#endif