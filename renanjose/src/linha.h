/* Uma linha é uma figura geométrica reta que liga dois pontos distintos no plano. */

/* Proteção contra múltiplas inclusões, garantindo que este cabeçalho seja incluído apenas uma vez 
 * durante a compilação, evitando redefinições e conflitos. */
#ifndef _LINHA_H_
#define _LINHA_H_

#include <stdbool.h>

typedef void* LinhaGenerica; /* LinhaGenerica representa um objeto linha. */
typedef void* ArvoreGenerica;
typedef void* NoGenerico;

/* Cria uma linha com identificador id, com extremidades nos pontos (x1,y1) e (x2,y2), com a cor 
 * especificada. A linha está orientada da origem (x1,y1) para outra extremidade, destino (x2,y2).*/
LinhaGenerica criarLinha(int id, double x1, double y1, double x2, double y2, char *cor);

/* Buscas */

/* Retorna o identificador da linha. */
int buscarIdLinha(LinhaGenerica);

/* Retorna a coordenada X1 (origem) da linha. */
double buscarX1Linha(LinhaGenerica);

/* Retorna a coordenada Y1 (origem) da linha. */
double buscarY1Linha(LinhaGenerica);

/* Retorna a coordenada X2 (destino) da linha. */
double buscarX2Linha(LinhaGenerica);

/* Retorna a coordenada Y2 (destino) da linha. */
double buscarY2Linha(LinhaGenerica);

/* Retorna a cor da linha. */
char *buscarCorLinha(LinhaGenerica);

/* Modificações */

/* Altera a coordenada X1 da linha. */
void mudarX1Linha(LinhaGenerica, double);

/* Altera a coordenada Y1 da linha. */
void mudarY1Linha(LinhaGenerica, double);

/* Altera a coordenada X2 da linha. */
void mudarX2Linha(LinhaGenerica, double);

/* Altera a coordenada Y2 da linha. */
void mudarY2Linha(LinhaGenerica, double);

/* Altera a cor da linha. */
void mudarCorLinha(LinhaGenerica, char *);

/* Utilitários */

/* Calcula o bounding box da linha. */
void calcularBbLinha(LinhaGenerica, double *, double *, double *, double *);

/* Libera toda a memória associada à linha. */
void desalocarLinha(LinhaGenerica);

/* Determina se a linha está completamente contida em uma região. */
bool linhaDentroRegiao(ArvoreGenerica, NoGenerico, LinhaGenerica, double, double, double, double);

/* Determina se o ponto (x,y) deva ser considerado "interno" à linha. */
bool pontoInternoLinha(ArvoreGenerica, NoGenerico, LinhaGenerica, double, double);

#endif /* _LINHA_H_ */