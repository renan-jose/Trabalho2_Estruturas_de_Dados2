// Importação das bibliotecas
#include "processamento.h"
#include "tag.h"
#include "lista.h"
#include "arquivo.h"
#include "espalhamento.h"
#include "graph.h"
#include "smutreap.h"
#include "prioridade.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////////////

struct Vertice{

    double x, y;
    char id[32];

};

struct Aresta{

    char ldir[30], lesq[30], nome[30];
    double cmp, vm;

};

struct Posicao{

    double x, y;
};

typedef struct Vertice Vertice;
typedef struct Aresta Aresta;
typedef struct Posicao Posicao;

// Funções auxiliares //////////////////////////////////////////////////////////////////////////////////////

void calcularBoundingBoxVertice(Info i, double *x, double *y, double *w, double *h){

    Vertice *vertice = (Vertice*)i;

    *x = vertice->x;
    *y = vertice->y;
    *w = 0.0;
    *h = 0.0;

}


static double calcularDistancia(Graph g, Node a, Node b){

    Vertice *v1 = (Vertice *)getNodeInfo(g, a);
    Vertice *v2 = (Vertice *)getNodeInfo(g, b);

    if (!v1 || !v2) return 1e9; // distância absurda se info faltar

    double dx = v1->x - v2->x;
    double dy = v1->y - v2->y;
    return sqrt(dx * dx + dy * dy);
    
}


static char *duplicarString(char *string){

    // Aloca a cópia da string
    char *copia = (char*)malloc(strlen(string) + 1);

    // Verifica a alocação.
    if(copia == NULL){
        printf("Erro: Falha na alocacao de memoria para a duplicao de strings!\n");
        return NULL;
    }

    // Função strcpy realiza o processo de cópia.
    strcpy(copia, string);

    // Retorna a string copiada.
    return copia;

}


static bool relaxarAresta(Graph g, Node atual, Node vizinho, Edge e, TabelaGenerica custo, TabelaGenerica pai, FilaPrioridadeGenerica fila, Node destino) {

    if (!isArestaAtiva(g, e)) return false;

    char *idAtual = getNodeName(g, atual);
    char *idVizinho = getNodeName(g, vizinho);

    double *custoAtualPtr = buscarElementoTabela(custo, idAtual);
    if (!custoAtualPtr) return false;

    double custoAtual = *custoAtualPtr;

    Aresta *info = (Aresta *)getEdgeInfo(g, e);
    double novoCusto = custoAtual + info->cmp;

    double *custoVizinhoPtr = buscarElementoTabela(custo, idVizinho);
    if (!custoVizinhoPtr || novoCusto < *custoVizinhoPtr) {
        double heuristica = calcularDistancia(g, vizinho, destino);
        double prioridade = novoCusto + heuristica;

        inserirFilaPrioridade(fila, (void *)(intptr_t)vizinho, prioridade);

        double *novoPtr = malloc(sizeof(double));
        *novoPtr = novoCusto;
        inserirElementoTabela(custo, idVizinho, novoPtr);

        inserirElementoTabela(pai, idVizinho, duplicarString(idAtual));
        return true;
    }

    return false;
}

