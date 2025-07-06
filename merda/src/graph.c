// Importação das bibliotecas.
#include "graph.h"
#include "espalhamento.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////////////

/* Grafo é uma estrutura de dados que contém vértices e arestas. Ele conta com 
 * subgrafos também. Assim, o seguinte grafo foi dividido dem 4 estruturas, oriundas
 * da interpretação do módulo. */

/* Relembrando (do graph.h):
 * typedef void *Graph;
 * typedef int Node;
 * typedef void *Edge;
 * typedef void *Info; */

/* OBS (Tirar depois): A struct com os dados do vértice e da aresta vão estar em outro .c,
 * e daí usamos o info daqui para passar como parâmetro. */

/* O vértice do grafo possui um nome, dados importantes (que podem ser manipulados depois), 
 * um id (que na verdade é a origem) e um apontamento para o 1° elemento da sua lista de
 * arestas adjacentes. */
struct Vertice{

    char *nome;
    Info dados;
    Node id;
    // OBS: Cada vértice aponta para o início da sua lista de arestas adjacentes
    struct Aresta *adjacentes; // Lista de arestas que saem deste vértice

};

/* A aresta do grafo possui dados importantes (que podem ser manipulados depois), um vértice 
 * destino e um apontamento para a próxima aresta. */
struct Aresta{

    Info dados;
    Node destino;
    struct Aresta *proxima; // Próxima aresta na lista de adjacência

};

/* O subgrafo do grafo possui um nome, um registro do n° de arestas e uma 
 * lista de arestas que dele pertencem. */
struct Subgrafo{

    char *nome;
    int qtdArestas;
    TabelaGenerica arestas;

};

/* O grafo possui ou não um direcionamento, um n° máximo de vértices suportado
 * pelo grafo, um registro do n° de vértices, arestas e subgrafos, uma tabela 
 * contendo o id de seus vértices por meio de seus respectivos nomes, e duas listas, 
 * contendo seus vértices e subgrafos. */
struct Grafo{

    int qtdVertices, qtdArestas, qtdSubgrafos, limiteVertices;
    bool direcionado;
    struct Vertice **vertices; // Lista dos seus vértices.
    TabelaGenerica subgrafos; // Tabela de espalhamento com os subgrafos do grafo.
    TabelaGenerica idVertices; // Tabela de espalhamento com os ids dos vértices do grafo associados aos seus nomes.

};

// Nomes alternativos para os tipos de dados existentes.
typedef struct Vertice Vertice;
typedef struct Aresta Aresta;
typedef struct Grafo Grafo;
typedef struct Subgrafo Subgrafo;

////////////////////////////////////////////////////////////////////////////////////////

Graph createGraph(int nVert, bool directed){

    if(nVert <= 0){
        printf("Erro: O numero de vertices maximos do grafo nao deve ser negativo ou igual a zero!\n");
        return NULL;
    }

    // Aloca um novo grafo.
    Grafo *novoGrafo = (Grafo*)malloc(sizeof(Grafo));

    // Verifica a alocação.
    if(novoGrafo == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao do grafo!\n");
        return NULL;
    }

    // Alguns dos campos do grafo novo são preenchidos.
    novoGrafo->direcionado = directed;
    novoGrafo->limiteVertices = nVert;

    // Aloca a lista de vértices do grafo.
    novoGrafo->vertices = (Vertice**)calloc(novoGrafo->limiteVertices, sizeof(Vertice*));
    
    // Verifica a alocação.
    if(novoGrafo->vertices == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao da lista de vertices do grafo!\n");
        free(novoGrafo);
        return NULL;
    }

    // Cria (aloca) uma tabela de espalhamento para os nomes dos vértices do novo grafo.
    novoGrafo->idVertices = criarTabela(novoGrafo->limiteVertices);

    // Cria (aloca) uma tabela de espalhamento para os subgrafos do novo grafo.
    novoGrafo->subgrafos = criarTabela(211);

    // Verifica a alocação.
    if(novoGrafo->idVertices == NULL){
        printf("Erro: Falha na criacao da tabela de espalhamento para os nomes dos vertices do grafo!\n");
        free(novoGrafo->vertices);
        free(novoGrafo);
        return NULL;
    }

    // Os campos restantes do grafo novo são preenchidos.
    novoGrafo->qtdVertices = 0;
    novoGrafo->qtdArestas = 0;
    novoGrafo->qtdSubgrafos = 0;

    // Retorna o grafo novo.
    return novoGrafo;

}

