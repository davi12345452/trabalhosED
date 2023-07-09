#ifndef GRAFO_H
#define GRAFO_H

#include <stdbool.h>

/*
  Defini um grafo usando a estrutura grafo e aresta. Os vértices
  estão dentro de aresta. O programa fornece códigos em char* como
  entradas, porém em local.h eles são indexados para auxiliar a mani-
  pulação com a função dijkstra
*/

typedef struct _aresta *Aresta;
typedef struct _grafo *Grafo;

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


Grafo g_cria(int n, bool orientado);

void g_destroi(Grafo self);

int g_nvertices(Grafo self);

void g_ins_aresta(Grafo self, int origem, int destino, float peso);

void g_rem_aresta(Grafo self, int origem, int destino);

void g_arestas(Grafo self);

void g_arestas_que_partem(Grafo self, int origem);

bool g_proxima_aresta(Grafo self, int *origem, int *destino, float *peso);

#endif 