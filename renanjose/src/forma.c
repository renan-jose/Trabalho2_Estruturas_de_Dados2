/* Importação das bibliotecas para a implementação da forma geométrica. */
#include "forma.h"
#include <stdio.h>
#include <stdlib.h>

/*****************************************************************************************************/

struct FormaGeometrica{

    FormaGeometricaGenerica forma;
    char tipo;
    double x, y;

};

typedef struct FormaGeometrica FormaGeometrica;

/*****************************************************************************************************/

FormaGeometricaGenerica formularRetangulo(int id, double x, double y, double w, double h, char *corb, char *corp){

    FormaGeometrica *novaForma = (FormaGeometrica*)malloc(sizeof(FormaGeometrica));

    if(novaForma == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao da forma do retangulo!\n");
        return NULL;
    }

    novaForma->forma = criarRetangulo(id, x, y, w, h, corb, corp);

    if (novaForma->forma == NULL) {
        free(novaForma);
        return NULL;
    }
    
    novaForma->tipo = 'r';
    novaForma->x = x;
    novaForma->y = y;

    return novaForma;

}

/*****************************************************************************************************/

FormaGeometricaGenerica formularCirculo(int id, double x, double y, double r, char *corb, char *corp){

    FormaGeometrica *novaForma = (FormaGeometrica*)malloc(sizeof(FormaGeometrica));

    if(novaForma == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao da forma do circulo!\n");
        return NULL;
    }

    novaForma->forma = criarCirculo(id, x, y, r, corb, corp);
    novaForma->tipo = 'c';
    novaForma->x = x;
    novaForma->y = y;

    return novaForma;
    
}

/*****************************************************************************************************/

FormaGeometricaGenerica formularLinha(int id, double x1, double y1, double x2, double y2, char *c){

    FormaGeometrica *novaForma = (FormaGeometrica*)malloc(sizeof(FormaGeometrica));

    if(novaForma == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao da forma da linha!\n");
        return NULL;
    }

    novaForma->forma = criarLinha(id, x1, y1, x2, y2, c);
    novaForma->tipo = 'l';
    novaForma->x = x1;
    novaForma->y = y1;

    return novaForma;

}

/*****************************************************************************************************/

FormaGeometricaGenerica formularTexto(int id, double x, double y, char *corb, char *corp, char p, char *t, char *fonte, char *peso, char *tamanho){

    FormaGeometrica *novaForma = (FormaGeometrica*)malloc(sizeof(FormaGeometrica));

    if(novaForma == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao da forma do texto!\n");
        return NULL;
    }
    
    novaForma->forma = criarTexto(id, x, y, corb, corp, p, t, fonte, peso, tamanho);
    novaForma->tipo = 't';
    novaForma->x = x;
    novaForma->y = y;

    return novaForma;

}

/*****************************************************************************************************/

FormaGeometricaGenerica buscarFormaGeometrica(FormaGeometricaGenerica f){

    FormaGeometrica *forma = (FormaGeometrica*)f;

    return forma->forma;

}

/*****************************************************************************************************/

void mudarTipoFormaGeometrica(FormaGeometricaGenerica f, char t){

    FormaGeometrica *forma = (FormaGeometrica*)f;

    forma->tipo = t;

}

/*****************************************************************************************************/

char buscarTipoFormaGeometrica(FormaGeometricaGenerica f){

    FormaGeometrica *forma = (FormaGeometrica*)f;

    return forma->tipo;

}

/*****************************************************************************************************/

int buscarIdFormaGeometrica(FormaGeometricaGenerica f){
    FormaGeometrica *forma = (FormaGeometrica*)f;

    if (forma == NULL || forma->forma == NULL) return -1;

    switch (forma->tipo) {
        case 'c':
            return buscarIdCirculo(forma->forma);

        case 'r':
            return buscarIdRetangulo(forma->forma);

        case 'l':
            return buscarIdLinha(forma->forma);

        case 't':
            return buscarIdTexto(forma->forma);

        default:
            fprintf(stderr, "Erro: tipo de forma inválido em buscarIdFormaGeometrica: %c\n", forma->tipo);
            return -1;
    }

}