//////////////////////////////////////////////////////////////////////////////////////// 

int getMaxNodes(Graph g){

    // Acesso para os campos da struct Grafo.
    Grafo *grafo = (Grafo*)g;

    // Retorna o número máximo de vértices permitidos no grafo. 
    return grafo->limiteVertices;

}

////////////////////////////////////////////////////////////////////////////////////////

int getTotalNodes(Graph g){

    // Acesso para os campos da struct Grafo.
    Grafo *grafo = (Grafo*)g;

    // Retorna a quantidade de vértices do grafo. 
    return grafo->qtdVertices;

}

////////////////////////////////////////////////////////////////////////////////////////

Node addNode(Graph g, char *nome, Info info){

    // Acesso para os campos da struct Grafo.  
    Grafo *grafo = (Grafo*)g;

    // Um novo vértice não pode ser adicionado se o limite tolerável de vértices do grafo for atingido. 
    if(grafo->qtdVertices >= grafo->limiteVertices){
        printf("Erro: Numero limite de vertices atingido no grafo!\n");
        return -1;
    }

    // Verifica se o vértice a ser adicionado já está no grafo. 
    if(buscarElementoTabela(grafo->idVertices, nome) != NULL){
        printf("Erro: O vertice novo nao foi inserido no grafo pois ja esta presente nele!\n");
        return -1;
    }

    // Aloca um novo vértice. 
    Vertice *novoVertice = (Vertice*)malloc(sizeof(Vertice));

    // Verifica a alocação.  
    if(novoVertice == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao do vertice!\n");
        return -1;
    }

    // Os demais campos do vértice são preenchidos. 
    novoVertice->nome = (char*)malloc(strlen(nome) + 1);

    if(novoVertice->nome == NULL){
        printf("Erro: Falha na alocacao de memoria para o nome no vertice!\n");
        free(novoVertice);
        return -1;
    }

    strcpy(novoVertice->nome, nome);
    novoVertice->id = grafo->qtdVertices;
    novoVertice->dados = info;
    novoVertice->adjacentes = NULL; // isso mesmo?

    // O demais campos do grafo são preenchidos.
    grafo->vertices[grafo->qtdVertices] = novoVertice;
    grafo->qtdVertices++;

    // Insere o id do vértice na tabela de espalhamento do grafo.
    inserirElementoTabela(grafo->idVertices, nome, (DadoGenerico)(long)novoVertice->id);

    // Retorna o id do vértice novo.
    return novoVertice->id;

}

////////////////////////////////////////////////////////////////////////////////////////

Node getNode(Graph g, char *nome){

    // Acesso para os campos da struct Grafo. 
    Grafo *grafo = (Grafo*)g;
    
    // A função chamada retorna o id do vértice por meio de seu nome.
    DadoGenerico busca = buscarElementoTabela(grafo->idVertices, nome);

    // Verificação para saber se o vértice foi ou não encontrado na tabela, ou seja, no gráfico. 
    if(busca == NULL){
        printf("Aviso: O vertice nao foi encontrado no grafo!\n");
        return -1;
    }

    Node id = (Node)(long)busca;

    // Retorna o id desse vértice.
    return id;

}

////////////////////////////////////////////////////////////////////////////////////////

Info getNodeInfo(Graph g, Node node){

    // Acesso para os campos da struct Grafo.
    Grafo *grafo = (Grafo*)g;

    /* O id do vértice do grafo não pode ser menor que zero ou maior do que a quantidade
     * atual de vértices do grafo. */
    if(node >= 0 && node < grafo->qtdVertices){
        /* O grafo tem uma lista de vértices, ou melhor, um vetor. 
         * Acessamos o vértice desse vetor pelo seu índice, node, nesse caso. */
        return grafo->vertices[node]->dados;
    }

    printf("Aviso: A informacao do vertice do grafo nao foi encontrada!\n");

    return NULL;

}