static Lista aEstrela(Graph g, char *idOrigem, char *idDestino, char *nomeSubgrafo) {
    Node origem = getNode(g, idOrigem);
    Node destino = getNode(g, idDestino);

    if (origem < 0 || destino < 0) {
        printf("[DEBUG] Origem ou destino inválido(s): origem=%d, destino=%d\n", origem, destino);
        return NULL;
    }

    FilaPrioridadeGenerica fila = criarFilaPrioridade();
    TabelaGenerica custo = criarTabela(500);
    TabelaGenerica pai = criarTabela(500);
    TabelaGenerica visitado = criarTabela(500);

    // custo inicial = 0
    double* custoInicial = malloc(sizeof(double));
    *custoInicial = 0.0;
    inserirElementoTabela(custo, idOrigem, custoInicial);

    double heuristica = calcularDistancia(g, origem, destino);
    inserirFilaPrioridade(fila, (void*)(intptr_t)origem, heuristica);

    while (!filaPrioridadeVazia(fila)) {
        Node atual = (Node)(intptr_t)removerFilaPrioridade(fila);
        char *idAtual = getNodeName(g, atual);

        if (buscarElementoTabela(visitado, idAtual)) continue;
        inserirElementoTabela(visitado, idAtual, (void*)1);

        if (atual == destino) break;

        Lista adjacentes = inicializarLista();
        if (nomeSubgrafo == NULL)
            adjacentEdges(g, atual, adjacentes);
        else
            adjacentEdgesSDG(g, nomeSubgrafo, atual, adjacentes);

        for (int i = 0; i < buscarTamanhoLista(adjacentes); i++) {
            Edge aresta = buscarElementoLista(adjacentes, i);
            if (!aresta || !isArestaAtiva(g, aresta)) continue;  // <<< ESSA LINHA É A CORREÇÃO >>>

            Node vizinho = getToNode(g, aresta);
            char *idVizinho = getNodeName(g, vizinho);

            if (!buscarElementoTabela(visitado, idVizinho)) {
                double* custoAtualPtr = buscarElementoTabela(custo, idAtual);
                double custoAtual = custoAtualPtr ? *custoAtualPtr : INFINITY;

                Aresta* dados = getEdgeInfo(g, aresta);
                double novoCusto = custoAtual + dados->cmp;

                double* custoExistente = buscarElementoTabela(custo, idVizinho);
                if (!custoExistente || novoCusto < *custoExistente) {
                    double* novoCustoPtr = malloc(sizeof(double));
                    *novoCustoPtr = novoCusto;
                    inserirElementoTabela(custo, idVizinho, novoCustoPtr);
                    inserirElementoTabela(pai, idVizinho, idAtual);

                    double heuristica = calcularDistancia(g, vizinho, destino);
                    inserirFilaPrioridade(fila, (void*)(intptr_t)vizinho, novoCusto + heuristica);
                }
            }
        }

        desalocarLista(adjacentes);
    }

    if (!buscarElementoTabela(pai, idDestino)) {
        desalocarFilaPrioridade(fila);
        desalocarTabela(custo);
        desalocarTabela(pai);
        desalocarTabela(visitado);
        return NULL;
    }

    Lista caminho = inicializarLista();
    char *atual = getNodeName(g, destino);

    while (atual != NULL) {
        inserirInicioLista(caminho, atual);
        atual = buscarElementoTabela(pai, atual);
    }

    desalocarFilaPrioridade(fila);
    desalocarTabela(custo);
    desalocarTabela(pai);
    desalocarTabela(visitado);

    return caminho;
}


char* acharVerticeMaisProximo(Graph g, double x, double y) {
    Lista nomes = inicializarLista();
    getNodeNames(g, nomes);

    double menorDist = __DBL_MAX__;
    char* maisProximo = NULL;

    for (int i = 0; i < buscarTamanhoLista(nomes); i++) {
        char* nome = buscarElementoLista(nomes, i);
        Node n = getNode(g, nome);
        Vertice* v = (Vertice*)getNodeInfo(g, n);

        if (v) {
            double dx = v->x - x;
            double dy = v->y - y;
            double dist = dx * dx + dy * dy;
            if (dist < menorDist) {
                menorDist = dist;
                maisProximo = nome;
            }
        }
    }

    desalocarLista(nomes);
    return maisProximo;
}


static Posicao encontrarPosicao(TabelaGenerica t, char *cep, char *face, double numero){

    Posicao posicao = {0, 0};

    FormaGeometricaGenerica quadra = buscarElementoTabela(t, cep);

    if(quadra == NULL){
        printf("Erro: Quadra com cep %s nao encontrada!\n", cep);
        return posicao;
    }

    double x = buscarCoordXFormaGeometrica(quadra), y = buscarCoordYFormaGeometrica(quadra), w = buscarLarguraFormaGeometrica(quadra), h = buscarAlturaFormaGeometrica(quadra);

    if(strcmp(face, "N") == 0){
        posicao.x = x + numero;
        posicao.y = y + h;
    }else if(strcmp(face, "S") == 0){
        posicao.x = x + numero;
        posicao.y = y;
    }else if(strcmp(face, "L") == 0){
        posicao.x = x + w;
        posicao.y = y + numero;
    }else if(strcmp(face, "O") == 0){
        posicao.x = x;
        posicao.y = y + numero;
    }else{
        printf("Aviso: Face invalida!\n");
    }

    return posicao;

}

