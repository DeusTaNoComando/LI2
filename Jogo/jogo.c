#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cgi.h"
#include "estado.h"

#define MAX_BUFFER		10240
#define TAM				10
#define ESCALA			40

int posicao_valida(int x, int y) {
	return x >= 0 && y >= 0 && x < TAM && y < TAM;
}

int posicao_igual(POSICAO p, int x, int y) {
    return p.x == x && p.y == y;
}

int tem_porta(ESTADO e, int x, int y) {
	return (posicao_igual(e.porta_entrada, x, y) || posicao_igual(e.porta_saida, x, y));
}

int tem_jogador(ESTADO e, int x, int y) {
    return posicao_igual(e.jog, x, y);
}

int tem_inimigo(ESTADO e, int x, int y) {
    int i;
    for(i = 0;i < e.num_inimigos;i++)
        if (posicao_igual(e.inimigo[i], x, y))
            return 1;
    return 0;
}

int tem_obstaculo(ESTADO e, int x, int y) {
    int i;
    for(i = 0;i < e.num_obstaculos;i++)
        if (posicao_igual(e.obstaculo[i], x, y))
            return 1;
    return 0;
}

int posicao_ocupada(ESTADO e, int x, int y) {
    return tem_jogador(e, x, y) || tem_inimigo(e, x, y) || tem_obstaculo(e, x, y) || tem_porta(e, x,y);
}

void imprime_quadr_link (int x, int y, char* link) {
	ABRIR_LINK(link);
	QUADRADO_LINK(x, y, ESCALA);
	FECHAR_LINK;
}

void imprime_casa_iluminada (ESTADO e, int x, int y) {
	if (!posicao_valida(x,y)) return;

	if (tem_inimigo(e, x, y))
		if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_R.png");
		else IMAGEM(x, y, ESCALA, "Ground2_R.png");
	else if (posicao_igual(e.porta_saida, x, y))
		if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_G.png");
		else IMAGEM(x, y, ESCALA, "Ground2_G.png");
	else if (tem_obstaculo(e, x,y ) || posicao_igual(e.porta_entrada, x, y))
		if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_BW.png");
		else IMAGEM(x, y, ESCALA, "Ground2_BW.png");
	else
		if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_L.png");
		else IMAGEM(x, y, ESCALA, "Ground2_L.png");
	}

void imprime_casa(int x, int y) {
	int idx = (x + y) % 2;
	if (idx == 0) IMAGEM(x, y, ESCALA, "Ground1_Y.png");
	else IMAGEM(x, y, ESCALA, "Ground2_B.png");
}

ESTADO inicializar_inimigo(ESTADO e) {
    int x, y;
    do {
        x = random() % TAM;
        y = random() % TAM;
    } while(posicao_ocupada(e, x, y));

    e.inimigo[(int)e.num_inimigos].x = x;
    e.inimigo[(int)e.num_inimigos].y = y;
    e.num_inimigos++;

    return e;
}

ESTADO inicializar_inimigos(ESTADO e, int num) {
    int i;
    for(i = 0; i < num; i++)
        e = inicializar_inimigo(e);
    return e;
}

ESTADO inicializar_obstaculo(ESTADO e) {
    int x, y;
    do {
        x = random() % TAM;
        y = random() % TAM;
    } while(posicao_ocupada(e, x, y));

    e.obstaculo[(int)e.num_obstaculos].x = x;
    e.obstaculo[(int)e.num_obstaculos].y = y;
    e.num_obstaculos++;

    return e;
}

ESTADO inicializar_obstaculos(ESTADO e, int num) {
    int i;
    for(i = 0; i < num; i++)
        e = inicializar_obstaculo(e);
    return e;
}

ESTADO inicializar() {
	ESTADO e = {{0}};
	e.porta_entrada.x = 9;
	e.porta_entrada.y = 9;
	e.porta_saida.x = 0;
	e.porta_saida.y = 0;
	e.jog.x = 9;
	e.jog.y = 9;
	e = inicializar_inimigos(e, 20);
	e = inicializar_obstaculos(e, 20);
	e.fase = 0;
	e.score = 0;
	e.ilumina.x = TAM; e.ilumina.y = TAM;
	e.PU_Shield = 0;
	e.num_vidas = 2;
	return e;
}