////////////////////////////////////////////////////////////////////////////////////////

char *getNodeName(Graph g, Node node){

    // Acesso para os campos da struct Grafo. 
    Grafo *grafo = (Grafo*)g;

    /* O id do vértice do grafo não pode ser menor que zero ou maior do que a quantidade
     * atual de vértices do grafo. */
    if(node >= 0 && node < grafo->qtdVertices){
        /* O grafo tem uma lista de vértices, ou melhor, um vetor. 
         * Acessamos o vértice desse vetor pelo seu índice, node, nesse caso. */
        return grafo->vertices[node]->nome;
    }

    printf("O nome do vertice do grafo nao foi encontrado!\n");

    return NULL;

}

////////////////////////////////////////////////////////////////////////////////////////

void setNodeInfo(Graph g, Node node, Info info){

    // Acesso para os campos da struct Grafo. 
    Grafo *grafo = (Grafo*)g;

    /* O id do vértice do grafo não pode ser menor que zero ou maior do que a quantidade
     * atual de vértices do grafo. */
    if(node >= 0 && node < grafo->qtdVertices){
        /* O grafo tem uma lista de vértices, ou melhor, um vetor. 
         * Acessamos o vértice desse vetor pelo seu índice, node, nesse caso. */
        grafo->vertices[node]->dados = info;
    }

    printf("Aviso: O vertice do grafo nao foi modificado!\n");

}

////////////////////////////////////////////////////////////////////////////////////////

// Função auxiliar
static Edge duplicarAresta(Graph g, Node from, Node to, Info info){

    // Acesso para os campos da struct Grafo. 
    Grafo *grafo = (Grafo*)g;

    /* Os ids dos vértices do grafo não podem ser menores que zero ou maiores do que a quantidade
     * atual de vértices do grafo. */
    if(from < 0 || from >= grafo->qtdVertices || to < 0 || to >= grafo->qtdVertices){
        printf("Erro: Falha na duplicacao da aresta no grafo!\n");
        return NULL;
    }

    // Aloca uma nova aresta. 
    Aresta *arestaDuplicada = (Aresta*)malloc(sizeof(Aresta));

    // Verifica a alocação.  
    if(arestaDuplicada == NULL){
        printf("Erro: Falha na alocacao de memoria para a duplicacao da aresta do grafo!\n");
        return NULL;
    }

    arestaDuplicada->destino = to;
    arestaDuplicada->dados = info;

    arestaDuplicada->proxima = grafo->vertices[from]->adjacentes;

    grafo->vertices[from]->adjacentes = arestaDuplicada;

    return arestaDuplicada;

}

Edge addEdge(Graph g, Node from, Node to, Info info){

    // Acesso para os campos da struct Grafo. 
    Grafo *grafo = (Grafo*)g;

    /* Os ids dos vértices do grafo não podem ser menores que zero ou maiores do que a quantidade
     * atual de vértices do grafo. */
    if(from < 0 || from >= grafo->qtdVertices || to < 0 || to >= grafo->qtdVertices){
        printf("Erro: Falha na insercao da aresta no grafo!\n");
        return NULL;
    }

    // Aloca uma nova aresta. 
    Aresta *novaAresta = (Aresta*)malloc(sizeof(Aresta));

    // Verifica a alocação.  
    if(novaAresta == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao da aresta do grafo!\n");
        return NULL;
    }

    novaAresta->destino = to;
    novaAresta->dados = info;

    Aresta *auxiliar =  grafo->vertices[from]->adjacentes;

    while(auxiliar != NULL){
        if(auxiliar->destino == to){
            printf("Aviso: Aresta nao inserida pois ja existe no grafo!\n");
            free(novaAresta);
            return NULL;
        }
        auxiliar = auxiliar->proxima;
    }

    novaAresta->proxima = grafo->vertices[from]->adjacentes;

    grafo->vertices[from]->adjacentes = novaAresta;
    grafo->qtdArestas++;

    if(grafo->direcionado == false){
        duplicarAresta(g, to, from, info);
    }

    // Retorna a aresta nova.
    return novaAresta;

}

