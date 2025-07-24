/* Um retângulo é uma figura geométrica plana que tem: quatro lados, lados opostos iguais em comprimento,
 * e todos os ângulos internos medem 90 graus (são retos). */

/* Proteção contra múltiplas inclusões, garantindo que este cabeçalho seja incluído apenas uma vez 
 * durante a compilação, evitando redefinições e conflitos. */
#ifndef _RETANGULO_H_
#define _RETANGULO_H_

#include <stdbool.h>

typedef void* RetanguloGenerico; /* RetanguloGenerico representa um objeto retangulo. */
typedef void* ArvoreGenerica;
typedef void* NoGenerico;

/* Cria um retângulo com identificador id: (x,y) é a âncora do retângulo, w é a largura do retângulo e h, 
 * a altura. corb é a cor da borda e corp é a cor do preenchimento. */
RetanguloGenerico criarRetangulo(char *id, double x, double y, double w, double h, char *corb, char *corp, char *expb);

/* Buscas */

/* Retorna o identificador do retângulo. */
char *buscarIdRetangulo(RetanguloGenerico);

/* Retorna a coordenada X da âncora do retângulo. */
double buscarCoordXRetangulo(RetanguloGenerico);

/* Retorna a coordenada Y da âncora do retângulo. */
double buscarCoordYRetangulo(RetanguloGenerico);

/* Retorna a largura do retângulo. */
double buscarLarguraRetangulo(RetanguloGenerico);

/* Retorna a altura do retângulo. */
double buscarAlturaRetangulo(RetanguloGenerico);

/* Retorna a cor da borda do retângulo. */
char *buscarCorBordaRetangulo(RetanguloGenerico);

/* Retorna a cor de preenchimento do retângulo. */
char *buscarCorPreenchimentoRetangulo(RetanguloGenerico);

/* Retorna a expessura da borda do retângulo. */
char *buscarExpessuraBordaRetangulo(RetanguloGenerico);

/* Modificações */

/* Altera a coordenada X do retângulo. */
void mudarCoordXRetangulo(RetanguloGenerico, double);

/* Altera a coordenada Y do retângulo. */
void mudarCoordYRetangulo(RetanguloGenerico, double);

/* Altera a largura do retângulo. */
void mudarLarguraRetangulo(RetanguloGenerico, double);

/* Altera a altura do retângulo. */
void mudarAlturaRetangulo(RetanguloGenerico, double);

/* Altera a cor da borda do retângulo. */
void mudarCorBordaRetangulo(RetanguloGenerico, char *);

/* Altera a cor de preenchimento do retângulo. */
void mudarCorPreenchimentoRetangulo(RetanguloGenerico, char *);

/* Altera a expessura da borda do retângulo. */
void mudarExpessuraBordaRetangulo(RetanguloGenerico, char *);

/* Utilitários */

/* Calcula a bounding box do retângulo. */
void calcularBbRetangulo(RetanguloGenerico, double *, double *, double *, double *); 

/* Libera toda a memória associada ao retângulo. */
void desalocarRetangulo(RetanguloGenerico);

/* Determina se o retângulo está completamente contido em uma região. */
bool retanguloDentroRegiao(ArvoreGenerica, NoGenerico, RetanguloGenerico, double, double, double, double);

/* Determina se o ponto (x,y) deva ser considerado "interno" ao retângulo. */
bool pontoInternoRetangulo(ArvoreGenerica, NoGenerico, RetanguloGenerico, double, double);

#endif /* _RETANGULO_H_ */