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

struct DadosVertice{

    char *id;
    double x, y;

};

struct DadosAresta{

    char *ldir, *lesq, *nome;
    double cmp, vm;

};

struct Posicao{

    double x, y;
};

typedef struct DadosVertice DadosVertice;
typedef struct DadosAresta DadosAresta;
typedef struct Posicao Posicao;

////////////////////////////////////////////////////////////////////////////////////////

static TabelaGenerica processarGeo(char *caminhoGeo, FILE *arquivoSvg){

    printf("Tentando abrir o arquivo GEO: '%s'\n", caminhoGeo);

    FILE *arquivoGeo = fopen(caminhoGeo, "r");

    if(arquivoGeo == NULL){
        printf("Erro: Nao foi possivel abrir o arquivo .geo: %s!\n", caminhoGeo);
        return NULL;
    }

    char linha[512];
    double x, y, w, h;
    char cep[32], corb[32], corp[32], expb[32];
    TabelaGenerica tabela = criarTabela(10000);

    while(fgets(linha, sizeof(linha), arquivoGeo)){
        if(linha[0] == '#' || linha[0] == '\n'){
            continue;
        }
       
        if(strncmp(linha, "cq", 2) == 0){
            sscanf(linha, "cq %s %s %s", corp, corb, expb);
        }else if(strncmp(linha, "q", 1) == 0){
            if(sscanf(linha, "q %s %lf %lf %lf %lf", cep, &x, &y, &w, &h) == 5){
                FormaGeometricaGenerica retangulo = formularRetangulo(cep, x, y, w, h, corp, corb, expb);
                if(retangulo != NULL){
                    inserirElementoTabela(tabela, cep, retangulo);
                    tagRetangulo(arquivoSvg, retangulo);
                    fprintf(arquivoSvg, "<text x='%.2lf' y='%.2lf' font-size='14' fill='black' font-weight='bold' "
                    "text-anchor='start' dominant-baseline='hanging'>%s</text>\n", x + 2, y + 2, cep);
                }else{
                    printf("Erro: inha malformada ignorada: %s", linha);
                }
            }else{
                printf("Erro: Linha de texto invalida: %s\n", linha);
            }
        }
    }

    fclose(arquivoGeo);

    return tabela;

}

////////////////////////////////////////////////////////////////////////////////////////

void calcularBoundingBoxVertice(Info i, double *x, double *y, double *w, double *h){

    DadosVertice *dadosVertice = (DadosVertice*)i;

    *x = dadosVertice->x;
    *y = dadosVertice->y;
    *w = 0.0;
    *h = 0.0;

}

// Função auxiliar.
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

////////////////////////////////////////////////////////////////////////////////////////

