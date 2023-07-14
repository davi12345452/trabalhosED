#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include <stdbool.h>
#include "grafo.h"
#include "local.h"

// Definição da estrutura Visita, usada para armazenar informações sobre uma visita ou caminho encontrado pelo algoritmo de Dijkstra
typedef struct _visita *Visita;

// Função auxiliar para encontrar o vértice com a menor distância não visitada
int encontrarMenorDistancia(double *distancias, bool *visitados, int n_vertices);

// Algoritmo de Dijkstra que encontra o caminho mais curto de um vértice de origem para um vértice de destino
Visita dijkstra(Grafo grafo, int origem, int destino);

// Função para imprimir o caminho contido em uma estrutura Visita
void _imprimirCaminho(Visita visita);

// Versão adaptada do algoritmo de Dijkstra que aplica o Dijkstra a partir de um array de vértices
Visita dijkstraAdaptado(Grafo grafo, int *ids, int n_ids);

// Função que permite acessar certas informações da estrutura sem quebrar o TAD
void info_visita(Visita visita, int **caminho, int *tam);
#endif