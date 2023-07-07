#include "caminho.h"
#include <stdio.h>

int main() {
    // Criando um objeto Caminho a partir do arquivo CSV
    Caminho caminho = caminho_cria("caminhos.csv");

    if (!caminho) {
        fprintf(stderr, "Falha ao criar o caminho.\n");
        return 1;
    }

    // Obtendo o grafo
    Grafo grafo = caminho_obtem_grafo(caminho);

    // Imprimindo as arestas do grafo
    printf("Arestas do grafo:\n");
    g_arestas(grafo);
    int origem, destino;
    float peso;
    while (g_proxima_aresta(grafo, &origem, &destino, &peso)) {
        printf("%d -> %d (%.2f)\n", origem, destino, peso);
    }

    // Limpando a memória
    caminho_destroi(caminho);

    return 0;
}
