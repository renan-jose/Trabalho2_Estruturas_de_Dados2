/* Importação das bibliotecas para a implementação da lista. */
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

struct NoLista{

    DadoGenerico dado;
    struct NoLista *proximo; 

};

struct EstruturaLista{

    struct NoLista *inicio;
    int tamanho;

};

typedef struct NoLista NoLista;
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

void listar(Lista L, DadoGenerico n){

    EstruturaLista *lista = (EstruturaLista*)L;

    NoLista *novoNo = (NoLista*)malloc(sizeof(NoLista));

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

void deslistar(Lista L){

    EstruturaLista *lista = (EstruturaLista*)L;

    if(lista->inicio == NULL){
        printf("Erro: Lista vazia! Sem remocao!\n");
        return;
    }

    NoLista *auxiliar = lista->inicio;
    lista->inicio = auxiliar->proximo;

    free(auxiliar);
    lista->tamanho--;

}

/*****************************************************************************************************/

bool listaVazia(Lista L){

    EstruturaLista *lista = (EstruturaLista*)L;

    if(lista->tamanho == 0){
        return true;
    }else{
        return false;
    } 
    
}

/*****************************************************************************************************/

void desalocarLista(Lista L){

    EstruturaLista *lista = (EstruturaLista*)L;

    NoLista *auxiliar = lista->inicio;

    while(auxiliar != NULL){
        NoLista *proximo = auxiliar->proximo;
        free(auxiliar);
        auxiliar = proximo;
    }

    free(lista);
    
}

/*****************************************************************************************************/

DadoGenerico buscarElementoLista(Lista L, int posicao){

    EstruturaLista *lista = (EstruturaLista*)L;

    if(posicao < 0 || posicao >= lista->tamanho){
        return NULL; 
    }

    NoLista *auxiliar = lista->inicio;

    for(int i = 0; i < posicao; i++){
        auxiliar = auxiliar->proximo;
    }

    return auxiliar->dado;

}

/*****************************************************************************************************/

int buscarTamanhoLista(Lista L){

    EstruturaLista *lista = (EstruturaLista*)L;

    return lista->tamanho;
    
}
