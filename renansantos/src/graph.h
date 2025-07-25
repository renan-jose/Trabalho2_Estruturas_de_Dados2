/*
 * Um Grafo G é constituído por um conjunto de vértices V e um conjunto de arestas E, denotado por G=(V,E). 
 *
 * Pode-se associar um dado a arestas e a vértices de G. A cada vértice é associado um nome.
 *
 * Podem ser definidos subgrafos de um dado grafo. Um subgrafo pode ser entendido como uma visão (ou filtro)
 * sobre o grafo original. Ou, ainda, um subgrafo define (delimita) uma "região" do grafo a ser acessada.
 *
 * Cada subgrafo possui um nome. Um grafo pode pode possuir vários subgrafos. Arestas podem ser acrescentas ou 
 * retiradas do subgrafo. Existem também outras operações que possibilitam percorrer o subgrafo, invocando funções 
 * quando arestas do grafo são "descobertas", podendo, inclusive, interromper o percurso.
 *
 * Este módulo faz parte de um trabalho de Estrutura de Dados 2, do curso de Ciência da Computação, da Universidade 
 * Estadual de Londrina. É expressamente proibido o uso de ferramentas de IA para a sua implementação.
 * A documentação deste módulo deve ser melhorada.
*/

/* Proteção contra múltiplas inclusões, garantindo que este cabeçalho seja incluído apenas uma vez 
 * durante a compilação, evitando redefinições e conflitos. */
#ifndef _GRAPH__
#define _GRAPH__

/* Importação das bibliotecas. */
#include "lista.h"
#include "smutreap.h"
#include <stdbool.h>

// Nomes alternativos para os tipos de dados existentes.
typedef void *Graph;
typedef int Node;
typedef void *Edge;
typedef void *Info;

/* Invocado quando uma aresta é "descoberta"/"percorrida"/"classificada". 
 * Também informa os tempos de descoberta e finalização. */
typedef bool (*procEdge)(Graph g, Edge e,int td, int tf, void *extra); 


/* Invocado quando percurso é recomeçado. */
typedef bool (*dfsRestarted)(Graph g, void *extra);


/* Cria um grafo com, no máximo, "nVert" vértices. 
 * Parâmetros:
 *  - nVert = número de vértices do grafo (tipo int).
 * Retorno: 
 *  - Novo grafo (tipo Graph). */
Graph createGraph(int nVert, bool directed, char *nome);


/* Retorna número máximo de vértices que grafo g pode conter. 
 * Parâmetros: 
 *    - g = grafo (tipo Graph);
 * Retorno: 
 *    - número (tipo int) */
int getMaxNodes(Graph g);


/* Retorna número total de vértices adicionados ao grafo g. 
 * Parâmetos: 
 *    - g = grafo (tipo Graph);
 * Retorno:
 *    - numero (tipo int); */
int getTotalNodes(Graph g);


/* Adiciona um novo vértice ao grafo g com o nome "nome". 
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - *nome = nome do vértice do grafo g (tipo char (string));
 *    - info = informação associada ao vértice do grafo g (tipo Info). 
 * Retorno:
 *    - número (tipo Node (int)); */
Node addNode(Graph g, char *nome, Info info);


/* Retorna o vértice do grafo g cujo nome é "nome". 
 * Parâmetros: 
 *    - g = grafo (tipo Graph);
 *    - *nome = nome do vértice do grafo g (tipo char (string)); 
 * Retorno:
 *    - número (tipo Node (int)); */
Node getNode(Graph g, char *nome);


/* Retorna a informação associada ao vértice node do grafo g.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - node = número do nó (vértice) do grafo g (tipo Node);
 * Retorno:
 *    - informação do vértice (tipo Info); */
Info getNodeInfo(Graph g, Node node); 


/* Retorna a nome do vértice node do grafo g.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - node = número do vértice do grafo g (tipo Node);
 * Retorno: 
 *    - nome do vértice do grafo g (tipo char* (string));
 */
char *getNodeName(Graph g, Node node);


/* Altera a informação associada vértice node do grafo g.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - node = número do vértice do grafo g (tipo Node);
 *    - info = nova informação do vértice do grafo g.
 * Retorno: 
 *    - nenhum;
 */
void setNodeInfo(Graph g, Node node, Info info);


/* Adiciona uma nova aresta no grafo g e a retorna.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - from = vértice de origem da nova aresta (tipo Node);
 *    - to = vértice de destino da nova aresta (tipo Node);
 *    - info = informação associada à nova aresta (tipo Info);
 * Retorno:
 *    - nova aresta (tipo Edge) 
 */
