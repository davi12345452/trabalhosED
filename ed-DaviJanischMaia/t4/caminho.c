#include "caminho.h"
#include "grafo.h"
#include "local.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 256

#define PI 3.14159265358979323846
#define EARTH_RADIUS 10000.0 // 1/4 da circunferência da Terra

double calcula_distancia(double latitude1, double longitude1,
double latitude2, double longitude2) {
  double delta_latitude =
      (latitude2 - latitude1) * PI / 180.0; // Convertendo graus para radianos
  double delta_longitude = (longitude2 - longitude1) * PI / 180.0;

  // Distância Norte-Sul (considerando um corte vertical na Terra)
  double dist_ns = delta_latitude * EARTH_RADIUS;

  // Distância Leste-Oeste (considerando um corte horizontal na Terra)
  double dist_ew = delta_longitude * EARTH_RADIUS *
                   cos((latitude1 + latitude2) / 2.0 * PI / 180.0);

  // Hipotenusa do triângulo retângulo formado pelas distâncias Norte-Sul e
  // Leste-Oeste
  double dist_total = sqrt(dist_ns * dist_ns + dist_ew * dist_ew);

  return dist_total;
}

Grafo caminho_cria(char *arquivo, Local locais) {
  Grafo caminho = g_cria(100, false);

  FILE *file = fopen(arquivo, "r");
  if (!file) {
    fprintf(stderr, "Erro ao abrir o arquivo.\n");
    return NULL;
  }

  char line[MAX_LINE_LEN];

  while (fgets(line, MAX_LINE_LEN, file)) {
    if (line[0] == '\n' || line[0] == '#') {
      continue;
    }

    char origem[MAX_LINE_LEN], destino[MAX_LINE_LEN];
    sscanf(line, "%[^,],%s", origem, destino);

    // Procura os locais correspondentes na lista
    Local local_origem = local_procura(locais, origem);
    Local local_destino = local_procura(locais, destino);

    if (local_origem == NULL || local_destino == NULL) {
      continue;
    }

    // Calcula a distância entre os locais
    double distancia = calcula_distancia(local_origem->latitude, local_origem->longitude,
                                         local_destino->latitude, local_destino->longitude);

    g_ins_aresta(caminho, local_origem->id, local_destino->id, distancia);
  }

  fclose(file);
  return caminho;
}


void caminho_destroi(Grafo caminho) {
  g_destroi(caminho);
  //free(caminho);
}

Grafo caminho_obtem_grafo(Grafo caminho) { return caminho; }