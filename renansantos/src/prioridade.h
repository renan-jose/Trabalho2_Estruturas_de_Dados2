/* Proteção contra múltiplas inclusões, garantindo que este cabeçalho seja incluído apenas uma vez 
 * durante a compilação, evitando redefinições e conflitos. */
#ifndef _PRIORIDADE_H_
#define _PRIORIDADE_H_

#include <stdbool.h>

// Explicar melhor tudo aqui depois

typedef void* FilaPrioridadeGenerica;
typedef void* DadoGenerico;

FilaPrioridadeGenerica criarFilaPrioridade();

void inserirFilaPrioridade(FilaPrioridadeGenerica, DadoGenerico, double);

DadoGenerico removerFilaPrioridade(FilaPrioridadeGenerica);

bool filaPrioridadeVazia(FilaPrioridadeGenerica);

void desalocarFilaPrioridade(FilaPrioridadeGenerica);

#endif // _PRIORIDADE_H_