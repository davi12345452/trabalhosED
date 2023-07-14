#include "saida.h"
#include "caminho.h"
#include "dijkstra.h"
#include "grafo.h"
#include "local.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Escreve o código BBMap com as informações dos locais em um arquivo de saída

/*
  Realizei algumas modificações aqui. Eu estava quebrando o TAD local, por isso, utilizei
  as funções de retirar os dados e de chamar o próximo item da lista. Além disso, modifiquei
  os ; que não estavam muito corretos. 
*/
void escreverBBMapCode(Local lista, char *arquivoSaida) {
  FILE *saida = fopen(arquivoSaida, "w");

  if (!saida) {
    printf("Não foi possível abrir o arquivo de saída: %s\n", arquivoSaida);
    return;
  }
  fprintf(saida, "[map]\n");

  Local temp = lista;
  int contador = 0;
  while (temp) {
    int id = local_getId(temp);
    double latitude = local_getLatitude(temp), longitude = local_getLongitude(temp);
    char  *descricao = local_getDescricao(temp);
    if (contador == 0) {
      if (descricao && descricao[0] != '\0') {
        fprintf(saida, "%.5f,%.5f(%s);\n", latitude, longitude,
                descricao);
        fprintf(saida, "%.5f,%.5f ", latitude, longitude);
      }
    }
    
    else if (local_getProximo(temp) == NULL) {
      if (descricao && descricao[0] != '\0') {
        fprintf(saida, "%.5f,%.5f;", latitude, longitude);
        fprintf(saida, "\n%.5f,%.5f(%s)", latitude, longitude,
                descricao);
      }
    }
    else if (descricao && descricao[0] != '\0') {
      fprintf(saida, "%.5f,%.5f;", latitude, longitude);
      fprintf(saida, "\n%.5f,%.5f(%s);\n", latitude, longitude,
              descricao);
      fprintf(saida, "%.5f,%.5f ", latitude, longitude);
    } else {
      fprintf(saida, "%.5f,%.5f ", latitude, longitude);
    }
    temp = local_getProximo(temp);
    contador++;
  }
  fprintf(saida, "\n[/map]");
  fclose(saida);
}

// Processa um arquivo de entrada, realiza o algoritmo de Dijkstra adaptado e escreve o código BBMap no arquivo de saída
void processarArquivo(Grafo caminho, char *arquivoEntrada, Local listaLocais,
                      char *arquivoSaida) {
  FILE *arquivo = fopen(arquivoEntrada, "r");
  int ids[1024]; 
  int n_ids = 0;
  char linha[256];
  if (!arquivo) {
    printf("Não foi possível abrir o arquivo: %s\n", arquivoEntrada);
    return;
  }
  while (fgets(linha, sizeof(linha), arquivo)) {
    if (linha[0] != '#' && linha[0] != '\n') {
      linha[strcspn(linha, "\n")] =
          0;
      int id = buscaLocalID(listaLocais, linha);
      if (id >= 0) {
        ids[n_ids++] = id;
      }
    }
  }
  fclose(arquivo);
  Visita visitaFinal = dijkstraAdaptado(caminho, ids, n_ids);
  int *cam;
  int tamanho;
  info_visita(visitaFinal, &cam, &tamanho);
  Local novaLista = criarListaPorIds(listaLocais, cam, tamanho);
  escreverBBMapCode(novaLista, arquivoSaida);
  

}

// Função principal do programa que realiza a leitura dos arquivos, cria o grafo de caminhos, processa o arquivo de visita e gera o código BBMap
void programa(char *arqLocal, char *arqCam, char *arqVisita, char *arqSaida) {
  Local listaLocais = lerLocaisDoArquivo(arqLocal);
  Grafo caminho = caminho_cria(arqCam, listaLocais);
  processarArquivo(caminho, arqVisita, listaLocais, arqSaida);
  liberarLocais(listaLocais);
  g_destroi(caminho);
}