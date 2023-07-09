//Funcionando
#include "caminho.h"
#include "grafo.h"
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

// Função que implementa o algoritmo de Dijkstra
void dijkstra(Grafo grafo, int origem, int destino) {
  int n_vertices = grafo->n_vertices;
  bool *visitados = malloc(n_vertices * sizeof(bool));
  double *distancias = malloc(n_vertices * sizeof(double));
  int *anteriores = malloc(n_vertices * sizeof(int));

  // Inicialização das estruturas auxiliares
  for (int i = 0; i < n_vertices; i++) {
    distancias[i] = DBL_MAX;
    visitados[i] = false;
    anteriores[i] = -1;
  }

  distancias[origem] = 0;

  // Execução do algoritmo de Dijkstra
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

  // Impressão do menor caminho encontrado
  printf("Menor caminho de %d para %d: ", origem, destino);
  int atual = destino;
  while (atual != -1) {
    printf("%d ", atual);
    atual = anteriores[atual];
  }

  printf("\nDistância total: %.2lf\n", distancias[destino]);
}

int main() {
  Local locais = lerLocaisDoArquivo("locais.csv");
  Grafo caminho = caminho_cria("caminhos.csv", locais);

  dijkstra(caminho, 3, 33);
}