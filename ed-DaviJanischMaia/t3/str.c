#include "str.h"
#include <string.h>
#include <stdlib.h>

// Implementação parcial do TAD Str, visto em aula
// Não implementa suporte a UTF8

// uma Str está sendo implementada como somente um ponteiro para a
//   área que contém a string. Poderia manter outros dados aqui,
//   para simplificar ou acelerar a implementação de alguma operação
//   (tamanho da string, por exemplo)
struct _str {
  char *bytes;
};

// função auxiliar que retorna o valor de 'a', ajustado entre min e max
static int ajusta(int a, int min, int max)
{
  if (a<min) return min;
  if (a>max) return max;
  return a;
}

Str str_cria(char *s)
{
  Str novo;
  novo = malloc(sizeof(struct _str));
  if (novo != NULL) {
    novo->bytes = malloc(strlen(s)+1);
    if (novo->bytes != NULL) {
      strcpy(novo->bytes, s);
    } else {
      free(novo);
      novo = NULL;
    }
  }
  return novo;
}

Str str_cria_linha(FILE *arq)
{
  Str res = str_cria("");
  const int tam = 20;
  char s[tam];
  bool achei_o_fim_da_linha = false;
  while (!achei_o_fim_da_linha && fgets(s, tam, arq) != NULL) {
    int n = strlen(s);
    if (s[n-1] == '\n') {
      s[n-1] = '\0';
      achei_o_fim_da_linha = true;
    }
    Str tmp = str_cria(s);
    str_altera(res, -1, 0, tmp);
    str_destroi(tmp);
  }
  if (!achei_o_fim_da_linha && str_tam(res) == 0) {
    str_destroi(res);
    return NULL;
  }
  return res;
}

void str_destroi(Str s)
{
  if (s != NULL) {
    free(s->bytes);
    free(s);
  }
}

static int utf8_strlen(char *s)
{
  int len = 0;
  while (*s != '\0') {
    s += utf8_num_bytes(*s);
    len++;
  }
  return len;
}

int str_tam(Str s)
{
  if (s == NULL) return 0;
  return utf8_strlen(s->bytes); // Uso de utf8_strlen em vez de strlen
}

int str_numbytes(Str s)
{
  if (s == NULL) return 0;
  return strlen(s->bytes);
}

chu str_char(Str s, int i)
{
  int t = str_tam(s);
  if (i < 0) {
    i += t;
  }
  if (i >= t || i < 0) return -1;
  return chu_de_utf8(utf8_nesimo_chu(s->bytes, i));
}

// p é uma posição em s, podendo ser positiva (medida a partir do início)
//   ou negativa (medida a partir do final)
// n é o tamanho de uma substring a partir de p
// esta função corrige p para ser positivo e estar em uma posição
//   existente de s, e n para representar um tamanho válido em s
static void ajeita_pos_e_tam_de_substr(Str s, int *p, int *n)
{
  int t = str_tam(s);
  if (*p < 0) {
    // converte posições negativas no equivalente positivo
    //   se p for -1, vira t; se for -2, vira t-1 etc
    *p = *p + t + 1;
  }
  // faz p ficar nos limites da string (entre 0 e t)
  *p = ajusta(*p, 0, t);  // este era o bug em aula, faltava ajustar p
  // faz n ficar nos limites da string
  *n = ajusta(*n, 0, t - *p);
}

static char *utf8_char_pointer(char *s, int n) {
    int i = 0;
    while (*s != '\0' && i < n) {
        s += utf8_num_bytes(*s);
        i++;
    }
    return s;
}

Str str_substr(Str s, int p, int n) {
    int tam = str_tam(s);
    p = ajusta(p, 0, tam);
    n = ajusta(n, 0, tam - p);

    char *inicio = utf8_char_pointer(s->bytes, p); // Ponteiro para o início da substring
    char *fim = utf8_char_pointer(inicio, n); // Ponteiro para o final da substring

    int tam_bytes = fim - inicio;
    char *nova_bytes = malloc(tam_bytes + 1);

    strncpy(nova_bytes, inicio, tam_bytes);
    nova_bytes[tam_bytes] = '\0';

    Str nova_str = str_cria(nova_bytes);
    free(nova_bytes);
    return nova_str;
}


// retorna a posição da primeira ocorrência do caractere 'c' em 's' ou -1
int str_poschar(Str s, chu c)
{
  int i = 0;
  char *ptr = s->bytes;
  while (*ptr != '\0') {
    if (chu_de_utf8(ptr) == c) return i;
    ptr += utf8_num_bytes(*ptr);
    i++;
  }
  return -1;
}

// retorna 'true' se as strings em 's' e 'o' forem iguais
bool str_igual(Str s, Str o)
{
  if (s == o) return true;
  if (s == NULL || o == NULL) return false;
  return strcmp(s->bytes, o->bytes) == 0;
}

// altera 's', substituindo os 'n' caracteres a partir de 'p' pelo conteúdo de 'o'
// valores negativos de 'n' são tratados como 0
// se 'p' além do final de 's', deve ser tratado como logo após o final
// se 'p' antes do início de 's', deve ser tratado cono logo antes do início
// valores negativos de 'p' referem-se ao final de 's' (-1 é logo após o final de 's',
//  -2 logo antes do último caractere, etc.)
void str_altera(Str s, int p, int n, Str o) {
    if (s == NULL || o == NULL) return;

    int tam = str_tam(s);
    p = ajusta(p, 0, tam);
    n = ajusta(n, 0, tam - p);

    char *inicio = utf8_char_pointer(s->bytes, p); // Ponteiro para o início da substring
    char *fim = utf8_char_pointer(inicio, n); // Ponteiro para o final da substring

    int tam_bytes_esquerda = inicio - s->bytes;
    int tam_bytes_direita = strlen(fim);
    int tam_bytes_o = strlen(o->bytes);

    char *nova_bytes = malloc(tam_bytes_esquerda + tam_bytes_o + tam_bytes_direita + 1);

    strncpy(nova_bytes, s->bytes, tam_bytes_esquerda);
    strncpy(nova_bytes + tam_bytes_esquerda, o->bytes, tam_bytes_o);
    strncpy(nova_bytes + tam_bytes_esquerda + tam_bytes_o, fim, tam_bytes_direita);
    nova_bytes[tam_bytes_esquerda + tam_bytes_o + tam_bytes_direita] = '\0';

    free(s->bytes);
    s->bytes = nova_bytes;
}

void str_cstring(Str s, char *p)
{
  if (s == NULL) {
    *p = '\0';
  } else {
    strcpy(p, s->bytes);
  }
}

void str_grava(Str s, FILE *arq)
{
  fputs(s->bytes, arq);
}

#ifdef TESTE
// testa o TAD

#include <assert.h>

void str_teste(void)
{
  Str s1, s2, s3, s4;
  s1 = str_cria("aba");
  s2 = str_cria("caxi");
  s3 = str_cria("abacaxi");
  str_altera(s1, -1, 0, s2);
  assert(str_igual(s1, s3));
  s4 = str_substr(s2, 20, 10);
  assert(str_tam(s4) == 0);
  str_altera(s3, 0, 3, s4);
  assert(str_igual(s2, s3));
}
#endif
