/* Um arquivo é uma unidade de armazenamento de dados em um sistema computacional, que permite 
 * guardar informações. Ele pode conter texto, por exemplo. Cada arquivo possui um nome, uma extensão 
 * (como .svg, .dot, .csv) e pode ser lido, escrito ou modificado por programas ou usuários. */

/* Este módulo trata o processamento dos parâmetros da linha de comando e geração de caminhos de arquivos
 * para o programa principal (ted). O programa aceita os seguintes parâmetros:
 * 
 * ted [-e path] -f arq.geo [-q consulta.qry] -o dir
 *
 * O primeiro parâmetro (-e) indica o diretório base de entrada ($DIR_ENTRADA). É opcional. 
 * Caso não seja informado, o diretório de entrada é o diretório corrente da aplicação. 
 * O segundo parâmetro (-f) especifica o nome do arquivo de entrada que deve ser encontrado sob o
 * diretório informado pelo primeiro parâmetro. 
 * O terceiro parâmetro (-q) é um arquivo de consultas (também sob o diretório de entrada). 
 * O último parâmetro (-o) indica o diretório onde os arquivos de saída (*.svg e *.txt) devem ser colocados. 
 * Os nomes de arquivos podem ser precedidos por um caminho relativo; dir e path podem ser caminhos absolutos ou relativos .
 * A ordem dos parâmetros pode variar. */

/* Proteção contra múltiplas inclusões, garantindo que este cabeçalho seja incluído apenas uma vez 
 * durante a compilação, evitando redefinições e conflitos. */
#ifndef _ARQUIVO_H_
#define _ARQUIVO_H_

typedef void* ArvoreGenerica; /* ArvoreGenerica representa uma árvore no todo. */
typedef void* ParametrosGenericos; /* ParametrosGenericos representa os parâmetros da linha de comando. */

#include <stdbool.h>

/* Gerações/Criações de arquivos e parâmetros */

/* Cria e inicializa a estrutura de parâmetros.
 * Retorno:
 * - ParametrosGenericos: ponteiro para o objeto criado.
 */
ParametrosGenericos criarParametros();

/* Buscas */

/* Retorna o diretório de entrada especificado ou "." se não informado.
 * Parâmetros:
 * - ParametrosGenericos: objeto de parâmetros.
 * Retorno:
 * - char *: caminho do diretório de entrada.
 */
char *buscarDiretorioEntrada(ParametrosGenericos);

/* Retorna o nome do arquivo .geo informado.
 * Parâmetros:
 * - ParametrosGenericos: objeto de parâmetros.
 * Retorno:
 * - char *: nome do arquivo .geo.
 */
char *buscarArquivoGeo(ParametrosGenericos);

/* Retorna o diretório de saída especificado.
 * Parâmetros:
 * - ParametrosGenericos: objeto de parâmetros.
 * Retorno:
 * - char *: caminho do diretório de saída.
 */
char *buscarDiretorioSaida(ParametrosGenericos);

/* Retorna o nome do arquivo .qry, se houver.
 * Parâmetros:
 * - ParametrosGenericos: objeto de parâmetros.
 * Retorno:
 * - char *: nome do arquivo .qry ou NULL se não especificado.
 */
char *buscarArquivoQry(ParametrosGenericos);

/* Retorna o caminho completo do arquivo .geo (diretório + nome).
 * Parâmetros:
 * - ParametrosGenericos: objeto de parâmetros.
 * Retorno:
 * - char *: caminho completo (string alocada dinamicamente).
 */
char *buscarCaminhoCompletoGeo(ParametrosGenericos);

/* Retorna o caminho completo do arquivo .qry (diretório + nome).
 * Parâmetros:
 * - ParametrosGenericos: objeto de parâmetros.
 * Retorno:
 * - char *: caminho completo (string alocada dinamicamente) ou NULL.
 */
char *buscarCaminhoCompletoQry(ParametrosGenericos);

/* Retorna o nome base do arquivo .geo (sem extensão e sem caminho).
 * Parâmetros:
 * - ParametrosGenericos: objeto de parâmetros.
 * Retorno:
 * - char *: nome base do .geo (string alocada dinamicamente).
 */
char *buscarNomeBaseGeo(ParametrosGenericos);

/* Retorna o nome base do arquivo .qry (sem extensão e sem caminho).
 * Parâmetros:
 * - ParametrosGenericos: objeto de parâmetros.
 * Retorno:
 * - char *: nome base do .qry (string alocada dinamicamente) ou NULL.
 */
char *buscarNomeBaseQry(ParametrosGenericos);

/* Gera o caminho completo do SVG após o processamento do .geo.
 * Parâmetros:
 * - ParametrosGenericos: objeto de parâmetros.
 * Retorno:
 * - char *: caminho do arquivo .svg (string alocada dinamicamente).
 */
char *buscarCaminhoSvgBase(ParametrosGenericos);

/* Gera o caminho completo do SVG após o processamento do .qry.
 * Parâmetros:
 * - ParametrosGenericos: objeto de parâmetros.
 * Retorno:
 * - char *: caminho do arquivo .svg (string alocada dinamicamente) ou NULL.
 */
char *buscarCaminhoSvgConsulta(ParametrosGenericos);

/* Gera o caminho completo do arquivo .txt gerado após o .qry.
 * Parâmetros:
 * - ParametrosGenericos: objeto de parâmetros.
 * Retorno:
 * - char *: caminho do arquivo .txt (string alocada dinamicamente) ou NULL.
 */
char *buscarCaminhoTxtConsulta(ParametrosGenericos);

/* Utilitários */

/* Verifica se foi especificado um arquivo .qry.
 * Parâmetros:
 * - ParametrosGenericos: objeto de parâmetros.
 * Retorno:
 * - int: 1 se houver .qry, 0 caso contrário.
 */
int temArquivoQry(ParametrosGenericos);

/* Processa os argumentos da linha de comando.
 * Parâmetros:
 * - ParametrosGenericos: objeto de parâmetros criado.
 * - int: quantidade de argumentos (argc).
 * - char *[]: vetor de argumentos (argv).
 * Retorno:
 * - int: 1 se sucesso, 0 em caso de erro.
 */
int processarArgumentos(ParametrosGenericos, int, char *[]);

/* Libera a memória alocada para os parâmetros.
 * Parâmetros:
 * - ParametrosGenericos: objeto a ser destruído.
 */
void desalocarParametros(ParametrosGenericos);

int temArquivoVia(ParametrosGenericos);

char *buscarArquivoVia(ParametrosGenericos);

char *buscarCaminhoCompletoVia(ParametrosGenericos);

#endif /* _ARQUIVO_H_ */