// Funções de armazenamento //////////////////////////////////////////////////////////////////////////////////////

static void processarGeo(TabelaGenerica t, Lista l, FILE *arquivoGeo, FILE *arquivoSvg){

    char linha[512], corb[32] = "yellow", corp[32] = "blue", expb[32] = "solid", cep[32];
    double x, y, w, h;

    while(fgets(linha, sizeof(linha), arquivoGeo)){
        if(linha[0] == '#' || linha[0] == '\n'){
            continue;
        }

        if(strncmp(linha, "cq", 2) == 0){
            if(sscanf(linha, "cq %s %s %s", corp, corb, expb) == 3){
                
            }
        }else if(linha[0] == 'q'){
            if(sscanf(linha, "q %s %lf %lf %lf %lf", cep, &x, &y, &w, &h) == 5){
                FormaGeometricaGenerica retangulo = formularRetangulo(cep, x, y, w, h, corp, corb, expb);

                tagRetangulo(arquivoSvg, retangulo);

                fprintf(arquivoSvg, "<text x='%.2lf' y='%.2lf' font-size='14' fill='black' font-weight='bold' "
                    "text-anchor='start' dominant-baseline='hanging'>%s</text>\n", x + 2, y + 2, cep);

                inserirElementoTabela(t, cep, retangulo);
                inserirInicioLista(l, retangulo);

            }
        }

    }

    fclose(arquivoGeo);

}


static void processarVia(GrafoGenerico g, FILE *arquivoVia, FILE *arquivoSvg, SmuTreap arvore){

    char linha[1024], ldir[30], lesq[30], nome[30], id[150], origem[20], destino[20];
    double cmp, vm, x, y;

    while(fgets(linha, sizeof(linha), arquivoVia)){
        if(linha[0] == '#' || linha[0] == '\n'){
            continue;
        }

        if(linha[0] == 'v'){
            if(sscanf(linha, "v %s %lf %lf", id, &x, &y) == 3){
                Vertice *vertice = malloc(sizeof(Vertice));

                strcpy(vertice->id, id);
                vertice->x = x;
                vertice->y = y;

                addNode(g, id, vertice);

                insertSmuT(arvore, x, y, vertice, 1, calcularBoundingBoxVertice);

                fprintf(arquivoSvg, "<circle cx='%.2lf' cy='%.2lf' r='3' fill='blue' stroke='black' stroke-width='0.5' />\n", x, y);

                fprintf(arquivoSvg, "<text x='%.2lf' y='%.2lf' font-size='7' fill='blue' text-anchor='middle' dominant-baseline='middle'>(%s)</text>\n", x, y - 6, id);
                
            }

        }else if(linha[0] == 'e'){
            if(sscanf(linha, "e %s %s %s %s %lf %lf  %[^\n]", origem, destino, ldir, lesq, &cmp, &vm, nome) == 7 || sscanf(linha, "e %s %s %s %s %lf %lf %[^\n]", origem, destino, ldir, lesq, &cmp, &vm, nome) == 6){
                Aresta *aresta = malloc(sizeof(Aresta));

                strcpy(aresta->ldir, ldir);
                strcpy(aresta->lesq, lesq);
                strcpy(aresta->nome, nome);
                
                aresta->cmp = cmp;
                aresta->vm = vm;

                Node from = getNode(g, origem);
                Node to = getNode(g, destino);

                if(from != -1 && to != -1){
                    addEdge(g, from, to, aresta);

                    Vertice *vOrigem = (Vertice *)getNodeInfo(g, from);
                    Vertice *vDestino = (Vertice *)getNodeInfo(g, to);

                    fprintf(arquivoSvg,
                            "<line x1='%.2lf' y1='%.2lf' x2='%.2lf' y2='%.2lf' stroke='gray' stroke-width='1' marker-end='url(#arrow)'/>\n",
                            vOrigem->x, vOrigem->y, vDestino->x, vDestino->y);

                }else{
                    printf("Erro: vertice de origem ou destino nao encontrado (%s -> %s), aresta ignorada.\n", origem, destino);
                    free(aresta); 
                }
            }
        }

    }

    fclose(arquivoVia);

}

