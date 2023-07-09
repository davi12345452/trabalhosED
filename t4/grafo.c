#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Grafo g_cria(int n, bool orientado) {
  Grafo g = malloc(sizeof(struct _grafo));
  g->n_vertices = n;
  g->n_arestas = 0;
  g->orientado = orientado;
  g->arestas = malloc(n * n * sizeof(Aresta));
  g->index = 0;
  return g;
}

void g_destroi(Grafo self) {
  for (int i = 0; i < self->n_arestas; i++) {
    free(self->arestas[i]);
  }
  free(self->arestas);
  free(self);
}

int g_nvertices(Grafo self) { return self->n_vertices; }

void g_ins_aresta(Grafo self, char *origem, char *destino, float peso) {
  Aresta a = malloc(sizeof(struct _aresta));
  strcpy(a->origem, origem);
  strcpy(a->destino, destino);
  a->peso = peso;
  self->arestas[self->n_arestas++] = a;
}

void g_rem_aresta(Grafo self, char *origem, char *destino) {
  for (int i = 0; i < self->n_arestas; i++) {
    if (strcmp(self->arestas[i]->origem, origem) == 0 &&
        strcmp(self->arestas[i]->destino, destino) == 0) {
      free(self->arestas[i]);
      self->arestas[i] = self->arestas[--self->n_arestas];
      break;
    }
  }
}

void g_arestas(Grafo self) { self->index = 0; }

void g_arestas_que_partem(Grafo self, char *origem) { self->index = 0; }

bool g_proxima_aresta(Grafo self, char **origem, char **destino, float *peso) {
  if (self->index < self->n_arestas) {
    *origem = self->arestas[self->index]->origem;
    *destino = self->arestas[self->index]->destino;
    *peso = self->arestas[self->index]->peso;
    self->index++;
    return true;
  }
  return false;
}

void g_cria_dot(Grafo self, char *filename) {
  FILE *fp = fopen(filename, "w");
  if (fp == NULL) {
    fprintf(stderr, "Erro ao abrir arquivo para escrita\n");
    return;
  }

  if (self->orientado)
    fprintf(fp, "digraph G {\n");
  else
    fprintf(fp, "graph G {\n");

  for (int i = 0; i < self->n_arestas; i++) {
    if (self->orientado)
      fprintf(fp, "  %s -> %s [label=\"%f\"];\n", self->arestas[i]->origem,
              self->arestas[i]->destino, self->arestas[i]->peso);
    else
      fprintf(fp, "  %s -- %s [label=\"%f\"];\n", self->arestas[i]->origem,
              self->arestas[i]->destino, self->arestas[i]->peso);
  }

  fprintf(fp, "}\n");
  fclose(fp);
}
