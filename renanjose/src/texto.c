/* Importação das bibliotecas para a implementação do texto. */
#include "texto.h"
#include "smutreap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*****************************************************************************************************/

struct Estilo{

    char fonte[64], peso[16], tamanho[16];

};

struct Texto{

    int id;
    double x, y;
    char *corp, *corb, *texto, a;
    struct Estilo estilo;

};

typedef struct Texto Texto;

/*****************************************************************************************************/

TextoGenerico criarTexto(int id, double x, double y, char *corb, char *corp, char a, char *texto, char *fonte, char *peso, char *tamanho){

    Texto *novoTexto = (Texto*)malloc(sizeof(Texto));

    if(novoTexto == NULL){
        printf("Erro: Falha na alocacao dinamica para a criacao do texto!\n");
        return NULL;
    }

    novoTexto->id = id;
    novoTexto->x = x;
    novoTexto->y = y;

    novoTexto->corb = (char*)malloc(strlen(corb) + 1);
    if(novoTexto->corb == NULL){
        printf("Erro: Falha na alocacao dinamica para a criacao do texto!\n");
        free(novoTexto);
        return NULL;
    }
    strcpy(novoTexto->corb, corb);

    novoTexto->corp = (char*)malloc(strlen(corp) + 1);
    if(novoTexto->corp == NULL){
        printf("Erro: Falha na alocacao dinamica para a criacao do texto!\n");
        free(novoTexto->corb);
        free(novoTexto);
        return NULL;
    }
    strcpy(novoTexto->corp, corp);

    novoTexto->texto = (char*)malloc(strlen(texto) + 1);

    if(novoTexto->texto == NULL){
        printf("Erro: Falha na alocacao dinamica para o conteúdo do texto!\n");
        free(novoTexto->corp);
        free(novoTexto->corb);
        free(novoTexto);
        return NULL;
    }

    strcpy(novoTexto->estilo.fonte, fonte);
    strcpy(novoTexto->estilo.peso, peso);
    strcpy(novoTexto->estilo.tamanho, tamanho);

    strcpy(novoTexto->texto, texto);

    novoTexto->a = a;

    return novoTexto;

}

/*****************************************************************************************************/

int buscarIdTexto(TextoGenerico t){

    Texto *texto = (Texto*)t;

    return texto->id;

}

/*****************************************************************************************************/

double buscarCoordXTexto(TextoGenerico t){

    Texto *texto = (Texto*)t;

    return texto->x;

}

/*****************************************************************************************************/

double buscarCoordYTexto(TextoGenerico t){

    Texto *texto = (Texto*)t;

    return texto->y;

}

/*****************************************************************************************************/

char *buscarCorBordaTexto(TextoGenerico t) {
    Texto *texto = (Texto*)t;
    if (!texto || !texto->corb) return "none";
    return texto->corb;
}


/*****************************************************************************************************/

char *buscarCorPreenchimentoTexto(TextoGenerico t) {
    Texto *texto = (Texto*)t;
    if (!texto || !texto->corp) return "none";
    return texto->corp;
}

/*****************************************************************************************************/

char buscarPeriodoAncoraTexto(TextoGenerico t){

    Texto *texto = (Texto*)t;

    return texto->a;

}

/*****************************************************************************************************/

char *buscarTextoTexto(TextoGenerico t) {
    Texto *texto = (Texto*)t;
    if (!texto || !texto->texto) return "none";
    return texto->texto;
}

/*****************************************************************************************************/

void mudarCoordXTexto(TextoGenerico t, double x){

    Texto *texto = (Texto*)t;

    texto->x = x;

}

/*****************************************************************************************************/

void mudarCoordYTexto(TextoGenerico t, double y){

    Texto *texto = (Texto*)t;

    texto->y = y;

}

/*****************************************************************************************************/

void mudarCorBordaTexto(TextoGenerico t, char *corb){

    Texto *texto = (Texto*)t;

    free(texto->corb);

    texto->corb = (char*)malloc(strlen(corb) + 1);
    if(texto->corb == NULL){
        printf("Erro: Falha ao alocar memoria para a nova cor da borda do texto!\n");
        return;
    }

    strcpy(texto->corb, corb);

}

/*****************************************************************************************************/

