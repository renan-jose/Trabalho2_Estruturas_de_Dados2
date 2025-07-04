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

/* Insere um elemento na tabela de espalhamento e retorna um indicador de sucesso.
 * Parâmetros: 
 *  - tabela de espalhamento (tipo TabelaGenerica);
 *  - chave do elemento na tabela de espalhamento (tipo char* (string));
 *  - dado da tabela de espalhamento (tipo DadoGenerico).
 * Retorno:
 *  - indicador de sucesso (tipo int).*/
int inserirElementoTabela(TabelaGenerica, char *, DadoGenerico);


// Remoção

/* Remove um elemento com determindada chave na tabela. 
 * Parâmetros: 
 *  - tabela de espalhamento (tipo TabelaGenerica);
 *  - chave do elemento na tabela de espalhamento (tipo char* (string)).
 * Retorno:
 *  - indicador de sucesso (tipo int). */
int removerElementoTabela(TabelaGenerica, char *);


// Busca

/* Busca e retorna um elemento com determinada chave na tabela. 
 * Parâmetros: 
 *  - tabela de espalhamento (tipo TabelaGenerica);
 *  - chave do elemento na tabela de espalhamento. (tipo char* (string)).
 * Retorno: 
 *  - elemento da tabela de espalhamento encontrado (tipo DadoGenerico).*/
DadoGenerico buscarElementoTabela(TabelaGenerica, char *);


// Desalocação

/* Desaloca a memória usada na construção da tabela de espalhamento. 
 * Parâmetros:
 *  - tabela de espalhamento (tipo TabelaGenerica).
 * Retorno: 
 *  - nenhum. */
void desalocarTabela(TabelaGenerica);

#endif // _ESPALHAMENTO_H_