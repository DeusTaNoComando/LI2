#ifndef ___ESTADO_H___
#define ___ESTADO_H___

/**
@file estado.h
Definição do estado e das funções que convertem estados em strings e vice-versa
*/

/** \brief O nº máximo de inimigos */
#define MAX_INIMIGOS		100

/** \brief O nº máximo de obstáculos */
#define MAX_OBSTACULOS		100

/** \brief O nº máximo de items a apararecer por nível */
#define MAX_ITEMS 3

/**
\brief Estrutura que armazena uma posição
*/
typedef struct posicao {
	char x;
	char y;
} POSICAO;

/**
\brief Estrutura que armazena o estado do jogo
*/
typedef struct estado {
	/** \brief A posição do jogador */
	POSICAO jog;
	/** \brief O nº de inimigos */
	char num_inimigos;
	/** \brief O nº de inimigos que consegue atingir o jogador de longe*/
	char num_inimigos_longe;
	/** \brief O nº de obstáculos */
	char num_obstaculos;
	/** \brief Array com a posição dos inimigos */
	POSICAO inimigo[MAX_INIMIGOS];
	/** \brief Array com a posição dos obstáculos */
	POSICAO obstaculo[MAX_OBSTACULOS];
	/** \brief Posciçoes das portas */
	POSICAO porta_entrada;
	POSICAO porta_saida;
	/** \brief Fase de jogo, usado para saber se é o turno do jogador e para as animações */
	char fase;
	/** \brief Score do jogador */
	char score;
	/** \brief Posição a iluminar */
	POSICAO ilumina;
	/** \brief Power Up Shield ativado */
	char PU_Shield;
	/** \brief Power Up Sword ativado */
	char PU_Sword;
		/** \brief Posicao da espada atirada */
	POSICAO espada;
	/** \brief Power Up Scroll ativado */
	char PU_Scroll;
	/** \brief Tipo de feitiço à custa da mana que gasta */
	char PU_Scroll_Mana;
	/** \brief Diz se a lava está seca ou não, ou seja, se o jogador pode andar por cima dos obstáculos */
	char Lava_on;
	/** \brief Diz se o jogador pode se mover para qualquer casa, ou seja, teletranspotar-se */
	char teleport_on;
	/** \brief Número de vidas restantes do jogador */
	char num_vidas;
	/** \brief Mana restante do jogador */
	char num_mana;
	/** \brief Stamina restante do jogador */
	char num_stamina;
	/** \brief O nº de items */
	char num_items;
	/** \brief Tipos do items que dão 'spawn' a cada nível */
	char tipo_item[MAX_ITEMS];
	/** \brief Posições dos items */
	POSICAO items[MAX_ITEMS];

} ESTADO;

/**
\brief Função que converte um estado numa string
@param e O estado
@returns A string correspondente ao estado e
*/
char *estado2str(ESTADO e);

/**
\brief Função que converte uma string num estado
@param argumentos Uma string contendo os argumentos passados à CGI
@returns O estado correspondente à string dos argumentos
*/
ESTADO str2estado(char *argumentos);

#endif