static Graph processarVia(char *caminhoVia, FILE *arquivoSvg, SmuTreap *saidaArvore){

    FILE *arquivoVia = fopen(caminhoVia, "r");

    if(arquivoVia == NULL){
        printf("Erro: Nao foi possivel abrir o arquivo .via: %s!\n", caminhoVia);
        return NULL;
    }

    char linha[1024], lesq[50], ldir[50], nome[50], id[50], nomeOrigem[30], nomeDestino[30];
    DadosVertice dadosVertice;
    DadosAresta dadosAresta;
    int limiteVertices;
    Node origem, destino;
    Graph grafo = NULL;
    SmuTreap arvore = newSmuTreap(5, 1.1, 0.001, 10000);
    *saidaArvore = arvore;

    double mx, my;

    while(fgets(linha, sizeof(linha), arquivoVia)){
        if(linha[0] == '#' || linha[0] == '\n'){
            continue;
        }

        if(grafo == NULL){
            if(sscanf(linha, "nv %d", &limiteVertices) == 1 || sscanf(linha, "%d", &limiteVertices) == 1){
                grafo = createGraph(limiteVertices, true, "g");
            }else{
                printf("Erro: Linha de texto invalida: %s\n", linha);
            }
        }else if(strncmp(linha, "v", 1) == 0){

            if(grafo == NULL){
                printf("Erro: Grafo ainda nao criado antes de adicionar vertices!\n");
                return NULL;
            }

            if(sscanf(linha, "v %s %lf %lf", id, &dadosVertice.x, &dadosVertice.y) == 3){
                
                DadosVertice *copiaDadosVertice = (DadosVertice*)malloc(sizeof(DadosVertice));

                if(copiaDadosVertice == NULL){
                    printf("Errinho!\n");
                    return NULL;
                }

                copiaDadosVertice->id = duplicarString(id);
                copiaDadosVertice->x = dadosVertice.x;
                copiaDadosVertice->y = dadosVertice.y;

                Node vertice = addNode(grafo, copiaDadosVertice->id, copiaDadosVertice);

                fprintf(arquivoSvg, "<circle cx='%.2lf' cy='%.2lf' r='8' fill='blue' stroke='black' stroke-width='0.5' />\n", dadosVertice.x, dadosVertice.y);

                fprintf(arquivoSvg, "<text x='%.2lf' y='%.2lf' font-size='7' fill='blue' text-anchor='middle' dominant-baseline='middle'>(%s)</text>\n", copiaDadosVertice->x, copiaDadosVertice->y - 6, copiaDadosVertice->id);

                insertSmuT(arvore, copiaDadosVertice->x, copiaDadosVertice->y, copiaDadosVertice, 1, calcularBoundingBoxVertice);

            }else{
                printf("Erro: Linha de texto invalida: %s\n", linha);
            }
        }else if(strncmp(linha, "e", 1) == 0){

            if(grafo == NULL){
                printf("Erro: Grafo ainda nao criado antes de adicionar vertices!\n");
                return NULL;
            }

            if(sscanf(linha, "e %s %s %s %s %lf %lf %s", nomeOrigem, nomeDestino, ldir, lesq, &dadosAresta.cmp, &dadosAresta.vm, nome) == 7){

                origem = getNode(grafo, nomeOrigem);
                destino = getNode(grafo, nomeDestino);

                if (!origem || !destino) {
                    printf("Aviso: O vertice '%s' ou '%s' nao foi encontrado no grafo!\n", nomeOrigem, nomeDestino);
                    continue;  // Nada foi alocado ainda, só ignora a linha
                }   

                dadosAresta.ldir = (char*)malloc(strlen(ldir) + 1);
                dadosAresta.lesq = (char*)malloc(strlen(lesq) + 1);
                dadosAresta.nome = (char*)malloc(strlen(nome) + 1);

                if(dadosAresta.ldir == NULL || dadosAresta.lesq == NULL || dadosAresta.nome == NULL){
                    printf("Erro: Falha nas alocoes de memoria na leitura dos dados da aresta do grafo do arquivo .via!\n");
                    return NULL;
                }

                strcpy(dadosAresta.ldir, ldir);
                strcpy(dadosAresta.lesq, lesq);
                strcpy(dadosAresta.nome, nome);

                DadosAresta *copiaDadosAresta = (DadosAresta*)malloc(sizeof(DadosAresta));

                copiaDadosAresta->ldir = duplicarString(ldir);
                copiaDadosAresta->lesq = duplicarString(lesq);
                copiaDadosAresta->nome = duplicarString(nome);
                copiaDadosAresta->cmp = dadosAresta.cmp;
                copiaDadosAresta->vm = dadosAresta.vm;

                addEdge(grafo, origem, destino, copiaDadosAresta);

                DadosVertice *dadosOrigem = getNodeInfo(grafo, origem);

                DadosVertice *dadosDestino = getNodeInfo(grafo, destino);

                fprintf(arquivoSvg, "<line x1='%.2lf' y1='%.2lf' x2='%.2lf' y2='%.2lf' stroke='black' stroke-width='3'/>\n", dadosOrigem->x, dadosOrigem->y, dadosDestino->x, dadosDestino->y);

            }else{
                printf("Erro: Linha de texto invalida: %s\n", linha);
            }
        }
    }
    
    fclose(arquivoVia);

    return grafo;

}

////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////

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

