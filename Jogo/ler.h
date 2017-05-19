#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estado.h"

#define MAX_BUFFER       10240

void ler(char * conteudo, char * fich) {
  FILE* f;

  f = fopen(fich,"r");
  fscanf(f, "%s", conteudo);
  fclose (f);
}
