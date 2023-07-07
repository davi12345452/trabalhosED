#include "caminho.h"
#include "local.h"
#include <stdio.h>
#include <stdbool.h>

// Função auxiliar para buscar um local pelo código
Local* buscaLocal(Local* lista, char* codigo) {
    Local* atual = lista;
    while (atual != NULL) {
        if (strcmp(atual->codigo, codigo) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

// Função auxiliar para imprimir as informações do local
void imprimeLocal(Local* local) {
    if (local == NULL) {
        printf("Local não encontrado\n");
    } else {
        printf("Código: %s, Latitude: %lf, Longitude: %lf, Descrição: %s\n", 
                local->codigo, local->latitude, local->longitude, 
                local->descricao ? local->descricao : "Sem descrição");
    }
}

int main() {
    // Carrega os caminhos
    Caminho caminho = caminho_cria("caminhos.csv");
    if (caminho == NULL) {
        return 1;
    }

    // Carrega os locais
    Local* locais = lerLocaisDoArquivo("locais.csv");
    if (locais == NULL) {
        caminho_destroi(caminho);
        return 1;
    }

    // Verifica se todos os códigos de caminho existem na lista de locais
    Grafo grafo = caminho_obtem_grafo(caminho);
    char *origem, *destino;
    float peso;
    g_arestas(grafo);
    while (g_proxima_aresta(grafo, &origem, &destino, &peso)) {
        Local *localOrigem = buscaLocal(locais, origem);
        Local *localDestino = buscaLocal(locais, destino);
        if (localOrigem == NULL || localDestino == NULL) {
            printf("Caminho inválido: %s -> %s\n", origem, destino);
        } else {
            printf("Caminho de:\n");
            imprimeLocal(localOrigem);
            printf("para:\n");
            imprimeLocal(localDestino);
            printf("\n");
        }
    }

    // Libera a memória
    liberarLocais(locais);
    caminho_destroi(caminho);
    return 0;
}