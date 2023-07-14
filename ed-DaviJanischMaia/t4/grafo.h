#ifndef GRAFO_H_
#define GRAFO_H_

#include <stdbool.h>

typedef struct _grafo *Grafo;

Grafo g_cria(int n, bool orientado);

void g_destroi(Grafo self);

int g_nvertices(Grafo self);

void g_ins_aresta(Grafo self, int origem, int destino, float peso);

void g_rem_aresta(Grafo self, int origem, int destino);

void g_arestas(Grafo self);

void g_arestas_que_partem(Grafo self, int origem);

bool g_proxima_aresta(Grafo self, int *origem, int *destino, float *peso);

/*
    Não estou usando esse função na lógica do programa. Ela serve apenas para visualizar as
    arestas e seus pesos. Usei apenas para ir verificando se estava implementando corretamente
*/
void imprimirCaminho(Grafo caminho);

#endif 