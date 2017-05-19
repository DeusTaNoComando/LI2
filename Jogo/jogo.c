#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cgi.h"
#include "estado.h"
#include "ler.h"
#include "escrever.h"

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
	return (posicao_igual(e.porta_saida, x, y));
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

void imprime_quadr_link (int x, int y, ESTADO e) {
	char nome[7] = {0};
	strncpy(nome, e.letra, 6);

	char link[MAX_BUFFER];
	sprintf(link, "http://localhost/cgi-bin/jogo?%s,%d,%d", nome, x, y);

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
		int r = random() % 11;
		if (r < 4) e.tipo_item[i] = 0;
		else if (r < 8) e.tipo_item[i] = 1;
		else if (r < 10) e.tipo_item[i] = 2;
		else e.tipo_item[i] = 3;
	}

	return e;
}

ESTADO inicializar() {
	ESTADO e = {0};
	sprintf(e.letra, "%s", "AAAAAA");
	e.porta_entrada.x = 9;
	e.porta_entrada.y = 9;
	e.porta_saida.x = 0;
	e.porta_saida.y = 0;
	e.jog.x = 9;
	e.jog.y = 9;
	e = inicializar_posicoes(e, 10, 0);
	e = inicializar_posicoes(e, 20, 1);
	e.num_inimigos_longe = 3;
	e.ilumina.x = TAM; e.ilumina.y = TAM;
	e.nivel = 1;
	e.espada.x = TAM; e.espada.y = TAM;
	e.Lava_on = 1;
	e.num_vidas = 3;
	e.num_mana = 3;
	e.num_stamina = 5;
	int num_items = random() % 3 + 1;
	e = inicializar_posicoes(e, num_items, 2);
	e = inicializar_tipo_items(e);
	return e;
}

void recolhe_scroll (ESTADO * e, int x, int y) {
	int tipo = random() % 10;

	if (tipo < 5) (*e).PU_Scroll = 1;
	else if (tipo < 8) (*e).PU_Scroll = 2;
	else (*e).PU_Scroll = 5;
}

void recolhe_item(ESTADO *e, int x, int y) {
	int i;
	for (i=0; e->items[i].x != x || e->items[i].y != y; i++);

	if (e->tipo_item[i] == 0 && e->num_vidas < 5) e->num_vidas++;
	else if (e->tipo_item[i] == 1 && e->num_stamina < 5) e->num_stamina++;
	else if (e->tipo_item[i] == 2 && e->num_mana < 5 ) e->num_mana++;
	else if (e->tipo_item[i] == 3) recolhe_scroll(e, x, y);

	int j = i;
	for (; i < e->num_items; i++) {e->items[i].x = e->items[i+1].x; e->items[i].y = e->items[i+1].y;}
	for (i = j; i < e->num_items; i++) e->tipo_item[i] = e->tipo_item[i+1];

	e->num_items--;

	e->score++;
}

void apaga_inimigo (ESTADO *e, int x, int y) {
	int i;
	for (i=0; i < (int)e->num_inimigos; i++)
		if (e->inimigo[i].x == x && e->inimigo[i].y == y) {
				if (tem_inim_longe(*e, x, y)) {e->num_inimigos_longe--; e->score++;}
				for (; i < (int)e->num_inimigos; i ++) e->inimigo[i] = e->inimigo[i+1];
				e->inimigo[i].x = 0; e->inimigo[i].y = 0;
				e->num_inimigos--;
				e->score++;
		}
}

void kill_line (ESTADO *e, int coord_fix, int d) {
	int dx = 0, dy = 0, inc;

	if (d > 0) inc = 1;
	else inc = -1;

	if (coord_fix == 0) {e->espada.x = e->jog.x; dy = d; apaga_inimigo(e, e->jog.x, (e->jog.y + dy - inc));}
	else {e->espada.y = e->jog.y; dx = d; apaga_inimigo(e, (e->jog.x + dx - inc), e->jog.y);}

	while (tem_inimigo(*e, (e->jog.x + dx), (e->jog.y + dy))) {
		apaga_inimigo(e, (e->jog.x + dx), (e->jog.y + dy));
		if (coord_fix == 0) {e->espada.y = e->jog.y + dy; dy += inc;}
		else {e->espada.x = e->jog.x + dx; dx += inc;}
	}
}

