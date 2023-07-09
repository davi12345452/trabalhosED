#include "saida.h"
#include "local.h"
#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int encontrarMenorDistancia(double *distancias, bool *visitados,
                            int n_vertices) {
  double min = DBL_MAX;
  int indice_min;

  for (int i = 0; i < n_vertices; i++) {
    if (!visitados[i] && distancias[i] < min) {
      min = distancias[i];
      indice_min = i;
    }
  }

  return indice_min;
}

void inicializarEstruturasAuxiliares(int n_vertices, double *distancias,
                                     bool *visitados, int *anteriores) {
  for (int i = 0; i < n_vertices; i++) {
    distancias[i] = DBL_MAX;
    visitados[i] = false;
    anteriores[i] = -1;
  }
}

void executarDijkstra(Grafo grafo, double *distancias, bool *visitados,
                      int *anteriores, int origem, int n_vertices) {
  distancias[origem] = 0;

  for (int count = 0; count < n_vertices - 1; count++) {
    int u = encontrarMenorDistancia(distancias, visitados, n_vertices);
    visitados[u] = true;

    for (int v = 0; v < n_vertices; v++) {
      if (!visitados[v] && grafo->matriz_adj[u][v] != NULL) {
        double peso = grafo->matriz_adj[u][v]->peso;

        if (distancias[u] != DBL_MAX && distancias[u] + peso < distancias[v]) {
          distancias[v] = distancias[u] + peso;
          anteriores[v] = u;
        }
      }
    }
  }
}

void construirVisita(int *caminho, int tamanho_caminho, int destino,
                     double *distancias, int *anteriores, Visita *visita) {
  int atual = destino;
  while (atual != -1) {
    caminho[tamanho_caminho] = atual;
    tamanho_caminho++;
    atual = anteriores[atual];
  }

  for (int i = 0; i < tamanho_caminho / 2; i++) {
    int temp = caminho[i];
    caminho[i] = caminho[tamanho_caminho - i - 1];
    caminho[tamanho_caminho - i - 1] = temp;
  }

  visita->peso = distancias[destino];
  visita->caminho = caminho;
  visita->tamanho = tamanho_caminho;
}

Visita dijkstra(Grafo grafo, int origem, int destino) {
  int n_vertices = grafo->n_vertices;
  bool *visitados = malloc(n_vertices * sizeof(bool));
  double *distancias = malloc(n_vertices * sizeof(double));
  int *anteriores = malloc(n_vertices * sizeof(int));

  inicializarEstruturasAuxiliares(n_vertices, distancias, visitados,
                                  anteriores);
  executarDijkstra(grafo, distancias, visitados, anteriores, origem,
                   n_vertices);

  int *caminho = malloc(n_vertices * sizeof(int));
  int tamanho_caminho = 0;

  Visita visita;
  construirVisita(caminho, tamanho_caminho, destino, distancias, anteriores,
                  &visita);

  free(visitados);
  free(distancias);
  free(anteriores);

  return visita;
}

Visita dijkstraAdaptado(Grafo grafo, int *ids, int n_ids) {
  bool *visitados = malloc(n_ids * sizeof(bool));
  for (int i = 0; i < n_ids; i++)
    visitados[i] = false;

  int atual = ids[0];
  visitados[0] = true;

  Visita visitaFinal;
  visitaFinal.peso = 0.0;
  visitaFinal.tamanho = 1;
  visitaFinal.caminho = malloc(n_ids * grafo->n_vertices * sizeof(int));
  visitaFinal.caminho[0] = atual;

  while (visitaFinal.tamanho < n_ids * grafo->n_vertices) {
    double minDist = DBL_MAX;
    int minIdx = -1;

    for (int i = 0; i < n_ids; i++) {
      if (!visitados[i]) {
        Visita visitaAtual = dijkstra(grafo, atual, ids[i]);
        if (visitaAtual.peso < minDist) {
          minDist = visitaAtual.peso;
          minIdx = i;
        }
        free(visitaAtual.caminho);
      }
    }

    if (minIdx != -1) {
      Visita visitaAtual = dijkstra(grafo, atual, ids[minIdx]);
      for (int j = 1; j < visitaAtual.tamanho; j++) {
        visitaFinal.caminho[visitaFinal.tamanho] = visitaAtual.caminho[j];
        visitaFinal.tamanho++;
      }
      visitaFinal.peso += visitaAtual.peso;
      free(visitaAtual.caminho);

      atual = ids[minIdx];
      visitados[minIdx] = true;
    } else {
      break;
    }
  }

  free(visitados);

  // Redimensionando a memória alocada para o caminho para combinar com o
  // tamanho final
  visitaFinal.caminho =
      realloc(visitaFinal.caminho, visitaFinal.tamanho * sizeof(int));

  return visitaFinal;
}

void imprimirCaminho(Visita visita) {
  printf("Menor caminho: ");
  for (int i = 0; i < visita.tamanho; i++) {
    printf("%d ", visita.caminho[i]);
  }
  printf("\nDistância total: %.2lf\n", visita.peso);
}