// Funções de manipulação //////////////////////////////////////////////////////////////////////////////////////

static void processarArrobaOInterrogacao(TabelaGenerica t, char *linha, FILE *arquivoSvg, FILE *arquivoTxt, Posicao *registradores){

    int numeroRegistrador;
    char cep[32], face[3], nomeRegistrador[10];
    double num;

    if(sscanf(linha, "@o? %s %s %s %lf", nomeRegistrador, cep, face, &num) == 4){
        Posicao posicao = encontrarPosicao(t, cep, face, num);

        if(sscanf(nomeRegistrador, "R%d", &numeroRegistrador) != 1){
            printf("Erro: Nome do registrador invalido!\n");
            return;
        }

        registradores[numeroRegistrador] = posicao;

        fprintf(arquivoSvg, "<line x1=\"%.2lf\" y1=\"%.2lf\" x2=\"%.2lf\" y2=\"0\" stroke=\"red\" stroke-width=\"2\" stroke-dasharray=\"5,5\" />\n", posicao.x, posicao.y, posicao.x);

        fprintf(arquivoSvg, "<text x=\"%.2lf\" y=\"0\" fill=\"black\" stroke=\"red\" font-weight=\"bold\" font-size=\"12\" text-anchor=\"middle\" dominant-baseline=\"middle\">%s</text>\n", posicao.x, nomeRegistrador);

        fprintf(arquivoTxt, "[*] @o?\n registrador %s -> (%.2lf, %.2lf)\n", nomeRegistrador, posicao.x, posicao.y);
    }else{
        printf("Erro: Linha invalida no comando @o?: %s\n", linha);
    }

}

////////////////////////////////////////////////////////////////////////////////////////

static void processarAlag(Graph g, SmuTreap t, char *linha, FILE *arquivoSvg, FILE *arquivoTxt) {
    int n;
    double x, y, w, h;

    if (sscanf(linha, "alag %d %lf %lf %lf %lf", &n, &x, &y, &w, &h) != 5) {
        fprintf(stderr, "[*] alag \n Comandos nao reconhecidos: %s\n", linha);
        return;
    }

    fprintf(arquivoSvg,
        "<rect x='%.2lf' y='%.2lf' width='%.2lf' height='%.2lf' "
        "fill='#AB37C8' stroke='#AA0044' stroke-width='1' fill-opacity='0.5'/>\n",
        x, y, w, h);

    // Busca os vértices dentro da região
    Lista verticesNaRegiao = inicializarLista();
    getNodisDentroRegiaoSmuT(t, x, y, x + w, y + h, verticesNaRegiao);

    int qtd = buscarTamanhoLista(verticesNaRegiao);
    if (qtd == 0) {
        fprintf(arquivoTxt, "[*] alag %d \n Nenhum vértice encontrado na região (%.2lf,%.2lf,%.2lf,%.2lf)\n", n, x, y, w, h);
        desalocarLista(verticesNaRegiao);
        return;
    }

    char nomeSubgrafo[32];
    sprintf(nomeSubgrafo, "alag%d", n);
    createSubgraphDG(g, nomeSubgrafo, NULL, 0, false);

    for (int i = 0; i < qtd; i++) {
        Nodi no = buscarElementoLista(verticesNaRegiao, i);
        Vertice *v = (Vertice *)getInfoSmuT(t, no);
        double vx = v->x;
        double vy = v->y;

        // Busca o vértice pelo grafo (você armazenou info == Vertice*, então compara pelas coordenadas)
        Lista nomes = inicializarLista();
        getNodeNames(g, nomes);

        for (int j = 0; j < buscarTamanhoLista(nomes); j++) {
            char *nome = buscarElementoLista(nomes, j);
            Node node = getNode(g, nome);
            Vertice *info = (Vertice *)getNodeInfo(g, node);
            if (info && info->x == vx && info->y == vy) {
                Lista arestas = inicializarLista();
                adjacentEdges(g, node, arestas);
                for (int k = 0; k < buscarTamanhoLista(arestas); k++) {
                    Edge e = buscarElementoLista(arestas, k);
                    desativarAresta(g, e);
                    includeEdgeSDG(g, nomeSubgrafo, e);

                    Aresta *dados = (Aresta *)getEdgeInfo(g, e);
                    fprintf(arquivoTxt, "[*] alag %d (%.2lf, %.2lf, %.2lf, %.2lf)\n", n, x, y, w, h);
                    fprintf(arquivoTxt, "Aresta desativada: %s -> %s\nRua: %s | Vm: %.2lf | Cmp: %.2lf\n",
                            getNodeName(g, getFromNode(g, e)),
                            getNodeName(g, getToNode(g, e)),
                            dados->nome, dados->vm, dados->cmp);
                }
                desalocarLista(arestas);
            }
        }

        desalocarLista(nomes);
    }

    desalocarLista(verticesNaRegiao);
}

