#include <stdio.h>
#include <stdlib.h>
#include "jogo.h"
#include <string.h>
#include "estado.h"
#define SCALE           50
#define TAM             10
#define MAX_BUFFER      10240
char* images[] = {
    "Pirate_23.png",
    "Orc_04.png", 
    "lava_pool1.png",
};

#define PLAYER_IMG      0
#define INIMIGO_IMG     1
#define OBSTACULO_IMG   2

void print_cell(int C, int L);

int valid_move(int x, int y) {
    return x >= 0 && x < TAM && y >= 0 && y < TAM;
}

void print_move(int x, int y) {
    if (!valid_move(x, y))
        return;

    printf("<a xlink:href=http://localhost/cgi-bin/jogo?%d,%d>\n", 
        x, y);
    print_cell(x, y);
    printf("</a>\n");
}

void print_moves(int x, int y) {
    POSICAO moves[] = {
        { -1,  0},
        { +1,  0},
        {  0, -1},
        {  0, +1},
        { -1, -1},
        { -1, +1},
        { +1, -1},
        { +1, +1},
    };
    int count = 8;
    int i;
    for(i = 0; i < count; i++)
        print_move(x + moves[i].x, y + moves[i].y);
}

void print_cell(int C, int L) {
   char * cor[] = {"#666600", "#555500"};
   int IDX = (L + C) % 2;
   printf("<rect x=%d y=%d width=%d height=%d fill=%s />\n",
        SCALE * C, SCALE * L, SCALE, SCALE, cor[IDX]);
}

void print_board() {
    int L, C;
    for(L = 0; L < TAM; L++)
        for(C = 0; C < TAM; C++) 
            print_cell(C, L);
}

void print_image(int x, int y, int img_idx) {
   char* nome = images[img_idx];
   printf("<image x=%d y=%d width=%d height=%d xlink:href=http://localhost/images/%s />\n",    
       SCALE * x, SCALE * y, SCALE, SCALE, nome);
}    

void print_player(ESTADO e) {
   int x = e.jog.x;
   int y = e.jog.y;
   print_image(x, y, PLAYER_IMG);
   print_moves(x, y);
}

ESTADO inicializar() {
    ESTADO e;
    e.jog.x = 5;
    e.jog.y = 9;
    e.num_inimigos = 3;
    e.inimigo[0].x = 7;
    e.inimigo[0].y = 4;
    e.inimigo[1].x = 4;
    e.inimigo[1].y = 5;
    e.inimigo[2].x = 1;
    e.inimigo[2].y = 2;
    e.num_obstaculos = 2;
    e.obstaculo[0].x = 2;
    e.obstaculo[0].y = 4;
    e.obstaculo[1].x = 3;
    e.obstaculo[1].y = 8;
    return e;
}

void print_obstaculos(ESTADO e) {
    int i;
    for(i=0;i < e.num_obstaculos;i++)
        print_image(e.obstaculo[i].x, e.obstaculo[i].y, OBSTACULO_IMG);
}

void print_inimigos(ESTADO e) {
    int i;
    for(i=0;i < e.num_inimigos;i++)
        print_image(e.inimigo[i].x, e.inimigo[i].y, INIMIGO_IMG);
}

  
ESTADO ler_inimigos(ESTADO e, char* args) {
    int i;
    char resto[MAX_BUFFER];
    sscanf(args, "%d,%s", &e.num_inimigos, resto);
    for(i = 0;i < e.num_inimigos;i++) {
        sscanf(resto, "%d,%d,%s", &e.inimigo[i].x, &e.inimigo[i].y, resto);
        printf("#'%s'\n", resto);
    }
    return e;
}

ESTADO str2estado(char* args) {
    ESTADO e;
    int x,y;
    char resto[MAX_BUFFER];
    sscanf(args, "%d,%d,%s", &x, &y, resto);
    e.jog.x = x;
    e.jog.y = y;
    
    e = ler_inimigos(e, resto);
    return e;
}

void parser(char* args) {
    ESTADO e;
    if(strlen(args) != 0)
        e = str2estado(args);
    else
        e = inicializar();

    print_board();
    print_player(e);
    print_inimigos(e);
    print_obstaculos(e);
}


