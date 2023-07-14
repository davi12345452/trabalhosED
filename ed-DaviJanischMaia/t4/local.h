#ifndef LOCAL_H_
#define LOCAL_H_

// Definição do tipo de dado 'Local' como um ponteiro para uma estrutura _local
typedef struct _local *Local;


// Função para criar um novo local com os dados fornecidos
Local criarLocal(int id, char* codigo, double latitude, double longitude, char* descricao);

// Função para adicionar um novo local à lista de locais
Local adicionarLocal(Local lista, Local novoLocal);

// Função para liberar a memória alocada para a lista de locais
void liberarLocais(Local lista);

// Função para ler os locais de um arquivo e retornar a lista de locais lidos
Local lerLocaisDoArquivo(char* nomeArquivo);

// Função para procurar um local pelo seu código na lista de locais
Local local_procura(Local local, char* cod);

// Função para procurar um local pelo seu id na lista de locais
Local local_procura_id(Local local, int id);

// Função para imprimir os locais presentes na lista
void imprimirLocais(Local lista);

// Função para criar uma nova lista de locais com base em uma lista de ids
Local criarListaPorIds(Local lista, int *ids, int tamanho);

// Função para buscar o id de um local na lista de locais pelo seu código
int buscaLocalID(Local lista, char *codigo);

/*
    Funções para manipular o TAD sem quebrá-lo em outros arquivos,
*/
char* local_getDescricao(Local local);
double local_getLatitude(Local local);
double local_getLongitude(Local local);
int local_getId(Local local);
Local local_getProximo(Local local);

#endif