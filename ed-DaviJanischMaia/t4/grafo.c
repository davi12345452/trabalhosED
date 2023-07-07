#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>

struct _grafo {
    int n_vertices;
    bool orientado;
    Aresta **matriz_adj;
};

struct _aresta {
    int id;
    int origem;
    int destino;
    double peso;
};

static int cursor_i = 0;
static int cursor_j = 0;

Grafo g_cria(int n, bool orientado) {
    Grafo novo_grafo = (Grafo)malloc(sizeof(struct _grafo));
    if (novo_grafo == NULL) {
        return NULL;
    }
    novo_grafo->n_vertices = n;
    novo_grafo->orientado = orientado;

    // Alocando a matriz de adjacência dinamicamente
    novo_grafo->matriz_adj = (Aresta **)malloc(n * sizeof(Aresta *));
    for (int i = 0; i < n; i++) {
        novo_grafo->matriz_adj[i] = (Aresta *)calloc(n, sizeof(Aresta *));
    }

    return novo_grafo;
}

void g_destroi(Grafo self) {
    if (self == NULL) {
        return;
    }
    for (int i = 0; i < self->n_vertices; i++) {
        for (int j = 0; j < self->n_vertices; j++) {
            free(self->matriz_adj[i][j]);
        }
        free(self->matriz_adj[i]);
    }
    free(self->matriz_adj);
    free(self);
}

int g_nvertices(Grafo self) {
    return (self != NULL) ? self->n_vertices : 0;
}

void g_ins_aresta(Grafo self, int origem, int destino, float peso) {
    if (self == NULL || origem < 0 || destino < 0 || origem >= self->n_vertices || destino >= self->n_vertices) {
        return;
    }

    Aresta *nova_aresta = (Aresta *)malloc(sizeof(Aresta));
    nova_aresta->id = origem * self->n_vertices + destino;
    nova_aresta->origem = origem;
    nova_aresta->destino = destino;
    nova_aresta->peso = peso;

    self->matriz_adj[origem][destino] = nova_aresta;

    if (!self->orientado) {
        Aresta *aresta_simetrica = (Aresta *)malloc(sizeof(Aresta));
        *aresta_simetrica = *nova_aresta;
        aresta_simetrica->id = destino * self->n_vertices + origem;
        aresta_simetrica->origem = destino;
        aresta_simetrica->destino = origem;
        self->matriz_adj[destino][origem] = aresta_simetrica;
    }
}

void g_rem_aresta(Grafo self, int origem, int destino) {
    if (self == NULL || origem < 0 || destino < 0 || origem >= self->n_vertices || destino >= self->n_vertices) {
        return;
    }

    free(self->matriz_adj[origem][destino]);
    self->matriz_adj[origem][destino] = NULL;

    if (!self->orientado) {
        free(self->matriz_adj[destino][origem]);
        self->matriz_adj[destino][origem] = NULL;
    }
}

void g_arestas(Grafo self) {
    cursor_i = 0;
    cursor_j = 0;
}

void g_arestas_que_partem(Grafo self, int origem) {
    cursor_i = origem;
    cursor_j = 0;
}

bool g_proxima_aresta(Grafo self, int *origem, int *destino, float *peso) {
    if (self == NULL) {
        return false;
    }

    for (; cursor_i < self->n_vertices; cursor_i++) {
        for (; cursor_j < self->n_vertices; cursor_j++) {
            Aresta *aresta = self->matriz_adj[cursor_i][cursor_j];
            if (aresta != NULL) {
                if (origem != NULL) {
                    *origem = aresta->origem;
                }
                if (destino != NULL) {
                    *destino = aresta->destino;
                }
                if (peso != NULL) {
                    *peso = aresta->peso;
                }
                cursor_j++;
                return true;
            }
        }
        cursor_j = 0;
    }
    return false;
}
