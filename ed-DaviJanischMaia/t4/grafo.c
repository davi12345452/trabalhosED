#include "grafo.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _aresta *Aresta;

struct _aresta{
    int id;
    int origem;
    int destino;
    double peso;
};

struct _grafo{
    int n_vertices;
    bool orientado;
    Aresta **matriz_adj;
    int aresta_atual;
    int vertice_atual;
    int proximo_id; 
};

Grafo g_cria(int n, bool orientado) {
  Grafo g = malloc(sizeof(struct _grafo));
  g->n_vertices = n;
  g->orientado = orientado;
  g->matriz_adj = malloc(n * sizeof(struct _aresta));
  for (int i = 0; i < n; i++) {
    g->matriz_adj[i] = calloc(n, sizeof(struct _aresta));
  }
  g->aresta_atual = 0;
  g->vertice_atual = 0;
  g->proximo_id = 1; 
  return g;
}

void g_destroi(Grafo g) {
  for (int i = 0; i < g->n_vertices; i++) {
    for (int j = 0; j < g->n_vertices; j++) {
      if (g->matriz_adj[i][j]) {
        free(g->matriz_adj[i][j]);
      }
    }
    free(g->matriz_adj[i]);
  }
  free(g->matriz_adj);
  free(g);
}

int g_nvertices(Grafo g) { return g->n_vertices; }

void g_ins_aresta(Grafo g, int origem, int destino, float peso) {
  if (!g->matriz_adj[origem][destino]) {
    g->matriz_adj[origem][destino] = malloc(sizeof(struct _aresta));
    g->matriz_adj[origem][destino]->id = g->proximo_id++;
  }
  g->matriz_adj[origem][destino]->origem = origem;
  g->matriz_adj[origem][destino]->destino = destino;
  g->matriz_adj[origem][destino]->peso = peso;

  if (!g->orientado) {
    if (!g->matriz_adj[destino][origem]) {
      g->matriz_adj[destino][origem] = malloc(sizeof(struct _aresta));
      g->matriz_adj[destino][origem]->id = g->proximo_id++;
    }
    g->matriz_adj[destino][origem]->origem = destino;
    g->matriz_adj[destino][origem]->destino = origem;
    g->matriz_adj[destino][origem]->peso = peso;
  }
}

void g_rem_aresta(Grafo g, int origem, int destino) {
  if (g->matriz_adj[origem][destino]) {
    free(g->matriz_adj[origem][destino]);
    g->matriz_adj[origem][destino] = NULL;
  }

  if (!g->orientado) {
    if (g->matriz_adj[destino][origem]) {
      free(g->matriz_adj[destino][origem]);
      g->matriz_adj[destino][origem] = NULL;
    }
  }
}

void g_arestas(Grafo g) {
  g->vertice_atual = 0;
  g->aresta_atual = 0;
}


/*
  Fiz algumas modificações aqui, de maneira que os códigos se encaixassem na minha lógica de funções
  em Dijkstra. Agora utilizo essas funções e não quebro o TAD.
*/

void g_arestas_que_partem(Grafo g, int origem) {
    g->vertice_atual = origem;
    g->aresta_atual = 0;
}

bool g_proxima_aresta(Grafo g, int *origem, int *destino, float *peso) {
    // Itera sobre as arestas que partem do vertice_atual
    for (int i = g->aresta_atual; i < g->n_vertices; i++) {
        if (g->matriz_adj[g->vertice_atual][i] != NULL) {
            // Se encontrou uma aresta, atualiza o índice da próxima aresta a ser retornada
            g->aresta_atual = i + 1;
            
            // Se o ponteiro origem não é NULL, preenche com o valor do vértice atual
            if (origem) *origem = g->vertice_atual;

            // Preenche os valores do destino e do peso da aresta encontrada
            *destino = g->matriz_adj[g->vertice_atual][i]->destino;
            *peso = g->matriz_adj[g->vertice_atual][i]->peso;

            return true;
        }
    }
    return false;
}


/*
  Essa é uma função de impressão caso queira testar o grafo. Imprime as arestas e seus pesos. 
*/

void imprimirCaminho(Grafo caminho) {
    printf("Caminho:\n");
    for (int i = 0; i < caminho->n_vertices; i++) {
        for (int j = 0; j < caminho->n_vertices; j++) {
            if (caminho->matriz_adj[i][j] != NULL) {
                printf("Aresta do local %d ao local %d, Distância: %f\n", caminho->matriz_adj[i][j]->origem, caminho->matriz_adj[i][j]->destino, caminho->matriz_adj[i][j]->peso);
            }
        }
    }
}