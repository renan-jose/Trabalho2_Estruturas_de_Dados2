#include "prioridade.h"
#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////////////

#define CAPACIDADE_INICIAL 128

struct No{

    DadoGenerico dado;
    double prioridade;

};

struct Fila{

    struct No *dados;
    int tamanho, capacidade;

};

typedef struct No No;
typedef struct Fila Fila;

////////////////////////////////////////////////////////////////////////////////////////

static void trocar(No *a, No *b){

    No auxiliar = *a;

    *a = *b;
    *b = auxiliar;

}

////////////////////////////////////////////////////////////////////////////////////////

static void subir(No *monte, int indice){

    while(indice > 0){
        int pai = (indice -1) / 2;

        if(monte[indice].prioridade >= monte[pai].prioridade){
            break;
        }
        
        trocar(&monte[indice], &monte[pai]);
        indice = pai;
    }

}

////////////////////////////////////////////////////////////////////////////////////////

static void descer(No *monte, int tamanho, int indice){

    while(true){
        int menor = indice;
        int esquerda = 2 * indice + 1;
        int direita = 2 * indice + 2;

        if(esquerda < tamanho && monte[esquerda].prioridade < monte[menor].prioridade){
            menor = esquerda;
        }

        if(direita < tamanho && monte[direita].prioridade < monte[menor].prioridade){
            menor = direita;
        }

        if(menor == indice){
            break;
        }
        trocar(&monte[indice], &monte[menor]);
        indice = menor;
    }

}

////////////////////////////////////////////////////////////////////////////////////////

FilaPrioridadeGenerica criarFilaPrioridade(){

    Fila *novaFila = (Fila*)malloc(sizeof(struct Fila));

    novaFila->dados = (No*)malloc(sizeof(No) * CAPACIDADE_INICIAL);
    novaFila->tamanho = 0;
    novaFila->capacidade = CAPACIDADE_INICIAL;

    return novaFila;

}

////////////////////////////////////////////////////////////////////////////////////////

void inserirFilaPrioridade(FilaPrioridadeGenerica f, DadoGenerico dado, double prioridade){

    Fila *fila = (Fila*)f;

    if(fila->tamanho == fila->capacidade){
        fila->capacidade *= 2;
        fila->dados = realloc(fila->dados, sizeof(No) * fila->capacidade);
    }

    fila->dados[fila->tamanho].dado = dado;
    fila->dados[fila->tamanho].prioridade = prioridade;
    subir(fila->dados, fila->tamanho);
    fila->tamanho++;

}

////////////////////////////////////////////////////////////////////////////////////////

DadoGenerico removerFilaPrioridade(FilaPrioridadeGenerica f){

    Fila *fila = (Fila*)f;

    if(fila->tamanho == 0){
        return NULL;
    }

    DadoGenerico busca = fila->dados[0].dado;
    fila->dados[0] = fila->dados[--fila->tamanho];
    descer(fila->dados, fila->tamanho, 0);

    return busca;

}

////////////////////////////////////////////////////////////////////////////////////////

bool filaPrioridadeVazia(FilaPrioridadeGenerica f){

    Fila *fila = (Fila*)f;

    if(fila->tamanho == 0){
        return true;
    }

    return false;

}

////////////////////////////////////////////////////////////////////////////////////////

void desalocarFilaPrioridade(FilaPrioridadeGenerica f){

    Fila *fila = (Fila*)f;

    free(fila->dados);
    free(fila);

}