void kill_streak (ESTADO *e, int dx, int dy) {
	POSICAO n_espada;
	n_espada.x = e->jog.x + dx; n_espada.y = e->jog.y + dy;
	if (!tem_inimigo(*e, (e->jog.x + dx), (e->jog.y + dy))) e->espada = n_espada;
	else {
		if (dx != 0) kill_line (e, 1, dx);
		else kill_line (e, 0, dy);
	}
}

void kill_replace (ESTADO *e, int dx, int dy) {
	int x = e->jog.x + dx;
	int y = e->jog.y + dy;

	if (e->PU_Sword == 2) {
		e->PU_Sword = 1;
		kill_streak(e, dx, dy);
	}
	else {
		if (tem_inimigo(*e, x, y)) apaga_inimigo(e, x, y);
		else if (tem_espada(*e, x, y)) {e->espada.x = TAM; e->espada.y = TAM; e->PU_Sword=0;}
		else if (tem_item(*e, x, y)) recolhe_item(e, x, y);
		e->jog.x = x; e->jog.y = y;
	}
}

void move(ESTADO *e, int dx, int dy) {
	kill_replace(e, dx, dy);

	e->fase = 1;
	e->ilumina.x = TAM; e->ilumina.y = TAM;
	e->teleport_on = 0;

	if (e->PU_Shield > 0) e->PU_Shield--;
}

int in_range(ESTADO e, int x, int y) {
	if (e.jog.x == x && e.jog.y == y) return 0;

	if (e.teleport_on) return 1;
	else if (e.PU_Sword == 2) return ((abs(e.jog.x - x) <= 2 && e.jog.y == y) || (abs(e.jog.y - y) <= 2 && e.jog.x == x));
	else return (abs(e.jog.x - x) <= 1 && abs(e.jog.y - y) <= 1);
}

void light(ESTADO *e, int x, int y) {
	if (!posicao_valida(e->ilumina.x, e->ilumina.y)){
		e->ilumina.x = x;
		e->ilumina.y = y;
	} else {
		e->ilumina.x = TAM;
		e->ilumina.y = TAM;
	}
	e->fase = 0;
}

void imprime_movimentos(ESTADO e) {
	int dx, dy;

	if (e.teleport_on) {
		for (dx = 0; dx < TAM; dx++)
			for (dy = 0; dy < TAM; dy++)
				if (dx != 0 || dy != 0) imprime_quadr_link (dx, dy, e);
	}
	else if (e.PU_Sword == 2) {
		dy = 0;
		for(dx = -2; dx <= 2; dx++) if (dx!=0) imprime_quadr_link((e.jog.x + dx), dy, e);
		dx = 0;
		for(dy = -2; dy <= 2; dy++) if (dy!=0) imprime_quadr_link(dx, (e.jog.y + dy), e);
	}
	else {
		for(dx = -1;dx <= 1;dx++)
			for(dy = -1;dy <= 1;dy++) {
				int x = e.jog.x + dx;
				int y = e.jog.y + dy;
				if(!posicao_valida(x, y)) continue;
				else if(posicao_ocupada(e, x, y)) {
							if (!tem_inimigo(e, x, y) && !tem_espada(e, x, y) && !tem_item(e, x, y) && !(tem_obstaculo(e, x, y) && !e.Lava_on)) continue;
							else if (tem_inimigo(e, x, y) && posicao_valida(e.espada.x, e.espada.y)) continue;
				}
				imprime_quadr_link(x, y, e);
			}
	}
}

void imprime_jogador(ESTADO e) {
	if (!posicao_valida(e.jog.x, e.jog.y)) return;
	IMAGEM(e.jog.x, e.jog.y, ESCALA, "Goblin.png");
	imprime_quadr_link(e.jog.x, e.jog.y, e);
	imprime_movimentos(e);
}

void imprime_inimigos(ESTADO e) {
	int i;
	for (i=0; i < (int)e.num_inimigos_longe; i++) {
		IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "Necromancer.png");
		imprime_quadr_link((int)e.inimigo[i].x, (int)e.inimigo[i].y, e);
	}
	for(; i < (int)e.num_inimigos; i++) {
		IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "Goon.png");
		imprime_quadr_link((int)e.inimigo[i].x, (int)e.inimigo[i].y, e);
	}
}

void imprime_obstaculos(ESTADO e) {
	int i;
	for(i = 0; i < e.num_obstaculos; i++)
		if (e.Lava_on) IMAGEM(e.obstaculo[i].x, e.obstaculo[i].y, ESCALA, "Lava.png");
		else IMAGEM(e.obstaculo[i].x, e.obstaculo[i].y, ESCALA, "Lava_Obsidian_2.png");

}

