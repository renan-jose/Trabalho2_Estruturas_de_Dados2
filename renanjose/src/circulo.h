/* Um círculo é uma figura geométrica plana composta por todos os pontos que estão a uma mesma distância 
 * (raio) de um ponto central chamado centro. */

/* Proteção contra múltiplas inclusões, garantindo que este cabeçalho seja incluído apenas uma vez 
 * durante a compilação, evitando redefinições e conflitos. */
#ifndef _CIRCULO_H_
#define _CIRCULO_H_

#include <stdbool.h>

typedef void* CirculoGenerico; /* CirculoGenerico representa um objeto círculo. */
typedef void* ArvoreGenerica;
typedef void* NoGenerico;

/* Cria um círculo com identificador id: (x,y) é o centro do círculo; r, seu raio, corb é a cor da borda e 
 * corp é a cor do preenchimento. */
CirculoGenerico criarCirculo(int id, double x, double y, double r, char *corb, char *corp);

/* Buscas */

/* Retorna o identificador do círculo. */
int buscarIdCirculo(CirculoGenerico);

/* Retorna a coordenada X do centro do círculo. */
double buscarCoordXCirculo(CirculoGenerico);

/* Retorna a coordenada Y do centro do círculo. */
double buscarCoordYCirculo(CirculoGenerico);

/* Retorna o raio do círculo. */
double buscarRaioCirculo(CirculoGenerico);

/* Retorna a cor da borda do círculo. */
char *buscarCorBordaCirculo(CirculoGenerico);

/* Retorna a cor de preenchimento do círculo. */
char *buscarCorPreenchimentoCirculo(CirculoGenerico);

/* Modificações */

/* Altera a coordenada X do centro do círculo. */
void mudarCoordXCirculo(CirculoGenerico, double);

/* Altera a coordenada Y do centro do círculo. */
void mudarCoordYCirculo(CirculoGenerico, double);

/* Altera o raio do círculo. */
void mudarRaioCirculo(CirculoGenerico, double);

/* Altera a cor da borda do círculo. */
void mudarCorBordaCirculo(CirculoGenerico, char *);

/* Altera a cor de preenchimento do círculo. */
void mudarCorPreenchimentoCirculo(CirculoGenerico c, char *); 

/* Utilitários */

/* Calcula a bounding box do círculo. */
void calcularBbCirculo(CirculoGenerico, double *, double *, double *, double *);

/* Libera toda a memória associada ao círculo. */
void desalocarCirculo(CirculoGenerico);

/* Determina se o círculo está completamente contido em uma região. */
bool circuloDentroRegiao(ArvoreGenerica, NoGenerico, CirculoGenerico, double, double, double, double);

/* Determina se o ponto (x,y) deva ser considerado "interno" ao círculo. */
bool pontoInternoCirculo(ArvoreGenerica, NoGenerico, CirculoGenerico, double, double);

#endif /* _CIRCULO_H_ */