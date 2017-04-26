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
\brief Macro para criar um quadrado
@param X A coordenada X do canto superior esquerdo
@param Y A coordenada Y do canto superior esquerdo
@param ESCALA A escala do quadrado
@param COR A cor de preenchimento do quadrado
*/
#define QUADRADO(X, Y, ESCALA, COR)			printf("<rect x=%d y=%d width=%d height=%d fill=%s />\n", \
												ESCALA * X, ESCALA* Y, ESCALA, ESCALA, COR)

#define QUADRADO_LINK(X, Y, ESCALA)			printf("<rect x=%d y=%d width=%d height=%d fill-opacity=0.0 />\n", \
												ESCALA * X, ESCALA* (Y+1), ESCALA, ESCALA)

#define TEXTO(TAM, TEXTO) 			printf("<p><font size=%s>%s</font></p>\n", \
												TAM, TEXTO)

#define HEX_LINK(X, Y, ESCALA)			printf("<image x=%d y=%d width=%d height=%d xlink:href=%s fill-opacity=0.0/>\n", \
												ESCALA * X, ESCALA* Y, ESCALA, ESCALA, "http://localhost/images/Hex1.png")

#define IMAGEM_DESV(X, Y, ESCALA, DESVIO, FICHEIRO)		printf("<image x=%d y=%d scale=3 xlink:href=%s />\n", \
												ESCALA * X + DESVIO, ESCALA * Y - 17 * Y, IMAGE_PATH FICHEIRO)

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
