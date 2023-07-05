#include "local.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Local* criarNoLocal(const char* codigo, double latitude, double longitude, const char* descricao) {
    Local* novoLocal = (Local*)malloc(sizeof(Local));
    if (novoLocal == NULL) {
        printf("Erro de alocação de memória!\n");
        return NULL;
    }

    strcpy(novoLocal->codigo, codigo);
    novoLocal->latitude = latitude;
    novoLocal->longitude = longitude;
    novoLocal->proximo = NULL;

    if (descricao != NULL) {
        novoLocal->descricao = strdup(descricao);
    } else {
        novoLocal->descricao = NULL;
    }

    return novoLocal;
}

Local* criarLocal(const char* codigo, double latitude, double longitude, const char* descricao) {
    return criarNoLocal(codigo, latitude, longitude, descricao);
}

Local* adicionarLocal(Local* lista, Local* novoLocal) {
    if (novoLocal == NULL) {
        return lista;
    }

    novoLocal->proximo = lista;
    return novoLocal;
}

void liberarLocais(Local* lista) {
    while (lista != NULL) {
        Local* proximo = lista->proximo;
        free(lista->descricao);
        free(lista);
        lista = proximo;
    }
}

Local* lerLocaisDoArquivo(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return NULL;
    }

    Local* lista = NULL;
    char linha[256];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (linha[0] == '#' || linha[0] == '\n') {
            continue;  // Ignora linhas com # ou em branco
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
            if (sscanf(linha, "%[^,],%lf,%lf,%[^\n]", codigo, &latitude, &longitude, descricao) < 4) {
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

        Local* novoLocal = criarLocal(codigo, latitude, longitude, descricao);
        lista = adicionarLocal(lista, novoLocal);
    }

    fclose(arquivo);

    return lista;
}