#ifndef STR_H
#define STR_H

#include <stdbool.h>
#include <stdio.h>

typedef struct str *Str;

Str str_cria(const char *s);
void str_destroi(Str s);
int str_tam(Str s);  // retorna o número de caracteres Unicode na string
int str_numbytes(Str s); // retorna o número de bytes ocupados pela string
Str str_substr(Str s, int inicio, int tam);
void str_altera(Str dest, int inicio, int tam, Str orig);
char *str_data(Str s);


#ifdef TESTE
void str_teste();
#endif

#endif
