#include "caminho.h"
#include "grafo.h"
#include "local.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 256

// Constantes definidas para usar a dica dada
#define PI 3.14159265358979323846
#define EARTH_RADIUS 10000.0 // 1/4 da circunferência da Terra

// Calcula a distância entre dois pontos na esfera com base em suas coordenadas de latitude e longitude
double calcula_distancia(double latitude1, double longitude1,
                         double latitude2, double longitude2) {
  double delta_latitude =
      (latitude2 - latitude1) * PI / 180.0;
  double delta_longitude = (longitude2 - longitude1) * PI / 180.0;

  double dist_ns = delta_latitude * EARTH_RADIUS;

  double dist_ew = delta_longitude * EARTH_RADIUS *
                   cos((latitude1 + latitude2) / 2.0 * PI / 180.0);

  double dist_total = sqrt(dist_ns * dist_ns + dist_ew * dist_ew);

  return dist_total;
}

// Cria um grafo de caminhos a partir de um arquivo de entrada
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

    Local local_origem = local_procura(locais, origem);
    Local local_destino = local_procura(locais, destino);

    if (local_origem == NULL || local_destino == NULL) {
      continue;
    }

    double distancia = calcula_distancia(local_getLatitude(local_origem), local_getLongitude(local_origem),
                                         local_getLatitude(local_destino), local_getLongitude(local_destino));

    g_ins_aresta(caminho, local_getId(local_origem), local_getId(local_destino), distancia);
  }

  fclose(file);
  return caminho;
}
