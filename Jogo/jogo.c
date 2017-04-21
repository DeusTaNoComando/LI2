#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cgi.h"
#include "estado.h"

#define MAX_BUFFER		10240
#define TAM				10
#define ESCALA			40

int posicao_valida(int x, int y) {
	return (x >= 0 && y >= 0 && x < TAM && y < TAM);
}

int posicao_igual(POSICAO p, int x, int y) {
    return p.x == x && p.y == y;
}

int tem_item (ESTADO e, int x, int y) {
	int i;
	for(i = 0;i < e.num_items;i++)
			if (posicao_igual(e.items[i], x, y))
					return 1;
	return 0;
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

int tem_inim_longe (ESTADO e, int x, int y) {
	int i;
	for(i = 0; i < e.num_inimigos_longe;i++)
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

int tem_espada(ESTADO e, int x, int y) {
	return posicao_igual(e.espada, x, y);
}

int posicao_ocupada(ESTADO e, int x, int y) {
    return tem_jogador(e, x, y) || tem_inimigo(e, x, y) || tem_obstaculo(e, x, y) || tem_porta(e, x, y) || tem_espada(e, x, y) || tem_item(e, x, y);
}

void imprime_quadr_link (int x, int y, char* link) {
	ABRIR_LINK(link);
	QUADRADO_LINK(x, y, ESCALA);
	FECHAR_LINK;
}

void cores_inimigo (ESTADO e, int x, int y) {
	if (tem_obstaculo(e, x, y) || tem_porta(e, x, y) || tem_inimigo (e, x, y))
		if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_BW.png");
		else IMAGEM(x, y, ESCALA, "Ground2_BW.png");
	else if (tem_jogador(e, x, y))
		if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_R.png");
		else IMAGEM(x, y, ESCALA, "Ground2_R.png");
	else
		if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_L.png");
		else IMAGEM(x, y, ESCALA, "Ground2_L.png");
}

void cores_jogador (ESTADO e, int x, int y) {
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

void imprime_casa_iluminada (ESTADO e, int x, int y, int jog_inim) {
	if (!posicao_valida(x,y)) return;

	if (jog_inim) cores_inimigo (e, x, y);
	else cores_jogador(e, x, y);

	}

void imprime_casa(int x, int y) {
	int idx = (x + y) % 2;
	if (idx == 0) IMAGEM(x, y, ESCALA, "Ground1_Y.png");
	else IMAGEM(x, y, ESCALA, "Ground2_B.png");
}

ESTADO inicializar_posicao(ESTADO e, int tipo_item) {
    int x, y;
    do {
        x = random() % TAM;
        y = random() % TAM;
    } while(posicao_ocupada(e, x, y));

		if (tipo_item == 0) {
			e.inimigo[(int)e.num_inimigos].x = x;
	    e.inimigo[(int)e.num_inimigos].y = y;
	    e.num_inimigos++;
		}
		else if (tipo_item == 1) {
    	e.obstaculo[(int)e.num_obstaculos].x = x;
    	e.obstaculo[(int)e.num_obstaculos].y = y;
    	e.num_obstaculos++;
		}
		else {
			e.items[(int)e.num_items].x = x;
    	e.items[(int)e.num_items].y = y;
			e.num_items++;
		}

    return e;
	}

ESTADO inicializar_posicoes (ESTADO e, int num, int tipo) {
	int i;
	for(i = 0; i < num; i++)
		e = inicializar_posicao(e, tipo);
	return e;
}

ESTADO inicializar_tipo_items (ESTADO e) {
	int i;
	for (i=0; i < (int)e.num_items; i++) {
		int r = random() % 4;
		e.tipo_item[i] = r;
	}

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
	e = inicializar_posicoes(e, 20, 0);
	e = inicializar_posicoes(e, 20, 1);
	e.num_inimigos_longe = 4;
	e.fase = 0;
	e.score = 0;
	e.ilumina.x = TAM; e.ilumina.y = TAM;
	e.PU_Shield = 0;
	e.PU_Sword = 0;
	e.espada.x = TAM; e.espada.y = TAM;
	e.num_vidas = 2;
	e.num_mana = 2;
	e.num_stamina = 4;
	int num_items = random() % 3;
	e = inicializar_posicoes(e, num_items, 2);
	e = inicializar_tipo_items(e);
	return e;
}

ESTADO recolhe_scroll (ESTADO e, int x, int y) {
	return e;
}

ESTADO recolhe_item(ESTADO e, int x, int y) {
	int i;
	for (i=0; e.items[i].x != x && e.items[i].x != x; i++);

	if (e.tipo_item[i] == 0 && e.num_vidas < 4) e.num_vidas++;
	else if (e.tipo_item[i] == 1 && e.num_stamina < 4) e.num_stamina++;
	else if (e.tipo_item[i] == 2 && e.num_mana < 4 ) e.num_mana++;
	else e = recolhe_scroll(e, x, y);

	e.items[i].x = 0; e.items[i].y = 0;
	e.num_items--;
	e.score++;

	return e;
}

ESTADO apaga_inimigo (ESTADO e, int x, int y) {
	int i;
	for (i=0; i < (int)e.num_inimigos; i++)
		if (e.inimigo[i].x == x && e.inimigo[i].y == y) {
				if (tem_inim_longe(e, x, y)) {e.num_inimigos_longe--; e.score++;}
				for (; i < (int)e.num_inimigos; i ++) e.inimigo[i] = e.inimigo[i+1];
				e.inimigo[i].x = 0; e.inimigo[i].y = 0;
				e.num_inimigos--;
				e.score++;
		}
	return e;
}

ESTADO kill_line (ESTADO e, int coord_fix, int d) {
	int dx = 0, dy = 0, inc;

	if (d > 0) inc = 1;
	else inc = -1;

	if (coord_fix == 0) {e.espada.x = e.jog.x; dy = d; e = apaga_inimigo(e, e.jog.x, (e.jog.y + dy - inc));}
	else {e.espada.y = e.jog.y; dx = d; e = apaga_inimigo(e, (e.jog.x + dx - inc), e.jog.y);}

	while (tem_inimigo(e, (e.jog.x + dx), (e.jog.y + dy))) {
		e = apaga_inimigo(e, (e.jog.x + dx), (e.jog.y + dy));
		if (coord_fix == 0) {e.espada.y = e.jog.y + dy; dy += inc;}
		else {e.espada.x = e.jog.x + dx; dx += inc;}
	}

	return e;
}

ESTADO kill_streak (ESTADO e, int dx, int dy) {
	POSICAO n_espada;
	n_espada.x = e.jog.x + dx; n_espada.y = e.jog.y + dy;
	if (!tem_inimigo(e, (e.jog.x + dx), (e.jog.y + dy))) {e.espada = n_espada; return e;}
	else {
		if (dx != 0) e = kill_line (e, 1, dx);
		else e = kill_line (e, 0, dy);
		return e;
	}
}

ESTADO kill_replace (ESTADO e, int dx, int dy) {
	int x = e.jog.x + dx;
	int y = e.jog.y + dy;

	if (e.PU_Sword == 1) {
		e.PU_Sword = 0;
		e = kill_streak(e, dx, dy);
		e.num_stamina--;
	}
	else {
		if (tem_inimigo(e, x, y)) e = apaga_inimigo(e, x, y);
		else if (tem_espada(e, x, y)) {e.espada.x = TAM; e.espada.y = TAM;}
		else if (tem_item(e, x, y)) e = recolhe_item(e, x, y);
		e.jog.x = x; e.jog.y = y;
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
    if(posicao_ocupada(e, x, y)) {
        if (!tem_inimigo(e, x, y) && !tem_espada(e, x, y) && !tem_item(e, x, y)) return;
				else if (tem_inimigo(e, x, y) && posicao_valida(e.espada.x, e.espada.y)) return;
		}

	novo = kill_replace(e, dx, dy);

	novo.fase = 1;
	novo.ilumina.x = TAM; novo.ilumina.y = TAM;

	if (novo.PU_Shield > 0) novo.PU_Shield--;

	sprintf(link, "http://localhost/cgi-bin/jogo?%s", estado2str(novo));
	imprime_quadr_link(x, y, link);
}

void imprime_movimentos(ESTADO e) {
	int dx, dy;

	if (e.PU_Sword == 1) {
		dy = 0;
		for(dx = -2; dx <= 2; dx++) if (dx!=0) imprime_movimento(e, dx, dy);
		dx = 0;
		for(dy = -2; dy <= 2; dy++) if (dy!=0) imprime_movimento(e, dx, dy);
	}
	else {
		for(dx = -1;dx <= 1;dx++)
			for(dy = -1;dy <= 1;dy++)
				imprime_movimento(e, dx, dy);
	}

	if (abs(e.espada.x - e.jog.x) <= 1 || abs(e.espada.y - e.jog.y) <= 1)
		imprime_movimento(e, e.jog.x - e.espada.x, e.jog.y - e.espada.y);
}

void cria_posicao_a_iluminar(ESTADO e, int x, int y) {
	ESTADO novo = e;
	if (posicao_igual(novo.ilumina, x, y)) {novo.ilumina.x = TAM; novo.ilumina.y = TAM; novo.fase = 0;}
	else {novo.ilumina.x = x; novo.ilumina.y = y; novo.fase = 0;}

	char link[MAX_BUFFER];
	sprintf(link, "http://localhost/cgi-bin/jogo?%s", estado2str(novo));
	imprime_quadr_link(x, y, link);
}

void game_over(ESTADO e) {
	return;
}

void imprime_jogador(ESTADO e) {
	if (!posicao_valida(e.jog.x, e.jog.y)) {game_over(e); return;}
	IMAGEM(e.jog.x, e.jog.y, ESCALA, "Warrior.png");
	cria_posicao_a_iluminar(e, e.jog.x, e.jog.y);
	imprime_movimentos(e);
}

void imprime_inimigos(ESTADO e) {
	int i;
	for (i=0; i < (int)e.num_inimigos_longe; i++) {
		IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "Necromancer.png");
		cria_posicao_a_iluminar(e, (int)e.inimigo[i].x, (int)e.inimigo[i].y);
	}
	for(; i < (int)e.num_inimigos; i++) {
		IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "Goon.png");
		cria_posicao_a_iluminar(e, (int)e.inimigo[i].x, (int)e.inimigo[i].y);
	}
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
			if (dx == 0 || dy == 0) imprime_casa_iluminada(e, x + dx, y + dy, 1);
}

void iluminar_jogador (ESTADO e, int x, int y) {
	int dx, dy;

	if (e.PU_Sword == 1) {
		dy = 0;
		for(dx = -2; dx <= 2; dx++) if (dx!=0) imprime_casa_iluminada(e, x + dx, y + dy, 0);
		dx = 0;
		for(dy = -2; dy <= 2; dy++) if (dy!=0) imprime_casa_iluminada(e, x + dx, y + dy, 0);
	}
	else {
		for (dx=-1; dx<= 1; dx++)
			for (dy=-1; dy<= 1; dy++) imprime_casa_iluminada(e, x + dx, y + dy ,0);
	}
}

void iluminar_inimigo_longe (ESTADO e, int x, int y) {
	int dx, dy;

	if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_L.png");
	else IMAGEM(x, y, ESCALA, "Ground2_L.png");

	for (dx=-1; dx >= -4; dx--) {
	 		if (tem_inimigo(e, x + dx, y) || tem_obstaculo(e, x + dx, y) || tem_jogador(e, x + dx, y)) {imprime_casa_iluminada(e, x + dx, y, 1); break;}
			else imprime_casa_iluminada(e, x + dx, y, 1);
	}

	for (dx=1; dx <= 4; dx++) {
	 		if (tem_inimigo(e, x + dx, y) || tem_obstaculo(e, x + dx, y) || tem_jogador(e, x + dx, y)) {imprime_casa_iluminada(e, x + dx, y, 1); break;}
			else imprime_casa_iluminada(e, x + dx, y, 1);
	}

	for (dy=-1; dy >= -4; dy--) {
	 		if (tem_inimigo(e, x, y + dy) || tem_obstaculo(e, x, y + dy) || tem_jogador(e, x, y + dy)) {imprime_casa_iluminada(e, x, y + dy, 1); break;}
			else imprime_casa_iluminada(e, x, y + dy, 1);
	}

	for (dy=1; dy <= 4; dy++) {
	 		if (tem_inimigo(e, x, y + dy) || tem_obstaculo(e, x, y + dy) || tem_jogador(e, x, y + dy)) {imprime_casa_iluminada(e, x, y + dy, 1); break;}
			else imprime_casa_iluminada(e, x, y + dy, 1);
	}

}

void imprime_ilumi (ESTADO e) {
	int x = e.ilumina.x;
	int y = e.ilumina.y;

	if (tem_jogador(e, x, y)) iluminar_jogador (e, x, y);
	else if (tem_inim_longe(e, x, y)) iluminar_inimigo_longe (e, x, y);
	else if (tem_inimigo(e, x, y)) iluminar_inimigo (e, x, y);
}

void imprime_shield (ESTADO e) {

	if (e.num_stamina < 1) {IMAGEM(0, (TAM+1), ESCALA, "X.png"); return;}
	else IMAGEM(0, (TAM + 1), ESCALA, "Shield.png");

	ESTADO novo = e;
	novo.PU_Shield = 2;
	novo.num_stamina = e.num_stamina - 2;
	char link[MAX_BUFFER];
	sprintf(link, "http://localhost/cgi-bin/jogo?%s", estado2str(novo));

	imprime_quadr_link(0, (TAM+1), link);
}

void imprime_sword (ESTADO e) {
	if (e.num_stamina < 0) {IMAGEM(0, TAM, ESCALA, "X.png");}
	else IMAGEM(0, TAM, ESCALA, "Sword.png");

	if (e.PU_Sword == 0 && e.espada.x == TAM && e.espada.y == TAM) {
		ESTADO novo = e;
		novo.PU_Sword = 1;
		novo.PU_Shield = 1;

		char link[MAX_BUFFER];
		sprintf(link, "http://localhost/cgi-bin/jogo?%s", estado2str(novo));

		imprime_quadr_link (0, TAM, link);
	}
	else if (e.espada.x != TAM && e.espada.y != TAM) IMAGEM (e.espada.x, e.espada.y, ESCALA, "Sword.png");
	else {
		ESTADO novo = e;
		novo.PU_Sword = 0;
		novo.PU_Shield = 1;

		char link[MAX_BUFFER];
		sprintf(link, "http://localhost/cgi-bin/jogo?%s", estado2str(novo));

		imprime_quadr_link (0, TAM, link);
	}
}

void imprime_coracoes (ESTADO e) {
	int i, c;
	for (c=0; c<=2; c++) {
		if (c == 0)
			for (i=0; i<=e.num_vidas; i++) IMAGEM ((5+i), (TAM + c), ESCALA, "Heart_Red.png");
		else if (c == 1)
			for (i=0; i<=e.num_stamina; i++) IMAGEM ((5+i), (TAM + c), ESCALA, "Heart_Green.png");
		else
			for (i=0; i<=e.num_mana; i++) IMAGEM ((5+i), (TAM + c), ESCALA, "Heart_Blue.png");

		for (; i<5; i++) IMAGEM ((5+i), (TAM + c), ESCALA, "Heart_Blocked.png");
	}
}

void imprime_items (ESTADO e) {
	int i;
	for (i = 0; i < (int)e.num_items; i++) {
		if (e.tipo_item[i] == 0) IMAGEM (e.items[i].x, e.items[i].y, ESCALA, "Potion_Red.png");
		else if (e.tipo_item[i] == 1) IMAGEM (e.items[i].x, e.items[i].y, ESCALA, "Potion_Green.png");
		else if (e.tipo_item[i] == 2) IMAGEM (e.items[i].x, e.items[i].y, ESCALA, "Potion_Blue.png");
		else IMAGEM (e.items[i].x, e.items[i].y, ESCALA, "Scroll.png");
	}
}

void jogo(ESTADO e) {
	int x, y;

	for(y = 0; y < TAM; y++)
		for(x = 0; x < TAM; x++)
			imprime_casa(x, y);

	for (; y < TAM + 3; y++)
		for (x=0; x < TAM; x++) {
			if (((x+y)%2) == 0) IMAGEM(x, y, ESCALA, "Ground1_LY.png");
			else IMAGEM(x, y, ESCALA, "Ground2_LY.png");
		}

	if (posicao_valida (e.ilumina.x, e.ilumina.y)) imprime_ilumi(e);

	imprime_obstaculos(e);

	if (e.fase != 0 && e.PU_Shield < 1) e = mover_inimigos(e);

	imprime_inimigos(e);
	imprime_porta(e);
	imprime_sword(e);
	imprime_items(e);

	imprime_jogador(e);
	imprime_shield(e);
	imprime_coracoes(e);

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