////////////////////////////////////////////////////////////////////////////////////////

Edge getEdge(Graph g, Node from, Node to){

    // Acesso para os campos da struct Grafo.
    Grafo *grafo = (Grafo*)g;

    /* Os ids dos vértices do grafo não podem ser menores que zero ou maiores do que a quantidade
     * atual de vértices do grafo. */
    if(from < 0 || from >= grafo->qtdVertices || to < 0 || to >= grafo->qtdVertices){
        printf("Erro: Falha na busca da aresta do grafo!\n");
        return NULL;
    }

    Aresta *auxiliar = grafo->vertices[from]->adjacentes;

    while(auxiliar != NULL){
        if(auxiliar->destino == to){
            return auxiliar;
        }
        auxiliar = auxiliar->proxima;
    }

    printf("Aviso: A aresta do grafo nao foi encontrada!\n");

    return NULL;

}

////////////////////////////////////////////////////////////////////////////////////////

Node getFromNode(Graph g, Edge e){

    // Acesso para os campos da struct Grafo.
    Grafo *grafo = (Grafo*)g;

    Aresta *auxiliar;
    int i;

    // for que percorre a lista de vértices do grafo.
    for(i = 0; i < grafo->qtdVertices; i++){
        auxiliar = grafo->vertices[i]->adjacentes;

        while(auxiliar != NULL){
            if(auxiliar == e){
                return grafo->vertices[i]->id;
            }
            auxiliar = auxiliar->proxima;
        }

    }
    
    printf("Aviso: O vertice de origem da aresta do grafo nao foi encontrado!\n");

    return -1;

}

////////////////////////////////////////////////////////////////////////////////////////

Node getToNode(Graph g, Edge e){

    // Acesso para os campos da struct Aresta. 
    Aresta *aresta = (Aresta*)e;

    // Retorna o id do vértice de destino da aresta do grafo.
    return aresta->destino;

}

////////////////////////////////////////////////////////////////////////////////////////

Info getEdgeInfo(Graph g, Edge e){

    // Acesso para os campos da struct Aresta. 
    Aresta *aresta = (Aresta*)e;

    // Retorna a informação/dado relacionada a aresta do grafo. 
    return aresta->dados;

}

////////////////////////////////////////////////////////////////////////////////////////

void setEdgeInfo(Graph g, Edge e, Info info){
    
    // Acesso para os campos da struct Aresta. 
    Aresta *aresta = (Aresta*)e;

    // A aresta recebe um novo dado/informação.  
    aresta->dados = info;

}

////////////////////////////////////////////////////////////////////////////////////////

// Função auxiliar.
static void removerArestaDuplicada(Graph g, Edge e){

    // Acesso para os campos da struct Grafo. 
    Grafo *grafo = (Grafo*)g;

    Node from = getFromNode(g, e);
    Node to = getToNode(g, e);
    Aresta *arestaDuplicada = getEdge(g, to, from);

    if(arestaDuplicada != NULL){
        removeEdge(g, arestaDuplicada);
    } 

}

void removeEdge(Graph g, Edge e){

    // Acesso para os campos da struct Grafo. 
    Grafo *grafo = (Grafo*)g;
    
    Aresta *auxiliar, *anterior;
    int i;

    if(grafo->direcionado == false){
        removerArestaDuplicada(g, e);
        grafo->qtdArestas++;
    }

    // for que percorre a lista de vértices do grafo.
    for(i = 0; i < grafo->qtdVertices; i++){
        auxiliar = grafo->vertices[i]->adjacentes;
        anterior = NULL;

        while(auxiliar != NULL){
            if(auxiliar == e){
                if(anterior == NULL){
                    grafo->vertices[i]->adjacentes = auxiliar->proxima;
                }else{
                    anterior->proxima = auxiliar->proxima;
                }
                free(auxiliar);
                grafo->qtdArestas--;
                return;
            }
            anterior = auxiliar;
            auxiliar = auxiliar->proxima;
        }
    }

}

