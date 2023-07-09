#include "local.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "local.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int id_counter = 0;

Local criarLocal(char *codigo, double latitude, double longitude, char *descricao) {
  Local novoLocal = (Local)malloc(sizeof(struct _local));
  if (novoLocal == NULL) {
    printf("Erro de alocação de memória!\n");
    return NULL;
  }
  
  novoLocal->id = id_counter++; // Adiciona um ID único a cada local
  strcpy(novoLocal->codigo, codigo);
  novoLocal->latitude = latitude;
  novoLocal->longitude = longitude;
  novoLocal->proximo = NULL;

  if (descricao != NULL) {
    novoLocal->descricao = (char*)malloc((strlen(descricao)+1) * sizeof(char));
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
  FILE *arquivo = fopen(nomeArquivo, "r");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
    return NULL;
  }

  Local lista = NULL;
  char linha[256];
  
  while (fgets(linha, sizeof(linha), arquivo) != NULL) {
    if (linha[0] == '#' || linha[0] == '\n') {
      continue; // Ignora linhas com # ou em branco
    }

    // Faz o parsing dos dados da linha
    char codigo[5];
    double latitude, longitude;
    char descricao[100] = ""; // inicializa descricao como vazio

    char *first_comma = strchr(linha, ',');
    char *second_comma = first_comma ? strchr(first_comma + 1, ',') : NULL;
    char *third_comma = second_comma ? strchr(second_comma + 1, ',') : NULL;

    if (third_comma) {
      // Caso tenha descrição
      if (sscanf(linha, "%[^,],%lf,%lf,%[^\n]", codigo, &latitude, &longitude,
                 descricao) < 4) {
        printf("Erro ao ler a linha do arquivo: %s\n", linha);
        continue;
      }
    } else {
      // Caso não tenha descrição
      if (sscanf(linha, "%[^,],%lf,%lf", codigo, &latitude, &longitude) < 3) {
        printf("Erro ao ler a linha do arquivo: %s\n", linha);
        continue;
      }
    }

    Local novoLocal = criarLocal(codigo, latitude, longitude, descricao);
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