ESTADO apaga_inimigo (ESTADO e, int x, int y) {
	int i;
	for (i=0; i < (int)e.num_inimigos; i++)
		if (e.inimigo[i].x == x && e.inimigo[i].y == y) {
				for (; i < (int)e.num_inimigos; i ++) e.inimigo[i] = e.inimigo[i+1];
				e.inimigo[i].x = 0; e.inimigo[i].y = 0;
				e.num_inimigos--;
				e.score++;
		}
	return e;
}

void imprime_movimento(ESTADO e, int dx, int dy) {
	ESTADO novo = e;
	int x = e.jog.x + dx;
	int y = e.jog.y + dy;
	char link[MAX_BUFFER];
	if(!posicao_valida(x, y))
		return;
    if(posicao_ocupada(e, x, y))
        if (!tem_inimigo(e, x, y)) return;

	novo.jog.x = x; novo.jog.y = y;
	novo.fase = 1;
	novo.ilumina.x = TAM; novo.ilumina.y = TAM;
	if (novo.PU_Shield > 0) novo.PU_Shield--;

	if (tem_inimigo(e, x, y)) novo = apaga_inimigo(novo, x, y);
	sprintf(link, "http://localhost/cgi-bin/jogo?%s", estado2str(novo));
	imprime_quadr_link(x, y, link);
}

void imprime_movimentos(ESTADO e) {
	int dx, dy;
    for(dx = -1;dx <= 1;dx++)
        for(dy = -1;dy <= 1;dy++)
            imprime_movimento(e, dx, dy);
}

void cria_posicao_a_iluminar(ESTADO e, int x, int y) {
	ESTADO novo = e;
	if (posicao_igual(novo.ilumina, x, y)) {novo.ilumina.x = TAM; novo.ilumina.y = TAM; novo.fase = 0;}
	else {novo.ilumina.x = x; novo.ilumina.y = y; novo.fase = 0;}

	char link[MAX_BUFFER];
	sprintf(link, "http://localhost/cgi-bin/jogo?%s", estado2str(novo));
	imprime_quadr_link(x, y, link);
}

void imprime_jogador(ESTADO e) {
	if (!posicao_valida(e.jog.x, e.jog.y)) return;
	IMAGEM(e.jog.x, e.jog.y, ESCALA, "Warrior.png");
	cria_posicao_a_iluminar(e, e.jog.x, e.jog.y);
	imprime_movimentos(e);
}

void imprime_inimigos(ESTADO e) {
	int i;
	for(i = 0; i < (int)e.num_inimigos; i++)
		IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "Goon.png");
		cria_posicao_a_iluminar(e, e.inimigo[i].x, e.inimigo[i].y);
}

void imprime_obstaculos(ESTADO e) {
	int i;
	for(i = 0; i < e.num_obstaculos; i++)
		IMAGEM(e.obstaculo[i].x, e.obstaculo[i].y, ESCALA, "Lava.png");

}

void imprime_porta(ESTADO e) {
			IMAGEM(e.porta_entrada.x, e.porta_entrada.y, ESCALA, "Bottom_hole.png");
			IMAGEM(e.porta_saida.x, e.porta_saida.y, ESCALA, "Hole.png");

			if (abs(e.jog.x - e.porta_saida.x) <= 1 && abs(e.jog.y - e.porta_saida.y) <=1) {
				char link[MAX_BUFFER];
				ESTADO novo = inicializar();
				novo.score = e.score;
				novo.num_vidas = e.num_vidas;
				sprintf (link, "http://localhost/cgi-bin/jogo?%s", estado2str(novo));

				imprime_quadr_link (e.porta_saida.x, e.porta_saida.y, link);
			}
}

ESTADO mover_inimigos (ESTADO e) {
	int i;
	for (i=0; i<(int)e.num_inimigos; i++)
		if ((abs (e.jog.x - e.inimigo[i].x) == 1 && abs(e.jog.y - e.inimigo[i].y) == 0 && e.num_vidas == 0) || (abs (e.jog.x - e.inimigo[i].x) == 0 && abs(e.jog.y - e.inimigo[i].y) == 1 && e.num_vidas == 0)) {
			e.inimigo[i].x = e.jog.x; e.inimigo[i].y = e.jog.y;
			e.jog.x = TAM; e.jog.y = TAM;
			e.num_vidas--;
		}
		else if ((abs (e.jog.x - e.inimigo[i].x) == 1 && abs(e.jog.y - e.inimigo[i].y) == 0) || (abs (e.jog.x - e.inimigo[i].x) == 0 && abs(e.jog.y - e.inimigo[i].y) == 1)) {
			e.num_vidas--;
			}
	return e;
}

