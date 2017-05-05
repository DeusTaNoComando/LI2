#ifndef ___CGI_H___
#define ___CGI_H___

/**
@file cgi.h
Macros úteis para gerar CGIs
*/

#include <stdio.h>

/**
* Caminho para as imagens
*/
#define IMAGE_PATH							"http://localhost/images/"

/**
\brief Macro para começar o html
*/
#define COMECAR_HTML						printf("Content-Type: text/html\n\n")

/**
\brief Macro para abrir um svg
@param tamx O comprimento do svg
@param tamy A altura do svg
*/
#define ABRIR_SVG(tamx, tamy)				printf("<svg width=%d height=%d>\n", tamx, tamy)
/**
\brief Macro para fechar um svg
*/
#define FECHAR_SVG							printf("</svg>\n")

/**
\brief Macro para criar uma imagem
@param X A coordenada X do canto superior esquerdo
@param Y A coordenada Y do canto superior esquerdo
@param ESCALA A escala da imagem
@param FICHEIRO O caminho para o link do ficheiro
*/
#define IMAGEM(X, Y, ESCALA, FICHEIRO)		printf("<image x=%d y=%d width=%d height=%d xlink:href=%s />\n", \
												ESCALA * X, ESCALA* (Y+1), ESCALA, ESCALA, IMAGE_PATH FICHEIRO)

/**
\brief Macro para criar um quadrado invisivel que serve de link
@param X A coordenada X do canto superior esquerdo
@param Y A coordenada Y do canto superior esquerdo
@param ESCALA A escala do quadrado
*/

#define QUADRADO_LINK(X, Y, ESCALA)			printf("<rect x=%d y=%d width=%d height=%d fill-opacity=0.0 />\n", \
												ESCALA * X, ESCALA* (Y+1), ESCALA, ESCALA)

/**
\brief Macros para introduzir texto de jogo
(Com devidos ajustes relativos à sua posição relativa)
@param X A coordenada X do canto superior esquerdo
@param Y A coordenada Y do canto superior esquerdo
@param ESCALA A escala dos quadrados para ajudar à colocação do texto
@param TAM O tamanho da letra
@param COR A cor da letra
@param TEXTO O texto a inserir
*/

#define TEXTO(X, Y, ESCALA, TAM, COR, TEXTO) 			printf("<text x=\"%d\" y=\"%d\" font-size=\"%d\" fill=\"%s\" text-anchor=\"start\" font-family= 'VT323' >%s</text> \n", \
												X * ESCALA + 10, ((Y + 1) * ESCALA) + 30, TAM, COR, TEXTO)

#define TEXTO_END(X, Y, ESCALA, TAM, COR, TEXTO) 			printf("<text x=\"%d\" y=\"%d\" font-size=\"%d\" fill=\"%s\" text-anchor=\"end\" font-family= 'VT323' >%s</text> \n", \
												X * ESCALA - 10, ((Y + 1) * ESCALA) + 30, TAM, COR, TEXTO)

#define TEXTO_MID(X, Y, ESCALA, TAM, COR, TEXTO) 	printf("<text x=\"%d\" y=\"%d\" font-size=\"%d\" fill=\"%s\" text-anchor=\"middle\" font-family= 'VT323' >%s</text> \n", \
												X * ESCALA, ((Y + 1) * ESCALA) + 32, TAM, COR, TEXTO)

/**
\brief Macro para a ir buscar o tipo de letra
*/

#define FONTE { printf("<style>\n"); printf("@import url('https://fonts.googleapis.com/css?family=VT323');\n"); printf("</style>\n");}

/**
\brief Macro para a inicialização das cookies
*/

#define COOKIE(NOME) printf("<meta http-equiv=\"set-cookie\" content=\"Name=%s; expires=Sat, 01-July-2017 12:00:00 GMT; domain=.http://localhost/cgi-bin/jogo; path=/\">", NOME);

/**
\brief Macro para abrir um link
@param link O caminho para o link
*/
#define ABRIR_LINK(link)					printf("<a xlink:href=%s>\n", link)

/**
\brief Macro para fechar o link
*/
#define FECHAR_LINK							printf("</a>\n")

#endif