////////////////////////////////////////////////////////////////////////////////////////

static void processarDren(Graph g, char *linha, FILE *arquivoTxt) {
    int n;
    if (sscanf(linha, "dren %d", &n) != 1) {
        fprintf(stderr, "[erro] Comando dren mal formatado: %s\n", linha);
        return;
    }

    char nomeSubgrafo[32];
    sprintf(nomeSubgrafo, "alag%d", n);

    Lista arestasSubgrafo = inicializarLista();
    getAllEdgesSDG(g, nomeSubgrafo, arestasSubgrafo);

    int qtd = buscarTamanhoLista(arestasSubgrafo);
    if (qtd == 0) {
        fprintf(arquivoTxt, "[!] dren %d — nenhuma aresta encontrada no subgrafo %s\n", n, nomeSubgrafo);
        desalocarLista(arestasSubgrafo);
        return;
    }

    for (int i = 0; i < qtd; i++) {
        Edge e = buscarElementoLista(arestasSubgrafo, i);
        ativarAresta(g, e);

        Aresta *dados = (Aresta *)getEdgeInfo(g, e);
        fprintf(arquivoTxt,
            "[*] dren %d — aresta reativada: %s -> %s\nRua: %s | Vm: %.2lf | Cmp: %.2lf\n",
            n,
            getNodeName(g, getFromNode(g, e)),
            getNodeName(g, getToNode(g, e)),
            dados->nome, dados->vm, dados->cmp);
    }

    desalocarLista(arestasSubgrafo);
}

////////////////////////////////////////////////////////////////////////////////////////

static void processarSg(Graph g, SmuTreap t, char *linha, FILE *arquivoSvg) {
    char nomeSubgrafo[32];
    double x, y, w, h;

    if (sscanf(linha, "sg %s %lf %lf %lf %lf", nomeSubgrafo, &x, &y, &w, &h) != 5) {
        fprintf(stderr, "[erro] Linha inválida no comando sg: %s\n", linha);
        return;
    }

    // Desenha o retângulo da região no SVG
    fprintf(arquivoSvg,
            "<rect x='%.2lf' y='%.2lf' width='%.2lf' height='%.2lf' "
            "fill='none' stroke='red' stroke-width='2' stroke-dasharray='5,5'/>\n",
            x, y, w, h);

    // Cria o subgrafo vazio (sem vértices nem arestas)
    createSubgraphDG(g, nomeSubgrafo, NULL, 0, false);

    Lista verticesNaRegiao = inicializarLista();
    getNodisDentroRegiaoSmuT(t, x, y, x + w, y + h, verticesNaRegiao);

    TabelaGenerica verticesTabela = criarTabela(512);

    int qtd = buscarTamanhoLista(verticesNaRegiao);
    for (int i = 0; i < qtd; i++) {
        Nodi no = buscarElementoLista(verticesNaRegiao, i);
        Vertice *info = (Vertice *)getInfoSmuT(t, no);
        if (!info) continue;

        inserirElementoTabela(verticesTabela, info->id, NULL);
    }

    // Agora percorremos os vértices e inserimos arestas válidas no subgrafo
    for (int i = 0; i < qtd; i++) {
        Nodi no = buscarElementoLista(verticesNaRegiao, i);
        Vertice *info = (Vertice *)getInfoSmuT(t, no);
        if (!info) continue;

        Node origem = getNode(g, info->id);
        if (origem == -1) continue;

        Lista arestas = inicializarLista();
        adjacentEdges(g, origem, arestas);

        int tamArestas = buscarTamanhoLista(arestas);
        for (int j = 0; j < tamArestas; j++) {
            Edge e = buscarElementoLista(arestas, j);
            Node destino = getToNode(g, e);
            char *idDestino = getNodeName(g, destino);

            if (idDestino && buscarElementoTabela(verticesTabela, idDestino)) {
                includeEdgeSDG(g, nomeSubgrafo, e);
            }
        }

        desalocarLista(arestas);
    }

    desalocarLista(verticesNaRegiao);
    desalocarTabela(verticesTabela);
}

