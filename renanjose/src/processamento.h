/* O seguinte módulo lida com a leitura e execução dos comandos contidos no arquivo de extensão .qry.
 * Esse arquivo representa o segundo estágio de execução do programa, e pode conter:
 * - Consultas sobre as formas inseridas via .geo (ex: verificar contenção, sobreposição etc.),
 * - Alterações de propriedades (ex: mudar cor, transladar),
 * - Remoções ou seleções de subconjuntos de formas,
 * - Inserção de novas formas derivadas (ex: clones, disparos).
 *
 * Os resultados dessas operações são registrados em arquivos de saída (.svg, .txt, .csv).
 */

/* Proteção contra múltiplas inclusões, garantindo que este cabeçalho seja incluído apenas uma vez 
 * durante a compilação, evitando redefinições e conflitos. */
#ifndef _PROCESSAMENTO_H_
#define _PROCESSAMENTO_H_

typedef void* ArvoreGenerica; /* ArvoreGenerica representa a árvore principal de armazenamento e manipulação. */

/* Processa os comandos presentes no arquivo .qry, operando sobre a árvore de formas e gerando os arquivos
 * de saída correspondentes.
 *
 * Parâmetros:
 * - ArvoreGenerica: estrutura que contém as formas previamente carregadas do .geo.
 * - char *: caminho para o arquivo .qry a ser processado.
 * - char *: caminho para o arquivo .svg de saída após o processamento do .qry.
 * - char *: caminho para o arquivo .txt que conterá os relatórios das operações.
 * - char *: caminho para o arquivo .csv com dados tabulares (ex: áreas, colisões, estatísticas).
 */
void processarFormas(ArvoreGenerica, char *, char *, char *, char *);

#endif /* _PROCESSAMENTO_H_ */
