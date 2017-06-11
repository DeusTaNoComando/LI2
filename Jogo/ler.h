#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estado.h"

#define MAX_BUFFER       10240

/**
@file ler.h
Ficheiro auxiliar a ler.c que lê o conteúdo de um ficheiro
*/

/**
\brief Função que lê o conteúdo de um ficheiro anteriormente guardado
@param * conteudo Conteúdo do ficheiro
@param * fich Ficheiro que contém a informação de jogo guardada
*/
void ler(char * conteudo, char * fich) {
  FILE* f;

  f = fopen(fich,"r");
  fscanf(f, "%s", conteudo);
  fclose (f);
}