void novo_nivel(ESTADO *e) {
		ESTADO novo = inicializar();
		novo.score = e->score + 5;
		novo.nivel = e->nivel + 1;
		novo.num_vidas = e->num_vidas;
		novo.num_stamina = e->num_stamina;
		novo.num_mana = e->num_mana;
		novo.PU_Scroll = e->PU_Scroll;

		int i;
		for (i=0; i<6; i++) novo.letra[i] = e->letra[i];

		*e = novo;
}

void imprime_porta(ESTADO e) {

		IMAGEM(e.porta_entrada.x, e.porta_entrada.y, ESCALA, "Bottom_hole.png");
		IMAGEM(e.porta_saida.x, e.porta_saida.y, ESCALA, "Hole.png");

		if ((e.jog.x - e.porta_saida.x) <= 1 && (e.jog.y - e.porta_saida.y <= 1) && !posicao_valida(e.espada.x, e.espada.y))
			imprime_quadr_link (e.porta_saida.x, e.porta_saida.y, e);
}

int valor_casas (ESTADO e, int x, int y) {
	if (posicao_valida(x, y)) {
			if (posicao_ocupada(e, x, y))
				return 0;
	} else if (!posicao_valida(x, y))
				return 0;
	return 1;
}

int dist(int x, int y, int ix, int iy) {
	int r = abs(x - ix) + abs(y - iy);
	return r;
}

int range_inimigo_longe(ESTADO e, int i) {
	int dist;
	if (i < e.num_inimigos_longe) {
		if ((abs (e.jog.x - e.inimigo[i].x) == 2 && abs(e.jog.y - e.inimigo[i].y) == 0) && !tem_obstaculo(e, e.jog.x, e.jog.y)) {
			dist = e.jog.x - e.inimigo[i].x;
			if (!posicao_ocupada(e, e.jog.x - dist/2, e.jog.y)) return 1;
		}
		else if ((abs (e.jog.x - e.inimigo[i].x) == 0 && abs(e.jog.y - e.inimigo[i].y) == 2) && !tem_obstaculo(e, e.jog.x, e.jog.y)) {
			dist = e.jog.y - e.inimigo[i].y;
			if (!posicao_ocupada(e, e.jog.x, e.jog.y - dist/2)) return 1;
		}
	}
	return 0;
}

int range_inimigo (ESTADO e, int i) {
	if (i >= e.num_inimigos_longe)
		if (((abs (e.jog.x - e.inimigo[i].x) == 1 && abs(e.jog.y - e.inimigo[i].y) == 0) || (abs (e.jog.x - e.inimigo[i].x) == 0 && abs(e.jog.y - e.inimigo[i].y) == 1)) && !tem_obstaculo(e, e.jog.x, e.jog.y))
			return 1;
	return 0;
}

ESTADO bot (ESTADO e, int i) {
	int dx, dy;
	int tam = dist(e.jog.x, e.jog.y, e.inimigo[i].x, e.inimigo[i].y);
		for(dx = -1; dx <= 1; dx++) {
			for(dy = -1; dy <= 1; dy++)
				if(valor_casas(e, e.inimigo[i].x + dx, e.inimigo[i].y + dy) && dist(e.jog.x, e.jog.y, e.inimigo[i].x + dx, e.inimigo[i].y + dy) < tam) {
					e.inimigo[i].x = e.inimigo[i].x + dx;
					e.inimigo[i].y = e.inimigo[i].y + dy;
					break;
				}
			if (dist(e.jog.x, e.jog.y, e.inimigo[i].x, e.inimigo[i].y) < tam) break;
		}
	return e;
}

ESTADO bot_longe (ESTADO e, int i) {
	int dx, dy;
	dx = 0;
	for(dy = -2; dy <= 2; dy += 4)
		if (dist(e.jog.x, e.jog.y + dy, e.inimigo[i].x, e.inimigo[i].y) == 1 && !posicao_ocupada(e, e.jog.x, e.jog.y + dy) && posicao_valida(e.jog.x, e.jog.y + dy)) {
			e.inimigo[i].y = e.jog.y + dy;
			return e;
	}
	dy = 0;
	for(dx = -2; dx <= 2; dx += 4)
		if (dist(e.jog.x + dx, e.jog.y, e.inimigo[i].x, e.inimigo[i].y) == 1 && !posicao_ocupada(e, e.jog.x + dx, e.jog.y) && posicao_valida(e.jog.x + dx, e.jog.y)) {
			e.inimigo[i].x = e.jog.x + dx;
			return e;
		}
	return e;
}

