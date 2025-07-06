/* Importação das bibliotecas para a implementação do círculo. */
#include "circulo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*****************************************************************************************************/

struct Circulo{

    int id;
    double x, y, r;
    char *corb, *corp;

};

typedef struct Circulo Circulo;

/*****************************************************************************************************/

CirculoGenerico criarCirculo(int id, double x, double y, double r, char *corb, char *corp){

    Circulo *novoCirculo = (Circulo*)malloc(sizeof(Circulo));

    if(novoCirculo == NULL){
        printf("Erro na alocacao de memoria para a criacao do circulo!\n");
        return NULL;
    }

    novoCirculo->id = id;
    novoCirculo->x = x;
    novoCirculo->y = y;
    novoCirculo->r = r;
    
    novoCirculo->corb = (char*)malloc(strlen(corb) + 1);
    if(novoCirculo->corb == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao do circulo!\n");
        free(novoCirculo);
        return NULL;
    }
    strcpy(novoCirculo->corb, corb);

    novoCirculo->corp = (char*)malloc(strlen(corp) + 1);
    if(novoCirculo->corp == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao do circulo!\n");
        free(novoCirculo->corb);
        free(novoCirculo);
        return NULL;
    }
    strcpy(novoCirculo->corp, corp);

    return novoCirculo;

}

/*****************************************************************************************************/

int buscarIdCirculo(CirculoGenerico c){

    Circulo *circulo = (Circulo*)c;

    return circulo->id;

}

/*****************************************************************************************************/

double buscarRaioCirculo(CirculoGenerico c){

    Circulo *circulo = (Circulo*)c;

    return circulo->r;

}

/*****************************************************************************************************/

void mudarRaioCirculo(CirculoGenerico c, double r){

    Circulo *circulo = (Circulo*)c;

    circulo->r = r;

}

/*****************************************************************************************************/

double buscarCoordXCirculo(CirculoGenerico c){

    Circulo *circulo = (Circulo*)c;

    return circulo->x;

}

/*****************************************************************************************************/

double buscarCoordYCirculo(CirculoGenerico c){

    Circulo *circulo = (Circulo*)c;

    return circulo->y;

}

/*****************************************************************************************************/

char *buscarCorBordaCirculo(CirculoGenerico c) {
    Circulo *circulo = (Circulo*)c;
    if (!circulo || !circulo->corb) return "none";
    return circulo->corb;
}

/*****************************************************************************************************/

char *buscarCorPreenchimentoCirculo(CirculoGenerico c) {
    Circulo *circulo = (Circulo*)c;
    if (!circulo || !circulo->corp) return "none";
    return circulo->corp;
}

/*****************************************************************************************************/

void mudarCoordXCirculo(CirculoGenerico c, double x){

    Circulo *circulo = (Circulo*)c;

    circulo->x = x;

}

/*****************************************************************************************************/

void mudarCoordYCirculo(CirculoGenerico c, double y){

    Circulo *circulo = (Circulo*)c;

    circulo->y = y;

}

/*****************************************************************************************************/

void mudarCorBordaCirculo(CirculoGenerico c, char *corb){

    Circulo *circulo = (Circulo*)c;

    free(circulo->corb);

    circulo->corb = (char*)malloc(strlen(corb) + 1);
    if(circulo->corb == NULL){
        printf("Erro: Falha na alocacao de memoria para a nova cor de borda do circulo!\n");
        return;
    }

    strcpy(circulo->corb, corb);

}

/*****************************************************************************************************/

void mudarCorPreenchimentoCirculo(CirculoGenerico c, char *corp){

    Circulo *circulo = (Circulo*)c;

    free(circulo->corp);

    circulo->corp = (char*)malloc(strlen(corp) + 1);
    if(circulo->corp == NULL){
        printf("Erro: Falha na alocacao de memoria para a nova cor de preenchimento do circulo!\n");
        return;
    }

    strcpy(circulo->corp, corp);

}

/*****************************************************************************************************/

void calcularBbCirculo(CirculoGenerico c, double *x, double *y, double *w, double *h){

    Circulo *circulo = (Circulo*)c;

    *x = circulo->x - circulo->r;
    *y = circulo->y - circulo->r;
    *w = 2 * circulo->r;
    *h = 2 * circulo->r;
    
}

/*****************************************************************************************************/

void desalocarCirculo(CirculoGenerico c){

    Circulo *circulo = (Circulo*)c;

    if(circulo == NULL){
        return;
    }

    free(circulo->corb);
    free(circulo->corp);
    free(circulo);

}

/*****************************************************************************************************/

bool circuloDentroRegiao(ArvoreGenerica a, NoGenerico n, CirculoGenerico c, double x1, double y1, double x2, double y2) {
    Circulo *circulo = (Circulo*)c;

    double xMin = fmin(x1, x2);
    double xMax = fmax(x1, x2);
    double yMin = fmin(y1, y2);
    double yMax = fmax(y1, y2);

    if (circulo->x >= xMin && circulo->x <= xMax && circulo->y >= yMin && circulo->y <= yMax) {
        return true;
    }

    return false;

}

/*****************************************************************************************************/

bool pontoInternoCirculo(ArvoreGenerica a, NoGenerico n, CirculoGenerico c, double x, double y) {
    Circulo *circulo = (Circulo*)c;

    double dx = x - circulo->x;
    double dy = y - circulo->y;
    double distancia = (dx * dx) + (dy * dy);

    if (distancia <= (circulo->r * circulo->r)) {
        return true;
    }

    return false;
    
}

/*****************************************************************************************************/