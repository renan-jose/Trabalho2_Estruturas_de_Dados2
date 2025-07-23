// Importação das bibliotecas
#include "processamento.h"
#include "tag.h"
#include "lista.h"
#include "arquivo.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////////////

static void processarGeo(char *caminhoGeo, FILE *arquivoSvg){

    FILE *arquivoGeo = fopen(caminhoGeo, "r");

    if(arquivoGeo == NULL){
        printf("Erro: Nao foi possivel abrir o arquivo .geo: %s!\n", caminhoGeo);
        return;
    }

    char linha[512];
    double x, y, w, h;
    char cep[32], corb[32], corp[32], expb[32];

    while(fgets(linha, sizeof(linha), arquivoGeo)){
        if(linha[0] == '#' || linha[0] == '\n'){
            continue;
        }
       
        if(strncmp(linha, "cq", 2) == 0){
            sscanf(linha, "cq %s %s %s", corp, corb, expb);
        }else if(strncmp(linha, "q", 1) == 0){
            if(sscanf(linha, "q %s %lf %lf %lf %lf", cep, &x, &y, &w, &h) == 5){
                FormaGeometricaGenerica retangulo = formularRetangulo(cep, x, y, w, h, "blue", "yellow", "1px");
                tagRetangulo(arquivoSvg, retangulo);
            }else{
                printf("Erro: Linha de texto invalida: %s\n", linha);
            }
        }
    }

    fclose(arquivoGeo);

}

////////////////////////////////////////////////////////////////////////////////////////

struct DadosVertice{

    char *id;
    double x, y;

};

struct DadosAresta{

    char *ldir, *lesq, *nome;
    double cmp, vm;

};

typedef struct DadosVertice DadosVertice;
typedef struct DadosAresta DadosAresta;

static Graph processarVia(char *caminhoVia){

    FILE *arquivoVia = fopen(caminhoVia, "r");

    if(arquivoVia == NULL){
        printf("Erro: Nao foi possivel abrir o arquivo .via: %s!\n", caminhoVia);
        return NULL;
    }

    char linha[1024], lesq[50], ldir[50], nome[50], id[50];
    DadosVertice dadosVertice;
    DadosAresta dadosAresta;
    int limiteVertices, origem, destino;
    Graph grafo = NULL;

    while(fgets(linha, sizeof(linha), arquivoVia)){
        if(linha[0] == '#' || linha[0] == '\n'){
            continue;
        }

        if(strncmp(linha, "nv", 2) == 0){
            if(sscanf(linha, "nv %d", &limiteVertices) == 1){
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

                dadosVertice.id = (char*)malloc(strlen(id) + 1);

                if(dadosVertice.id == NULL){
                    printf("Erro: Falha na alocaco de memoria na leitura dos dados do vertice do grafo do arquivo .via!\n");
                    return NULL;
                }
                strcpy(dadosVertice.id, id);

                addNode(grafo, dadosVertice.id, &dadosVertice);

                free(dadosVertice.id);
            }else{
                printf("Erro: Linha de texto invalida: %s\n", linha);
            }
        }else if(strncmp(linha, "e", 1) == 0){

            if(grafo == NULL){
                printf("Erro: Grafo ainda nao criado antes de adicionar vertices!\n");
                return NULL;
            }

            if(sscanf(linha, "e %d %d %s %s %lf %lf %s", &origem, &destino, ldir, lesq, &dadosAresta.cmp, &dadosAresta.vm, nome) == 7){

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

                addEdge(grafo, origem, destino, &dadosAresta);

                free(dadosAresta.ldir);
                free(dadosAresta.lesq);
                free(dadosAresta.nome);
            }else{
                printf("Erro: Linha de texto invalida: %s\n", linha);
            }
        }
    }

    fclose(arquivoVia);

    return grafo;

}

////////////////////////////////////////////////////////////////////////////////////////

static void processarQry(Graph g, char *caminhoQry, FILE *arquivoSvg, FILE *arquivoTxt){

    FILE *arquivoQry = fopen(caminhoQry, "r");

    if(arquivoQry == NULL){
        printf("Erro: Falha ao abrir o arquivo .qry!\n", caminhoQry);
        return;
    }

    char linha [512];


    
}

////////////////////////////////////////////////////////////////////////////////////////

void processarArquivos(char *caminhoGeo, char *caminhoVia, char *caminhoQry, char *caminhoSvg, char *caminhoTxt){

    FILE *arquivoSvg = fopen(caminhoSvg, "w");

    if(arquivoSvg == NULL){
        printf("Erro: Falha na abertura do arquivo %s!\n", caminhoGeo);
        return;
    }

    FILE *arquivoTxt = fopen(caminhoTxt, "w");

    if(arquivoTxt == NULL){
        printf("Erro: Falha a abertura do arquivo %s!\n", caminhoGeo);
        fclose(arquivoSvg);
        return;
    }

    tagCabecalho(arquivoSvg);

    processarGeo(caminhoGeo, arquivoSvg);

    Graph grafo = processarVia(caminhoVia);

    if(grafo == NULL){
        printf("Erro: Falha ao gerar o grafo com o arquivo .via!\n");
        fclose(arquivoSvg);
        fclose(arquivoTxt);
        return;
    }

    processarVia(caminhoVia);

    tagRodape(arquivoSvg);

    fclose(arquivoSvg);
    fclose(arquivoTxt);
}