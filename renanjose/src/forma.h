/* Uma forma geométrica é qualquer figura que pode ser representada no plano ou no espaço,
 * formada por pontos, linhas,  curvas ou superfícies com características bem definidas, 
 * como tamanho, posição e contorno. */

/* Proteção contra múltiplas inclusões, garantindo que este cabeçalho seja incluído apenas 
 * uma vez durante a compilação, evitando redefinições e conflitos. */
#ifndef _FORMA_H_
#define _FORMA_H_

/* Importação das bibliotecas para a implementação da forma geométrica. */
#include "retangulo.h"  
#include "circulo.h" 
#include "linha.h"
#include "texto.h" 

typedef void *FormaGeometricaGenerica; /* FormaGeometricaGenerica representa um objeto forma geométrica */
typedef void* ArvoreGenerica; /* ArvoreGenerica representa uma árvore no todo. */
typedef void* NoGenerico; /* NoGenerico representa um nó da árvore. */

/* Criações/Formulações */

/* Cria um retângulo com identificador id: (x,y) é a âncora do retângulo, w é a largura do retângulo e h, 
 * a altura. corb é a cor da borda e corp é a cor do preenchimento. */
FormaGeometricaGenerica formularRetangulo(int id, double x, double y, double w, double h, char *corb, char *corp);

/* Cria um círculo com identificador id: (x,y) é o centro do círculo; r, seu raio, corb é a cor da borda e 
 * corp é a cor do preenchimento. */
FormaGeometricaGenerica formularCirculo(int id, double x, double y, double r, char *corb, char *corp);

/* Cria uma linha com identificador id, com extremidades nos pontos (x1,y1) e (x2,y2), com a cor 
 * especificada. A linha está orientada da origem (x1,y1) para outra extremidade, destino (x2,y2).*/
FormaGeometricaGenerica formularLinha(int id, double x1, double y1, double x2, double y2, char *c);

/* Cria o texto txto com identificador id, nas coordenadas (x,y) e com a cores indicadas. corb é 
 * a cor da borda e corp é a cor do preenchimento. O parâmetro p determina a posição da âncora do 
 * texto: i, no início; m, no meio, f, no fim. O texto txto é o último parâmetro do comando. Pode 
 * incluir espaços em branco. O texto é delimitado por aspas, porém, as aspas não fazem parte dele.*/
FormaGeometricaGenerica formularTexto(int id, double x, double y, char *corb, char *corp, char p, char *t, char *fonte, char *peso, char *tamanho);

/* Retorna a forma concreta da forma geométrica. */
FormaGeometricaGenerica buscarFormaGeometrica(FormaGeometricaGenerica);

/* Buscas */

/* Retorna a coordenada X da forma geométrica. */
double buscarCoordXFormaGeometrica(FormaGeometricaGenerica);

/* Retorna a coordenada Y da forma geométrica. */
double buscarCoordYFormaGeometrica(FormaGeometricaGenerica);

/* Retorna o tipo da forma geométrica. */
char buscarTipoFormaGeometrica(FormaGeometricaGenerica);

/* Retorna o identificador da forma geométrica */
int buscarIdFormaGeometrica(FormaGeometricaGenerica);

/* Retorna a cor da borda da forma geométrica. */
char *buscarCorBordaFormaGeometrica(FormaGeometricaGenerica);

/* Retorna a cor de preenchimento da forma geométrica. */
char *buscarCorPreenchimentoFormaGeometrica(FormaGeometricaGenerica);

/* Retorna a largura da forma geométrica (se aplicável, como em retângulo ou texto). */
double buscarLarguraFormaGeometrica(FormaGeometricaGenerica);

/* Retorna a altura da forma geométrica (se aplicável, como em retângulo ou texto). */
double buscarAlturaFormaGeometrica(FormaGeometricaGenerica);

/* Modificações */

/* Altera a coordenada X da forma geométrica. */
void mudarCoordXFormaGeometrica(FormaGeometricaGenerica, double);

/* Altera a coordenada Y da forma geométrica. */
void mudarCoordYFormaGeometrica(FormaGeometricaGenerica, double);

/* Altera o tipo da forma geométrica. */
void mudarTipoFormaGeometrica(FormaGeometricaGenerica, char);

/* Utilitários */

/* Calcula a bounding box da forma geométrica. */
void calcularBbFormaGeometrica(FormaGeometricaGenerica, double *, double *, double *, double *);

/* Libera a memória associada à forma geométrica. */
void desalocarFormaGeometrica(FormaGeometricaGenerica);

/* Determina se a forma está completamente contida em uma região. */
bool formaDentroRegiao(ArvoreGenerica, NoGenerico, FormaGeometricaGenerica, double, double, double, double);

/* Determina se o ponto (x,y) deva ser considerado "interno" à forma. */
bool pontoInternoforma(ArvoreGenerica, NoGenerico, FormaGeometricaGenerica, double, double);

/* Converte o tipo de forma (ex: 'r', 'c', 'l', 't') em um valor inteiro descritivo. */
int tipoFormaParaDescritor(char);

/* Clona a forma fornecida, deslocando-a por dx e dy, e aplicando novas cores de borda e preenchimento. */
FormaGeometricaGenerica clonarForma(FormaGeometricaGenerica, double, double, char *, char *);

#endif /* _FORMA_H_ */