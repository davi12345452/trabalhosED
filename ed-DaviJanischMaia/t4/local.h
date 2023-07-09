#ifndef LOCAL_H  // Verifica se a constante LOCAL_H não está definida, para evitar duplicidade na inclusão desse arquivo.
#define LOCAL_H  // Define a constante LOCAL_H.

// Define um ponteiro para 'struct _local' chamado 'Local'.
typedef struct _local *Local;

// Define a estrutura '_local' que representa um local no programa.
struct _local {
    int id;  // ID do local.
    char codigo[5];  // Código do local.
    double latitude;  // Latitude do local.
    double longitude;  // Longitude do local.
    char* descricao;  // Descrição do local.
    struct _local* proximo;  // Ponteiro para o próximo local na lista.
};

// Protótipo da função para criar um novo local. Recebe os dados do local e retorna um ponteiro para a estrutura criada.
Local criarLocal(int id, char* codigo, double latitude, double longitude, char* descricao);

// Protótipo da função para adicionar um local na lista de locais. Recebe a lista atual e o novo local, retorna a lista atualizada.
Local adicionarLocal(Local lista, Local novoLocal);

// Protótipo da função para liberar a memória alocada para a lista de locais. Recebe a lista e não retorna nada.
void liberarLocais(Local lista);

// Protótipo da função para ler os locais de um arquivo. Recebe o nome do arquivo e retorna a lista de locais lida.
Local lerLocaisDoArquivo(char* nomeArquivo);

// Protótipo da função para procurar um local na lista pelo código. Recebe a lista de locais e o código do local, retorna o local encontrado.
Local local_procura(Local local, char* cod);

Local local_procura_id(Local local, int id);

void imprimirLocais(Local lista); 

Local criarListaPorIds(Local lista, int *ids, int tamanho);

#endif  