////////////////////////////////////////////////////////////////////////////////////////

bool isAdjacent(Graph g, Node from, Node to){

    // Acesso para os campos da struct Grafo. 
    Grafo *grafo = (Grafo*)g;

    /* Os ids dos vértices do grafo não podem ser menores que zero ou maiores do que a quantidade
     * atual de vértices do grafo. */
    if(from < 0 || from >= grafo->qtdVertices || to < 0 || to >= grafo->qtdVertices){
        printf("Erro: Falha na verificacao de adjacencia da aresta!\n");
        return false;
    }
    
    Aresta *auxiliar = grafo->vertices[from]->adjacentes;

    while(auxiliar != NULL){
        if(auxiliar->destino == to){
            return true;
        }
        auxiliar = auxiliar->proxima;
    }

    return false;
  
}

////////////////////////////////////////////////////////////////////////////////////////

void adjacentNodes(Graph g, Node node, Lista nosAdjacentes){

    // Acesso para os campos da struct Grafo.
    Grafo *grafo = (Grafo*)g;

    if(node < 0 || node >= grafo->qtdVertices){
        printf("Erro: Falha ao colocar vertices adjacentes na lista!\n");
        return;
    }

    int i;
    Aresta *auxiliar = grafo->vertices[node]->adjacentes;

    while(auxiliar != NULL){
        inserirInicioLista(nosAdjacentes, auxiliar->destino);
        auxiliar = auxiliar->proxima;
    }

}

////////////////////////////////////////////////////////////////////////////////////////

void adjacentEdges(Graph g, Node node, Lista arestasAdjacentes){

    // Acesso para os campos do grafo. 
    Grafo *grafo = (Grafo*)g;

    if(node < 0 || node >= grafo->qtdVertices){
        printf("Erro: Falha ao colocar arestas adjacentes na lista!\n");
        return;
    }

    int i;
    Aresta *auxiliar = grafo->vertices[node]->adjacentes;

    while(auxiliar != NULL){
        inserirInicioLista(arestasAdjacentes, auxiliar);
        auxiliar = auxiliar->proxima;
    }

}

////////////////////////////////////////////////////////////////////////////////////////

void getNodeNames(Graph g, Lista nomesNodes){

    /* Acesso para os campos da struct Grafo. */
    Grafo *grafo = (Grafo*)g;

    int i;
    char *nome;

    /* for que percorre a lista de vértices do grafo. */
    for(i = 0; i < grafo->qtdVertices; i++){
        /* Variável nome recebe o nome do vértice do grafo. */
        nome = grafo->vertices[i]->nome;
        /* E a mesma é inserida na lista. */
        inserirInicioLista(nomesNodes, nome); 
    }

}

////////////////////////////////////////////////////////////////////////////////////////

void getEdges(Graph g, Lista arestas){

    /* Acesso para os campos da struct Grafo. */
    Grafo *grafo = (Grafo*)g;

    int i;
    Aresta *auxiliar;

    /* for que percorre a lista de vértices do grafo. */
    for(i = 0; i < grafo->qtdVertices; i++){
        auxiliar = grafo->vertices[i]->adjacentes;
        while(auxiliar != NULL){
            inserirInicioLista(arestas, auxiliar);
            auxiliar = auxiliar->proxima;
        }
    }

}

////////////////////////////////////////////////////////////////////////////////////////

// dfs

////////////////////////////////////////////////////////////////////////////////////////

// bfs

////////////////////////////////////////////////////////////////////////////////////////

