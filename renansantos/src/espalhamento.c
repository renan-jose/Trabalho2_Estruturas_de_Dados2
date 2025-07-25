// Importação das bibliotecas necessárias
#include "espalhamento.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////////////

/* O nó da tabela de espalhamento possui uma chave, um dado de qualquer tipo e um
 * apontamento para o próximo nó. */
struct No{

    char *chave;
    DadoGenerico dado;
    struct No *proximo;

};

// A tabela de espalhamento possui um tamanho e uma lista com seus nós. 
struct Tabela{

    int tamanho;
    struct No **baldes;

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

    if(tamanho <= 0){
        printf("Erro: Tamanho invalido para tabela de espalhamento: %d\n", tamanho);
        return NULL;
    }

    // Aloca a tabela nova.
    Tabela *novaTabela = (Tabela*)malloc(sizeof(Tabela));

    // Verifica a alocação.
    if(novaTabela == NULL){
        printf("Erro: Falha na alocacao de memoria para criacao da tabela de espalhamento!\n");
        return NULL;
    }

    // Os demais campos da tabela nova são preenchidos.
    novaTabela->tamanho = tamanho;
    novaTabela->baldes = (No**)calloc(tamanho, sizeof(No*)); // Aloca a lista de nós da tabela.

    // Verifica a alocação.
    if(novaTabela->baldes == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao da lista de nos da tabela de espalhamento!\n");
        free(novaTabela);
        return NULL;
    }

    // Retorna a tabela nova.
    return novaTabela;

}

////////////////////////////////////////////////////////////////////////////////////////

// Função auxiliar.
static char *duplicarString(char *string) {
    if (string == NULL) return NULL;
    char *copia = malloc(strlen(string) + 1);
    if (!copia) return NULL;
    strcpy(copia, string);
    return copia;
}


////////////////////////////////////////////////////////////////////////////////////////

int inserirElementoTabela(TabelaGenerica t, char *chave, DadoGenerico dado){

    // Acesso para os campos da struct Tabela.
    Tabela *tabela = (Tabela*)t;

    /*
    // Antes do processo de inserção, verifica se o elemento a ser colocado está na tabela.
    if(buscarElementoTabela(t, chave) != NULL){
        printf("Aviso: O elemento nao foi inserido pois ja existe na tabela de espalhamento!\n");
        return 0;
    }*/

    unsigned int indice = converterStringParaIndice(chave, tabela->tamanho);

    // Aloca o nó novo
    No *novoNo = (No*)malloc(sizeof(No));

    // Verifica a alocação.
    if(novoNo == NULL){
        printf("Erro: Falha na alocacao de memoria para a criacao do novo elemento na tabela de espalhamento!\n");
        return 0;
    }

    // Preenche os demais campos do nó.
    novoNo->chave = duplicarString(chave);
    novoNo->dado = dado;
    novoNo->proximo = tabela->baldes[indice];
    tabela->baldes[indice] = novoNo;

    // Retorna 1 (Inserção bem-sucedida).
    return 1;

}

////////////////////////////////////////////////////////////////////////////////////////

int removerElementoTabela(TabelaGenerica t, char *chave){

    // Acesso para os campos da struct Tabela.
    Tabela *tabela = (Tabela*)t;

    DadoGenerico dado = buscarElementoTabela(t, chave);

    // Antes do processo de remoção, verifica se o elemento a ser colocado está fora na tabela.
    if(dado == NULL){
        printf("Aviso: O elemento nao foi removido pois nao existe na tabela de espalhamento!\n");
        return 0;
    }

    unsigned int indice = converterStringParaIndice(chave, tabela->tamanho);
    No *auxiliar = tabela->baldes[indice], *anterior = NULL;

    while(auxiliar != NULL){
        if(strcmp(auxiliar->chave, chave) == 0){
            if(anterior == NULL){
                tabela->baldes[indice] = auxiliar->proximo;
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
    No *auxiliar = tabela->baldes[indice];
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

bool buscarElementoTabelaComValor(TabelaGenerica t, char *chave, DadoGenerico *valor){
    
    Tabela *tabela = (Tabela*)t;

    unsigned int indice = converterStringParaIndice(chave, tabela->tamanho);
    No *auxiliar = tabela->baldes[indice];
    DadoGenerico valorEncontrado;

    while(auxiliar != NULL){
        if(strcmp(auxiliar->chave, chave) == 0){
            *valor = auxiliar->dado; 
            return true;
        }
        auxiliar = auxiliar->proximo;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////////////

void percorrerTabela(TabelaGenerica t, void (*callback)(DadoGenerico, void*), void *extra){
    Tabela *tabela = (Tabela*)t;

    for(int i = 0; i < tabela->tamanho; i++){
        No *atual = tabela->baldes[i];

        while(atual != NULL){
            callback(atual->dado, extra);
            atual = atual->proximo;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////

int buscarTamanhoTabela(TabelaGenerica t){

    Tabela *tabela = (Tabela*)t;

    return tabela->tamanho;
    
}

////////////////////////////////////////////////////////////////////////////////////////

void desalocarTabela(TabelaGenerica t){

    Tabela *tabela = (Tabela*)t;

    int i;
    No *auxiliar, *temporario;

    for(i = 0; i < tabela->tamanho; i++){
        auxiliar = tabela->baldes[i];
        while(auxiliar != NULL){
            temporario = auxiliar;
            auxiliar = auxiliar->proximo;
            free(temporario->chave);
            free(temporario);

        }
    }

    free(tabela->baldes);
    free(tabela);

}

