/* O seguinte módulo ou interface lida com a leitura e execução dos comandos contidos nos arquivos de extensões 
 * .geo, .via e .qry. Uma das possibilidades vindas da execução é a manipuçlação de estruturas de dados.
 *
 * Os resultados dessas operações são registrados em arquivos de saída (.svg e .txt).
 */


/* Proteção contra múltiplas inclusões, garantindo que este cabeçalho seja incluído apenas uma vez 
 * durante a compilação, evitando redefinições e conflitos. */
#ifndef _PROCESSAMENTO_H_
#define _PROCESSAMENTO_H_

/* Processa os comandos presentes nos arquivos .geo, .via e .qry, operando sobre estruturas de dados e gerando os 
 * arquivos de saída correspondentes. 
 */

// Explicar depois  
void processarArquivos(char *, char *,  char *, char *, char *);

#endif // _PROCESSAMENTO_H_