static void processarAlag(Graph g, SmuTreap t, char *linha, FILE *arquivoSvg, FILE *arquivoTxt){

    int n;
    double x, y, w, h;

    if(sscanf(linha, "alag %d %lf %lf %lf %lf", &n, &x, &y, &w, &h) != 5){
        printf("Erro: Linha invalida no comando alag: %s\n", linha);
        return;
    }

    fprintf(arquivoSvg, "<rect x='%.2lf' y='%.2lf' width='%.2lf' height='%.2lf' "
        "fill='#AB37C8' stroke='#AA0044' stroke-width='1' fill-opacity='0.5'/>\n",
        x, y, w, h);

    Lista verticesNaRegiao = inicializarLista();
    getNodisDentroRegiaoSmuT(t, x, y, x + w, y + h, verticesNaRegiao);

    // Verificar se há pelo menos 1 vértice existente no grafo
    int qtdVertices = buscarTamanhoLista(verticesNaRegiao);
    int verticesValidos = 0;
    for (int i = 0; i < qtdVertices; i++) {
        Nodi no = (Nodi)buscarElementoLista(verticesNaRegiao, i);
        DadosVertice *dados = getInfoSmuT(t, no);
        if (dados && getNode(g, dados->id)) {
            verticesValidos++;
        }
    }

    if (verticesValidos == 0) {
        fprintf(stderr, "[!] alag %d ignorado: nenhum vértice da região existe no grafo\n", n);
        fprintf(arquivoTxt, "[!] alag %d — nenhum vértice da região (%.2lf,%.2lf,%.2lf,%.2lf) existe no grafo\n", n, x, y, w, h);
        desalocarLista(verticesNaRegiao);
        return;
    }

    char nomeSubgrafo[32];
    sprintf(nomeSubgrafo, "alag%d", n);
    createSubgraphDG(g, nomeSubgrafo, NULL, 0, false);

    for (int i = 0; i < qtdVertices; i++) {
        Nodi no = (Nodi)buscarElementoLista(verticesNaRegiao, i);
        DadosVertice *dados = getInfoSmuT(t, no);
        Node vertice = getNode(g, dados->id);
        if (!vertice) continue;

        Lista arestas = inicializarLista();
        adjacentEdges(g, vertice, arestas);

        int qtdArestas = buscarTamanhoLista(arestas);
        for (int j = 0; j < qtdArestas; j++) {
            Edge aresta = (Edge)buscarElementoLista(arestas, j);
            desativarAresta(g, aresta);
            includeEdgeSDG(g, nomeSubgrafo, aresta);

            DadosAresta *info = (DadosAresta*)getEdgeInfo(g, aresta);
            fprintf(arquivoTxt, "[*] alag %d %lf %lf %lf %lf\n", n, x, y, w, h);
            fprintf(arquivoTxt, " Aresta desativada: %s -> %s\nRua: %s\nVm: %.2lf\nCmp: %.2lf\n",
                getNodeName(g, getFromNode(g, aresta)),
                getNodeName(g, getToNode(g, aresta)),
                info->nome, info->vm, info->cmp);
        }

        desalocarLista(arestas);
    }

    desalocarLista(verticesNaRegiao);
}


////////////////////////////////////////////////////////////////////////////////////////

static void processarDren(Graph g, char *linha, FILE *arquivoTxt){

    int n;

    if(sscanf(linha, "dren %d", &n) != 1){
        printf("Erro: Linha invalida no comando dren: %s\n", linha);
        return;
    }

    char nomeSubgrafo[32];
    sprintf(nomeSubgrafo, "alag%d", n);
    
    if (!subgraphExisteEValido(g, nomeSubgrafo)) {
        fprintf(stderr, "[!] dren %d ignorado: subgrafo '%s' inexistente ou vazio.\n", n, nomeSubgrafo);
        fprintf(arquivoTxt, "[!] dren %d — subgrafo inexistente ou vazio.\n", n);
        return;
    }

    Lista arestasSubgrafo = inicializarLista();
    getAllEdgesSDG(g, nomeSubgrafo, arestasSubgrafo);

    int qtdArestasGrafo = buscarTamanhoLista(arestasSubgrafo), i;

    for(i = 0; i < qtdArestasGrafo; i++){
        Edge aresta = (Edge)buscarElementoLista(arestasSubgrafo, i);
        ativarAresta(g, aresta);

        fprintf(arquivoTxt, "[*] dren %d\n", n);
        fprintf(arquivoTxt, " Aresta ativada: %s -> %s\n", getNodeName(g, getFromNode(g, aresta)), getNodeName(g, getToNode(g, aresta)));
    }

    desalocarLista(arestasSubgrafo);

}

