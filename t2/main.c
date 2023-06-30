#include "str.h"
#include <stdio.h>

int main() {
  Str s = str_cria("\U0001F603ó");
  printf("%d %d\n", str_tam(s), str_numbytes(s));

  Str hello = str_cria("Olá");
  Str world = str_cria(" Mundo!");
  str_altera(hello, 3, 0, world);
  
  printf("%s\n", str_data(hello)); // Deve imprimir "Olá Mundo!"

  str_destroi(s);
  str_destroi(hello);
  str_destroi(world);
  return 0;
}