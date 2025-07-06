/* Importação das bibliotecas para a implementação do processamento do arquivo .geo. */
#include "armazenamento.h"
#include "forma.h"
#include "smutreap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************************************/

static void armazenarRetangulo(ArvoreGenerica t, char *linha){

    int id;
    double x, y, w, h;
    char corb[32], corp[32];

    if(sscanf(linha, "r %d %lf %lf %lf %lf %s %s", &id, &x, &y, &w, &h, corb, corp) == 7){
        FormaGeometricaGenerica retangulo = formularRetangulo(id, x, y, w, h, corb, corp);
        insertSmuT(t, x, y, retangulo, tipoFormaParaDescritor('r'), calcularBbFormaGeometrica);
    }else{
        printf("Erro: Linha de texto invalida: %s\n", linha);
    }

}

/*****************************************************************************************************/

static void armazenarCirculo(ArvoreGenerica t, char *linha){

    int id;
    double x, y, r;
    char corb[32], corp[32];

    if(sscanf(linha, "c %d %lf %lf %lf %s %s", &id, &x, &y, &r, corb, corp) == 6){
        FormaGeometricaGenerica circulo = formularCirculo(id, x, y, r, corb, corp);
        insertSmuT(t, x, y, circulo, tipoFormaParaDescritor('c'), calcularBbFormaGeometrica);
    }else{
        printf("Erro: Linha de texto invalida: %s\n", linha);
    }

}

/*****************************************************************************************************/

static void armazenarLinha(ArvoreGenerica t, char *linha){
    
    int id;
    double x1, y1, x2, y2;
    char cor[32];

    if(sscanf(linha, "l %d %lf %lf %lf %lf %s", &id, &x1, &y1, &x2, &y2, cor) == 6){
        FormaGeometricaGenerica linha = formularLinha(id, x1, y1, x2, y2, cor);
        insertSmuT(t, x1, y1, linha, tipoFormaParaDescritor('l'), calcularBbFormaGeometrica);
    }else{
        printf("Erro: Linha de texto invalida: %s\n", linha);
    }

}

/*****************************************************************************************************/

static void armazenarTexto(ArvoreGenerica t, char *linha, char *fonte, char *peso, char *tamanho) {

    int id;
    double x, y;
    char corb[32], corp[32], txt[256], ancora;

    if(sscanf(linha, "t %d %lf %lf %s %s %c \"%1023[^\"]\"", &id, &x, &y, corb, corp, &ancora, txt) == 7){
        FormaGeometricaGenerica texto = formularTexto(id, x, y, corb, corp, ancora, txt, fonte, peso, tamanho);
        insertSmuT(t, x, y, texto, tipoFormaParaDescritor('t'), calcularBbFormaGeometrica);
    }else{
        printf("Erro: Linha de texto invalida: %s\n", linha);
    }

}

/*****************************************************************************************************/

static void armazenarEstiloTexto(char *linha, char *fonte, char *peso, char *tamanho){

    if(sscanf(linha, "ts %s %s %s", fonte, peso, tamanho) != 3){
        printf("Erro: Linha de texto invalida: %s\n", linha);
    }

}

/*****************************************************************************************************/

void armazenarFormas(ArvoreGenerica t, char *nome){

    FILE *arquivoGeo = fopen(nome, "r");

    if(arquivoGeo == NULL){
        printf("Erro: Nao foi possível abrir o arquivo .geo: %s\n", nome);
        return;
    }

    char fonte[64] = "sans", peso[16] = "n", tamanho[16] = "12px", linha[512];

    while(fgets(linha, sizeof(linha), arquivoGeo)){
        if(linha[0] == '#' || linha[0] == '\n'){
            continue;
        }

        if(strncmp(linha, "ts", 2) == 0){
            armazenarEstiloTexto(linha, fonte, peso, tamanho);
        }else if(linha[0] == 'r'){
            armazenarRetangulo(t, linha);
        }else if (linha[0] == 'c'){
            armazenarCirculo(t, linha);
        }else if(linha[0] == 'l'){
            armazenarLinha(t, linha);
        }else if(linha[0] == 't' && strncmp(linha, "ts", 2) != 0){
            armazenarTexto(t, linha, fonte, peso, tamanho);
        }
    }

    fclose(arquivoGeo);

}

/*****************************************************************************************************/