////////////////////////////////////////////////////////////////////////////////////////

static void processarSg(Graph g, SmuTreap t, char *linha, FILE *arquivoSvg){

    char nomeSubgrafo[32];
    double x, y, w, h;

    if(sscanf(linha, "sg %s %lf %lf %lf %lf", nomeSubgrafo, &x, &y, &w, &h) != 5){
        printf("Erro: Linha invalida no comando sg: %s", linha);
        return;
    }

    fprintf(arquivoSvg, "<rect x='%.2lf' y='%.2lf' width='%.2lf' height='%.2lf' "
        "fill='none' stroke='red' stroke-width='2' stroke-dasharray='5,5'/>\n",
        x, y, w, h);

    createSubgraphDG(g, nomeSubgrafo, NULL, 0, false);

    Lista verticesNaRegiao = inicializarLista();

    getNodisDentroRegiaoSmuT(t, x, y, x + w, y + h, verticesNaRegiao);

    int qtdVerticesGrafo = buscarTamanhoLista(verticesNaRegiao), i;
    TabelaGenerica tabelaVertices = criarTabela(500);
    Nodi no;
    Node vertice, destino;
    Edge aresta;
    DadosVertice *dadosVertice;
    

    for(i = 0; i < qtdVerticesGrafo; i++){
        no = (Nodi)buscarElementoLista(verticesNaRegiao, i);
        dadosVertice = (DadosVertice*)getInfoSmuT(t, no);
        inserirElementoTabela(tabelaVertices, dadosVertice->id, NULL);

        vertice = getNode(g, dadosVertice->id);

        includeNodeSDG(g, nomeSubgrafo, vertice);
    }

    for(i = 0; i < qtdVerticesGrafo; i++){
        no = buscarElementoLista(verticesNaRegiao, i);
        dadosVertice = (DadosVertice*)getInfoSmuT(t, no);
        vertice = getNode(g, dadosVertice->id);

        Lista arestas = inicializarLista();
        adjacentEdges(g, vertice, arestas);

        int qtdArestasGrafo = buscarTamanhoLista(arestas), j;

        for(j = 0; j < qtdArestasGrafo; j++){
            aresta = buscarElementoLista(arestas, j);
            destino = getToNode(g, aresta);
            char *idDestino = getNodeName(g, destino);

            if(verificarElementoLista(tabelaVertices, idDestino)){
                includeEdgeSDG(g, nomeSubgrafo, aresta);
            }
        }

        desalocarLista(arestas);

    }

    desalocarLista(verticesNaRegiao);
    desalocarTabela(tabelaVertices);
    
}

////////////////////////////////////////////////////////////////////////////////////////

static double calcularDistancia(Graph g, Node a, Node b){

    DadosVertice *dadosVertice1 = (DadosVertice*)getNodeInfo(g, a);
    DadosVertice *dadosVertice2 = (DadosVertice*)getNodeInfo(g, b);

    double x = dadosVertice1->x - dadosVertice2->x;
    double y = dadosVertice1->y- dadosVertice2->y;

    return sqrt((x * x) + (y * y));

}

