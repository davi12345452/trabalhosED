#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int matricula;
  char *nome;
  float nota1;
  float nota2;
  float media;
} aluno_t;

char *lerStringEntreAspas(FILE *arq) {
  char *str = (char *)malloc(31 * sizeof(char));
  int pos = 0;
  int c;

  // Encontra a primeira aspa
  while ((c = fgetc(arq)) != EOF && c != '\"') {
  }

  // Se não encontrou a primeira aspa, retorna NULL
  if (c != '\"') {
    free(str);
    return NULL;
  }

  bool encontrarAspaFinal = false;
  // Lê os caracteres entre as aspas até atingir o limite de 30 ou encontrar a
  // segunda aspa
  while ((c = fgetc(arq)) != EOF) {
    if (c == '\"') {
      encontrarAspaFinal = true;
      break;
    }
    if (pos < 30) {
      str[pos++] = (char)c;
    }
  }

  // Se não encontrou a segunda aspa, retorna NULL
  if (!encontrarAspaFinal) {
    free(str);
    return NULL;
  }

  str[pos] = '\0';
  return str;
}

int le_arq(aluno_t **alunos, char *nome_do_arquivo) {
  int num_alunos;
  FILE *file = fopen(nome_do_arquivo, "r");

  if (!file) {
    return -1;
  }
  if (fscanf(file, "%d", &num_alunos) != 1) {
    return -1;
  }

  *alunos = (aluno_t *)malloc(num_alunos * sizeof(aluno_t));

  /*
      Preciso mudar a lógica da string, precisa estar entre "" no arquvio txt,
     logo, preciso criar uma função especial para identificar a primeira aspa e
     a partir dela gravar os caracteres da string, ou seja, ela vai gravar até
     achar a outra aspa, ou quando atingir o espaço, 30.
  */

  for (int i = 0; i < num_alunos; i++) {
    fscanf(file, "%d", &((*alunos)[i].matricula));

    (*alunos)[i].nome = lerStringEntreAspas(file);
    if (!(*alunos)[i].nome) {
      return -1;
    }

    fscanf(file, "%f %f", &((*alunos)[i].nota1), &((*alunos)[i].nota2));

    int c;
    while ((c = fgetc(file)) != '\n' && c != EOF) {
    }
  }

  /*
      Para evitar a leitura de uma terceira nota, fica-se fazendo
      a leitura até o final da linha ou até o final do arquivo (EOF).
      Assim, o loop vai lendo os dados, mas sem armazená-los, acabando
      quando inicia-se uma nova linha, ou quando o arquivo acaba.
  */

  fclose(file);

  return num_alunos;
}

void grava_arq(int n, aluno_t *alunos, char *nome_do_arquivo) {
  FILE *file = fopen(nome_do_arquivo, "w");
  if (!file) {
    return;
  }

  fprintf(file, "%d\n", n);
  for (int i = 0; i < n; i++) {
    fprintf(file, "%d %s %.1f %.1f %.1f\n", alunos[i].matricula, alunos[i].nome,
            alunos[i].nota1, alunos[i].nota2, alunos[i].media);
  }

  fclose(file);
}

void calcula_medias(int n, aluno_t alunos[n]) {
  for (int i = 0; i < n; i++) {
    alunos[i].media = (alunos[i].nota1 + alunos[i].nota2) / 2;
  }
}

int main() {
  /*
    Aqui há duas maneiras de lidar com os arquivos, ler um e modificar
    sobre ele mesmo ou ler um arquivo de entrada e salvar as alterações
    em outro de saída, o que facilita o teste do código. Deixei comentado.
  */
  char *entrada = "entrada.txt";
  // Se quiser, comentar aqui abaixo e utilizar entrada.txt com saída também
  char *saida = "saida.txt";

  aluno_t *alunos;

  int n = le_arq(&alunos, entrada);
  if (n < 0) {
    printf("ERRO NA LEITURA DO ARQUIVO");
    return 0;
  }

  calcula_medias(n, alunos);
  // Não precisa passar &aluno pois não há modificação do ponteiro

  // Aqui, se estiver usando o mesmo arquivo como entrada e saída, basta
  // alterar as chamadas de função;
  // grava_arq(n, alunos, entrada);
  grava_arq(n, alunos, saida);

  // Aqui estou liberando a alocação de memória dos nomes
  // e da struct.
  for (int i = 0; i < n; i++) {
    free(alunos[i].nome);
  }
  free(alunos);
  return 0;
}