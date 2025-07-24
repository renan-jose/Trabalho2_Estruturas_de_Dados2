/* Explicar a lista. */

/* Proteção contra múltiplas inclusões, garantindo que este cabeçalho seja incluído apenas uma vez 
 * durante a compilação, evitando redefinições e conflitos. */
#ifndef _LISTA_H_
#define _LISTA_H_

/* Importação das bibliotecas */
#include <stdbool.h>

typedef void* Lista; /* Lista representa uma lista ao todo. */
typedef void* DadoGenerico; /* DadoGenerico representa um dado genérico a ser armazenado na lista. */

/* Inicialização */

/* Cria e retorna uma nova lista genérica vazia. */
Lista inicializarLista();


/* Inserções */

/* Insere um dado genérico (qualquer tipo) no início da lista. */
void inserirInicioLista(Lista, DadoGenerico);

void inserirFimLista(Lista, DadoGenerico);


/* Remoções */

/* Remove o primeiro elemento da lista. */
void removerInicioLista(Lista);


/* Buscas/Verificações */

/* Retorna o número total de elementos presentes na lista. */
int buscarTamanhoLista(Lista);

/* Retorna verdadeiro se o dado estiver na lista. */
bool verificarElementoLista(Lista, DadoGenerico);

/* Retorna o dado armazenado na posição indicada da lista. */
DadoGenerico buscarElementoLista(Lista, int);

/* Desalocação */

/* Libera toda a memória associada à lista. */
void desalocarLista(Lista);

#endif /* _LISTA_H_ */