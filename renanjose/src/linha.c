/* Importação das bibliotecas para a implementação da linha. */
#include "linha.h"
#include "smutreap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*****************************************************************************************************/

struct Linha{

    int id;
    double x1, y1, x2, y2;
    char *cor;

};

typedef struct Linha Linha;

/*****************************************************************************************************/

LinhaGenerica criarLinha(int id, double x1, double y1, double x2, double y2, char *cor){

    Linha *novaLinha = (Linha*)malloc(sizeof(Linha));

    if(novaLinha == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao da linha!\n");
        return NULL;
    }

    novaLinha->id = id;
    novaLinha->x1 = x1;
    novaLinha->y1 = y1;
    novaLinha->x2 = x2;
    novaLinha->y2 = y2;

    novaLinha->cor = (char*)malloc(strlen(cor) + 1);
    if(novaLinha->cor == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao da linha!\n");
        free(novaLinha);
        return NULL;
    }
    strcpy(novaLinha->cor, cor);

    return novaLinha;

}

/*****************************************************************************************************/

int buscarIdLinha(LinhaGenerica l){

    Linha *linha = (Linha*)l;

    return linha->id;

}

/*****************************************************************************************************/

double buscarX1Linha(LinhaGenerica l){

    Linha *linha = (Linha*)l;

    return linha->x1;

}

/*****************************************************************************************************/

double buscarY1Linha(LinhaGenerica l){

    Linha *linha = (Linha*)l;

    return linha->y1;

}

/*****************************************************************************************************/

double buscarX2Linha(LinhaGenerica l){

    Linha *linha = (Linha*)l;

    return linha->x2;

}

/*****************************************************************************************************/

double buscarY2Linha(LinhaGenerica l){

    Linha *linha = (Linha*)l;

    return linha->y2;

}

/*****************************************************************************************************/

char *buscarCorLinha(LinhaGenerica l) {
    Linha *linha = (Linha*)l;
    if (!linha || !linha->cor) return "none";
    return linha->cor;
}

/*****************************************************************************************************/

void mudarX1Linha(LinhaGenerica l, double x1){

    Linha *linha = (Linha*)l;

    linha->x1 = x1;

}

/*****************************************************************************************************/

void mudarY1Linha(LinhaGenerica l, double y1){

    Linha *linha = (Linha*)l;

    linha->y1 = y1;

}

/*****************************************************************************************************/

void mudarX2Linha(LinhaGenerica l, double x2){
    
    Linha *linha = (Linha*)l;

    linha->x2 = x2;

}

/*****************************************************************************************************/

void mudarY2Linha(LinhaGenerica l, double y2){

    Linha *linha = (Linha*)l;

    linha->y2 = y2;

}

/*****************************************************************************************************/

void mudarCorLinha(LinhaGenerica l, char *cor){

    Linha *linha = (Linha*)l;

    free(linha->cor);
    linha->cor = (char*)malloc(strlen(cor) + 1);
    if(linha->cor == NULL){
        printf("Erro: Falha na alocacao de memoria para a nova cor da linha!\n");
        return;
    }

    strcpy(linha->cor, cor);

}

/*****************************************************************************************************/

void calcularBbLinha(LinhaGenerica l, double *x, double *y, double *w, double *h){

    Linha *linha = (Linha*)l;

    *x = fmin(linha->x1, linha->x2);
    *y = fmin(linha->y1, linha->y2);
    *w = fabs(linha->x1 - linha->x2);
    *h = fabs(linha->y1 - linha->y2);

}

/*****************************************************************************************************/

void desalocarLinha(LinhaGenerica l){

    Linha *linha = (Linha*)l;

    if(linha == NULL){
        return;
    }

    free(linha->cor);
    free(linha);

}

/*****************************************************************************************************/

bool linhaDentroRegiao(ArvoreGenerica a, NoGenerico n, LinhaGenerica l, double x1, double y1, double x2, double y2){

    Linha *linha = (Linha*)l;

    double xMin = fmin(x1, x2);
    double xMax = fmax(x1, x2);
    double yMin = fmin(y1, y2);
    double yMax = fmax(y1, y2);
    double epsilon = getEpsilonSmuTreap(a);

    // Filtro extra: evita linhas degeneradas (ponto)
    if(fabs(linha->x1 - linha->x2) < 1e-6 && fabs(linha->y1 - linha->y2) < 1e-6){
        return false;
    }

    bool p1Dentro = linha->x1 >= xMin && linha->x1 <= xMax && linha->y1 >= yMin && linha->y1 <= yMax;

    bool p2Dentro = linha->x2 >= xMin && linha->x2 <= xMax && linha->y2 >= yMin && linha->y2 <= yMax;

    return p1Dentro && p2Dentro;

}

/*****************************************************************************************************/

bool pontoInternoLinha(ArvoreGenerica a, NoGenerico n, LinhaGenerica l, double x, double y){

    Linha *linha = (Linha*)l;

    double dist_ab = sqrt(pow(linha->x2 - linha->x1, 2) + pow(linha->y2 - linha->y1, 2));
    double dist_ap = sqrt(pow(x - linha->x1, 2) + pow(y - linha->y1, 2));
    double dist_pb = sqrt(pow(linha->x2 - x, 2) + pow(linha->y2 - y, 2));
    double eps = getEpsilonSmuTreap(a);

    return fabs((dist_ap + dist_pb) - dist_ab) <= eps;
    
}



