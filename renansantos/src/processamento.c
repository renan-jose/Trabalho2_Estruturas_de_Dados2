// Importação das bibliotecas
#include "processamento.h"
#include "tag.h"
#include "lista.h"
#include "arquivo.h"
#include "espalhamento.h"
#include "graph.h"
#include "smutreap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    char nomeSubgrafo[32];
    sprintf(nomeSubgrafo, "alag%d", n);
    createSubgraphDG(g, nomeSubgrafo, NULL, 0, false);

    Lista verticesNaRegiao = inicializarLista();
    getNodisDentroRegiaoSmuT(t, x, y, x + w, y + h, verticesNaRegiao);

    int qtdVerticesGrafo = buscarTamanhoLista(verticesNaRegiao), i;

    for(i = 0; i < qtdVerticesGrafo; i++){
        Nodi no = (Nodi)buscarElementoLista(verticesNaRegiao, i);
        DadosVertice *dadosVertice = (DadosVertice*)getInfoSmuT(t, no);
        Node vertice = getNode(g, dadosVertice->id);

        Lista arestas = inicializarLista();
        adjacentEdges(g, vertice, arestas);

        int qtdArestasGrafo = buscarTamanhoLista(arestas);

        for(int j = 0; j < qtdArestasGrafo; j++){
            Edge aresta = (Edge)buscarElementoLista(arestas, j);

            desativarAresta(g, aresta);
            includeEdgeSDG(g, nomeSubgrafo, aresta);

            DadosAresta *dadosAresta = (DadosAresta*)getEdgeInfo(g, aresta);
            fprintf(arquivoTxt, "[*] alag %d %lf %lf %lf %lf\n", n, x, y, w, h);
            fprintf(arquivoTxt, " Aresta desativada: %s -> %s\nRua: %s\nVm: %.2lf\nCmp: %.2lf", getNodeName(g, getFromNode(g, aresta)), getNodeName(g, getToNode(g, aresta)), 
                    dadosAresta->nome, dadosAresta->vm, dadosAresta->cmp);

        }

        desalocarLista(arestas);
    }

    desalocarLista(verticesNaRegiao);

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
            processarArrobaOInterrogacao(g, linha, arquivoSvg, arquivoTxt, registradores);
        }else if(strncmp(linha, "alag", 4) == 0){
            processarAlag(g, tr, linha, arquivoSvg, arquivoTxt);
        }else if(strncmp(linha, "dren", 4) == 0){

        }else if(strncmp(linha, "sg", 2) == 0){

        }else if(strncmp(linha, "p?", 2) == 0){

        }else if(strncmp(linha, "join", 4) == 0){

        }else if(strncmp(linha, "shw", 3) == 0){

        }
    }
    
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