////////////////////////////////////////////////////////////////////////////////////////

static void processarPInterrogacao(Graph g, char *linha, FILE *arquivoTxt, Posicao *registradores, TabelaGenerica caminhos) {
    char nomePercurso[32], nomeSubgrafo[32], reg1[8], reg2[8];

    if (sscanf(linha, "p? %s %s %s %s", nomePercurso, nomeSubgrafo, reg1, reg2) != 4) {
        fprintf(stderr, "[erro] Linha inválida no comando p?: %s\n", linha);
        return;
    }

    int r1, r2;
    if (sscanf(reg1, "R%d", &r1) != 1 || sscanf(reg2, "R%d", &r2) != 1) {
        fprintf(stderr, "[erro] Registradores inválidos em p?: %s, %s\n", reg1, reg2);
        return;
    }

    Posicao origem = registradores[r1];
    Posicao destino = registradores[r2];

    fprintf(arquivoTxt, "[*] p? %s %s %s %s\n", nomePercurso, nomeSubgrafo, reg1, reg2);

    char* idOrigem = acharVerticeMaisProximo(g, origem.x, origem.y);
    char* idDestino = acharVerticeMaisProximo(g, destino.x, destino.y);

    if (!idOrigem || !idDestino) {
        fprintf(arquivoTxt, " Caminho inacessível (nenhum vértice próximo).\n");
        return;
    }

    char *subgrafo = strcmp(nomeSubgrafo, "-") == 0 ? NULL : nomeSubgrafo;
    Lista caminho = aEstrela(g, idOrigem, idDestino, subgrafo);

    if (!caminho || buscarTamanhoLista(caminho) == 0) {
        fprintf(arquivoTxt, " Caminho inacessível.\n");
        if (caminho) desalocarLista(caminho);
        return;
    }

    fprintf(arquivoTxt, " Caminho mais curto: ");
    for (int i = 0; i < buscarTamanhoLista(caminho); i++) {
        char *id = (char *)buscarElementoLista(caminho, i);
        fprintf(arquivoTxt, "%s", id);
        if (i < buscarTamanhoLista(caminho) - 1)
            fprintf(arquivoTxt, " -> ");
    }
    fprintf(arquivoTxt, "\n");

    inserirElementoTabela(caminhos, nomePercurso, caminho);
}

////////////////////////////////////////////////////////////////////////////////////////

