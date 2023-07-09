#ifndef SAIDA_H
#define SAIDA_H

#include "grafo.h"
#include "local.h"

typedef struct {
  double peso;
  int *caminho;
  int tamanho;
} Visita;

int encontrarMenorDistancia(double *distancias, bool *visitados,
                            int n_vertices);

Visita dijkstra(Grafo grafo, int origem, int destino);

void imprimirCaminho(Visita visita);

// adicionada a função gerarCaminhos
Visita dijkstraAdaptado(Grafo grafo, int *ids, int n_ids);

#endif /* SAIDA_H */