/* Um texto é uma entidade gráfica que representa uma mensagem ou palavra escrita que pode 
 * ser posicionada na tela.*/

/* Proteção contra múltiplas inclusões, garantindo que este cabeçalho seja incluído apenas 
 * uma vez durante a compilação, evitando redefinições e conflitos. */
#ifndef _TEXTO_H_
#define _TEXTO_H_

#include <stdbool.h>

typedef void* TextoGenerico; /* TextoGenerico representa um objeto texto. */
typedef void* ArvoreGenerica;
typedef void* NoGenerico;

/* Cria o texto txto com identificador id, nas coordenadas (x,y) e com a cores indicadas. corb é 
 * a cor da borda e corp é a cor do preenchimento. O parâmetro a determina a posição da âncora do 
 * texto: i, no início; m, no meio, f, no fim. O texto txto é o último parâmetro do comando. Pode 
 * incluir espaços em branco. O texto é delimitado por aspas, porém, as aspas não fazem parte dele.*/
TextoGenerico criarTexto(int id, double x, double y, char *corb, char *corp, char a, char *texto, char *fonte, char *peso, char *tamanho);

/* Buscas */

/* Retorna o identificador do texto. */
int buscarIdTexto(TextoGenerico);

/* Retorna a coordenada X do texto. */
double buscarCoordXTexto(TextoGenerico);

/* Retorna a coordenada Y do texto. */
double buscarCoordYTexto(TextoGenerico);

/* Retorna a cor da borda do texto. */
char *buscarCorBordaTexto(TextoGenerico);

/* Retorna a cor de preenchimento do texto. */
char *buscarCorPreenchimentoTexto(TextoGenerico);

/* Retorna a posição (período) da âncora do texto. */
char buscarPeriodoAncoraTexto(TextoGenerico);

/* Retorna o texto do objeto texto. */
char *buscarTextoTexto(TextoGenerico);

/* Retorna a fonte do texto do objeto texto. */
char *buscarFonteTexto(TextoGenerico);

/* Retorna o peso do texto do objeto texto. */
char *buscarPesoTexto(TextoGenerico);

/* Retorna o tamanho do texto do objeto texto. */
char *buscarTamanhoTexto(TextoGenerico);

/* Modificações */

/* Altera a coordenada X do texto */
void mudarCoordXTexto(TextoGenerico, double);

/* Altera a coordenada Y do texto */
void mudarCoordYTexto(TextoGenerico, double);

/* Altera a cor da borda da âncora do texto */
void mudarCorBordaTexto(TextoGenerico , char *);

/* Altera a cor de preenchimento da âncora do texto */
void mudarCorPreenchimentoTexto(TextoGenerico, char *);

/* Altera a posição (período) da âncora do texto. */
void mudarPeriodoAncoraTexto(TextoGenerico , char );

/* Altera o texto do objeto texto.*/
void mudarTextoTexto(TextoGenerico, char *);

/* Altera a fonte do texto. */
void mudarFonteTexto(TextoGenerico, char *);

/* Altera o peso (bold/normal) da fonte do texto. */
void mudarPesoTexto(TextoGenerico, char *);

/* Altera o tamanho da fonte do texto. */
void mudarTamanhoTexto(TextoGenerico, char *);

/* Utilitários */

/* Calcula a bounding box do texto. */
void calcularBbTexto(TextoGenerico, double *, double *, double *, double *);

/* Libera toda a memória associada ao texto. */
void desalocarTexto(TextoGenerico);

/* Determina se o texto está completamente contido em uma região. */
bool textoDentroRegiao(ArvoreGenerica, NoGenerico, TextoGenerico, double, double, double, double);

/* Determina se o ponto (x,y) deva ser considerado "interno" ao texto. */
bool pontoInternoTexto(ArvoreGenerica, NoGenerico, TextoGenerico, double, double);

#endif /* _TEXTO_H_ */