void killDG(Graph g){

    /* Acesso para os campos do grafo. */
    Grafo *grafo = (Grafo*)g;

    Aresta *auxiliar, *auxiliar2;
    int i;

    /* for que percorre a lista de vértices do grafo. */
    for(i = 0; i < grafo->qtdVertices; i++){
        /* Desaloca o vértice do grafo. */
        auxiliar = grafo->vertices[i]->adjacentes;
        while(auxiliar != NULL){
            auxiliar2 = auxiliar->proxima;
            free(auxiliar);
            auxiliar = auxiliar2;
        }
        /* Desaloca o vértice do grafo. */
        free(grafo->vertices[i]->nome);
        free(grafo->vertices[i]);
    }
    
    /* A lista de vértices e subgrafos do grafo é desalocada. */
    free(grafo->vertices);
    free(grafo->subgrafos);

    /* A tabela com o id dos vértices do grafo é desalocada. */
    desalocarTabela(grafo->idVertices);
    desalocarTabela(grafo->subgrafos);

    /* Por fim, desaloca o grafo ao todo. */
    free(grafo);

}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

// Função auxiliar.
static bool preencherVetor(Graph g, Node vetor[], int nVert, char *nomesVerts[]){

    int i;
    Node id;

    for(i = 0; i < nVert; i++){
        id = getNode(g, nomesVerts[i]);

        if(id == -1){
            printf("Erro: O vetor auxiliar nao foi preenchido pois o vertice nao foi encontrado no grafo!\n");
            return false;
        }

        vetor[i] = id;
    }

    return true;

}

// Função auxiliar.
static bool estaNoVetor(Node id, Node vetor[], int nVert){

    int i;

    for(i = 0; i < nVert; i++){
        if(id == vetor[i]){
            return true;
        }
    }

    return false;
    
}

void createSubgraphDG(Graph g, char *nomeSubgrafo, char *nomesVerts[], int nVert, bool comArestas){

    /* Acesso para os campos da struct Grafo. */
    Grafo *grafo = (Grafo*)g;

    if(nVert <= 0){
        printf("Erro: O numero de vertices maximos do subgrafo nao deve ser negativo ou igual a zero!\n");
        return;
    }

    /* Aloca o subgrafo novo. */
    Subgrafo *novoSubgrafo = (Subgrafo*)malloc(sizeof(Subgrafo));

    /* Verifica a alocação. */
    if(novoSubgrafo == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao do subgrafo do grafo!\n");
        return;
    }

    /* Preenche os demais campos do subgrafo. */

    novoSubgrafo->nome = (char*)malloc(strlen(nomeSubgrafo) + 1);

    if(novoSubgrafo->nome == NULL){
        printf("Erro: Falha na alocacao de memoria para a atribuicao do nome do subgrafo!\n");
        free(novoSubgrafo);
        return;
    }

    strcpy(novoSubgrafo->nome, nomeSubgrafo);
    novoSubgrafo->qtdArestas = 0;
    novoSubgrafo->arestas = criarTabela(211);

    int i, j;
    Node vetorIds[nVert];
    Aresta *auxiliar;  
    char chave[50];

    if(preencherVetor(g, vetorIds, nVert, nomesVerts) == false){
        free(novoSubgrafo->nome);
        desalocarTabela(novoSubgrafo->arestas);
        free(novoSubgrafo);
        return;
    }

    /* Verifica se o subgrafo conterá arestas. */
    if(comArestas == true){
        /* Se sim, for percorre a lista de vértices do grafo. */
        for(i = 0; i < grafo->qtdVertices; i++){
            if(estaNoVetor(grafo->vertices[i]->id, vetorIds, nVert) == true){
                auxiliar = grafo->vertices[i]->adjacentes;
                while(auxiliar != NULL){
                    for(j = 0; j< nVert; j++){
                        if(auxiliar->destino == vetorIds[j]){
                            snprintf(chave, "%d-%d", grafo->vertices[i]->id, auxiliar->destino);
                            if(buscarElementoTabela(novoSubgrafo->arestas, chave) == NULL){
                                inserirElementoTabela(novoSubgrafo->arestas, chave, auxiliar);
                                novoSubgrafo->qtdArestas++;
                            }
                            break;
                        }
                    }
                    auxiliar = auxiliar->proxima;
                }
            }
        }
    }

    inserirElementoTabela(grafo->subgrafos, nomeSubgrafo, novoSubgrafo);
    grafo->qtdSubgrafos++;
    
}

