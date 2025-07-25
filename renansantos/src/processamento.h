/* O seguinte módulo ou interface lida com a leitura e execução dos comandos contidos nos arquivos de extensões 
 * .geo, .via e .qry. Uma das possibilidades vindas da execução é a manipuçlação de estruturas de dados.
 *
 * Os resultados dessas operações são registrados em arquivos de saída (.svg e .txt). */

/* Proteção contra múltiplas inclusões, garantindo que este cabeçalho seja incluído apenas uma vez 
 * durante a compilação, evitando redefinições e conflitos. */
#ifndef _PROCESSAMENTO_H_
#define _PROCESSAMENTO_H_

typedef void* GrafoGenerico;
typedef void* TabelaGenerica;

/* Processa os comandos presentes nos arquivos .geo, .via e .qry, operando sobre estruturas de dados e gerando os 
 * arquivos de saída correspondentes. 
 * Parametros:
 *      - char * = nome do arquivo de extensão .geo;
 *      - char * = nome do arquivo de extensão .via;
 *      - char * = nome do arquivo de extensão .qry;
 *      - char * = nome do arquivo de extensão .svg a ser gerado;
 *      - char * = nome do arquivo de extensão .txt a ser gerado;
 * Retorno:
 *      - nenhum;
 */
void processarQry(char *, char *,  char *, char *, char *);

#endif // _PROCESSAMENTO_H_