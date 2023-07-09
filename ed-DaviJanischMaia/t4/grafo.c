#include "grafo.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Grafo g_cria(int n, bool orientado) {
  Grafo g = (Grafo*)malloc(sizeof(struct _grafo));
  g->n_vertices = n;
  g->orientado = orientado;
  g->matriz_adj = (Aresta **)malloc(n * sizeof(Aresta *));
  for (int i = 0; i < n; i++) {
    g->matriz_adj[i] = (Aresta *)calloc(n, sizeof(Aresta));
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
    g->matriz_adj[origem][destino] = (Aresta)malloc(sizeof(struct _aresta));
    g->matriz_adj[origem][destino]->id = g->proximo_id++;
  }
  g->matriz_adj[origem][destino]->origem = origem;
  g->matriz_adj[origem][destino]->destino = destino;
  g->matriz_adj[origem][destino]->peso = peso;

  if (!g->orientado) {
    if (!g->matriz_adj[destino][origem]) {
      g->matriz_adj[destino][origem] = (Aresta)malloc(sizeof(struct _aresta));
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

void g_arestas_que_partem(Grafo g, int origem) {
  g->vertice_atual = origem;
  g->aresta_atual = 0;
}

bool g_proxima_aresta(Grafo g, int *origem, int *destino, float *peso) {
  for (int i = g->vertice_atual; i < g->n_vertices; i++) {
    for (int j = g->aresta_atual; j < g->n_vertices; j++) {
      if (g->matriz_adj[i][j]) {
        if (origem)
          *origem = g->matriz_adj[i][j]->origem;
        if (destino)
          *destino = g->matriz_adj[i][j]->destino;
        if (peso)
          *peso = g->matriz_adj[i][j]->peso;
        g->aresta_atual = j + 1;
        return true;
      }
    }
    g->aresta_atual = 0;
    g->vertice_atual++;
  }
  return false;
}