static bool relaxarAresta(Graph g, Node verticeAtual, Node verticeVizinho, Edge aresta, TabelaGenerica custo, TabelaGenerica pai, FilaPrioridadeGenerica fila, Node destino){

    char *idAtual = getNodeName(g, verticeAtual);
    char *idVizinho = getNodeName(g, verticeVizinho);

    double custoAtual = (double)(intptr_t)buscarElementoTabela(custo, idAtual);
    DadosAresta *dadosAresta = (DadosAresta*)getEdgeInfo(g, aresta);
    double novoCusto = custoAtual + dadosAresta->cmp;

    void *custoExistente = buscarElementoTabela(custo, idVizinho);
    if(custoExistente == NULL || novoCusto < (double)(intptr_t)custoExistente){
        double distancia = calcularDistancia(g, verticeVizinho, destino);
        double f = novoCusto + distancia;

        inserirFilaPrioridade(fila, (void*)(intptr_t)verticeVizinho, f);
        inserirElementoTabela(custo, idVizinho, (void *)(intptr_t)novoCusto);
        inserirElementoTabela(pai, idVizinho, idAtual);

        return true;

    }

    return false;

}

static Lista dijkstraParaguaio(Graph g, char *idOrigem, char *idDestino, char *nomeSubgrafo){

    Node origem = getNode(g, idOrigem);
    Node destino = getNode(g, idDestino);

    if (origem < 0 || destino < 0) {
        printf("[DEBUG] Origem ou destino inválido(s): origem=%d, destino=%d\n", origem, destino);
        return NULL;
    }

    FilaPrioridadeGenerica fila = criarFilaPrioridade();
    TabelaGenerica custo = criarTabela(500), pai = criarTabela(500), visitado = criarTabela(500);

    inserirFilaPrioridade(fila, (void*)(intptr_t)origem, calcularDistancia(g, origem, destino));
    inserirElementoTabela(custo, idOrigem, (void*)(intptr_t)0);

    while(!filaPrioridadeVazia(fila)){
        Node atual = (Node)(intptr_t)removerFilaPrioridade(fila);

        if (atual < 0 || atual >= getTotalNodes(g)) {
            printf("[DEBUG] Valor de 'atual' fora dos limites: %d\n", atual);
            continue; // ou return NULL;
        }

        char *idAtual = getNodeName(g, atual);

        if(buscarElementoTabela(visitado, idAtual)) continue;

        inserirElementoTabela(visitado, idAtual, (void *)1);

        if(atual == destino) break;

        Lista adjacentes = inicializarLista();

        if(strcmp(nomeSubgrafo, "-") == 0){
            adjacentEdges(g, atual, adjacentes);
        }else{
            adjacentEdgesSDG(g, nomeSubgrafo, atual, adjacentes);
        }

        int i;

        for(i = 0; i < buscarTamanhoLista(adjacentes); i++){
            Edge aresta = buscarElementoLista(adjacentes, i);

            if (!aresta) {
                printf("[DEBUG] Aresta nula na posição %d\n", i);
                continue;
            }

            if ((intptr_t)aresta < 1000) {
                printf("[DEBUG] Ponteiro de aresta suspeito: %p\n", aresta);
            }

            Node vizinho = getToNode(g, aresta);
            char *idvizinho = getNodeName(g, vizinho);

            if(!verificarElementoLista(visitado, idvizinho)){
                relaxarAresta(g, atual, vizinho, aresta, custo, pai, fila, destino);
            }
        }

        desalocarLista(adjacentes);
    }

    if(!buscarElementoTabela(pai, idDestino)){
        desalocarFilaPrioridade(fila);
        desalocarTabela(custo);
        desalocarTabela(pai);
        desalocarTabela(visitado);
        return NULL;
    }

    Lista caminho = inicializarLista();

    char *atual = getNodeName(g, destino);
    while(atual != NULL){
        inserirInicioLista(caminho, atual);
        atual = buscarElementoTabela(pai, atual);
    }
    
    desalocarFilaPrioridade(fila);
    desalocarTabela(custo);
    desalocarTabela(pai);
    desalocarTabela(visitado);

    return caminho;

}

