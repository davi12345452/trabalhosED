// local.h

#ifndef LOCAL_H
#define LOCAL_H

typedef struct _local {
    char codigo[5];
    double latitude;
    double longitude;
    char* descricao;
    struct _local* proximo;
} Local;

// Função para criar um novo local
Local* criarLocal(const char* codigo, double latitude, double longitude, const char* descricao);

// Função para adicionar um novo local no início da lista
Local* adicionarLocal(Local* lista, Local* novoLocal);

// Função para liberar a memória alocada pela lista de locais
void liberarLocais(Local* lista);

// Função para ler os locais do arquivo CSV e retornar a lista
Local* lerLocaisDoArquivo(const char* nomeArquivo);

#endif  // LOCAL_H