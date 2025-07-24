/* A fila normal foi explicada anteriormente. A presente é uma estrutura de dados que funciona como 
 * uma fila comum, mas com prioridade: os elementos são removidos de acordo com sua prioridade, 
 * e não pela ordem de chegada. */

/* Proteção contra múltiplas inclusões, garantindo que este cabeçalho seja incluído apenas uma vez 
 * durante a compilação, evitando redefinições e conflitos. */
#ifndef _PRIORIDADE_H_
#define _PRIORIDADE_H_

// Importação das bibliotecas.
#include <stdbool.h>

// Nomes alternativos para os tipos de dados existentes.
typedef void* FilaPrioridadeGenerica; // Representa um fila como um todo.
typedef void* DadoGenerico; // Representa um dado qualquer como um todo.

// Criação.

// Cria e retorna uma nova fila de prioridade genérica vazia. 
FilaPrioridadeGenerica criarFilaPrioridade();


// Inserção.

// Insere um dado genérico (qualquer tipo) na fila de prioridade.
void inserirFilaPrioridade(FilaPrioridadeGenerica, DadoGenerico, double);


// Remoção.

// Remove um dado genérico (qualquer tipo) da fila de prioridade.
DadoGenerico removerFilaPrioridade(FilaPrioridadeGenerica);


// Buscas.

// Confirma ou não a não existÊncia de um elemento na fila de prioridade.
bool filaPrioridadeVazia(FilaPrioridadeGenerica);


// Desalocação.

// Desaloca a memória alocada pela fila de prioridade.
void desalocarFilaPrioridade(FilaPrioridadeGenerica);

#endif // _PRIORIDADE_H_