ESTADO mover_inimigos (ESTADO e) {
	int i;
	for (i=0; i<(int)e.num_inimigos; i++)
		if ((range_inimigo(e, i) || range_inimigo_longe(e, i)) && e.num_vidas == 1) {
			e.inimigo[i].x = e.jog.x; e.inimigo[i].y = e.jog.y;
			e.jog.x = TAM; e.jog.y = TAM;
			e.num_vidas--;
			e.num_mana = 0;
			e.num_stamina = 0;
			e.game_over = 1;
		}
		else if (range_inimigo(e, i) || range_inimigo_longe(e, i)) {
			e.num_vidas--;
			}
		else if (i < e.num_inimigos_longe && (abs(e.jog.x - e.inimigo[i].x) <= 2 && abs(e.jog.y - e.inimigo[i].y) <= 2))
		 	e = bot_longe(e, i);
		else e = bot(e, i);
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

	if (e.PU_Sword == 2) {
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

void shield(ESTADO * e) {
		if (e->PU_Shield == 2) {e->PU_Shield = 0; e->num_stamina += 2;}
		else {e->PU_Shield = 2; e->num_stamina -= 2;}

		if (e->PU_Sword == 2) {e->PU_Sword = 0; e->num_stamina ++;}
}

void imprime_shield (ESTADO e) {

	if ((e.num_stamina < 1 || (e.num_stamina == 1 && e.PU_Sword !=2)) && e.PU_Shield < 2 && !e.teleport_on) {IMAGEM(0, (TAM+1), ESCALA, "X.png"); return;}
	else if (e.PU_Shield == 2) IMAGEM(0, (TAM+1), ESCALA, "X.png");
	else IMAGEM(0, (TAM + 1), ESCALA, "Shield_2.png");

	imprime_quadr_link(0, (TAM+1), e);
}

void sword (ESTADO *e) {
		if (e->PU_Shield == 2) {e->PU_Shield = 0; e->num_stamina += 2;}

		if (e->PU_Sword == 0 && e->espada.x == TAM && e->espada.y == TAM && (e->num_stamina > 0 || (e->num_stamina == 0 && e->PU_Shield == 2)) ) {
			e->PU_Sword = 2;
			e->PU_Shield = 1;
			e->num_stamina --;
		}
		else if (e->PU_Sword == 2) {
			e->PU_Sword = 0;
			e->PU_Shield = 1;
			e->num_stamina ++;
		}
}

void imprime_sword (ESTADO e) {
	if (e.PU_Sword == 0 && e.espada.x == TAM && e.espada.y == TAM && (e.num_stamina > 0 || (e.num_stamina == 0 && e.PU_Shield == 2)) && !e.teleport_on) IMAGEM(0, TAM, ESCALA, "Mace.png");
	else if (e.espada.x != TAM && e.espada.y != TAM) {IMAGEM (e.espada.x, e.espada.y, ESCALA, "Mace.png"); IMAGEM(0, TAM, ESCALA, "X.png"); return;}
	else if (e.PU_Sword == 2) IMAGEM(0, TAM, ESCALA, "X.png");

	imprime_quadr_link (0, TAM, e);
}

void teleport(ESTADO *e) {
	(*e).teleport_on = 1;
}

void earthquake(ESTADO *e, int x, int y) {
	int dx, dy;
	for (dx = -2; dx <= 2; dx++)
		for (dy = -2; dy <= 2; dy++) apaga_inimigo(e, x + dx, y+ dy);
}

void obsidian (ESTADO *e) {
	(*e).Lava_on = 0;
}

void scroll(ESTADO *e) {
	if (e->PU_Shield == 2) {e->PU_Shield = 0; e->num_stamina += 2;}
	else if (e->PU_Sword == 2) {e->PU_Sword = 0; e->num_stamina++;}

	e->num_mana -=  e->PU_Scroll;

	if (e->PU_Scroll == 1) obsidian(e);
	else if (e->PU_Scroll == 2) earthquake(e, e->jog.x, e->jog.y);
	else teleport(e);

	e->PU_Scroll = 0;

}

void imprime_scroll (ESTADO e) {
	if (e.num_mana >= e.PU_Scroll && e.PU_Scroll != 0) {
		IMAGEM(0, 12, ESCALA, "Scroll.png");
		imprime_quadr_link(0, 12, e);
	}
	else IMAGEM(0, 12, ESCALA, "X.png");
}

void imprime_coracoes (ESTADO e) {
	int i, c;
	for (c=0; c<=2; c++) {
		if (c == 0)
			for (i=1; i<=e.num_vidas; i++) IMAGEM ((4+i), (TAM + c), ESCALA, "Heart_Red.png");
		else if (c == 1)
			for (i=1; i<=e.num_stamina; i++) IMAGEM ((4+i), (TAM + c), ESCALA, "Heart_Green.png");
		else
			for (i=1; i<=e.num_mana; i++) IMAGEM ((4+i), (TAM + c), ESCALA, "Heart_Blue.png");

		for (; i<=5; i++) IMAGEM ((4+i), (TAM + c), ESCALA, "Heart_Blocked.png");
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

void imprime_texto(ESTADO e) {
	char nome[7] = {0};
	strncpy(nome, e.letra, 6);
	TEXTO(1, -1, ESCALA, 35, "#583a25", nome);

	char score[MAX_BUFFER];
	sprintf(score, "Sc:%d", e.score);

	char level[MAX_BUFFER];
	sprintf(level, "L:%d", (int)e.nivel);

	TEXTO_END(10, -1, ESCALA, 35, "#583a25", score);
	TEXTO_END(7, -1, ESCALA, 35, "#583a25", level);

	TEXTO(1, 10, ESCALA, 35, "#583a25", "Club");
	TEXTO(1, 11, ESCALA, 35, "#583a25", "Shield");

	if (e.PU_Scroll == 1) TEXTO(1, 12, ESCALA, 35, "#583a25", "Petrify");
	else if (e.PU_Scroll == 2) TEXTO(1, 12, ESCALA, 35, "#583a25", "Quake");
	else if (e.PU_Scroll == 5) TEXTO(1, 12, ESCALA, 35, "#583a25", "Portal");
	else TEXTO(1, 12, ESCALA, 35, "#583a25", "Scroll");

	TEXTO(4, 10, ESCALA, 35, "#583a25", "L:");
	TEXTO(4, 11, ESCALA, 35, "#583a25", "S:");
	TEXTO(4, 12, ESCALA, 35, "#583a25", "M:");
}

void imprime_canvas() {
	int x, y;

	for (y = -1; y < 0; y++)
		for (x=0; x < TAM; x++) {
			if (((x+y)%2) == 0) IMAGEM(x, y, ESCALA, "Ground1_LY.png");
			else IMAGEM(x, y, ESCALA, "Ground2_LY.png");
	}

	for(; y < TAM; y++)
		for(x = 0; x < TAM; x++)
			imprime_casa(x, y);

	for (; y < TAM + 3; y++)
		for (x=0; x < TAM; x++) {
			if (((x+y)%2) == 0) IMAGEM(x, y, ESCALA, "Ground1_LY.png");
			else IMAGEM(x, y, ESCALA, "Ground2_LY.png");
		}
}

void imprime_mapa(ESTADO e) {
	IMAGEM(0, -1, ESCALA, "Map.png");
	e.PU_Shield = 1;
	imprime_quadr_link(0, -1, e);
}

void imprime_botao(char* link, int y, char* texto) {

	ABRIR_LINK(link);

	int c;
	for (c=1; c<9; c++) {
		if ((c+y)%2) IMAGEM(c, y, ESCALA, "Ground1_B.png");
		else IMAGEM(c, y, ESCALA, "Ground2_B.png");
	}

	TEXTO_MID(5, y, ESCALA, 40, "#e4a91f", texto);

	FECHAR_LINK;
}

void imprime_link_letra(int x, int y, char * nome) {
	char link[MAX_BUFFER];
	sprintf(link, "http://localhost/cgi-bin/jogo?Start_%s", nome);

	ABRIR_LINK(link);
	QUADRADO_LINK(x, y, ESCALA);
	FECHAR_LINK;
}

void imprime_letras(char * nome) {
	int i;

	for (i=0; i<6; i++) {

			char letra[2] = {0};
			letra[0] = nome[i];
			TEXTO((2+i), 7, ESCALA, 40, "#583a25", letra);

			IMAGEM((2+i), 5, ESCALA, "Up2.png");
			IMAGEM((2+i), 6, ESCALA, "Up.png");
			IMAGEM((2+i), 8, ESCALA, "Down.png");
			IMAGEM((2+i), 9, ESCALA, "Down2.png");

			if (nome[i] != 'Z') nome[i]++;
			else nome[i] = 'A';
			imprime_link_letra((2+i), 6, nome);

			nome[i] = letra[0];

			if (nome[i] != 'A') nome[i]--;
			else nome[i] = 'Z';
			imprime_link_letra((2+i), 8, nome);

			nome[i] = letra[0];

			if (nome[i] < 'V') nome[i]+=5;
			else nome[i] = ('A' + (nome[i] - 'V'));
			imprime_link_letra((2+i), 5, nome);

			nome[i] = letra[0];

			if (nome[i] > 'E') nome[i]-=5;
			else nome[i] = ('Z' - ('E' - nome[i]));
			imprime_link_letra((2+i), 9, nome);

			nome[i] = letra[0];
		}
}

void avanca_linha(char * texto) {
	int i, j;
	for (i=0; texto[i]!=0 && texto[i]!=';'; i++);
	if (texto[i] != 0) {
		for(j = 0; texto[i + j + 1]!= 0; j++)
		texto[j] = texto[j+i+1];
		texto[j] = 0;
	}
}

void muda_linha(char * leader, char * nome, char *  lvl, char * scr, int i, char * new_leader) {
	int j, n;
	char antes[MAX_BUFFER], depois[MAX_BUFFER];
	for(j=0; n<i-1; j++) {if (leader[j] == ';') n++; antes[j] = leader[j];}
	antes[j] = 0;
	n++;
	int pos = j;
	for(; n < 5; j++) {if (leader[j] == ';') n++; depois[j-pos] = leader[j];}
	depois[j-pos] = 0;
	sprintf(new_leader, "%s%s,%s,%s;%s",antes, nome, lvl, scr, depois);
}

void guarda_score(char * nome, int nivel, int ponto) {
	char leader[MAX_BUFFER], save_leader[MAX_BUFFER];
	ler(leader, "LEADERS.dat");
	strcpy(save_leader, leader);

	char nomes[5][7] = {{0}}, level[5][4], score[6][6];
	int i;
	int pontos[5];

	for (i=0; i<5; i++) {
		if (sscanf(leader, "%6[^,],%4[^,],%6[^;]", nomes[i], level[i], score[i]) != 3) break;
		sscanf(leader, "%*6[^,],%*4[^,],%d", &pontos[i]);
		avanca_linha(leader);
	}

	strcpy(leader, save_leader);
	TEXTO_MID(5, 4, ESCALA, 10, "#583a25", leader);

	char lvl[4];
	sprintf(lvl, "%d", nivel);
	char scr[6];
	sprintf(scr, "%d", ponto);

	char new_leader[MAX_BUFFER];
	strcpy(new_leader, leader);

	for(i=0; i<5 && pontos[i] > ponto; i++);
	if (i<5) muda_linha(leader, nome, lvl, scr, (i+1), new_leader);
	TEXTO_MID(5, 6, ESCALA, 10, "#583a25", new_leader);

	escrever(new_leader, "LEADERS.dat");
}

void game_over(ESTADO e) {
	int y,x;
	for (y=-1; y<TAM + 3; y++)
		for (x=0; x<TAM; x++)
			if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_LY.png");
			else IMAGEM(x, y, ESCALA, "Ground2_LY.png");

	TEXTO(1, 1, ESCALA, 110, "#583a25", "GAME");
	TEXTO_END(9, 3, ESCALA, 110, "#583a25", "OVER");

	char score[MAX_BUFFER];
	sprintf(score, "Score: %d", e.score);

	char level[MAX_BUFFER];
	sprintf(level, "Level: %d", e.nivel);

	TEXTO_MID(5, 5, ESCALA, 60, "#583a25", score);
	TEXTO_MID(5, 7, ESCALA, 60, "#583a25", level);

	imprime_botao("http://localhost/cgi-bin/jogo?", 9, "Main Menu");

	char nome[6];
	strncpy(nome, e.letra, 6);
	char link[MAX_BUFFER];
	sprintf(link, "http://localhost/cgi-bin/jogo?%s", nome);

	guarda_score(nome, e.nivel, e.score);

	imprime_botao(link, 11, "Retry");
}

void imprime_background(char* texto) {
	int y,x;
	for (y=-1; y<TAM + 3; y++)
		for (x=0; x<TAM; x++)
			if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_LY.png");
			else IMAGEM(x, y, ESCALA, "Ground2_LY.png");

	for (x=1; x<9; x++) {
		if ((x+1)%2) IMAGEM(x, 0, ESCALA, "Ground1_B.png");
		else IMAGEM(x, 0, ESCALA, "Ground2_B.png");
	}

	TEXTO_MID(5, 0, ESCALA, 40, "#e4a91f", texto);
}

void introduzir_nome(char * nome) {

	imprime_background("New Game");

	TEXTO_MID(5, 2, ESCALA, 40, "#583a25", "Introduza um nome");
	TEXTO_MID(5, 3, ESCALA, 40, "#583a25", "e inicie o jogo:");

	imprime_letras(nome);

	char link[MAX_BUFFER];
	sprintf(link, "http://localhost/cgi-bin/jogo?%s", nome);

	imprime_botao(link, 11, "Start");
}

void load(char * args) {
	ESTADO e;

	char estado[MAX_BUFFER];
	ler(estado, "SAVE.dat");
	e = str2estado(estado);

	char nome[6];
	strncpy(nome, e.letra, 6);

	char fich[11];
	sprintf(fich, "%s.dat", nome);

	escrever(estado2str(e), fich);
	sprintf(args, "%s,10,10", nome);
}

void save(ESTADO *e) {
	e->fase = 0;
	escrever(estado2str(*e), "SAVE.dat");
}

void jogo(ESTADO e) {

	if (e.game_over) {
		game_over(e);

		char nome[6];
		strncpy(nome, e.letra, 6);

		char fich[11];
		sprintf(fich, "%s.dat", nome);
		remove(fich);

		return;
	}

	imprime_canvas();
	imprime_texto(e);

	if (posicao_valida (e.ilumina.x, e.ilumina.y)) imprime_ilumi(e);

	imprime_obstaculos(e);

	imprime_mapa(e);
	imprime_inimigos(e);
	imprime_porta(e);
	imprime_sword(e);
	imprime_items(e);

	imprime_jogador(e);
	imprime_shield(e);
	imprime_scroll(e);
	imprime_coracoes(e);

}

void interface() {
	int y,x;
	for (y=-1; y<TAM + 3; y++)
		for (x=0; x<TAM; x++)
			if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_LY.png");
			else IMAGEM(x, y, ESCALA, "Ground2_LY.png");

	TEXTO_MID(5, 1, ESCALA, 110, "#583a25", "GOBLIN");
	TEXTO_MID(5, 3, ESCALA, 110, "#583a25", "N'BONES");

	imprime_botao("http://localhost/cgi-bin/jogo?Start_AAAAAA", 5, "New Game");
	imprime_botao("http://localhost/cgi-bin/jogo?Continue", 7, "Continue Game");
	imprime_botao("http://localhost/cgi-bin/jogo?Leaderboards", 9, "Leaderboards");
	imprime_botao("http://localhost/cgi-bin/jogo?Help", 11, "Help");
}

void imprime_botao_ajuda(int x, int y) {
	char link[MAX_BUFFER];

	if (y==4) {
		if (x==3) sprintf(link, "http://localhost/cgi-bin/jogo?Help_%s", "Goblin");
		else if (x==5) sprintf(link, "http://localhost/cgi-bin/jogo?Help_%s", "Goon");
		else if (x==7) sprintf(link, "http://localhost/cgi-bin/jogo?Help_%s", "Necromancer");
	}
	else if (y==6) {
		if (x==3) sprintf(link, "http://localhost/cgi-bin/jogo?Help_%s", "Hearts");
		else if (x==7) sprintf(link, "http://localhost/cgi-bin/jogo?Help_%s", "Potions");
	}
	else if (y==8) {
		if (x==3) sprintf(link, "http://localhost/cgi-bin/jogo?Help_%s", "Club");
		else if (x==5) sprintf(link, "http://localhost/cgi-bin/jogo?Help_%s", "Shield");
		else if (x==7) sprintf(link, "http://localhost/cgi-bin/jogo?Help_%s", "Scroll");
	}

	ABRIR_LINK(link);
	QUADRADO_LINK(x, y, ESCALA);
	FECHAR_LINK;
}

void help() {

	imprime_background("Help");

	int x,y;
	for (y = 4; y <= 8; y+= 2)
		for (x = 3; x <= 7; x += 2)
			if (x != 5 || y != 6) {
				if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_B.png");
				else IMAGEM(x, y, ESCALA, "Ground2_B.png");
			}

	TEXTO(1, 2, ESCALA, 35, "#583a25", "Click the items below:");
	TEXTO(1, 4, ESCALA, 35, "#583a25", "Ch:");
	TEXTO(1, 6, ESCALA, 35, "#583a25", "H:");
	TEXTO(5, 6, ESCALA, 35, "#583a25", "P:");
	TEXTO(1, 8, ESCALA, 35, "#583a25", "A:");

	IMAGEM(3, 4, ESCALA, "Goblin.png");
	IMAGEM(5, 4, ESCALA, "Goon.png");
	IMAGEM(7, 4, ESCALA, "Necromancer.png");
	IMAGEM(3, 6, ESCALA, "Heart_Red.png");
	IMAGEM(7, 6, ESCALA, "Potion_Red.png");
	IMAGEM(3, 8, ESCALA, "Mace.png");
	IMAGEM(5, 8, ESCALA, "Shield_2.png");
	IMAGEM(7, 8, ESCALA, "Scroll.png");

	for (y = 4; y <= 8; y+= 2)
		for (x = 3; x <= 7; x += 2)
			if (x != 5 || y != 6) imprime_botao_ajuda(x, y);

	imprime_botao("http://localhost/cgi-bin/jogo?", 11, "Main Menu");
}

void leaderboards() {
	imprime_background("Leaderboards");

	TEXTO(1, 2, ESCALA, 35, "#583a25", "Pl:");
	TEXTO_MID(6, 2, ESCALA, 35, "#583a25", "L:");
	TEXTO_END(9, 2, ESCALA, 35, "#583a25", "Sc:");

	char leader[MAX_BUFFER];
	ler(leader, "LEADERS.dat");

	char nomes[5][7] = {{0}}, level[5][4], score[6][6];
	int i, num_scans = 0;

	for (i=0; i<5; i++) {
		sscanf(leader, "%6[^,],%4[^,],%6[^;]", nomes[i], level[i], score[i]);
		num_scans++;
		avanca_linha(leader);
	}

	for (i = 4; i < 4 + num_scans; i++) {
		TEXTO(1, i, ESCALA, 35, "#583a25", nomes[i-4]);
		TEXTO_MID(6, i, ESCALA, 35, "#583a25", level[i-4]);
		TEXTO_END(9, i, ESCALA, 35, "#583a25", score[i-4]);
	}

	imprime_botao("http://localhost/cgi-bin/jogo?", 11, "Main Menu");
}

void ajudas(char* args) {
	char tipo[12];
	sscanf(args, "Help_%s", tipo);

	imprime_background(tipo);
}

ESTADO agir (char* args) {
	ESTADO e;
	char nome[6];
	char fich[11];
	int x, y;

	if (sscanf(args, "%6c,%d,%d", nome, &x, &y) == 1) {
		sprintf(fich, "%s.dat", nome);

		e = inicializar();
		int i;
		for (i=0; i<6; i++) e.letra[i] = nome[i];
	}
	else {
		sprintf(fich, "%s.dat", nome);

		char estado[MAX_BUFFER];
		ler(estado, fich);
		e = str2estado(estado);

		int dx = x - e.jog.x;
		int dy = y - e.jog.y;

		if (posicao_igual(e.porta_saida, x, y)) novo_nivel(&e);
		else if (posicao_valida(x, y) && in_range(e, x, y)) move(&e, dx, dy);
		else if (posicao_valida(x, y)) light(&e, x, y);
		else if (x == 0 && y == 10) sword(&e);
		else if (x == 0 && y == 11) shield(&e);
		else if (x == 0 && y == 12)	scroll(&e);
		else save(&e);

	}

	return e;
}

void ler_estado(char *args) {
	char nome[6];

	if(strlen(args) == 0) interface();
	else if(sscanf(args, "Start_%s", nome) == 1) introduzir_nome(nome);
	else if(strcmp(args, "Leaderboards") == 0) leaderboards();
	else if (strcmp(args, "Help") == 0) help();
	//*else if (strlen(args) <= 17) ajudas(args);
	else {
		if (strcmp(args, "Continue") == 0) load(args);

		ESTADO e = agir(args);

		if (e.fase != 0 && e.PU_Shield < 1) e = mover_inimigos(e);
		jogo(e);

		char nome[6];
		sscanf(args, "%6c", nome);
		char fich[6];
		sprintf(fich, "%s.dat", nome);
		escrever(estado2str(e), fich);
	}
}

int main() {
    srandom(time(NULL));

	COMECAR_HTML;

	FONTE;

	ABRIR_SVG(600, 600);

	ler_estado(getenv("QUERY_STRING"));

	FECHAR_SVG;

	return 0;
}
