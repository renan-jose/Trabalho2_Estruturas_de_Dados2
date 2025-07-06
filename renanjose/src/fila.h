/* Fila é uma das várias estruturas de dados existentes atualmente. Sua característica principal é
 * extamente igual a de uma fila de supermecado, por exemplo, com a inserção de um determinado dado 
 * no final e a remoção no início. 
 * A presente fila é genérica. Ou seja, ela pode armazenar qualquer tipo de dado*/

/* Proteção contra múltiplas inclusões, garantindo que este cabeçalho seja incluído apenas uma vez 
 * durante a compilação, evitando redefinições e conflitos. */
#ifndef _FILA_H_
#define _FILA_H_

/* Importação das bibliotecas para a implementação da fila. */
#include <stdbool.h> 

typedef void* FilaGenerica; /* FilaGenerica representa a fila como um todo */
typedef void* DadoGenerico; /* DadoGenerico representa um dado genérico a ser armazenado na fila. */

/* Cria e retorna uma nova fila genérica vazia. */
FilaGenerica criarFila();

/* Insere um dado (nó do tipo Node) no final da fila. */
void enfileirar(FilaGenerica, DadoGenerico);

/* Remove e retorna o primeiro elemento da fila (do tipo Node). */
DadoGenerico desenfileirar(FilaGenerica);

/* Retorna verdadeiro se a fila estiver vazia, falso caso contrário. */
bool filaVazia(FilaGenerica);

/* Libera toda a memória associada à fila. */
void desalocarFila(FilaGenerica);

#endif /* _FILA_H_ */