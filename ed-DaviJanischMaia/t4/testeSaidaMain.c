// main.c

#include "caminho.h"
#include "local.h"
#include "saida.h"
#include <string.h>
#include <stdio.h>

Local* procurarLocalPorCodigo(Local* lista, const char* codigo) {
    Local* atual = lista;
    while (atual != NULL) {
        if (strcmp(atual->codigo, codigo) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}
void escreverMapaBBCode(Local *lista,Grafo grafo, const char *arquivoSaida) {
  FILE *arquivo = fopen(arquivoSaida, "w");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo %s\n", arquivoSaida);
    return;
  }

  char *origem, *destino;
  float peso;
  fprintf(arquivo, "[map]\n");

  while (g_proxima_aresta(grafo, &origem, &destino, &peso)) {
    Local *localOrigem = procurarLocalPorCodigo(lista, origem);
    Local *localDestino = procurarLocalPorCodigo(lista, destino);

    fprintf(arquivo, "%lf,%lf", localOrigem->latitude, localOrigem->longitude);
    if (localOrigem->descricao != NULL && strlen(localOrigem->descricao) > 0) {
      fprintf(arquivo, "(%s)", localOrigem->descricao);
    }
    fprintf(arquivo, ";\n");

    fprintf(arquivo, "%lf,%lf", localDestino->latitude,
            localDestino->longitude);
    if (localDestino->descricao != NULL &&
        strlen(localDestino->descricao) > 0) {
      fprintf(arquivo, "(%s)", localDestino->descricao);
    }
    fprintf(arquivo, ";\n");
  }

  fprintf(arquivo, "[/map]\n");
  fclose(arquivo);
}

int main() {
  Local *locais = lerLocaisDoArquivo("locais.csv");
  Caminho caminho = caminho_cria("caminhos.csv");
  Local *visitas = lerVisitasDoArquivo("visita.csv");
  Grafo grafo = encontrarCaminhoMaisCurto(visitas);
  escreverMapaBBCode(locais, grafo, "saida.txt");

  liberarLocais(locais);
  caminho_destroi(caminho);
  liberarLocais(visitas);
  g_destroi(grafo);

  return 0;
}