////////////////////////////////////////////////////////////////////////////////////////

Edge includeEdgeSDG(Graph g, char *nomeSubgrafo, Edge e){

    /* Acesso aos campos da struct Grafo. */
    Grafo *grafo = (Grafo*)g;

    Subgrafo *subgrafo = buscarElementoTabela(grafo->subgrafos, nomeSubgrafo);

    if(subgrafo == NULL){
        printf("Aviso: O subgrafo nao foi encontrado no grafo!\n");
        return NULL;
    }

    char chave[50];
    Node origem = getFromNode(g, e), destino = getToNode(g, e);

    snprintf(chave, "%d-%d", origem, destino);

    if(buscarElementoTabela(subgrafo->arestas, chave) == NULL){
        inserirElementoTabela(subgrafo->arestas, chave, e);
        subgrafo->qtdArestas++;
    }else{
        printf("Aviso: A aresta nao foi inserida pois ja existe no subgrafo!\n");
    }

    return e;

}

////////////////////////////////////////////////////////////////////////////////////////

bool existsEdgeSDG(Graph g, char *nomeSubgrafo, Edge e){

    /* Acesso aos campos da struct Grafo. */
    Grafo *grafo = (Grafo*)g;

    Subgrafo *subgrafo = buscarElementoTabela(grafo->subgrafos, nomeSubgrafo);

    if(subgrafo != NULL){
        Node origem = getFromNode(g, e), destino = getToNode(g, e);
        char chave[50];

        snprintf(chave, "%d-%d", origem, destino);

        if(buscarElementoTabela(subgrafo->arestas, chave) != NULL){
            return true;
        }
    }

    return false;

}

////////////////////////////////////////////////////////////////////////////////////////

void excludeEdgeSDG(Graph g, char *nomeSubgrafo, Edge e){

    Grafo *grafo = (Grafo*)g;

    Subgrafo *subgrafo = buscarElementoTabela(grafo->subgrafos, nomeSubgrafo);

    if(subgrafo != NULL){
        Node origem = getFromNode(g, e), destino = getToNode(g, e);
        char chave[50];

        snprintf(chave, "%d-%d", origem, destino);

        if(buscarElementoTabela(subgrafo->arestas, chave) != NULL){
            removerElementoTabela(subgrafo->arestas, chave);
            subgrafo->qtdArestas--;
        }
    }

}

////////////////////////////////////////////////////////////////////////////////////////

void adjacentEdgesSDG(Graph g, char *nomeSubgrafo, Node node, Lista arestasAdjacentes){

    Grafo *grafo = (Grafo*)g;

    if(node < 0 || node >= grafo->qtdVertices){
        printf("Erro: Falha ao colocar arestas adjacentes do subgrafo na lista!\n");
        return;
    }

    Subgrafo *subgrafo = buscarElementoTabela(grafo->subgrafos, nomeSubgrafo);

    if(subgrafo != NULL){
        int i;
        Aresta *auxiliar = buscarElementoTabela(subgrafo->arestas, /*?*/);
    }


}

////////////////////////////////////////////////////////////////////////////////////////

void getAllNodesSDG(Graph g, char *nomeSubgrafo, Lista lstNodes){

    Grafo *grafo = (Grafo*)g;

    int i, j;

    for(i = 0; i < grafo->qtdSubgrafos; i++){
        if(strcmp(grafo->subgrafos[i]->nome, nomeSubgrafo) == 0){
            for(j = 0; j < grafo->subgrafos[i]->qtdArestas; j++){  
                /* Pode repetidos? */
                inserirInicioLista(lstNodes, grafo->subgrafos[i]->arestas[j]->origem);
                inserirInicioLista(lstNodes, grafo->subgrafos[i]->arestas[j]->destino);  
            }
        }
    }

}

