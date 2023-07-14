#include "local.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Definição da estrutura _local que representa um local
struct _local {
    int id;                 
    char codigo[5];         
    double latitude;       
    double longitude;       
    char* descricao;        
    struct _local* proximo;
};


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

// Função para procurar local pelo código - Manipulação posterior do Dijkstra
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

// Função para procurar local pelo Id - Manipulação posterior do Dijkstra
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

// Função que busca um local pelo código, devolvendo o ID - Preencher o grafo com ID ao invés de codigo char*
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


// Função para imprimir os locais de lista, mais para testes mesmo
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

// Recebe uma lista de IDs e cria uma lista de Locais
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



/*
  As funções abaixo são para o acesso de informações das estruturas, sem que o TAD seja quebrado. Aqui há
  funções para devolver descrição, longitude, latitude e id de um local, bem como o próximo da lista. Foi
  a maneira que eu encontrei de deixar os TADs encapsulados, porém fornecendo o acesso de informações em 
  determinadas chamadas. 
*/

char* local_getDescricao(Local local){
    if(local == NULL){
        return "";
    }
    return local->descricao;
}


double local_getLatitude(Local local) {
    if (local == NULL) {
        return -1;
    }

    return local->latitude;
}

double local_getLongitude(Local local) {
    if (local == NULL) {
        return -1;
    }

    return local->longitude;
}

int local_getId(Local local) {
    if (local == NULL) {
        return -1;
    }

    return local->id;
}

Local local_getProximo(Local local) {
    if (local == NULL) {
        return NULL;
    }
    return local->proximo;
}