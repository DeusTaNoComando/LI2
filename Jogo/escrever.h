#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "estado.h"

/**
@file escrever.h
Ficheiro auxiliar a escrever.c que escreve o save do jogo
*/

/**
\brief Função que escreve o ficheiro a ser gravado
@param * conteudo Conteúdo do estado do jogo
@param * fich Ficheiro que vai ser escrito
*/
void escrever(char * conteudo, char * fich) {
  FILE* f;

  if((f = fopen(fich,"w")) == NULL) {
    perror("Não consegui abrir o ficheiro");
    exit(1);
  }
  fprintf(f, "%s\n", conteudo);
  fclose (f);
}