static void processarPInterrogacao(Graph g, char *linha, FILE *arquivoTxt, FILE *arquivoSvg, TabelaGenerica tabelaCaminhos){

    char nomePercurso[32], nomeSubgrafo[32], reg1[32], reg2[32];

    if(sscanf(linha, "p? %s %s %s %s", nomePercurso, nomeSubgrafo, reg1, reg2) != 4){
        printf("Erro: Falha no comando p?: %s\n", linha);
        return;
    }

    Node origem = getNode(g, reg1);
    Node destino = getNode(g, reg2);

    fprintf(arquivoTxt, "[*] p? %s %s %s %s\n", nomePercurso, nomeSubgrafo, reg1, reg2);

    Lista caminho = dijkstraParaguaio(g, reg1, reg2, nomeSubgrafo);

    if (caminho == NULL) {
        fprintf(arquivoTxt, " Trajeto inacessivel a partir da origem!\n");
        return;
    }

    if (buscarTamanhoLista(caminho) == 0) {
        fprintf(arquivoTxt, " Caminho retornado está vazio!\n");
        desalocarLista(caminho);
        return;
    }

    fprintf(arquivoTxt, " Caminho mais curto: ");
    for(int i = 0; i < buscarTamanhoLista(caminho); i++){
        fprintf(arquivoTxt, "%s", (char*)buscarElementoLista(caminho, i));
        if(i < buscarTamanhoLista(caminho) - 1){
            fprintf(arquivoTxt, "->");
        }
    }
    fprintf(arquivoTxt, "\n");

    inserirElementoTabela(tabelaCaminhos, nomePercurso, caminho);

    // Desenhar caminho no SVG com setas
    for (int i = 0; i < buscarTamanhoLista(caminho) - 1; i++) {
        char *idA = (char *)buscarElementoLista(caminho, i);
        char *idB = (char *)buscarElementoLista(caminho, i + 1);

        Node a = getNode(g, idA);
        Node b = getNode(g, idB);

        DadosVertice *da = (DadosVertice *)getNodeInfo(g, a);
        DadosVertice *db = (DadosVertice *)getNodeInfo(g, b);

        if (da && db) {
            fprintf(arquivoSvg,
                        "<defs>\n"
                        "  <marker id='arrow' viewBox='0 0 10 10' refX='10' refY='5'\n"
                        "          markerWidth='6' markerHeight='6' orient='auto-start-reverse'>\n"
                        "    <path d='M 0 0 L 10 5 L 0 10 z' fill='red' />\n"
                        "  </marker>\n"
                        "</defs>\n"
                    );
        }
    }

}



////////////////////////////////////////////////////////////////////////////////////////

static void processarJoin(Graph g, char *linha, FILE *arquivoTxt, TabelaGenerica caminho){

    char nomeFinal[32], nome1[32], nome2[32];
    if(sscanf(linha, "join %s %s %s", nomeFinal, nome1, nome2) != 3){
        printf("Erro: Comando join invalido: %s\n", linha);
        return;
    }

    Lista caminho1 = buscarElementoTabela(caminho, nome1);
    Lista caminho2 = buscarElementoTabela(caminho, nome2);

    if(!caminho1 || !caminho2){
        fprintf(arquivoTxt, "[*] join %s %s %s\n Caminhos %s ou %s inexistentes.\n", nomeFinal , nome1, nome2, nome1, nome2);
        return;
    }

    char *destino = buscarElementoLista(caminho1, buscarTamanhoLista(caminho1) - 1);
    char *origem = buscarElementoLista(caminho2, 0);

    Lista intermediario = dijkstraParaguaio(g, destino, origem, "-");

    fprintf(arquivoTxt, "[*] join %s %s %s\n", nomeFinal, nome1, nome2);

    if(intermediario == NULL || buscarTamanhoLista(intermediario) == 0){
        fprintf(arquivoTxt, " Caminho intermediario entre %s e %s nao encontrado!\n", destino, origem);
        return;
    }

    Lista caminhoFinal = inicializarLista();

    int i;

    for(i = 0; i < buscarTamanhoLista(caminho1) - 1; i++){
        inserirFimLista(caminhoFinal, buscarElementoLista(caminho1, i));
    }

    for(i = 0; i < buscarTamanhoLista(intermediario) - 1; i++){
        inserirFimLista(caminhoFinal, buscarElementoLista(intermediario, i));
    }

    for(i = 0; i < buscarTamanhoLista(caminho2); i++){
        inserirFimLista(caminhoFinal, buscarElementoLista(caminho2, i));
    }

    inserirElementoTabela(caminho, nomeFinal, caminhoFinal);

    fprintf(arquivoTxt, " Caminho combinado com sucesso.\n");

}

