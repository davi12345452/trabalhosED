#ifndef CAMINHO_H
#define CAMINHO_H

#include "grafo.h"
#include "local.h"

double calcula_distancia(double latitude1, double longitude1,
double latitude2, double longitude2);
// Cria um objeto Caminho a partir de um arquivo CSV.
// Retorna um ponteiro para o objeto Caminho.
Grafo caminho_cria(char *arquivo, Local locais);

// Destroi o objeto Caminho e libera a memória associada.
void caminho_destroi(Grafo caminho);

// Retorna o grafo associado ao objeto Caminho.
Grafo caminho_obtem_grafo(Grafo caminho);

#endif