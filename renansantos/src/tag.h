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

typedef void* GrafoGenerico;   // Representa o grafo como um todo

/* Insere a tag de abertura padrão de um arquivo SVG. 
 * FILE*: ponteiro para o arquivo onde o cabeçalho será escrito. */
void tagCabecalho(FILE*);

/* Insere a tag de fechamento padrão de um arquivo SVG. 
 * FILE*: ponteiro para o arquivo onde o rodapé será escrito. */
void tagRodape(FILE*);

/* Escreve a representação SVG de um retângulo.
 * FILE*: destino do conteúdo SVG.
 * FormaGeometricaGenerica: forma retângulo a ser desenhada. */
void tagRetangulo(FILE*, FormaGeometricaGenerica);

#endif // _TAG_H_ 
