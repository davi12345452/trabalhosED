#include <stdio.h>
#include "local.h"

int main() {
    Local* listaLocais = lerLocaisDoArquivo("locais.csv");
    if (listaLocais == NULL) {
        return 1;  // Lidar com erro ao ler o arquivo
    }

    // Exemplo de uso para verificar se os locais foram lidos corretamente
    Local* localAtual = listaLocais;
    int n = 2;
    while (localAtual != NULL) {
        printf("Código: %s\n", localAtual->codigo);
        printf("%d", n);
        printf("Latitude: %.15lf\n", localAtual->latitude);
        printf("Longitude: %.15lf\n", localAtual->longitude);
        if (localAtual->descricao != NULL) {
            printf("Descrição: %s\n", localAtual->descricao);
        } else {
            printf("Descrição: N/A\n");
        }
        printf("\n");
        n++;
        localAtual = localAtual->proximo;
    }

    liberarLocais(listaLocais);
    return 0;
}