/*****************************************************************************************************/

double buscarCoordXFormaGeometrica(FormaGeometricaGenerica f){

    FormaGeometrica *forma = (FormaGeometrica*)f;

    return forma->x;

}

/*****************************************************************************************************/

void mudarCoordXFormaGeometrica(FormaGeometricaGenerica f, double x){

    FormaGeometrica *forma = (FormaGeometrica*)f;

    forma->x = x;

}

/*****************************************************************************************************/

double buscarCoordYFormaGeometrica(FormaGeometricaGenerica f){

    FormaGeometrica *forma = (FormaGeometrica*)f;

    return forma->y;

}

/*****************************************************************************************************/

void mudarCoordYFormaGeometrica(FormaGeometricaGenerica f, double y){

    FormaGeometrica *forma = (FormaGeometrica*)f;

    forma->y = y;

}

/*****************************************************************************************************/

void calcularBbFormaGeometrica(FormaGeometricaGenerica f, double *x, double *y, double *w, double *h){
    
    FormaGeometrica *forma = (FormaGeometrica*)f;

    switch(forma->tipo){
        case 'r':
            calcularBbRetangulo(forma->forma, x, y, w, h);
            break;

        case 'c':
            calcularBbCirculo(forma->forma, x, y, w, h);
            break;

        case 'l':
            calcularBbLinha(forma->forma, x, y, w, h);
            break;

        case 't':
            calcularBbTexto(forma->forma, x, y, w, h);
            break;

        default:
            *x = *y = *w = *h = 0;
            break;

    }

}

/*****************************************************************************************************/

void desalocarFormaGeometrica(FormaGeometricaGenerica f){

    FormaGeometrica *forma = (FormaGeometrica*)f;

    switch(forma->tipo){
        case 'r':
            desalocarRetangulo(forma->forma);
            break;

        case 'c':
            desalocarCirculo(forma->forma);
            break;

        case 'l':
            desalocarLinha(forma->forma);
            break;

        case 't':
            desalocarTexto(forma->forma);
            break;

        default:
            printf("Forma desconhecida para desalocacao!\n");
            break;
    }

    free(forma);

}

/*****************************************************************************************************/

bool formaDentroRegiao(ArvoreGenerica a, NoGenerico n, FormaGeometricaGenerica f, double x1, double y1, double x2, double y2){

    FormaGeometrica *forma = (FormaGeometrica*)f;

    switch(forma->tipo){
        case 'r':
            return retanguloDentroRegiao(a, n, forma->forma, x1, y1, x2, y2);

        case 'c':
            return circuloDentroRegiao(a, n, forma->forma, x1, y1, x2, y2);

        case 'l':
            return linhaDentroRegiao(a, n, forma->forma, x1, y1, x2, y2);

        case 't':
            return textoDentroRegiao(a, n, forma->forma, x1, y1, x2, y2);

        default:
            return false;
    }

}

/*****************************************************************************************************/

bool pontoInternoforma(ArvoreGenerica a, NoGenerico n, FormaGeometricaGenerica f, double x, double y){

    FormaGeometrica *forma = (FormaGeometrica*)f;

    switch(forma->tipo){
        case 'r':
            return pontoInternoRetangulo(a, n, forma->forma, x, y);

        case 'c':
            return pontoInternoCirculo(a, n, forma->forma, x, y);

        case 'l':
            return pontoInternoLinha(a, n, forma->forma, x, y);

        case 't':
            return pontoInternoTexto(a, n, forma->forma, x, y);

        default:
            return false;
    }

}

/*****************************************************************************************************/

double buscarLarguraFormaGeometrica(FormaGeometricaGenerica f){

    FormaGeometrica *forma = (FormaGeometrica*)f;
    char tipo = buscarTipoFormaGeometrica(forma);

    if(tipo == 'r'){
        return buscarLarguraRetangulo(forma);
    }else if(tipo == 'c'){
        return 2 * buscarRaioCirculo(forma);
    }

    return 0;

}

