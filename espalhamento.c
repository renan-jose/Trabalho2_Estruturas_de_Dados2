// Importação das bibliotecas necessárias
#include "espalhamento.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////////////

/* Um nó da tabela de espalhamento possui uma chave, um dado de qualquer tipo e um
 * apontamento para o próximo nó. */
struct No{

    char *chave;
    DadoGenerico dado;
    struct No *proximo;

};

// Uma tabela de espalhamento possui um tamanho e uma lista com seus nós. 
struct Tabela{

    int tamanho;
    struct No **dados;

};

// Nomes alternativos para os tipos de dados existentes.
typedef struct No No;
typedef struct Tabela Tabela;

////////////////////////////////////////////////////////////////////////////////////////

/* OBS: unsigned antes do tipo de dado da variável garante que a variável guarde apenas 
 * valores positivos ou 0. */

// Função auxiliar.
static unsigned int converterStringParaIndice(char *string, int tamanho){

    unsigned int hash = 0;
    int conversao;
    
    while(*string != '\0'){
        hash = (hash * 31) + (*string++);
    }

    conversao = hash % tamanho;

    return conversao;

}

////////////////////////////////////////////////////////////////////////////////////////

TabelaGenerica criarTabela(int tamanho){

    // Aloca a tabela nova.
    Tabela *novaTabela = (Tabela*)malloc(sizeof(Tabela));

    // Verifica a alocação.
    if(novaTabela == NULL){
        printf("Erro: Falha na alocacao de memoria para criacao da tabela de espalhamento!\n");
        return NULL;
    }

    // Os demais campos da tabela nova são preenchidos.
    novaTabela->tamanho = tamanho;
    novaTabela->dados = (No**)calloc(tamanho, sizeof(No*)); // Aloca a lista de nós da tabela.

    // Verifica a alocação.
    if(novaTabela->dados == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao da lista de nos da tabela de espalhamento!\n");
        free(novaTabela);
        return NULL;
    }

    // Retorna a tabela nova.
    return novaTabela;

}

////////////////////////////////////////////////////////////////////////////////////////

// Função auxiliar.
static char *duplicarString(char *string){

    char *copia = (char*)malloc(strlen(string) + 1);

    if(copia == NULL){
        printf("Erro: Falha na alocacao de memoria para a duplicao de strings!\n");
        return NULL;
    }

    strcpy(copia, string);

    return copia;

}

////////////////////////////////////////////////////////////////////////////////////////

int inserirElementoTabela(TabelaGenerica t, char *chave, DadoGenerico dado){

    // Acesso para os campos da struct Tabela.
    Tabela *tabela = (Tabela*)t;

    unsigned int indice = converterStringParaIndice(chave, tabela->tamanho);
    No *auxiliar = tabela->dados[indice];

    while(auxiliar != NULL){
        if(strcmp(auxiliar->chave, chave) == 0){
            printf("Alerta: Elemento ja existente na tabela de espalhamento!\n");
            return 0;
        }
        auxiliar = auxiliar->proximo;
    }

    No *novoNo = (No*)malloc(sizeof(No));

    if(novoNo == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao do novo elemento na tabela de espalhamento!\n");
        return 0;
    }

    novoNo->chave = duplicarString(chave);
    novoNo->dado = dado;
    novoNo->proximo = tabela->dados[indice];
    tabela->dados[indice] = novoNo;

    return 1;

}

////////////////////////////////////////////////////////////////////////////////////////

int removerElementoTabela(TabelaGenerica t, char *chave){

    Tabela *tabela = (Tabela*)t;

    unsigned int indice = converterStringParaIndice(chave, tabela->tamanho);
    No *auxiliar = tabela->dados[indice], *anterior = NULL;

    while(auxiliar != NULL){
        if(strcmp(auxiliar->chave, chave) == 0){
            if(anterior == NULL){
                tabela->dados[indice] = auxiliar->proximo;
            }else{
                anterior->proximo = auxiliar->proximo;
            }

            free(auxiliar->chave);
            free(auxiliar);
            return 1;
        }
        anterior = auxiliar;
        auxiliar = auxiliar->proximo;
    }

    return 0;
    
}

////////////////////////////////////////////////////////////////////////////////////////

DadoGenerico buscarElementoTabela(TabelaGenerica t, char *chave){

    Tabela *tabela = (Tabela*)t;

    unsigned int indice = converterStringParaIndice(chave, tabela->tamanho);
    No *auxiliar = tabela->dados[indice];
    DadoGenerico valorEncontrado;

    while(auxiliar != NULL){
        if(strcmp(auxiliar->chave, chave) == 0){
            valorEncontrado = auxiliar->dado;

            return valorEncontrado;
        }
        auxiliar = auxiliar->proximo;
    }

    return NULL;

}

////////////////////////////////////////////////////////////////////////////////////////

void desalocarTabela(TabelaGenerica t){

    Tabela *tabela = (Tabela*)t;

    int i;
    No *auxiliar, *temporario;

    for(i = 0; i < tabela->tamanho; i++){
        auxiliar = tabela->dados[i];
        while(auxiliar != NULL){
            temporario = auxiliar;
            auxiliar = auxiliar->proximo;
            free(temporario->chave);
            free(temporario);

        }
    }

    free(tabela->dados);
    free(tabela);

}

