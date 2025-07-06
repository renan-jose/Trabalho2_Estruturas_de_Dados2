/* Importação das bibliotecas para a implementação da fila. */
#include "fila.h"
#include <stdio.h>
#include <stdlib.h>

/*****************************************************************************************************/

struct NoFila{

    DadoGenerico dado;
    struct NoFila *proximo;

};

struct Fila{

    struct NoFila *inicio, *fim;
    int tamanho;

};

typedef struct NoFila NoFila;
typedef struct Fila Fila;

/*****************************************************************************************************/

FilaGenerica criarFila(){

    Fila *fila = (Fila*)malloc(sizeof(Fila));

    if(fila == NULL){
        printf("Erro: Falha na alocacao de memoria da fila!\n");
        return NULL;
    }

    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;

    return fila;

}

/*****************************************************************************************************/

void enfileirar(FilaGenerica F, DadoGenerico n){

    Fila *fila = (Fila*)F;

    NoFila *novoNo = (NoFila*)malloc(sizeof(NoFila));

    if(novoNo == NULL){
        printf("Erro: Falha na alocacao de memoria para a insercao do valor na fila!\n");
        return;
    }

    novoNo->dado = n;
    novoNo->proximo = NULL;

    if(fila->inicio == NULL){
        fila->inicio = novoNo;
        fila->fim = novoNo;
    }else{
        fila->fim->proximo = novoNo;
        fila->fim = novoNo;
    }

    fila->tamanho++;

}

/*****************************************************************************************************/

DadoGenerico desenfileirar(FilaGenerica F){

    Fila *fila = (Fila*)F;

    if(fila->inicio == NULL){
        printf("Fila vazia! Sem remocao!\n");
        return NULL;
    }

    NoFila *auxiliar = fila->inicio;
    DadoGenerico removido = auxiliar->dado;
    fila->inicio = auxiliar->proximo;

    if(fila->inicio == NULL){
        fila->fim = NULL;
    }

    free(auxiliar);

    fila->tamanho--;

    return removido;

}

/*****************************************************************************************************/

bool filaVazia(FilaGenerica F){

    Fila *fila = (Fila*)F;

    if(fila->tamanho == 0){
        return true;
    }else{
        return false;
    }

}

/*****************************************************************************************************/

void desalocarFila(FilaGenerica F){

    Fila *fila = (Fila*)F;

    NoFila *auxiliar = fila->inicio;
    NoFila *auxiliar2;

    while(auxiliar != NULL){
        auxiliar2 = auxiliar->proximo;
        free(auxiliar);
        auxiliar = auxiliar2;
    }

    free(fila);
    
}

/*****************************************************************************************************/