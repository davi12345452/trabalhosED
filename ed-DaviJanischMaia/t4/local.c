#include "local.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// static int id_counter = 0;

Local criarLocal(int id, char *codigo, double latitude, double longitude,
                 char *descricao) {
  Local novoLocal = (Local)malloc(sizeof(struct _local));
  if (novoLocal == NULL) {
    printf("Erro de alocação de memória!\n");
    return NULL;
  }

  novoLocal->id = id; // Adiciona um ID único a cada local
  strcpy(novoLocal->codigo, codigo);
  novoLocal->latitude = latitude;
  novoLocal->longitude = longitude;
  novoLocal->proximo = NULL;

  if (descricao != NULL) {
    novoLocal->descricao =
        (char *)malloc((strlen(descricao) + 1) * sizeof(char));
    if (novoLocal->descricao) {
      strcpy(novoLocal->descricao, descricao);
    } else {
      printf("Erro de alocação de memória para a descrição!\n");
    }
  } else {
    novoLocal->descricao = NULL;
  }

  return novoLocal;
}

Local adicionarLocal(Local lista, Local novoLocal) {
  if (novoLocal == NULL) {
    return lista;
  }

  novoLocal->proximo = lista;
  return novoLocal;
}

void liberarLocais(Local lista) {
  while (lista != NULL) {
    Local proximo = lista->proximo;
    free(lista->descricao);
    free(lista);
    lista = proximo;
  }
}

Local lerLocaisDoArquivo(char *nomeArquivo) {
  int id_counter = 0;
  FILE *arquivo = fopen(nomeArquivo, "r");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
    return NULL;
  }

  Local lista = NULL;
  char linha[256];

  while (fgets(linha, sizeof(linha), arquivo) != NULL) {
    if (linha[0] == '#' || linha[0] == '\n') {
      continue;
    }

    char codigo[5];
    double latitude, longitude;
    char descricao[100] = ""; 

    char *first_comma = strchr(linha, ',');
    char *second_comma = first_comma ? strchr(first_comma + 1, ',') : NULL;
    char *third_comma = second_comma ? strchr(second_comma + 1, ',') : NULL;

    if (third_comma) {
      if (sscanf(linha, "%[^,],%lf,%lf,%[^\n]", codigo, &latitude, &longitude,
                 descricao) < 4) {
        printf("Erro ao ler a linha do arquivo: %s\n", linha);
        continue;
      }
    } else {
      if (sscanf(linha, "%[^,],%lf,%lf", codigo, &latitude, &longitude) < 3) {
        printf("Erro ao ler a linha do arquivo: %s\n", linha);
        continue;
      }
    }

    Local novoLocal = criarLocal(
        id_counter++, codigo, latitude, longitude,
        descricao);
    lista = adicionarLocal(lista, novoLocal);
  }

  fclose(arquivo);

  return lista;
}

Local local_procura(Local lista, char *cod) {
  Local atual = lista;
  while (atual != NULL) {
    if (strcmp(atual->codigo, cod) == 0) {
      return atual;
    }
    atual = atual->proximo;
  }
  return NULL;
}

Local local_procura_id(Local local, int id) {
  Local cursor = local;
  while (cursor != NULL) {
    if (cursor->id == id) {
      return cursor;
    }
    cursor = cursor->proximo;
  }
  return NULL;
}

int estaNoArray(int numero, int *array, int tamanho) {
  for (int i = 0; i < tamanho; i++) {
    if (array[i] == numero) {
      return 1;
    }
  }
  return 0;
}

void imprimirLocais(Local lista) {
  Local atual = lista;
  while (atual != NULL) {
    printf("ID: %d\n", atual->id);
    printf("Código: %s\n", atual->codigo);
    printf("Latitude: %f\n", atual->latitude);
    printf("Longitude: %f\n", atual->longitude);
    printf("Descrição: %s\n", atual->descricao);
    printf("\n");
    atual = atual->proximo;
  }
}

Local criarListaPorIds(Local lista, int *ids, int tamanho) {
  Local novaLista = NULL;
  for (int i = 0; i < tamanho; i++) {
    Local local = local_procura_id(lista, ids[i]);
    if (local != NULL) {
      novaLista = adicionarLocal(
          novaLista, criarLocal(local->id, local->codigo, local->latitude,
                                local->longitude, local->descricao));
    }
  }
  return novaLista;
}

int buscaLocalID(Local lista, char *codigo) {
  Local temp = lista;

  while (temp) {
    if (strcmp(temp->codigo, codigo) == 0) {
      return temp->id;
    }
    temp = temp->proximo;
  }

  return -1;
}