////////////////////////////////////////////////////////////////////////////////////////

void getAllEdgesSDG(Graph g, char *nomeSubgrafo, Lista lstEdges){

    Grafo *grafo = (Grafo*)g;

    int i, j;

    for(i = 0; i < grafo->qtdSubgrafos; i++){
        if(strcmp(grafo->subgrafos[i]->nome, nomeSubgrafo) == 0){
            for(j = 0; j < grafo->subgrafos[i]->qtdArestas; j++){
                inserirInicioLista(lstEdges, grafo->subgrafos[i]->arestas[j]);
            }
        }
    }

}

////////////////////////////////////////////////////////////////////////////////////////

/* Função auxiliar */
static Vertice *buscarVerticePorId(Graph g, Node n){

    Grafo *grafo = (Grafo*)g;

    int i;

    for(i = 0; i < grafo->qtdVertices; i++){
        if(grafo->vertices[i]->id == n){
            return grafo->vertices[i];
        }
    }

    return NULL;

}

Graph produceGraph(Graph g, char *nomeSubgrafo){

    Grafo *grafo = (Grafo*)g;

    int i, j, k;

    for(i = 0; i < grafo->qtdSubgrafos; i++){
        if(strcmp(grafo->subgrafos[i]->nome, nomeSubgrafo) == 0){

            Grafo *novoGrafo = (Grafo*)malloc(sizeof(Grafo));

            if(novoGrafo == NULL){
                printf("Erro: Falha na alocacao de memoria para a criacao do grafo a partir do subgrafo!\n");
                return NULL;
            }
            
            novoGrafo->qtdArestas = grafo->subgrafos[i]->qtdArestas;
            novoGrafo->arestas = (Aresta**)malloc(novoGrafo->qtdArestas * sizeof(Aresta*));

            if(novoGrafo->arestas == NULL){
                printf("Erro: Falha na alocacao de memoria para a criacao da lista de arestas do novo grafo a partir do subgrafo!\n");
                return NULL;
            }

            novoGrafo->qtdVertices = 0;
            novoGrafo->vertices = NULL;

            for(j = 0; j < novoGrafo->qtdArestas; j++){
                novoGrafo->arestas[j] = grafo->subgrafos[i]->arestas[j];

                for(k = 0; k < novoGrafo->qtdVertices; k++){
                    if(novoGrafo->arestas[k]->origem != grafo->subgrafos[i]->arestas[j]->origem){

                        novoGrafo->vertices = (Vertice**)realloc(novoGrafo->vertices, (novoGrafo->qtdVertices + 1) * sizeof(Vertice*));

                        if(novoGrafo->vertices == NULL){
                            printf("Erro: Falha na alocacao de memoria para a criacao da lista de vertices do novo grafo a partir do subgrafo!\n");
                            return NULL;
                        }

                        novoGrafo->vertices[novoGrafo->qtdVertices] = buscarVerticePorId(g, grafo->subgrafos[i]->arestas[j]->origem);
                        novoGrafo->qtdVertices++;

                    }

                    if(novoGrafo->arestas[k]->destino != grafo->subgrafos[i]->arestas[j]->destino){

                        novoGrafo->vertices = (Vertice**)realloc(novoGrafo->vertices, (novoGrafo->qtdVertices + 1) * sizeof(Vertice*));

                        if(novoGrafo->vertices == NULL){
                            printf("Erro: Falha na alocacao de memoria para a criacao da lista de vertices do novo grafo a partir do subgrafo!\n");
                            return NULL;
                        }

                        novoGrafo->vertices[novoGrafo->qtdVertices] = buscarVerticePorId(g, grafo->subgrafos[i]->arestas[j]->destino);
                        novoGrafo->qtdVertices++;
                    }
                }

            }

            novoGrafo->subgrafos = NULL;
            novoGrafo->qtdSubgrafos = 0;
            novoGrafo->limiteVertices = grafo->limiteVertices;
            novoGrafo->direcionado = grafo->direcionado;
            
            return novoGrafo;
        }
        
    }

    return NULL;
    
}
