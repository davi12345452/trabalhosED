
#include "dijkstra.h"
#include <stdbool.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>


// Definição da estrutura Visita, usada para armazenar informações sobre uma visita ou caminho encontrado pelo algoritmo de Dijkstra
struct _visita{
  double peso;
  int *caminho;
  int tamanho; 
};

// Encontra o índice do vértice com a menor distância não visitado
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

// Inicializa as estruturas auxiliares necessárias para o algoritmo de Dijkstra
static void inicializarEstruturasAuxiliares(int n_vertices, double *distancias,
                                     bool *visitados, int *anteriores) {
  for (int i = 0; i < n_vertices; i++) {
    distancias[i] = DBL_MAX;
    visitados[i] = false;
    anteriores[i] = -1;
  }
}

// Executa o algoritmo de Dijkstra para encontrar as menores distâncias
static void executarDijkstra(Grafo grafo, double *distancias, bool *visitados,
                      int *anteriores, int origem, int n_vertices) {
  distancias[origem] = 0;

  for (int count = 0; count < n_vertices - 1; count++) {
    int u = encontrarMenorDistancia(distancias, visitados, n_vertices);
    visitados[u] = true;

    // Use g_arestas_que_partem para definir o vertice_atual e aresta_atual
    g_arestas_que_partem(grafo, u);

    int v;
    float peso;

    // Itera sobre todas as arestas partindo de u
    while(g_proxima_aresta(grafo, NULL, &v, &peso)) {
      if (!visitados[v]) {
        if (distancias[u] != DBL_MAX && distancias[u] + peso < distancias[v]) {
          distancias[v] = distancias[u] + peso;
          anteriores[v] = u;
        }
      }
    }
  }
}


// Constrói uma estrutura de visita com base nos resultados do algoritmo de Dijkstra
static void construirVisita(int *caminho, int tamanho_caminho, int destino,
                     double *distancias, int *anteriores, Visita visita) {
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

// Implementa o algoritmo de Dijkstra para encontrar o caminho mínimo entre dois vértices
Visita dijkstra(Grafo grafo, int origem, int destino) {
  int n_vertices = g_nvertices(grafo);
  bool *visitados = malloc(n_vertices * sizeof(bool));
  double *distancias = malloc(n_vertices * sizeof(double));
  int *anteriores = malloc(n_vertices * sizeof(int));

  inicializarEstruturasAuxiliares(n_vertices, distancias, visitados,
                                  anteriores);
  executarDijkstra(grafo, distancias, visitados, anteriores, origem,
                   n_vertices);

  int *caminho = malloc(n_vertices * sizeof(int));
  int tamanho_caminho = 0;

  Visita visita = malloc(sizeof(struct _visita));
  construirVisita(caminho, tamanho_caminho, destino, distancias, anteriores, visita);


  free(visitados);
  free(distancias);
  free(anteriores);

  return visita;
}

// Imprime o caminho encontrado pelo algoritmo de Dijkstra
void _imprimirCaminho(Visita visita) {
  printf("Menor caminho: ");
  for (int i = 0; i < visita->tamanho; i++) {
    printf("%d ", visita->caminho[i]);
  }
  printf("\nDistância total: %.2lf\n", visita->peso);
}

// Adaptação do algoritmo de Dijkstra para encontrar o caminho mínimo passando por uma lista de vértices
Visita dijkstraAdaptado(Grafo grafo, int *ids, int n_ids) {
  bool *visitados = malloc(n_ids * sizeof(bool));
  int grafo_nVertices = g_nvertices(grafo);
  for (int i = 0; i < n_ids; i++)
    visitados[i] = false;
  
  int atual = ids[0];
  visitados[0] = true;
  
  Visita visitaFinal = malloc(sizeof(struct _visita));
  
  visitaFinal->peso = 0.0;
  
  visitaFinal->tamanho = 1;
  
  visitaFinal->caminho = malloc(n_ids * grafo_nVertices * sizeof(int));
  
  visitaFinal->caminho[0] = atual;
  
  while (visitaFinal->tamanho < n_ids * grafo_nVertices) {
    double minDist = DBL_MAX;
    int minIdx = -1;
  
    for (int i = 0; i < n_ids; i++) {
      if (!visitados[i]) {
          
        Visita visitaAtual = dijkstra(grafo, atual, ids[i]);
          
        if (visitaAtual->peso < minDist) {
          minDist = visitaAtual->peso;
          minIdx = i;
        }
        free(visitaAtual->caminho);
      }
    }
  
    if (minIdx != -1) {
      Visita visitaAtual = dijkstra(grafo, atual, ids[minIdx]);
      for (int j = 1; j < visitaAtual->tamanho; j++) {
        visitaFinal->caminho[visitaFinal->tamanho] = visitaAtual->caminho[j];
        visitaFinal->tamanho++;
      }
      visitaFinal->peso += visitaAtual->peso;
      free(visitaAtual->caminho);
  
      atual = ids[minIdx];
      visitados[minIdx] = true;
    } else {
      break;
    }
  }

  free(visitados);

  visitaFinal->caminho = realloc(visitaFinal->caminho, visitaFinal->tamanho * sizeof(int));

  return visitaFinal;
}

// Essa foi uma maneira que achei de poder retirar informações da estrutura Visita, sem quebrar o TAD. Preciso retirar
// array para transformar ele em uma lista de Locais e poder ter as informações para o arquivo. 

void info_visita(Visita visita, int **caminho, int *tam) {
    if(visita == NULL) return;
    *caminho = visita->caminho;
    *tam = visita->tamanho;
}