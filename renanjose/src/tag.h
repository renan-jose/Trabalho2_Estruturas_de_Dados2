/* Um arquivo SVG (Scalable Vector Graphics) é um arquivo XML usado para descrever 
 * gráficos vetoriais bidimensionais. Este módulo gera essas representações a partir 
 * das formas geométricas armazenadas em uma árvore genérica. */

/* Proteção contra múltiplas inclusões, garantindo que este cabeçalho seja incluído apenas uma vez 
 * durante a compilação, evitando redefinições e conflitos. */
#ifndef _TAG_H_
#define _TAG_H_

/* Importação das bibliotecas. */
#include "forma.h"  
#include <stdio.h>

typedef void* ArvoreGenerica;   // Representa a árvore como um todo
typedef void* NoGenerico;       // Representa o nó da árvore

/* Insere a tag de abertura padrão de um arquivo SVG. 
 * FILE*: ponteiro para o arquivo onde o cabeçalho será escrito. */
void tagCabecalho(FILE*);

/* Insere a tag de fechamento padrão de um arquivo SVG. 
 * FILE*: ponteiro para o arquivo onde o rodapé será escrito. */
void tagRodape(FILE*);

/* Função de visita chamada ao percorrer a árvore. 
 * Gera a tag SVG correspondente a uma forma.
 * ArvoreGenerica: árvore onde a forma está inserida.
 * NoGenerico: nó da árvore correspondente à forma.
 * FormaGeometricaGenerica: forma geométrica a ser desenhada.
 * double, double: coordenadas de âncora (podem ser ignoradas dependendo da forma).
 * void* dado auxiliar genérico (usado como contexto ou arquivo de saída). */
void tagForma(ArvoreGenerica, NoGenerico, FormaGeometricaGenerica, double, double, void*);

/* Escreve a representação SVG de um retângulo.
 * FILE*: destino do conteúdo SVG.
 * FormaGeometricaGenerica: forma retângulo a ser desenhada. */
void tagRetangulo(FILE*, FormaGeometricaGenerica);

/* Escreve a representação SVG de um círculo.
 * FILE*: destino do conteúdo SVG.
 * FormaGeometricaGenerica: forma círculo a ser desenhada. */
void tagCirculo(FILE*, FormaGeometricaGenerica);

/* Escreve a representação SVG de uma linha.
 * FILE*: destino do conteúdo SVG.
 * FormaGeometricaGenerica: forma linha a ser desenhada. */
void tagLinha(FILE*, FormaGeometricaGenerica);

/* Escreve a representação SVG de um texto.
 * FILE*: destino do conteúdo SVG.
 * FormaGeometricaGenerica: forma texto a ser desenhada. */
void tagTexto(FILE*, FormaGeometricaGenerica);

#endif /* _TAG_H_ */
