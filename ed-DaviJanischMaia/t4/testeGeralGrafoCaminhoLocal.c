#include "local.h"
#include "caminho.h"
#include "grafo.h"
#include <stdio.h>

void imprimirLocais(Local lista) {
    printf("Locais:\n");
    for (Local l = lista; l != NULL; l = l->proximo) {
        printf("ID: %d, Código: %s, Latitude: %f, Longitude: %f\n", l->id, l->codigo, l->latitude, l->longitude);
        if (l->descricao) {
            printf("Descrição: %s\n", l->descricao);
        }
    }
}

void imprimirCaminho(Grafo caminho) {
    printf("Caminho:\n");
    for (int i = 0; i < caminho->n_vertices; i++) {
        for (int j = 0; j < caminho->n_vertices; j++) {
            if (caminho->matriz_adj[i][j] != NULL) {
                printf("Aresta do local %d ao local %d, Distância: %f\n", caminho->matriz_adj[i][j]->origem, caminho->matriz_adj[i][j]->destino, caminho->matriz_adj[i][j]->peso);
            }
        }
    }
}

int main() {
    Local listaLocais = lerLocaisDoArquivo("locais.csv");
    if (listaLocais == NULL) {
        printf("Erro ao criar a lista de locais.\n");
        return 1;
    }

    Grafo caminho = caminho_cria("caminhos.csv", listaLocais);
    if (caminho == NULL) {
        printf("Erro ao criar o caminho.\n");
        liberarLocais(listaLocais);
        return 1;
    }

    imprimirLocais(listaLocais);
    imprimirCaminho(caminho);

    caminho_destroi(caminho);
    liberarLocais(listaLocais);

    return 0;
}