/*****************************************************************************************************/

double buscarAlturaFormaGeometrica(FormaGeometricaGenerica f){

    FormaGeometrica *forma = (FormaGeometrica*)f;
    char tipo = buscarTipoFormaGeometrica(forma);

    if (tipo == 'r'){
        return buscarAlturaRetangulo(buscarFormaGeometrica(forma));
    }else if (tipo == 'c'){
        return 2 * buscarRaioCirculo(buscarFormaGeometrica(forma));
    }

    return 0;

}

/*****************************************************************************************************/

int tipoFormaParaDescritor(char tipo){

    switch(tipo){
        case 'r': 
            return 1;

        case 'c': 
            return 2;

        case 'l': 
            return 3;

        case 't':
            return 4;

        default: 
            return -1; 
    }

}

/*****************************************************************************************************/

FormaGeometricaGenerica clonarForma(FormaGeometricaGenerica f, double dx, double dy, char* novaCorBorda, char* novaCorPreenchimento){

    FormaGeometrica *forma = (FormaGeometrica*)f;

    char tipo = buscarTipoFormaGeometrica(forma);
    int id = buscarIdFormaGeometrica(forma);
    double x = buscarCoordXFormaGeometrica(forma);
    double y = buscarCoordYFormaGeometrica(forma);

    void* dados = buscarFormaGeometrica(forma); // acesso genérico para o conteúdo real

    switch(tipo){
        case 'r':{
            double w = buscarLarguraRetangulo(dados);
            double h = buscarAlturaRetangulo(dados);
            return formularRetangulo(id, x + dx, y + dy, w, h, novaCorBorda, novaCorPreenchimento);
        }
        case 'c':{
            double r = buscarRaioCirculo(dados);
            return formularCirculo(id, x + dx, y + dy, r, novaCorBorda, novaCorPreenchimento);
        }
        case 'l':{
            return formularLinha(id, buscarX1Linha(dados) + dx, buscarY1Linha(dados) + dy, buscarX2Linha(dados) + dx, buscarY2Linha(dados) + dy, novaCorBorda);
        }
        case 't':{
            return formularTexto(id, x + dx, y + dy, novaCorBorda, novaCorPreenchimento, buscarPeriodoAncoraTexto(f), buscarTextoTexto(f), buscarFonteTexto(f), buscarPesoTexto(f), buscarTamanhoTexto(f));
        }
        default:
            return NULL;
    }
    
}

/*****************************************************************************************************/

char *buscarCorBordaFormaGeometrica(FormaGeometricaGenerica f){

    FormaGeometrica *forma = (FormaGeometrica*)f;

    switch(forma->tipo){
        case 'r': 
            return buscarCorBordaRetangulo(buscarFormaGeometrica(forma));

        case 'c': 
            return buscarCorBordaCirculo(buscarFormaGeometrica(forma));

        case 'l': 
            return buscarCorLinha(buscarFormaGeometrica(forma));

        case 't': 
            return buscarCorBordaTexto(buscarFormaGeometrica(forma));

        default: 
            return "none";
    }

}

/*****************************************************************************************************/

char *buscarCorPreenchimentoFormaGeometrica(FormaGeometricaGenerica f){

    FormaGeometrica *forma = (FormaGeometrica*)f;

    switch (forma->tipo) {
        case 'r': 
            return buscarCorPreenchimentoRetangulo(buscarFormaGeometrica(forma));

        case 'c': 
            return buscarCorPreenchimentoCirculo(buscarFormaGeometrica(forma));

        case 'l': 
            return buscarCorLinha(buscarFormaGeometrica(forma));

        case 't': 
            return buscarCorPreenchimentoTexto(buscarFormaGeometrica(forma));

        default: 
            return "none";
    }

}

/*****************************************************************************************************/