Edge addEdge(Graph g, Node from, Node to, Info info);


/* Retorna a aresta do grafo g com vértice de origem from e destino to.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - from = vértice de origem da aresta pertencente ao grafo g (tipo Node);
 *    - to = vértice de destino da aresta pertencente ao grafo g (tipo Node);
 * Retorno:
 *    - aresta alvo da busca (tipo Edge);
 */
Edge getEdge(Graph g, Node from, Node to);


/* Retorna o vértice de origem da aresta e, pertencente ao grafo g.
 * Parâmetros: 
 *    - g = grafo (tipo Graph);
 *    - e = aresta do grafo g (tipo Edge);
 * Retorno:
 *    - número do vértice (tipo Node);
 */
Node getFromNode(Graph g, Edge e);

  
/* Retorna o vértice de destino da aresta e, pertencente ao grafo g.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - e = aresta do grafo g (tipo Edge);
 * Retorno: 
 *    - número do vértice (tipo Node);
 */  
Node getToNode(Graph g, Edge e);


/* Retorna a informação associada à aresta e, pertencente ao grafo g.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - e = aresta (tipo Edge);
 * Retorno:
 *    - informação da aresta e (tipo Info);
 */
Info getEdgeInfo(Graph g, Edge e);


/* Altera a informação associada à aresta e, pertencenete ao grafo g.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - e = aresta do grafo g (tipo Edge);
 *    - info = nova informação da aresta e, pertencente ao grafo g;
 * Retorno:
 *    - nenhum;
 */
void setEdgeInfo(Graph g, Edge e, Info info);


/* Remove a aresta e, pertencente ao grafo g.
 * Parâmetros: 
 *    - g = grafo (tipo Graph);
 *    - e = aresta do grafo g (tipo Edge);
 * Retorno:
 *    - nenhum;
 */
void removeEdge(Graph g, Edge e);


/* Verifica a adjacência de um vérice do grafo g.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - from = vértice de origem da aresta (tipo Node);
 *    - to = vértice de destino da aresta (tipo Node);
 * Retorno:
 *    - true se o vértice for adjacente, ou false, caso contrário (tipo bool);
 */
bool isAdjacent(Graph g, Node from, Node to);


/* Adiciona a lista nosAdjacentes os vértices adjacentes à node.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - node = vértice do grafo g (tipo Node);
 *    - nosAdjacentes = lista simples (tipo Lista);
 * Retorno:
 *    - nenhum;
 */
void adjacentNodes(Graph g, Node node, Lista nosAdjacentes);


/* Adiciona à lista arestaAdjacentes as arestas (x,y), tal que, x == node.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - node = vértice do grafo g;
 *    - arestasAdjacentes = lista simples (tipo Lista);
 * Retorno:
 *    - nenhum;
 */
void adjacentEdges(Graph g, Node node, Lista arestasAdjacentes);


/* Insere na lista nomesNodes os nomes atribuidos aos vértices do grafo.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - nomesNodes = lista simples (tipo Lista);
 * Retorno:
 *    - nenhum;
 */
void getNodeNames(Graph g, Lista nomesNodes);


/* Insere na lista arestas, as arestas do grafo g.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - arestas = lista simples (tipo Lista);
 * Retorna:
 *    - nenhum;
 */
void getEdges(Graph g, Lista arestas);


/*
   Faz percurso em profundidade sobre g, a partir do nó node, classificando 
   as arestas do grafo, invocando a respectiva funcao.
      A busca em profundidade, eventualmente, pode produzir uma floresta.
   newTree e' invocada sempre que o percurso for retomado.
 */  
bool dfs(Graph g, Node node, procEdge treeEdge, procEdge forwardEdge, procEdge returnEdge, procEdge crossEdge, dfsRestarted newTree, void *extra);


/*
   Percorre o grafo g em largura, a partir do no' node. discoverNode e' usada
   para a aresta (x,y) usada para "descobrir" o y.
 */
bool bfs(Graph g, Node node, procEdge discoverNode, void *extra);


/* Desaloca a memória usada pelo grafo g.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 * Retorno:
 *    - nenhum;
 */
void killDG(Graph g);