static void processarJoin(Graph g, char *linha, FILE *arquivoTxt, TabelaGenerica caminhos)
{
    char nomeFinal[32], nome1[32], nome2[32];
    if (sscanf(linha, "join %s %s %s", nomeFinal, nome1, nome2) != 3) {
        fprintf(stderr, "Erro: Comando join inválido: %s\n", linha);
        return;
    }

    Lista caminho1 = buscarElementoTabela(caminhos, nome1);
    Lista caminho2 = buscarElementoTabela(caminhos, nome2);

    fprintf(arquivoTxt, "[*] join %s %s %s\n", nomeFinal, nome1, nome2);

    if (!caminho1 || !caminho2) {
        fprintf(arquivoTxt, " Caminhos %s ou %s inexistentes.\n", nome1, nome2);
        return;
    }

    char *destino = buscarElementoLista(caminho1, buscarTamanhoLista(caminho1) - 1);
    char *origem  = buscarElementoLista(caminho2, 0);

    Lista intermediario = aEstrela(g, destino, origem, NULL); // Grafo completo

    if (!intermediario || buscarTamanhoLista(intermediario) == 0) {
        fprintf(arquivoTxt, " Caminho intermediário entre %s e %s não encontrado!\n", destino, origem);
        if (intermediario) desalocarLista(intermediario);
        return;
    }

    Lista caminhoFinal = inicializarLista();

    // Copia caminho1 exceto o último vértice
    for (int i = 0; i < buscarTamanhoLista(caminho1) - 1; i++)
        inserirFimLista(caminhoFinal, buscarElementoLista(caminho1, i));

    // Copia intermediário exceto o último vértice
    for (int i = 0; i < buscarTamanhoLista(intermediario) - 1; i++)
        inserirFimLista(caminhoFinal, buscarElementoLista(intermediario, i));

    // Copia caminho2 completo
    for (int i = 0; i < buscarTamanhoLista(caminho2); i++)
        inserirFimLista(caminhoFinal, buscarElementoLista(caminho2, i));

    inserirElementoTabela(caminhos, nomeFinal, caminhoFinal);

    fprintf(arquivoTxt, " Caminho combinado com sucesso.\n");

    desalocarLista(intermediario);

}

////////////////////////////////////////////////////////////////////////////////////////

// Função auxiliar: sanitiza string para uso seguro em IDs SVG
static void sanitizarIdSvg(char *str) {
    for (int i = 0; str[i]; i++) {
        if (!((str[i] >= 'a' && str[i] <= 'z') ||
              (str[i] >= 'A' && str[i] <= 'Z') ||
              (str[i] >= '0' && str[i] <= '9') ||
              str[i] == '-' || str[i] == '_')) {
            str[i] = '_';
        }
    }
}

static void processarShw(Graph g, char *linha, FILE *arquivoSvg, TabelaGenerica caminho) {
    char nomePercurso[32], corCurto[16], corRapido[16];

    if (sscanf(linha, "shw %s %s %s", nomePercurso, corCurto, corRapido) != 3) {
        printf("Erro: Linha inválida no comando shw: %s\n", linha);
        return;
    }

    Lista listaCaminho = buscarElementoTabela(caminho, nomePercurso);
    if (!listaCaminho || buscarTamanhoLista(listaCaminho) < 2) {
        fprintf(arquivoSvg, "<!-- Caminho '%s' inválido ou inexistente -->\n", nomePercurso);
        return;
    }

    // Cria cópia sanitizada para usar nos IDs
    char nomeSvg[32];
    strncpy(nomeSvg, nomePercurso, sizeof(nomeSvg));
    nomeSvg[sizeof(nomeSvg) - 1] = '\0';
    sanitizarIdSvg(nomeSvg);

    double offsetCurto = -4.0, offsetRapido = 4.0;

    // Caminho mais curto
    fprintf(arquivoSvg, "<path id='path-%s-curto' d='", nomeSvg);
    for (int i = 0; i < buscarTamanhoLista(listaCaminho); i++) {
        char *id = buscarElementoLista(listaCaminho, i);
        Vertice *info = getNodeInfo(g, getNode(g, id));
        fprintf(arquivoSvg, "%c%.2lf,%.2lf ", (i == 0 ? 'M' : 'L'), info->x + offsetCurto, info->y + offsetCurto);
    }
    fprintf(arquivoSvg, "' stroke='%s' stroke-width='8' fill='none' stroke-opacity='0.9'/>\n", corCurto);

    // Caminho mais rápido
    fprintf(arquivoSvg, "<path id='path-%s-rapido' d='", nomeSvg);
    for (int i = 0; i < buscarTamanhoLista(listaCaminho); i++) {
        char *id = buscarElementoLista(listaCaminho, i);
        Vertice *info = getNodeInfo(g, getNode(g, id));
        fprintf(arquivoSvg, "%c%.2lf,%.2lf ", (i == 0 ? 'M' : 'L'), info->x + offsetRapido, info->y + offsetRapido);
    }
    fprintf(arquivoSvg, "' stroke='%s' stroke-width='5' fill='none' stroke-opacity='0.7'/>\n", corRapido);

    // Animação
    fprintf(arquivoSvg,
        "<circle r='10' fill='black'>\n"
        "  <animateMotion dur='10s' repeatCount='indefinite'>\n"
        "    <mpath href='#path-%s-curto'/>\n"
        "  </animateMotion>\n"
        "</circle>\n",
        nomeSvg);

}