void mudarCorPreenchimentoTexto(TextoGenerico t, char *corp){

    Texto *texto = (Texto*)t;

    free(texto->corp);

    texto->corp = (char*)malloc(strlen(corp) + 1);
    if(texto->corp == NULL){
        printf("Erro: Falha ao alocar memoria para a nova cor do preenchimento do texto!\n");
        return;
    }

    strcpy(texto->corp, corp);

}

/*****************************************************************************************************/

void mudarPeriodoAncoraTexto(TextoGenerico t, char a){

    Texto *texto = (Texto*)t;

    texto->a = a;

}

/*****************************************************************************************************/

void mudarTextoTexto(TextoGenerico t, char *txt){

    Texto *texto = (Texto*)t;

    free(texto->texto);

    texto->texto = (char*)malloc(strlen(txt) + 1);
    if(texto->texto == NULL){
        printf("Erro: Falha na alocacao dinamica para o novo texto do texto!\n");
        return;
    }
    
    strcpy(texto->texto, txt);

}

/*****************************************************************************************************/

char *buscarFonteTexto(TextoGenerico t) {
    Texto *texto = (Texto*)t;
    if (!texto || !texto->estilo.fonte) return "none";
    return texto->estilo.fonte;
}


/*****************************************************************************************************/

char *buscarPesoTexto(TextoGenerico t) {
    Texto *texto = (Texto*)t;
    if (!texto || !texto->estilo.peso) return "none";
    return texto->estilo.peso;
}


/*****************************************************************************************************/

char *buscarTamanhoTexto(TextoGenerico t) {
    Texto *texto = (Texto*)t;
    if (!texto || !texto->estilo.tamanho) return "none";
    return texto->estilo.tamanho;
}

/*****************************************************************************************************/

void mudarFonteTexto(TextoGenerico t, char *fonte){

    Texto *texto = (Texto *)t;

    strncpy(texto->estilo.fonte, fonte, sizeof(texto->estilo.fonte) - 1);
    texto->estilo.fonte[sizeof(texto->estilo.fonte) - 1] = '\0';

}

/*****************************************************************************************************/

void mudarPesoTexto(TextoGenerico t, char *peso){

    Texto *texto = (Texto *)t;

    strncpy(texto->estilo.peso, peso, sizeof(texto->estilo.peso) - 1);
    texto->estilo.peso[sizeof(texto->estilo.peso) - 1] = '\0';

}

/*****************************************************************************************************/

void mudarTamanhoTexto(TextoGenerico t, char *tamanho){

    Texto *texto = (Texto *)t;

    strncpy(texto->estilo.tamanho, tamanho, sizeof(texto->estilo.tamanho) - 1);
    texto->estilo.tamanho[sizeof(texto->estilo.tamanho) - 1] = '\0';
    
}

/*****************************************************************************************************/

void calcularBbTexto(TextoGenerico t, double *x, double *y, double *w, double *h){
    
    Texto *texto = (Texto*)t;

    *x = texto->x;
    *y = texto->y;
    *w = 0;
    *h = 0;

}

/*****************************************************************************************************/

void desalocarTexto(TextoGenerico t){

    Texto *texto = (Texto*)t;

    if(texto == NULL){
        return;
    }

    free(texto->corb);
    free(texto->corp);
    free(texto->texto);
    free(texto);

}

/*****************************************************************************************************/

bool textoDentroRegiao(ArvoreGenerica a, NoGenerico n, TextoGenerico t, double x1, double y1, double x2, double y2){

    Texto *texto = (Texto*)t;

    double x, y, w, h;
    
    calcularBbTexto(texto, &x, &y, &w, &h);

    double xmin = fmin(x1, x2);
    double xmax = fmax(x1, x2);
    double ymin = fmin(y1, y2);
    double ymax = fmax(y1, y2);

    return (x >= xmin && x + w <= xmax && y >= ymin && y + h <= ymax);

}

/*****************************************************************************************************/

bool pontoInternoTexto(ArvoreGenerica a, NoGenerico n, TextoGenerico t, double x, double y){

    Texto *texto = (Texto*)t;

    double eps = getEpsilonSmuTreap(a);

    return (fabs(x - texto->x) <= eps && fabs(y - texto->y) <= eps);

}

/*****************************************************************************************************/