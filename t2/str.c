#include "str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct str {
  char *data;
  int size;   // Tamanho em bytes
  int length; // Tamanho em caracteres Unicode
};

int utf8_num_bytes(char b) {
  // se o byte for 0xxxxxxx, o caractere ocupa só um byte (código entre 0000 e
  // 007F)
  if ((b & 0b10000000) == 0b00000000)
    return 1;
  // se o byte for 110xxxxx, o caractere ocupa 2 bytes, 5 bits no primeiro e 6
  // no segundo (0080 a 07FF)
  if ((b & 0b11100000) == 0b11000000)
    return 2;
  // se o byte for 1110xxxx, o caractere ocupa 3 bytes, 4 bits no primeiro e 6
  // nos outros (0800 a FFFF)
  if ((b & 0b11110000) == 0b11100000)
    return 3;
  // se o byte for 11110xxx, o caractere ocupa 4 bytes, 3 bits no primeiro e 6
  // nos outros (10000 a 10FFFF)
  if ((b & 0b11111000) == 0b11110000)
    return 4;
  // se começar com outra coisa, temos um problema
  return 0;
}

int utf8_para_codigo_unicode(char *p) {
  int n = utf8_num_bytes(p[0]);
  if (n == 0)
    return 0; // erro!
  if (n == 1)
    return p[0];
  if (n == 2)
    return ((p[0] & 0b00011111) << 6) | (p[1] & 0b00111111);
  if (n == 3)
    return ((p[0] & 0b00001111) << 12) | ((p[1] & 0b00111111) << 6) |
           (p[2] & 0b00111111);
  if (n == 4)
    return ((p[0] & 0b00000111) << 18) | ((p[1] & 0b00111111) << 12) |
           ((p[2] & 0b00111111) << 6) | (p[3] & 0b00111111);
  // erro se chegar aqui
}

void utf8_de_codigo_unicode(int c, char s[5]) {
  if (c <= 0x007F) {
    s[0] = c;
    s[1] = '\0';
  } else if (c <= 0x07FF) {
    s[0] = (c >> 6) | 0b11000000;
    s[1] = (c & 0b00111111) | 0b10000000;
    s[2] = '\0';
  } else if (c <= 0xFFFF) {
    s[0] = (c >> 12) | 0b11100000;
    s[1] = ((c >> 6) & 0b00111111) | 0b10000000;
    s[2] = (c & 0b00111111) | 0b10000000;
    s[3] = '\0';
  } else if (c <= 0x10FFFF) {
    s[0] = (c >> 18) | 0b11110000;
    s[1] = ((c >> 12) & 0b00111111) | 0b10000000;
    s[2] = ((c >> 6) & 0b00111111) | 0b10000000;
    s[3] = (c & 0b00111111) | 0b10000000;
    s[4] = '\0';
  } else {
    // erro!
    s[0] = '\0';
  }
}

Str str_cria(const char *s) {
  Str nova = malloc(sizeof(struct str));
  nova->data = strdup(s);
  nova->size = strlen(s);

  // Calcula o número de caracteres Unicode na string
  int len = 0;
  for (int i = 0; i < nova->size;) {
    int num_bytes = utf8_num_bytes(nova->data[i]);
    if (num_bytes == 0) {
      // Tratar erro...
    }
    i += num_bytes;
    len++;
  }
  nova->length = len;

  return nova;
}

void str_destroi(Str s) {
  free(s->data);
  free(s);
}

int str_tam(Str s) { return s->length; }

int str_numbytes(Str s) { return s->size; }

Str str_substr(Str s, int inicio, int tam) {
    if (inicio < 0 || inicio + tam > s->length) {
        // Tratar erro...
        return NULL;
    }

    // Encontrar o início em bytes da substring
    int byte_inicio = 0;
    int unicode_index = 0;
    while (unicode_index < inicio) {
        byte_inicio += utf8_num_bytes(s->data[byte_inicio]);
        unicode_index++;
    }

    // Encontrar o tamanho em bytes da substring
    int byte_tam = 0;
    for (int i = 0; i < tam; i++) {
        byte_tam += utf8_num_bytes(s->data[byte_inicio + byte_tam]);
    }

    // Criar a nova substring
    Str sub = malloc(sizeof(struct str));
    sub->data = malloc(byte_tam + 1);
    memcpy(sub->data, s->data + byte_inicio, byte_tam);
    sub->data[byte_tam] = '\0';
    sub->size = byte_tam;
    sub->length = tam;

    return sub;
}

void str_altera(Str dest, int inicio, int tam, Str orig) {
  // Encontrar o início em bytes
  int byte_inicio = 0;
  int unicode_index = 0;
  while (unicode_index < inicio) {
    byte_inicio += utf8_num_bytes(dest->data[byte_inicio]);
    unicode_index++;
  }

  // Encontrar o tamanho em bytes
  int byte_tam = 0;
  for (int i = 0; i < tam; i++) {
    byte_tam += utf8_num_bytes(dest->data[byte_inicio + byte_tam]);
  }

  // Criar espaço para a nova string
  int new_size = dest->size - byte_tam + orig->size;
  char *new_data = malloc(new_size + 1);

  // Copiar a parte inicial
  memcpy(new_data, dest->data, byte_inicio);

  // Copiar os dados de 'orig'
  memcpy(new_data + byte_inicio, orig->data, orig->size);

  // Copiar a parte final
  memcpy(new_data + byte_inicio + orig->size,
         dest->data + byte_inicio + byte_tam,
         dest->size - byte_inicio - byte_tam);

  // Atualizar metadados
  new_data[new_size] = '\0';
  free(dest->data);
  dest->data = new_data;
  dest->size = new_size;
  dest->length = dest->length - tam + orig->length;
}

char *str_data(Str s) {
    if (!s) {
        return NULL;
    }
    return s->data;
}

#ifdef TESTE
void str_teste() {
  // Teste 1: Criar uma string e verificar tamanho e número de bytes
  Str teste1 = str_cria("\U0001F603ó");
  printf("Teste 1: ");
  if (str_tam(teste1) == 2 && str_numbytes(teste1) == 6) {
    printf("Passou\n");
  } else {
    printf("Falhou\n");
  }
  str_destroi(teste1);

  // Teste 2: Criar uma string ASCII e verificar tamanho e número de bytes
  Str teste2 = str_cria("Hello");
  printf("Teste 2: ");
  if (str_tam(teste2) == 5 && str_numbytes(teste2) == 5) {
    printf("Passou\n");
  } else {
    printf("Falhou\n");
  }
  str_destroi(teste2);

  // Teste 3: Verificar str_substr com uma string UTF-8
  Str teste3 = str_cria("ação");
  Str subTeste3 = str_substr(teste3, 1, 2);
  printf("Teste 3: ");
  if (strcmp(subTeste3->data, "ção") == 0) {
    printf("Passou\n");
  } else {
    printf("Falhou\n");
  }
  str_destroi(teste3);
  str_destroi(subTeste3);

  // Teste 4: Verificar str_altera
  Str teste4a = str_cria("Olá ");
  Str teste4b = str_cria("Mundo!");
  str_altera(teste4a, 3, 0, teste4b);
  printf("Teste 4: ");
  if (strcmp(teste4a->data, "Olá Mundo!") == 0) {
    printf("Passou\n");
  } else {
    printf("Falhou\n");
  }
  str_destroi(teste4a);
  str_destroi(teste4b);
}
#endif