/*
 **********************
 * SUB-GRAFOS
 **********************


/* Calcula o subgrafo composto pelos vértices cujos nomes estão no vetor nomesVerts (nVerts é o tamanho deste vetor). 
 * Caso comAresta seja true, calcula o subgrafo induzido pelos vértices em nomesVers.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - nomeSubgrafo = nome do novo subgrafo (tipo char* (string));
 *    - nomesverts = vetor com os nomes do vértices (tipo char*[] (vetor de strings));
 *    - nVert = tamanho do vetor nomesverts (tipo int);
 *    - comArestas = verificação de existência ou não de arestas no novo subgrafo (tipo bool);
 * Retorno:
 *    - nenhum;
 */
void createSubgraphDG(Graph g, char *nomeSubgrafo, char *nomesVerts[], int nVert, bool comArestas);


/* Adiciona a aresta e ao subgrafo nomeado nomeSubgrafo.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - nomeSubgrafo = nome do subgrafo (tipo char* (string));
 *    - e = aresta do grafo g (tipo Edge);
 * Retorno:
 *    - aresta recém adicionada (tipo Edge);
 */
Edge includeEdgeSDG(Graph g, char *nomeSubgrafo, Edge e);


/* Verifica o pertencimento da aresta 'e' ao subgrafo nomeado nomeSubgrafo.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - nomeSubgrafo = nome do subgrafo (tipo char* (string));
 *    - e = aresta do grafo g (tipo Edge);
 * Retorno:
 *    - true, caso sim, e false, caso não (tipo bool);
 */
bool existsEdgeSDG(Graph g, char *nomeSubgrafo, Edge e);


/* Retira a aresta 'e' do subgrafo nomeSubgrafo. Ou seja, desfaz a correspondente 
 * operação includeEdgeSg previamente executada. A aresta 'e' não é removida do grafo g.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - nomeSubgrafo = nome do subgrafo (tipo char* (string));
 *    - e = aresta do grafo g (tipo Edge);
 * Retorno:
 *    - nenhum;
 */
void excludeEdgeSDG(Graph g, char *nomeSubgrafo, Edge e);


/* Adiciona a lista arestaAdjacentes as arestas (x,y), tal que: x == node; 
 * x pertence ao subgrafo nomeSubgrafo, (x,y) tambem é aresta do subgrafo.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - nomeSubgrafo = nome do subgrafo (tipo char* (string));
 *    - node = vértice do grafo g (tipo Node);
 *    - arestasAdjacentes = lista simples (tipo Lista);
 * Retorno:
 *    - nenhum;
 */
void adjacentEdgesSDG(Graph g, char *nomeSubgrafo, Node node, Lista arestasAdjacentes);

  
/* Adiciona a lista lstNodes os vértices do subgrafo nomeSubgrafo.
 * Parâmetros: 
 *    - g = grafo (tipo Graph);
 *    - nomeSubgrafo = nome do subgrafo (tipo char* (string));
 *    - lstNodes = lista simples (tipo Lista);
 * Retorno:
 *    - retorno;
 */
void getAllNodesSDG(Graph g, char *nomeSubgrafo, Lista lstNodes);


/* Adiciona a lista lstEdges as arestas do subgrafo nomeSubgrafo.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - nomeSubgrafo = nome do subgrafo (tipo char* (string));
 *    - lstEdges = lista simples (tipo Lista);
 * Retorno:
 *    - nenhum;
 */
void getAllEdgesSDG(Graph g, char *nomeSubgrafo, Lista lstEdges);


/* Cria um novo grafo a partir dos dados do subgrafo nomeado nomeSubgrafo.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - nomeSubgrafo = nome do subgrafo (tipo char* (string));
 * Retorno:
 *    - novo grafo (tipo Graph)
 */
Graph produceGraph(Graph g, char *nomeSubgrafo);


/* Ativa a aresta e do grafo g. 
 * Parâmetros:
 *    - g = grafo (tipo Graph)
 *    - e = aresta do grafo g (tipo Edge) 
 * Retorno:
 *    - nenhum; 
 */
void ativarAresta(Graph g, Edge e);


/* Desativa a aresta e do grafo g. 
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - e = aresta do grafo g (tipo Edge)
 * Retorno:
 *    - nenhum; 
 */
void desativarAresta(Graph g, Edge e);

/* Verifica se a aresta e está ativa.
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - e = aresta (tipo Edge):
 * Retorno:
 *    - true se a aresta estiver ativa, false caso contrário;
 */
bool isArestaAtiva(Graph g, Edge e);

/* Retorna a arvore do grafo. 
 * Parametros:
 *    - g = grafo (tipo Graph);
 * Retorno:
 *    - árvore do grafo (tipo SmuTreap);
 */
SmuTreap getArvoreGrafo(Graph g);

#endif 