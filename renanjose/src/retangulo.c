/* Importação das bibliotecas para a implementação do retângulo. */
#include "retangulo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*****************************************************************************************************/

struct Retangulo{
 
    int id;
    double x, y, w, h;
    char *corb, *corp;

};

typedef struct Retangulo Retangulo;

/*****************************************************************************************************/

RetanguloGenerico criarRetangulo(int id, double x, double y, double w, double h, char *corb, char *corp){

    Retangulo *novoRetangulo = (Retangulo*)malloc(sizeof(Retangulo));

    if(novoRetangulo == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao do retangulo\n");
        return NULL;
    }

    novoRetangulo->id = id;
    novoRetangulo->x = x;
    novoRetangulo->y = y;
    novoRetangulo->w = w;
    novoRetangulo->h = h;

    novoRetangulo->corb = (char*)malloc(strlen(corb) + 1);
    if(novoRetangulo->corb == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao do retangulo!\n");
        free(novoRetangulo);
        return NULL;
    }
    strcpy(novoRetangulo->corb, corb);

    novoRetangulo->corp = (char*)malloc(strlen(corp) + 1);
    if(novoRetangulo->corp == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao do retangulo!\n");
        free(novoRetangulo->corb);
        free(novoRetangulo);
        return NULL;
    }
    strcpy(novoRetangulo->corp, corp);

    return novoRetangulo;

}

/*****************************************************************************************************/

int buscarIdRetangulo(RetanguloGenerico r){

    Retangulo *retangulo = (Retangulo*)r;

    return retangulo->id;

}

/*****************************************************************************************************/

double buscarCoordXRetangulo(RetanguloGenerico r){

    Retangulo *retangulo = (Retangulo*)r;

    return retangulo->x;

}

/*****************************************************************************************************/

double buscarCoordYRetangulo(RetanguloGenerico r){

    Retangulo *retangulo = (Retangulo*)r;

    return retangulo->y;

}

/*****************************************************************************************************/

double buscarLarguraRetangulo(RetanguloGenerico r){

    Retangulo *retangulo = (Retangulo*)r;

    return retangulo->w;

}

/*****************************************************************************************************/

double buscarAlturaRetangulo(RetanguloGenerico r){

    Retangulo *retangulo = (Retangulo*)r;

    return retangulo->h;

}

/*****************************************************************************************************/

char *buscarCorBordaRetangulo(RetanguloGenerico r) {
    Retangulo *retangulo = (Retangulo*)r;
    if (!retangulo || !retangulo->corb) return "none";
    return retangulo->corb;
}


/*****************************************************************************************************/

char *buscarCorPreenchimentoRetangulo(RetanguloGenerico r) {
    Retangulo *retangulo = (Retangulo*)r;
    if (!retangulo || !retangulo->corp) return "none";
    return retangulo->corp;
}

/*****************************************************************************************************/

void mudarCoordXRetangulo(RetanguloGenerico r, double x){

    Retangulo *retangulo = (Retangulo*)r;

    retangulo->x = x;

}

/*****************************************************************************************************/

void mudarCoordYRetangulo(RetanguloGenerico r, double y){

    Retangulo *retangulo = (Retangulo*)r;

    retangulo->y = y;

}

/*****************************************************************************************************/

void mudarLarguraRetangulo(RetanguloGenerico r, double w){

    Retangulo *retangulo = (Retangulo*)r;

    retangulo->w = w;

}

/*****************************************************************************************************/

void mudarAlturaRetangulo(RetanguloGenerico r, double h){

    Retangulo *retangulo = (Retangulo*)r;

    retangulo->h = h;

}

/*****************************************************************************************************/

void mudarCorBordaRetangulo(RetanguloGenerico r, char *corb){

    Retangulo *retangulo = (Retangulo*)r;

    free(retangulo->corb);

    retangulo->corb = (char*)malloc(strlen(corb) + 1);
    if(retangulo->corb == NULL){
        printf("Erro: Falha ao alocar memoria para a nova cor da borda do retangulo!");
        return;
    }

    strcpy(retangulo->corb, corb);

}

/*****************************************************************************************************/

void mudarCorPreenchimentoRetangulo(RetanguloGenerico r, char *corp){

    Retangulo *retangulo = (Retangulo*)r;

    free(retangulo->corp);

    retangulo->corp = (char*)malloc(strlen(corp) + 1);
    if(retangulo->corp == NULL){
        printf("Erro: Falha ao alocar memoria para a nova cor de preenchimento do retangulo!");
        return;
    }

    strcpy(retangulo->corp, corp);
}

/*****************************************************************************************************/

void calcularBbRetangulo(RetanguloGenerico r, double *x, double *y, double *w, double *h){

    Retangulo *retangulo = (Retangulo*)r;

    *x = retangulo->x;
    *y = retangulo->y;
    *w = retangulo->w;
    *h = retangulo->h;

}

/*****************************************************************************************************/

void desalocarRetangulo(RetanguloGenerico r){

    Retangulo *retangulo = (Retangulo*)r;
    
    if(retangulo == NULL){
        return;
    }

    free(retangulo->corb);
    free(retangulo->corp);
    free(retangulo);

}

/*****************************************************************************************************/

bool retanguloDentroRegiao(ArvoreGenerica a, NoGenerico n, RetanguloGenerico r, double x1, double y1, double x2, double y2){

    Retangulo *retangulo = (Retangulo*)r;

    double xMin = fmin(x1, x2);
    double xMax = fmax(x1, x2);
    double yMin = fmin(y1, y2);
    double yMax = fmax(y1, y2);

    if(retangulo->x >= xMin && retangulo->x + retangulo->w <= xMax && retangulo->y >= yMin && retangulo->y + retangulo->h <= yMax){
        return true;
    }

    return false;

}

/*****************************************************************************************************/

bool pontoInternoRetangulo(ArvoreGenerica a, NoGenerico n, RetanguloGenerico r, double x, double y){

    Retangulo *retangulo = (Retangulo*)r;

    return (x >= retangulo->x && x <= retangulo->x + retangulo->w && y >= retangulo->y && y <= retangulo->y + retangulo->h);

}

/*****************************************************************************************************/