////////////////////////////////////////////////////////////////////////////////////////

void processarQry(char *caminhoGeo, char *caminhoVia, char *caminhoQry, char *caminhoSvg, char *caminhoTxt){

    TabelaGenerica quadras = criarTabela(1200);
    Lista lista = inicializarLista();

    FILE *arquivoSvg = fopen(caminhoSvg, "w");

    if(arquivoSvg == NULL){
        printf("Erro: Falha ao gerar o arquivo .svg!\n");
        return;
    }

    FILE *arquivoTxt = fopen(caminhoTxt, "w");

    if(arquivoTxt == NULL){
        printf("Erro? Falha ao gerar o arquivo .txt!\n");
        fclose(arquivoSvg);
        return;
    }

    FILE *arquivoGeo = fopen(caminhoGeo, "r");

    if(arquivoGeo == NULL){
        printf("Erro: Falha na abertura do arquivo .geo!\n");
        return;
    }

    FILE *arquivoVia = fopen(caminhoVia, "r");

    if(arquivoVia == NULL){
        printf("Erro: Falha na abertura do arquivo .via!\n");
        fclose(arquivoSvg);
        fclose(arquivoTxt);
        return;
    }

    tagCabecalho(arquivoSvg);

    processarGeo(quadras, lista, arquivoGeo, arquivoSvg);

    int qtdVertices;

    fscanf(arquivoVia, "%d", &qtdVertices);

    Graph grafo = createGraph(qtdVertices, true, "g");
    SmuTreap arvore = newSmuTreap(1, 5.0, 0.00001, 1000000);
    TabelaGenerica caminhos = criarTabela(300);

    processarVia(grafo, arquivoVia, arquivoSvg, arvore);

    FILE *arquivoQry = fopen(caminhoQry, "r");

    if(arquivoQry == NULL){
        printf("Erro: Falha na abertura do arquivo .qry!\n");
        return;
    }

    char linha [1024];
    Posicao registradores[10];

    while(fgets(linha, sizeof(linha), arquivoQry)){
        if(linha[0] == '#' || linha[0] == '\n'){
            continue;
        }

        if(strncmp(linha, "@o?", 3) == 0){
            processarArrobaOInterrogacao(quadras, linha, arquivoSvg, arquivoTxt, registradores);
        }else if(strncmp(linha, "alag", 4) == 0){
            processarAlag(grafo, arvore, linha, arquivoSvg, arquivoTxt);
        }else if(strncmp(linha, "dren", 4) == 0){
            processarDren(grafo, linha, arquivoTxt);
        }else if(strncmp(linha, "sg", 2) == 0){
            processarSg(grafo, arvore, linha, arquivoSvg);
        }else if(strncmp(linha, "p?", 2) == 0){
            processarPInterrogacao(grafo, linha, arquivoTxt, registradores, caminhos);
        }else if(strncmp(linha, "join", 4) == 0){
            processarJoin(grafo, linha, arquivoTxt, caminhos);
        }else if(strncmp(linha, "shw", 3) == 0){
            processarShw(grafo, linha, arquivoSvg, caminhos);
        }
    }

    tagRodape(arquivoSvg);

    killDG(grafo);
    desalocarLista(lista);
    desalocarTabela(quadras);

    fclose(arquivoQry);
    fclose(arquivoSvg);
    fclose(arquivoTxt);

}
