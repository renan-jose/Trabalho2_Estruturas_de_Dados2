/* Lista é uma das várias estruturas de dados existentes atualmente. Sua característica principal é 
 * a flexibilidade nas inserções e remoções de dados. Ou seja, ela pode atuar como uma pilha, 
 * inserindo e removendo os dados no início da lista, como uma fila, inserindo os dados no final e
 * removendo no início, ou como uma pilha e uma fila e mais um extra: inserindo e removendo os dados
 * no meio da lista.
 * A presente lista atua como uma pilha e é genérica, armazenando qualquer tipo de dado. */

/* Proteção contra múltiplas inclusões, garantindo que este cabeçalho seja incluído apenas uma vez 
 * durante a compilação, evitando redefinições e conflitos. */
#ifndef _LISTA_H_
#define _LISTA_H_

/* Importação das bibliotecas para a implementação da lista. */
#include <stdbool.h> 

typedef void* Lista; /* Lista representa a lista como um todo. */
typedef void* DadoGenerico; /* DadoGenerico representa um dado genérico a ser armazenado na lista. */

/* Cria e retorna uma nova lista genérica vazia. */
Lista inicializarLista();

/* Insere um dado (nó do tipo Node) no início da lista. */
void listar(Lista, DadoGenerico);

/* Remove o primeiro elemento da lista. */
void deslistar(Lista);

/* Retorna verdadeiro se a lista estiver vazia, falso caso contrário. */
bool listaVazia(Lista);

/* Libera toda a memória associada à lista. */
void desalocarLista(Lista);

/* Retorna o dado armazenado na posição indicada da lista. */
DadoGenerico buscarElementoLista(Lista, int);

/* Retorna o número total de elementos presentes na lista. */
int buscarTamanhoLista(Lista);

#endif /* _LISTA_H_ */