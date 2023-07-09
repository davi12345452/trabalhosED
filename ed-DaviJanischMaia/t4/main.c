#include "caminho.h"
#include "grafo.h"
#include "local.h"
#include "saida.h"
#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Função para escrever a lista no arquivo no formato BBMapCode
// Função para escrever a lista no arquivo no formato BBMapCode
// Função para escrever a lista no arquivo no formato BBMapCode
void escreverBBMapCode(Local lista, char *arquivoSaida) {
  FILE *saida = fopen(arquivoSaida, "w");

  if (!saida) {
    printf("Não foi possível abrir o arquivo de saída: %s\n", arquivoSaida);
    return;
  }

  // Início do BBMapCode
  fprintf(saida, "[map]\n");

  // Escrever cada local no arquivo
  Local temp = lista;
  int contador = 0; // Contador para acompanhar a posição do local

  while (temp) {
    // Verificar o primeiro local
    if (contador == 0) {
      if (temp->descricao && temp->descricao[0] != '\0') {
        // Imprimir a coordenada com descrição
        fprintf(saida, "%.5f,%.5f(%s);\n", temp->latitude, temp->longitude,
                temp->descricao);
        fprintf(saida, "%.5f,%.5f ", temp->latitude, temp->longitude);
      }
    }

    else if (temp->proximo == NULL) {
      if (temp->descricao && temp->descricao[0] != '\0') {
        // Imprimir a coordenada novamente sem descrição
        fprintf(saida, "%.5f,%.5f;", temp->latitude, temp->longitude);
        fprintf(saida, "\n%.5f,%.5f(%s)", temp->latitude, temp->longitude,
                temp->descricao);
      }
    }
    // Verificar os locais com descrição
    else if (temp->descricao && temp->descricao[0] != '\0') {
      // Imprimir a coordenada com descrição
      fprintf(saida, "%.5f,%.5f ", temp->latitude, temp->longitude);
      fprintf(saida, "\n%.5f,%.5f(%s)\n", temp->latitude, temp->longitude,
              temp->descricao);

      // Imprimir a coordenada novamente sem descrição
      fprintf(saida, "%.5f,%.5f ", temp->latitude, temp->longitude);
    } else {
      // Imprimir a coordenada sem descrição
      fprintf(saida, "%.5f,%.5f ", temp->latitude, temp->longitude);
    }
  temp = temp->proximo;
  contador++;
  }
  // Final do BBMapCode
  fprintf(saida, "\n[/map]");

  fclose(saida);
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

// Função principal
void processarArquivo(Grafo caminho, char *arquivoEntrada, Local listaLocais,
                      char *arquivoSaida) {
  FILE *arquivo = fopen(arquivoEntrada, "r");
  int ids[1024]; // Array para guardar os IDs, considerando um máximo de 1024
                 // locais.
  int n_ids = 0;
  char linha[256];

  if (!arquivo) {
    printf("Não foi possível abrir o arquivo: %s\n", arquivoEntrada);
    return;
  }

  // Lendo o arquivo
  while (fgets(linha, sizeof(linha), arquivo)) {
    if (linha[0] != '#' && linha[0] != '\n') {
      linha[strcspn(linha, "\n")] =
          0; // Removendo a quebra de linha do final da string
      int id = buscaLocalID(listaLocais, linha);
      if (id >= 0) {
        ids[n_ids++] = id;
      }
    }
  }

  fclose(arquivo);

  // Chamada para o dijkstra adaptado
  Visita visitaFinal = dijkstraAdaptado(caminho, ids, n_ids);

  // Imprimindo o caminho
  imprimirCaminho(visitaFinal);

  // Criando uma nova lista
  Local novaLista =
      criarListaPorIds(listaLocais, visitaFinal.caminho, visitaFinal.tamanho);

  // Salvando a nova lista no arquivo saída.txt
  escreverBBMapCode(novaLista, arquivoSaida);
}
int main() {
  Local listaLocais = lerLocaisDoArquivo("locais.csv");
  if (listaLocais == NULL) {
    printf("Erro ao criar a lista de locais.\n");
    return 1;
  }
  // imprimirLocais(listaLocais);
  Grafo caminho = caminho_cria("caminhos.csv", listaLocais);
  if (caminho == NULL) {
    printf("Erro ao criar o caminho.\n");
    liberarLocais(listaLocais);
    return 1;
  }
  // Aqui você deve criar o grafo e preenchê-lo com os dados do seu arquivo

  processarArquivo(caminho, "visita.csv", listaLocais, "saida.txt");

  return 0;
}