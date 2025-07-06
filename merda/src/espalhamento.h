/* Comentar depois o que é uma tabela de espalhamento. */

/* Proteção contra múltiplas inclusões, garantindo que este cabeçalho seja incluído apenas uma vez 
 * durante a compilação, evitando redefinições e conflitos. */
#ifndef _ESPALHAMENTO_H_
#define _ESPALHAMENTO_H_

// Nomes alternativos para os tipos de dados existentes.
typedef void* TabelaGenerica; // Representa uma tabela de espalhamento ao todo.
typedef void* DadoGenerico; // Representa um dado de um tipo qualquer a ser armazenado na tabela de espalhamento.

// Inicialização

/* Cria e retorna uma nova tabela de espalhamento vazia de algum tamanho inteiro. 
 * Parâmetros: 
 *  - tamanho da tabela (tipo int). 
 * Retorno: 
 *  - tabela de espalhamento (tipo TabelaGenerica). */
TabelaGenerica criarTabela(int);


// Inserção

// Explicar
/* Insere um elemento na tabela de espalhamento e retorna um indicador de sucesso.
 * Parâmetros: 
 *  - tabela de espalhamento (tipo TabelaGenerica);
 *  - chave do elemento da tabela de espalhamento (tipo char* (string));
 *  - dado da tabela de espalhamento (tipo DadoGenerico).
 * Retorno:
 *  - indicador de sucesso (1 para inserção bem-sucedida e 0 para mal-sucedida) (tipo int).*/
int inserirElementoTabela(TabelaGenerica, char *, DadoGenerico);


// Remoção

// Explicar
int removerElementoTabela(TabelaGenerica, char *);


// Busca

//Explicar
DadoGenerico buscarElementoTabela(TabelaGenerica, char *);


// Desalocação

// Explicar
void desalocarTabela(TabelaGenerica);

#endif // _ESPALHAMENTO_H_