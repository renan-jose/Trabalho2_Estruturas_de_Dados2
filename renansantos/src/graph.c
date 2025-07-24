// Importação das bibliotecas.
#include "graph.h"
#include "espalhamento.h"
#include "fila.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////////////

#define NAO_VISITADO 0
#define DESCOBERTO 1
#define FINALIZADO 2

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
    bool ativa;
    struct Aresta *proxima; // Próxima aresta na lista de adjacência

};

/* O subgrafo do grafo possui um nome, um registro do n° de arestas e uma 
 * lista de arestas que dele pertencem. */
struct Subgrafo{

    char *nome;
    int qtdArestas, qtdVertices;
    struct Vertice **vertices;
    struct Aresta **arestas;

};

/* O grafo possui ou não um direcionamento, um n° máximo de vértices suportado
 * pelo grafo, um registro do n° de vértices, arestas e subgrafos, uma tabela 
 * contendo o id de seus vértices por meio de seus respectivos nomes, e duas listas, 
 * contendo seus vértices e subgrafos. */
struct Grafo{

    char *nome;
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

Graph createGraph(int nVert, bool directed, char *nome){

    if(nVert < 0){
        printf("Erro: O numero de vertices maximos do grafo nao deve ser negativo!\n");
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

    novoGrafo->nome = (char*)malloc(strlen(nome) + 1);

    if(novoGrafo->nome == NULL){
        printf("Erro: Falha na alocacao de memoria para o nome no grafo!\n");
        free(novoGrafo);
        return NULL;
    }

    strcpy(novoGrafo->nome, nome);

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
    novaAresta->ativa = true;

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
        inserirInicioLista(nosAdjacentes, (void*)(intptr_t)auxiliar->destino);
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

// Função auxiliar.
static void dfsAuxiliar(Graph g, Node node, int *tempo, int *visitado, int *descoberto, int *finalizado, procEdge treeEdge , procEdge forwardEdge, procEdge returnEdge, procEdge crossEdge, void *extra){

    Grafo *grafo = (Grafo*)g;

    visitado[node] = DESCOBERTO;
    descoberto[node] = ++(*tempo);

    Aresta *auxiliar = grafo->vertices[node]->adjacentes;

    while(auxiliar != NULL){
        if(auxiliar->ativa == false){
            auxiliar = auxiliar->proxima;
            continue;
        }

        Node destino = auxiliar->destino;

        if(visitado[destino] == NAO_VISITADO){
            if(treeEdge != NULL && !treeEdge(g, auxiliar, *tempo, *tempo, extra)){
                return;
            }
            dfsAuxiliar(g, destino, tempo, visitado, descoberto, finalizado, treeEdge, forwardEdge, returnEdge, crossEdge, extra);
        }else if(visitado[destino] == DESCOBERTO){
            if(returnEdge != NULL && !returnEdge(g, auxiliar, *tempo, *tempo, extra)){
                return;
            }
        }else if(visitado[destino] == FINALIZADO){
            if(crossEdge != NULL && !crossEdge(g, auxiliar, *tempo, *tempo, extra)){
                return;
            }
        }

        auxiliar = auxiliar->proxima;
    }

    visitado[node] = FINALIZADO;
    finalizado[node] = ++(*tempo);
    
}

// Levando em consideração: 0 - não visitado; 1 - descoerto; 2 - finalizado.

bool dfs(Graph g, Node node, procEdge treeEdge, procEdge forwardEdge, procEdge returnEdge, procEdge crossEdge, dfsRestarted newTree, void *extra){

    Grafo *grafo = (Grafo*)g;

    int *vetorVisitado = (int*)calloc(grafo->qtdVertices, sizeof(int));
    int *vetorDescoberto = (int*)calloc(grafo->qtdVertices, sizeof(int));
    int *vetorFinalizado = (int*)calloc(grafo->qtdVertices, sizeof(int));
    int tempo = 0, i;

    if(vetorVisitado == NULL || vetorDescoberto == NULL || vetorFinalizado == NULL){
        printf("Erro: Falha nas alocacoes dos vetores para busca dfs no grafo!\n");
        return false;
    }

    for(i = 0; i < grafo->qtdVertices; i++){
        if(vetorVisitado[i] == NAO_VISITADO){
            if(newTree != NULL && !newTree(g, extra)){
                free(vetorVisitado);
                free(vetorDescoberto);
                free(vetorFinalizado);
                return false;
            }
            dfsAuxiliar(g, i, &tempo, vetorVisitado, vetorDescoberto, vetorFinalizado, treeEdge, forwardEdge, returnEdge, crossEdge, extra);
        }

    }
    
    free(vetorVisitado);
    free(vetorFinalizado);
    free(vetorDescoberto);

    return true;

}

////////////////////////////////////////////////////////////////////////////////////////

bool bfs(Graph g, Node node, procEdge discoverNode, void *extra){

    Grafo *grafo = (Grafo*)g;

    int *vetorVisitado = (int*)calloc(grafo->qtdVertices, sizeof(int));

    if(vetorVisitado == NULL){
        printf("Erro: Falha na alocacao de memoria para o vetor para a busca bfs do grafo!\n");
        return false;
    }

    FilaGenerica fila = criarFila();
    int tempo = 0;

    vetorVisitado[node] = 1;
    enfileirar(fila, (void *)(long)node);

    while(filaVazia(fila) == false){
        Node id = (Node)(long)desenfileirar(fila);
        Aresta *auxiliar = grafo->vertices[id]->adjacentes;

        while(auxiliar != NULL){
            if(auxiliar->ativa == false){
                auxiliar = auxiliar->proxima;
                continue;
            }

            Node destino = auxiliar->destino;

            if(vetorVisitado[destino] == 0){
                vetorVisitado[destino] = 1;
                if(discoverNode != NULL && !discoverNode(g, auxiliar, tempo, tempo, extra)){
                    desalocarFila(fila);
                    free(vetorVisitado);
                    return false;
                }
                enfileirar(fila, (void *)(long)destino);
            }

            auxiliar = auxiliar->proxima;
        }

        vetorVisitado[id] = 2;
    }

    desalocarFila(fila);
    free(vetorVisitado);

    return true;

}

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
    
    /* A lista de vértices do grafo é desalocada. */
    free(grafo->vertices);

    /* A tabela com o id dos vértices do grafo é desalocada. */
    desalocarTabela(grafo->idVertices);
    desalocarTabela(grafo->subgrafos);

    /* Por fim, desaloca o grafo ao todo. */
    free(grafo);

}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

// Função auxiliar.
static bool estaNoVetorIds(Node id, Node vetor[], int nVert){

    int i;

    for(i = 0; i < nVert; i++){
        if(id == vetor[i]){
            return true;
        }
    }

    return false;
    
}

// Função auxiliar.
static Vertice *buscarEstruturaVertice(Graph g, Node n){

    Grafo *grafo = (Grafo*)g;

    if(n < 0 || n >= grafo->qtdVertices){
        printf("Erro: Falha na busca do vertice do grafo pelo seu id!\n");
        return NULL;
    }

    return grafo->vertices[n];

}

// Função auxiliar.
static bool preencherVetorIds(Graph g, Node vetor[], int nVert, char *nomesVerts[]){

    int i;
    Node id;

    for(i = 0; i < nVert; i++){
        id = getNode(g, nomesVerts[i]);

        if(id == -1){
            printf("Erro: O vetor auxiliar de id dos vertices nao foi preenchido pois o vertice nao foi encontrado no grafo!\n");
            return false;
        }

        vetor[i] = id;
    }

    return true;

}

// Função auxiliar.
static bool preencherVetorVertices(Graph g, Vertice *vetor[], int nVert, char *nomesVerts[]){

    int i;
    Vertice *vertice;
    Node id;

    for(i = 0; i < nVert; i++){
        id = getNode(g, nomesVerts[i]);
        vertice = buscarEstruturaVertice(g, id);

        if(vertice == NULL){
            printf("Erro: O vetor auxiliar de vertices nao foi preenchido pois o vertice nao foi encontrado no grafo!\n");
            return false;
        }

        vetor[i] = vertice;
    }

    return true;
}

// Função auxiliar.
static void preencherVerticesSubgrafos(Graph g, char *nomeSubgrafo, Vertice *vetor[], int nVert){

    // Acesso para os campos da struct Grafo. 
    Grafo *grafo = (Grafo*)g;

    Subgrafo *subgrafo = buscarElementoTabela(grafo->subgrafos, nomeSubgrafo);

    if(subgrafo == NULL){
        printf("Erro: Falha ao colocar os vertices no subgrafo do grafo!\n");
        return;
    }

    int i;

    for(i = 0; i < nVert; i++){
        subgrafo->vertices[i] = vetor[i];
        subgrafo->qtdVertices++;
    }

}

// Função auxiliar.
static void preencherArestasSubgrafos(Graph g, char *nomeSubgrafo, Node vetor[], int nVert){

    // Acesso para os campos da struct Grafo. 
    Grafo *grafo = (Grafo*)g;
    
    Subgrafo *subgrafo = buscarElementoTabela(grafo->subgrafos, nomeSubgrafo);

    if(subgrafo == NULL){
        printf("Erro: Falha ao colocar as arestas no subgrafo do grafo!\n");
        return;
    }

    int i, j;
    Aresta *auxiliar;

    for(i = 0; i < grafo->qtdVertices; i++){
            if(estaNoVetorIds(grafo->vertices[i]->id, vetor, nVert) == true){
                auxiliar = grafo->vertices[i]->adjacentes;
                while(auxiliar != NULL){
                    for(j = 0; j < nVert; j++){
                        if(auxiliar->destino == vetor[j]){
                            subgrafo->arestas[subgrafo->qtdArestas] = auxiliar;
                            subgrafo->qtdArestas++;
                            break;
                        }
                    }
                    auxiliar = auxiliar->proxima;
                }
            }
        }

}

void createSubgraphDG(Graph g, char *nomeSubgrafo, char *nomesVerts[], int nVert, bool comArestas){

    /* Acesso para os campos da struct Grafo. */
    Grafo *grafo = (Grafo*)g;

    if(nVert < 0){
        printf("Erro: O numero de vertices maximos do subgrafo nao deve ser negativo!\n");
        return;
    }

    /* Aloca o subgrafo novo. */
    Subgrafo *novoSubgrafo = (Subgrafo*)malloc(sizeof(Subgrafo));

    /* Verifica a alocação. */
    if(novoSubgrafo == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao do subgrafo do grafo!\n");
        return;
    }

    novoSubgrafo->arestas = (Aresta**)calloc(grafo->qtdArestas, sizeof(Aresta*));
    if(novoSubgrafo->arestas == NULL){
        printf("Erro: Falha na alocacao de memoria para as arestas do subgrafo!\n");
        free(novoSubgrafo);
        return;
    }

    novoSubgrafo->vertices = (Vertice**)calloc(grafo->limiteVertices, sizeof(Vertice*));
    if(novoSubgrafo->vertices == NULL){
        printf("Erro: Falha na alocacao de memoria para os vertices do subgrafo!\n");
        free(novoSubgrafo->arestas);
        free(novoSubgrafo);
        return;
    }

    /* Preenche os demais campos do subgrafo. */

    novoSubgrafo->nome = (char*)malloc(strlen(nomeSubgrafo) + 1);
    if(novoSubgrafo->nome == NULL){
        printf("Erro: Falha na alocacao de memoria para a atribuicao do nome do subgrafo!\n");
        free(novoSubgrafo->vertices);
        free(novoSubgrafo->arestas);
        free(novoSubgrafo);
        return;
    }
    strcpy(novoSubgrafo->nome, nomeSubgrafo);

    novoSubgrafo->qtdArestas = 0;
    novoSubgrafo->qtdVertices = 0;

    int i, j;
    Node vetorIds[nVert];
    Vertice *vetorVertices[nVert];
    Aresta *auxiliar;  

    if(preencherVetorIds(g, vetorIds, nVert, nomesVerts) == false || preencherVetorVertices(g, vetorVertices, nVert, nomesVerts) == false){
        free(novoSubgrafo->nome);
        free(novoSubgrafo->arestas);
        free(novoSubgrafo->vertices);
        free(novoSubgrafo);
        return;
    }

    /* Verifica se o subgrafo conterá arestas. */
    if(comArestas == true){
        preencherArestasSubgrafos(g, novoSubgrafo->nome, vetorIds, nVert);
        preencherVerticesSubgrafos(g, novoSubgrafo->nome, vetorVertices, nVert);

    }else{
        preencherVerticesSubgrafos(g, novoSubgrafo->nome, vetorVertices, nVert);
        // As arestas ficam NULL (calloc).
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

    subgrafo->arestas[subgrafo->qtdArestas] = e;
    subgrafo->qtdArestas++;

    return e;

}

////////////////////////////////////////////////////////////////////////////////////////

bool existsEdgeSDG(Graph g, char *nomeSubgrafo, Edge e){

    /* Acesso aos campos da struct Grafo. */
    Grafo *grafo = (Grafo*)g;

    Subgrafo *subgrafo = buscarElementoTabela(grafo->subgrafos, nomeSubgrafo);
    Aresta *auxiliar;
    int i;

    for(i = 0; i < subgrafo->qtdArestas; i++){
        if(subgrafo->arestas[i] == e){
            return true;
        }
    }

    return false;

}

////////////////////////////////////////////////////////////////////////////////////////

void excludeEdgeSDG(Graph g, char *nomeSubgrafo, Edge e){

    Grafo *grafo = (Grafo*)g;

    Subgrafo *subgrafo = buscarElementoTabela(grafo->subgrafos, nomeSubgrafo);

    if(subgrafo == NULL){
        printf("Aviso: O subgrafo nao foi encontrado no grafo!\n");
        return;
    }

    int i, j;

    for(i = 0; i < subgrafo->qtdArestas; i++){
        if(subgrafo->arestas[i] == e){
            for(j = i; j < subgrafo->qtdArestas - 1; j++){
                subgrafo->arestas[j] = subgrafo->arestas[j + 1];
            }
            subgrafo->qtdArestas--;
            break;
            // Sem dar free porque é subgrafo.
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
    Aresta *auxiliar = subgrafo->vertices[node]->adjacentes;

    while(auxiliar != NULL){
        inserirInicioLista(arestasAdjacentes, auxiliar);
        auxiliar = auxiliar->proxima;
    }

}

////////////////////////////////////////////////////////////////////////////////////////

// Edit: correção q evita falha na segmentação

void getAllNodesSDG(Graph g, char *nomeSubgrafo, Lista lstNodes) {
    Grafo *grafo = (Grafo*)g;

    if (!grafo || !nomeSubgrafo || !lstNodes) return;

    Subgrafo *subgrafo = buscarElementoTabela(grafo->subgrafos, nomeSubgrafo);
    if (!subgrafo || subgrafo->qtdVertices <= 0 || !subgrafo->vertices) return;

    for (int i = 0; i < subgrafo->qtdVertices; i++) {
        inserirInicioLista(lstNodes, subgrafo->vertices[i]);
    }
}


////////////////////////////////////////////////////////////////////////////////////////

// Edit: correção q evita falha na segmentação

void getAllEdgesSDG(Graph g, char *nomeSubgrafo, Lista lstEdges) {
    Grafo *grafo = (Grafo*)g;

    if (!grafo || !nomeSubgrafo || !lstEdges) return;

    Subgrafo *subgrafo = buscarElementoTabela(grafo->subgrafos, nomeSubgrafo);
    if (!subgrafo || subgrafo->qtdVertices <= 0 || !subgrafo->arestas) return;

    for (int i = 0; i < subgrafo->qtdVertices; i++) {
        if (subgrafo->arestas[i]) {
            inserirInicioLista(lstEdges, subgrafo->arestas[i]);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////

Graph produceGraph(Graph g, char *nomeSubgrafo){

    Grafo *grafo = (Grafo*)g;

    Subgrafo *subgrafo = buscarElementoTabela(grafo->subgrafos, nomeSubgrafo);

    if(subgrafo == NULL){
        printf("Aviso: O subgrafo nao foi encontrado no grafo!\n");
        return NULL;
    }

    Grafo *novoGrafo = (Grafo*)malloc(sizeof(Grafo));

    if(novoGrafo == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao do grafo a partir do subgrafo!\n");
        return NULL;
    }

    novoGrafo->direcionado = grafo->direcionado;
    novoGrafo->limiteVertices = grafo->limiteVertices;

    novoGrafo->nome = (char*)malloc(strlen(nomeSubgrafo) + 1);

    if(novoGrafo->nome == NULL){
        printf("Erro: Falha na alocacao de memoria para o nome do grafo a partir do subgrafo!\n");
        free(novoGrafo);
        return NULL;
    }

    novoGrafo->vertices = (Vertice**)calloc(subgrafo->qtdVertices, sizeof(Vertice*));

    if(novoGrafo->vertices == NULL){
        printf("Erro: Falha na alocacao de memoria para a lista de vertices do grafo a partir do subgrafo!\n");
        free(novoGrafo->nome);
        free(novoGrafo);
        return NULL;
    }

    int i;

    for(i = 0; i < subgrafo->qtdVertices; i++){
        Vertice *novoVertice = subgrafo->vertices[i];
        addNode(novoGrafo, novoVertice->nome, novoVertice->dados);
    }

    for(i = 0; i < subgrafo->qtdArestas; i++){
        // arrumar aqui depois
    }

    novoGrafo->qtdSubgrafos = 0;
    novoGrafo->subgrafos = criarTabela(211);

    return novoGrafo;
    
}

////////////////////////////////////////////////////////////////////////////////////////

void ativarAresta(Graph g, Edge e){

    Aresta *aresta = (Aresta*)e;

    aresta->ativa = true;

}

////////////////////////////////////////////////////////////////////////////////////////

void desativarAresta(Graph g, Edge e){

    Aresta *aresta = (Aresta*)e;

    aresta->ativa = false;
    
}

////////////////////////////////////////////////////////////////////////////////////////

Node includeNodeSDG(Graph g, char *nomeSubgrafo, Node node){

    Grafo *grafo = (Grafo*)g;

    Subgrafo *subgrafo = buscarElementoTabela(grafo->subgrafos, nomeSubgrafo);

    if(subgrafo == NULL){
        printf("Aviso: O subgrafo nao foi encontrado no grafo!\n");
        return -1;
    }

    Vertice *vertice = buscarEstruturaVertice(g, node);

    subgrafo->vertices[subgrafo->qtdVertices] = vertice;
    subgrafo->qtdVertices++;

    return vertice->id;

}

////////////////////////////////////////////////////////////////////////////////////////

bool subgraphExisteEValido(Graph g, char *nomeSubgrafo) {
    Grafo *grafo = (Grafo*)g;

    if (!grafo || !nomeSubgrafo) return false;

    Subgrafo *sub = buscarElementoTabela(grafo->subgrafos, nomeSubgrafo);
    if (!sub || sub->qtdVertices <= 0 || !sub->vertices) return false;

    return true;
}

