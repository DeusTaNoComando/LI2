#define MAX_INIMIGOS        100
#define MAX_OBSTACULOS      100

typedef struct pos {
    int x;
    int y;
} POSICAO;

typedef struct estado {
    POSICAO jog;
    int num_inimigos;
    POSICAO inimigo[MAX_INIMIGOS];
    int num_obstaculos;
    POSICAO obstaculo[MAX_OBSTACULOS];
} ESTADO;