////////////////////////////////////////////////////////////////////////////////////////

static void processarShw(Graph g, char *linha, FILE *arquivoSvg, TabelaGenerica caminho){

    char nomePercurso[32], camCurto[16], camRapido[16];

    if(sscanf(linha, "shw %s %s %s", nomePercurso, camCurto, camRapido) != 3){
        printf("Erro: Linha invalida no comando shw: %s\n", linha);
        return;
    }

    Lista listacaminho = buscarElementoTabela(caminho, nomePercurso);

    if(!listacaminho || buscarTamanhoLista(listacaminho) < 2){
        fprintf(arquivoSvg, "<!-- Caminho '%s' inacessível ou inválido -->\n", nomePercurso);

        if(listacaminho && buscarTamanhoLista(listacaminho) == 1){
            // Só tem origem, desenhar linha até destino inválido (repetido por segurança)
            char *inicio = (char*)buscarElementoLista(listacaminho, 0);
            DadosVertice *dadosInicio = (DadosVertice*)getNodeInfo(g, getNode(g, inicio));
            fprintf(arquivoSvg,
                "<line x1='%.2lf' y1='%.2lf' x2='%.2lf' y2='%.2lf' "
                "stroke='red' stroke-width='2' stroke-dasharray='5,5' />\n",
                dadosInicio->x, dadosInicio->y,
                dadosInicio->x + 20, dadosInicio->y + 20 // arbitrário
            );
        }else if(listacaminho && buscarTamanhoLista(listacaminho) >= 2) {
            char *inicio = (char*)buscarElementoLista(listacaminho, 0);
            char *fim = (char*)buscarElementoLista(listacaminho, buscarTamanhoLista(listacaminho) - 1);

            DadosVertice *dadosInicio = (DadosVertice*)getNodeInfo(g, getNode(g, inicio));
            DadosVertice *dadosFim = (DadosVertice*)getNodeInfo(g, getNode(g, fim));

            fprintf(arquivoSvg,
                "<line x1='%.2lf' y1='%.2lf' x2='%.2lf' y2='%.2lf' "
                "stroke='red' stroke-width='2' stroke-dasharray='5,5' />\n",
                dadosInicio->x, dadosInicio->y,
                dadosFim->x, dadosFim->y
            );
        }

        return;
    }

    fprintf(arquivoSvg,  "<polyline fill='none' stroke='%s' stroke-width='3' stroke-opacity='0.6' points='", camCurto);
    int i;
    for(i = 0; i < buscarTamanhoLista(listacaminho); i++){
        char *id = (char*)buscarElementoLista(listacaminho, i);
        Node vertice = getNode(g, id);
        DadosVertice *dadosVertice = (DadosVertice*)getNodeInfo(g, vertice);
        fprintf(arquivoSvg, "%.2lf,%.2lf ", dadosVertice->x, dadosVertice->y);
    }
    fprintf(arquivoSvg, "' />\n");

    fprintf(arquivoSvg, "<polyline fill='none' stroke='%s' stroke-width='1.5' stroke-opacity='0.6' points='", camRapido);
    for(i = 0; i < buscarTamanhoLista(listacaminho); i++){
        char *id = (char*)buscarElementoLista(listacaminho, i);
        Node vertice = getNode(g, id);
        DadosVertice *dadosVertice = (DadosVertice*)getNodeInfo(g, vertice);
        fprintf(arquivoSvg, "%.2lf,%.2lf ", dadosVertice->x, dadosVertice->y);
    }
    fprintf(arquivoSvg, "' />\n");

    char *inicio = (char*)buscarElementoLista(listacaminho, 0);
    char *fim = (char*)buscarElementoLista(listacaminho, buscarTamanhoLista(listacaminho) - 1);
    DadosVertice *dadosVerticeInicio = (DadosVertice*)getNodeInfo(g, getNode(g, inicio));
    DadosVertice *dadosVerticeFim = (DadosVertice*)getNodeInfo(g, getNode(g, fim));

     fprintf(arquivoSvg, "<circle cx='%.2lf' cy='%.2lf' r='5' fill='green' stroke='black' stroke-width='1'/>\n", dadosVerticeInicio->x, dadosVerticeInicio->y);

    fprintf(arquivoSvg, "<rect x='%.2lf' y='%.2lf' width='8' height='8' fill='red' stroke='black' stroke-width='1' transform='translate(-4 -4)'/>\n", dadosVerticeFim->x, dadosVerticeFim->y);

}

