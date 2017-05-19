#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "estado.h"

void escrever(char * conteudo, char * fich) {
  FILE* f;

  if((f = fopen(fich,"w")) == NULL) {
    perror("Não consegui abrir o ficheiro");
    exit(1);
  }
  fprintf(f, "%s\n", conteudo);
  fclose (f);
}
