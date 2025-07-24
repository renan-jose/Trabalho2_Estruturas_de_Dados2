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

FormaGeometricaGenerica formularRetangulo(char *id, double x, double y, double w, double h, char *corb, char *corp, char *expb){

    FormaGeometrica *novaForma = (FormaGeometrica*)malloc(sizeof(FormaGeometrica));

    if(novaForma == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao da forma do retangulo!\n");
        return NULL;
    }

    novaForma->forma = criarRetangulo(id, x, y, w, h, corb, corp, expb);

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
/*
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

}*/

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

char *buscarIdFormaGeometrica(FormaGeometricaGenerica f){
    FormaGeometrica *forma = (FormaGeometrica*)f;

    if (forma == NULL || forma->forma == NULL) return NULL;

    switch (forma->tipo) {
        case 'r':
            return buscarIdRetangulo(forma->forma);

        default:
            fprintf(stderr, "Erro: tipo de forma inválido em buscarIdFormaGeometrica: %c\n", forma->tipo);
            return NULL;
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
    }
    
    return 0;

}

/*****************************************************************************************************/

double buscarAlturaFormaGeometrica(FormaGeometricaGenerica f){

    FormaGeometrica *forma = (FormaGeometrica*)f;
    char tipo = buscarTipoFormaGeometrica(forma);

    if (tipo == 'r'){
        return buscarAlturaRetangulo(buscarFormaGeometrica(forma));
    }

    return 0;

}

/*****************************************************************************************************/

int tipoFormaParaDescritor(char tipo){

    switch(tipo){
        case 'r': 
            return 1;

        default: 
            return -1; 
    }

}

/*****************************************************************************************************/

char *buscarCorBordaFormaGeometrica(FormaGeometricaGenerica f){

    FormaGeometrica *forma = (FormaGeometrica*)f;

    switch(forma->tipo){
        case 'r': 

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

        default: 
            return "none";
    }

}

/*****************************************************************************************************/

char *buscarExpessuraBordaFormaGeometrica(FormaGeometricaGenerica f){

    FormaGeometrica *forma = (FormaGeometrica*)f;

    switch(forma->tipo){
        case 'r':
            return buscarExpessuraBordaRetangulo(forma);

        default:
            return "none";
    }

}

/*****************************************************************************************************/

void mudarExpessuraBordaFormaGeometrica(FormaGeometricaGenerica f, char *expb){

    FormaGeometrica *forma = (FormaGeometrica*)f;

    switch(forma->tipo){
        case 'r':
            mudarExpessuraBordaRetangulo(forma, expb);
            break;
        default:
            break;
    }

}