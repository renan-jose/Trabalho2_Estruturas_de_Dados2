/* Importação das bibliotecas. */
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

struct No{

    DadoGenerico dado;
    struct No *proximo; 

};

struct EstruturaLista{

    struct No *inicio;
    int tamanho;

};

typedef struct No No;
typedef struct EstruturaLista EstruturaLista;

/*****************************************************************************************************/

Lista inicializarLista(){

    EstruturaLista *lista = (EstruturaLista*)malloc(sizeof(EstruturaLista));

    if(lista == NULL){
        printf("Erro: Falha na alocacao de memoria da lista!\n");
        return NULL;
    }

    lista->inicio = NULL;
    lista->tamanho = 0;

    return lista;

}

/*****************************************************************************************************/

void inserirInicioLista(Lista l, DadoGenerico n){

    EstruturaLista *lista = (EstruturaLista*)l;

    No *novoNo = (No*)malloc(sizeof(No));

    if(novoNo == NULL){
        printf("Erro: Falha na alocacao de memoria para a insercao do valor na lista!\n");
        return;
    }

    novoNo->dado = n;
    novoNo->proximo = lista->inicio;
    lista->inicio = novoNo;
    lista->tamanho++;

}

/*****************************************************************************************************/

void inserirFimLista(Lista l, DadoGenerico n){

    EstruturaLista *lista = (EstruturaLista*)l;

    No *novoNo = (No*)malloc(sizeof(No));

    if(novoNo == NULL){
        printf("Erro: Falha na alocacao de memoria para a insercao do valor na lista!\n");
        return;
    }

    novoNo->dado = n;
    novoNo->proximo = NULL;

    if(lista->inicio == NULL){
        lista->inicio = novoNo;
    }else{
        No *atual = lista->inicio;

        while(atual->proximo != NULL){
            atual = atual->proximo;
        }

        atual->proximo = novoNo;
    }

    lista->tamanho++;

}

/*****************************************************************************************************/

void removerInicioLista(Lista l){

    EstruturaLista *lista = (EstruturaLista*)l;

    if(lista->inicio == NULL){
        printf("Erro: Lista vazia! Sem remocao!\n");
        return;
    }

    No *auxiliar = lista->inicio;
    lista->inicio = auxiliar->proximo;

    free(auxiliar);
    lista->tamanho--;

}

/*****************************************************************************************************/

int buscarTamanhoLista(Lista l){

    EstruturaLista *lista = (EstruturaLista*)l;

    return lista->tamanho;
    
}

/*****************************************************************************************************/

bool verificarElementoLista(Lista l, DadoGenerico d){

    EstruturaLista *lista = (EstruturaLista*)l;

    No *auxiliar = lista->inicio;

    while(auxiliar != NULL){
        if(auxiliar->dado == d){
            return true;
        }
        auxiliar = auxiliar->proximo;
    }

    return false;

}

/*****************************************************************************************************/

DadoGenerico buscarElementoLista(Lista l, int posicao){

    EstruturaLista *lista = (EstruturaLista*)l;

    if (!lista || posicao < 0 || posicao >= lista->tamanho || lista->inicio == NULL){
        return NULL;
    }

    No *auxiliar = lista->inicio;

    for(int i = 0; i < posicao; i++){
        if (!auxiliar) return NULL;
        auxiliar = auxiliar->proximo;
    }

    return auxiliar ? auxiliar->dado : NULL;

}


/*****************************************************************************************************/

void desalocarLista(Lista l){

    EstruturaLista *lista = (EstruturaLista*)l;

    No *auxiliar = lista->inicio;

    while(auxiliar != NULL){
        No *proximo = auxiliar->proximo;
        free(auxiliar);
        auxiliar = proximo;
    }

    free(lista);
    
}
