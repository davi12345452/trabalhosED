#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "grafo.h"
#include "local.h"

// Definição da estrutura Visita, usada para armazenar informações sobre uma visita ou caminho encontrado pelo algoritmo de Dijkstra
typedef struct {
  double peso;
  int *caminho;
  int tamanho;
} Visita;

// Função auxiliar para encontrar o vértice com a menor distância não visitada
int encontrarMenorDistancia(double *distancias, bool *visitados, int n_vertices);

// Algoritmo de Dijkstra que encontra o caminho mais curto de um vértice de origem para um vértice de destino
Visita dijkstra(Grafo grafo, int origem, int destino);

// Função para imprimir o caminho contido em uma estrutura Visita
void imprimirCaminho(Visita visita);

// Versão adaptada do algoritmo de Dijkstra que aplica o Dijkstra a partir de um array de vértices
Visita dijkstraAdaptado(Grafo grafo, int *ids, int n_ids);

#endif