////////////////////////////////////////////////////////////////////////////////////////

static void processarQry(TabelaGenerica t, Graph g, SmuTreap tr, char *caminhoQry, FILE *arquivoSvg, FILE *arquivoTxt){

    FILE *arquivoQry = fopen(caminhoQry, "r");

    if(arquivoQry == NULL){
        printf("Erro: Falha ao abrir o arquivo .qry!\n");
        return;
    }

    char linha [512];
    Posicao registradores[10];

    while(fgets(linha, sizeof(linha), arquivoQry)){
        if(linha[0] == '#' || linha[0] == '\n'){
            continue;
        }

        if(strncmp(linha, "@o?", 3) == 0){
            processarArrobaOInterrogacao(t, linha, arquivoSvg, arquivoTxt, registradores);
        }else if(strncmp(linha, "alag", 4) == 0){
            processarAlag(g, tr, linha, arquivoSvg, arquivoTxt);
        }else if(strncmp(linha, "dren", 4) == 0){
            processarDren(g, linha, arquivoTxt);
        }else if(strncmp(linha, "sg", 2) == 0){
            processarSg(g, tr, linha, arquivoSvg);
        }else if(strncmp(linha, "p?", 2) == 0){
            processarPInterrogacao(g, linha, arquivoTxt, arquivoSvg, t);
        }else if(strncmp(linha, "join", 4) == 0){
            processarJoin(g, linha, arquivoTxt, t);
        }else if(strncmp(linha, "shw", 3) == 0){
            processarShw(g, linha, arquivoSvg, t);
        }
    }
    
    fclose(arquivoQry);

}

////////////////////////////////////////////////////////////////////////////////////////

void processarArquivos(char *caminhoGeo, char *caminhoVia, char *caminhoQry, char *caminhoSvg, char *caminhoTxt){

    FILE *arquivoSvg = fopen(caminhoSvg, "w");

    if(arquivoSvg == NULL){
        printf("Erro: Falha na abertura do arquivo %s!\n", caminhoGeo);
        return;
    }

    FILE *arquivoTxt = fopen(caminhoTxt, "w");

    printf("[DEBUG] Caminho recebido: '%s'\n", caminhoTxt);

    if(arquivoTxt == NULL){
        printf("Erro: Falha na abertura do arquivo %s!\n", caminhoTxt);
        fclose(arquivoSvg);
        return;
    }

    tagCabecalho(arquivoSvg);

    TabelaGenerica quadras = processarGeo(caminhoGeo, arquivoSvg);

    Graph grafo = NULL;
    SmuTreap arvore = NULL;

    if(caminhoVia != NULL){
        grafo = processarVia(caminhoVia, arquivoSvg, &arvore);

        if(grafo == NULL){
            printf("Erro: Falha ao gerar o grafo com o arquivo .via!\n");
            fclose(arquivoSvg);
            fclose(arquivoTxt);
            return;
        }
    }

    if(caminhoQry != NULL && grafo != NULL){
        processarQry(quadras, grafo, arvore, caminhoQry, arquivoSvg, arquivoTxt);
    }

    tagRodape(arquivoSvg);

    fclose(arquivoSvg);
    fclose(arquivoTxt);
}