void iluminar_inimigo (ESTADO e, int x, int y) {
	int dx, dy;
	for (dx=-1; dx<= 1; dx++)
		for (dy=-1; dy<= 1; dy++)
			if (dx == 0 || dy == 0) imprime_casa_iluminada(e, x + dx, y + dy);
}

void iluminar_jogador (ESTADO e, int x, int y) {
	int dx, dy;
	for (dx=-1; dx<= 1; dx++)
		for (dy=-1; dy<= 1; dy++) imprime_casa_iluminada(e, x + dx, y + dy);
}

void imprime_ilumi (ESTADO e) {
	int x = e.ilumina.x;
	int y = e.ilumina.y;

	if (tem_jogador(e, x, y)) iluminar_jogador (e, x, y);
	else if (tem_inimigo(e, x, y)) iluminar_inimigo (e, x, y);
}

void imprime_shield (ESTADO e) {
	ESTADO novo = e;
	novo.PU_Shield = 2;
	char link[MAX_BUFFER];
	sprintf(link, "http://localhost/cgi-bin/jogo?%s", estado2str(novo));

	IMAGEM(0, TAM, ESCALA, "Shield.png");
	imprime_quadr_link(0, TAM, link);
}

void imprime_sword (ESTADO e) {
	IMAGEM(1, TAM, ESCALA, "Sword.png");
}

void imprime_vidas (ESTADO e) {
	int i;
	for (i=0; i<=e.num_vidas; i++) IMAGEM((5+i), TAM, ESCALA, "Heart3.png");
	for (; i<5; i++) IMAGEM((5+i), TAM, ESCALA, "Heart_Blocked.png");
}

void jogo(ESTADO e) {
	int x, y;

	for(y = 0; y < TAM; y++)
		for(x = 0; x < TAM; x++)
			imprime_casa(x, y);

	for (x=0; x < TAM; x++) {
		if (((x+y)%2) == 0) IMAGEM(x, y, ESCALA, "Ground1_LY.png");
		else IMAGEM(x, y, ESCALA, "Ground2_LY.png");
	}

	if (posicao_valida (e.ilumina.x, e.ilumina.y)) imprime_ilumi(e);

	imprime_obstaculos(e);

	if (e.fase != 0 && e.PU_Shield < 1) e = mover_inimigos(e);

	imprime_inimigos(e);
	imprime_porta(e);
	imprime_jogador(e);

	imprime_shield(e);
	imprime_sword(e);
	imprime_vidas(e);

}

void imprimir_link_jogar() {
	char link[MAX_BUFFER];
	sprintf(link, "http://localhost/cgi-bin/jogo?%s", estado2str(inicializar()));

	int c;
	for (c=0; c<6; c++) {
		if (((c+2)+4)%2) IMAGEM((c+2), 4, ESCALA, "Ground1_LY.png");
		else IMAGEM((c+2), 4, ESCALA, "Ground2_LY.png");
	}

	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d xlink:href=%s />\n", 3*ESCALA , 4*ESCALA, 4*ESCALA, ESCALA, "http://localhost/images/Jogar.png");
	FECHAR_LINK;
}

void interface() {
	int l,c;
	for (l=0; l<TAM; l++)
		for (c=0; c<=TAM; c++)
			if ((c+l)%2) IMAGEM(l, c, ESCALA, "Ground1_B.png");
			else IMAGEM(l, c, ESCALA, "Ground2_B.png");

	imprimir_link_jogar();
}

void ler_estado(char *args) {
	if(strlen(args) == 0)
		return interface();
	return jogo(str2estado(args));
}

int main() {
    srandom(time(NULL));

	COMECAR_HTML;
	ABRIR_SVG(600, 600);

	ler_estado(getenv("QUERY_STRING"));

	FECHAR_SVG;

	return 0;
}
