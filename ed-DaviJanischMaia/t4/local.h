#ifndef LOCAL_H
#define LOCAL_H

typedef struct _local *Local;
struct _local {
    int id;
    char codigo[5];
    double latitude;
    double longitude;
    char* descricao;
    struct _local* proximo;
};

Local criarLocal(char* codigo, double latitude, double longitude, char* descricao);
Local adicionarLocal(Local lista, Local novoLocal);
void liberarLocais(Local lista);
Local lerLocaisDoArquivo(char* nomeArquivo);
Local local_procura(Local local, char* cod);

#endif  // LOCAL_H
