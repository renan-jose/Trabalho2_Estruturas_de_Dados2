// Testando integração vscode com github

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
#include <stdbool.h>

// Nomes alternativos para os tipos de dados existentes.
typedef void *Graph;
typedef int Node;
typedef void *Edge;
typedef void *Info;

/* Invocado quando uma aresta é "descoberta"/"percorrida"/"classificada". 
 * Também informa os tempos de descoberta e finalização. */
bool (*procEdge)(Graph g, Edge e,int td, int tf, void *extra); 


/* Invocado quando percurso é recomeçado. */
bool (*dfsRestarted)(Graph g, void *extra);


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


/* Retorna número total de vértices adicionados ao grafo gr. 
 * Parâmetos: 
 *    - g = grafo (tipo Graph);
 * Retorno:
 *    - numero (tipo int); */
int getTotalNodes(Graph g);


/* Adiciona um novo vértice ao grafo "g" com o nome "nome". 
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - *nome = nome do vértice do grafo g (tipo char (string));
 *    - info = informação associada ao vértice do grafo g (tipo Info). 
 * Retorno:
 *    - número (tipo Node (int)); */
Node addNode(Graph g, char *nome, Info info);


/* Retorna nó cujo de nome é "nome". 
 * Parâmetros: 
 *    - g = grafo (tipo Graph);
 *    - *nome = nome do vértice do grafo g (tipo char (string)); 
 * Retorno:
 *    - número (tipo Node (int)); */
Node getNode(Graph g, char *nome);


/* Retorna a informação associada ao nó node
 * Parâmetros:
 *    - g = grafo (tipo Graph);
 *    - node = número do nó (vértice) do grafo (tipo Node);
 * Retorno:
 *    - informação do vértice (tipo Info); */
Info getNodeInfo(Graph g, Node node); 


/*
 */
char *getNodeName(Graph g, Node node);


/*
 */
void setNodeInfo(Graph g, Node node, Info info);


/*
 */
Edge addEdge(Graph g, Node from, Node to, Info info);


/*
 */
Edge getEdge(Graph g, Node from, Node to);


/*
 */
Node getFromNode(Graph g, Edge e);

  
/*
 */  
Node getToNode(Graph g, Edge e);


/*
 */
Info getEdgeInfo(Graph g, Edge e);


/*
 */
void setEdgeInfo(Graph g, Edge e, Info info);


/*
 */
void removeEdge(Graph g, Edge e);


/*
 */
bool isAdjacent(Graph g, Node from, Node to);


/* 
   Adiciona 'a lista "nosAdjacentes" os nos adjacentes 'a "node".
 */
void adjacentNodes(Graph g, Node node, Lista nosAdjacentes);


/*
   Adiciona 'a lista "arestaAdjacentes" as arestas (x,y), tal que,
   x == node.
 */
void adjacentEdges(Graph g, Node node, Lista arestasAdjacentes);


/*
   Insere na lista "nomesNodes" os nomes atribuidos aos nos do grafo.
 */
void getNodeNames(Graph g, Lista nomesNodes);


/*
   Insere na lista "arestas", as arestas de g.
 */
void getEdges(Graph g, Lista arestas);


/*
   Faz percurso em profundidade sobre g, a partir do no' node, classificando 
   as arestas do grafo, invocando a respectiva funcao.
      A busca em profundidade, eventualmente, pode produzir uma floresta.
   newTree e' invocada sempre que o percurso for retomado.
 */  
bool dfs(Graph g, Node node, procEdge treeEdge, forwardEdge, returnEdge,
	 crossEdge, newTree, void *extra);


/*
   Percorre o grafo g em largura, a partir do no' node. discoverNode e' usada
   para a aresta (x,y) usada para "descobrir" o y.
 */
bool bfs(Graph g, node, discoverNode, void *extra);


/*
   Destroi o grafo "g".
 */
void killDG(Graph g);

/*
 **********************
 * SUB-GRAFOS
 **********************
/*
    Calcula o subgrafo composto  pelos vertices cujos nomes estao no vetor nomesVerts
(nVerts e' o tamanho deste vetor). Caso comAresta seja true calcula o subgrafo 
induzido pelos vertices em nomesVers
 */
void createSubgraphDG(Graph g, char *nomeSubgrafo, char *nomesVerts[], int nVert, bool comArestas);


/*
    Adiciona a aresta ao subgrafo.
 */
Edge includeEdgeSDG(Graph g, char *nomeSubgrafo, Edge e);

/*
  Retorna verdadeiro se a aresta "e" pertence ao subgrafo "nomeSubgrafo" do grafo g; 
  falso, caso contrario.
 */
bool existsEdgeSDG(Graph g, char *nomeSubgrafo, Edge e);

/*
  Retira a aresta "e" do subgrafo "nomeSubgrafo". Ou seja, desfaz a correspondente 
  operacao includeEdgeSg previamente executada. 
  Note que a aresta  "e" NAO e' removida do grafo g.
 */
void excludeEdgeSDG(Graph g, char *nomeSubgrafo, Edge e);

/*
   Adiciona 'a lista "arestaAdjacentes" as arestas (x,y), tal que:
   x == node; x pertence ao subgrafo "nomeSubgrafo", (x,y) tambem e' aresta
   do subgrafo.
 */
void adjacentEdgesSDG(Graph g, char *nomeSubgrafo, Node node, Lista arestasAdjacentes);

/*
   Adiciona 'a lista "lstNodes" (Lista<Node>) os n�s do subgrafo "nomeSubgrafo".
 */
void getAllNodesSDG(Graph g, char *nomeSubgrafo, Lista lstNodes);

/*
   Adiciona 'a lista "lstEdges" (Lista<Edge>) as arestas do subgrafo "nomeSubgrafo".
 */
void getAllEdgesSDG(Graph g, char *nomeSubgrafo, Lista lstEdges);

/*
  Novo grafo.
 */
Graph produceGraph(Graph g, char *nomeSubgrafo);

#endif 