/* O seguinte módulo lida com a leitura do arquivo de extensão .geo, o qual contém os comandos que especificam 
 * os dados que serão manipulados pelo programa, como retângulos, círculos, linhas e textos.
 * O objetivo é interpretar cada comando do arquivo e armazenar as formas correspondentes em uma árvore genérica,
 * preservando suas propriedades (posição, dimensões, cores etc.). */

/* Proteção contra múltiplas inclusões, garantindo que este cabeçalho seja incluído apenas uma vez 
 * durante a compilação, evitando redefinições e conflitos. */
#ifndef _ARMAZENAMENTO_H_
#define _ARMAZENAMENTO_H_

typedef void* ArvoreGenerica; /* ArvoreGenerica representa a estrutura de dados onde as formas serão armazenadas. */

/* Realiza a leitura de um arquivo .geo e insere as formas encontradas em uma árvore genérica.
 *
 * Parâmetros:
 * - ArvoreGenerica: ponteiro para a árvore onde as formas devem ser armazenadas.
 * - char *: caminho completo para o arquivo .geo que será lido.
 *
 * Cada linha do arquivo é interpretada como um comando (como `r`, `c`, `l`, `t`) que gera uma forma
 * geométrica a ser inserida na estrutura de dados.
 */
void armazenarFormas(ArvoreGenerica, char *);

#endif /* _ARMAZENAMENTO_H_ */
