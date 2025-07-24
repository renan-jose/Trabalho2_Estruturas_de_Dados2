#include "graph.h"
#include "espalhamento.h"
#include "arquivo.h"
#include "processamento.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *duplicarString(char *string){

    // Aloca a cópia da string
    char *copia = (char*)malloc(strlen(string) + 1);

    // Verifica a alocação.
    if(copia == NULL){
        printf("Erro: Falha na alocacao de memoria para a duplicao de strings!\n");
        return NULL;
    }

    // Função strcpy realiza o processo de cópia.
    strcpy(copia, string);

    // Retorna a string copiada.
    return copia;

}

/*******************************************************************************/

int main(int argc, char *argv[]) {
    ParametrosGenericos parametros = criarParametros();

    if (!processarArgumentos(parametros, argc, argv)) {
        desalocarParametros(parametros);
        return 1;
    }

    char *caminhoGeo = buscarCaminhoCompletoGeo(parametros);
    char *caminhoQry = buscarCaminhoCompletoQry(parametros);
    char *caminhoVia = buscarCaminhoCompletoVia(parametros);

    // Se não tiver .geo, erro fatal
    if (caminhoGeo == NULL) {
        printf("Erro: Arquivo .geo eh obrigatorio!\n");
        desalocarParametros(parametros);
        return 1;
    }

    // Geração de nome base combinado para arquivos de saída
    char *nomeBaseGeo = buscarNomeBaseGeo(parametros);
    char *nomeBaseQry = temArquivoQry(parametros) ? buscarNomeBaseQry(parametros) : NULL;

    // Combina nomes
    char *nomeCombinado = NULL;
    if(nomeBaseQry){
        size_t tam = strlen(nomeBaseGeo) + strlen(nomeBaseQry) + 2;
        nomeCombinado = malloc(tam);
        snprintf(nomeCombinado, tam, "%s-%s", nomeBaseGeo, nomeBaseQry);
    }else{
        nomeCombinado = duplicarString(nomeBaseGeo);
    }

    // Diretório de saída
    char *dirSaida = buscarDiretorioSaida(parametros);
    size_t tamSvg = strlen(dirSaida) + strlen(nomeCombinado) + 6;
    size_t tamTxt = strlen(dirSaida) + strlen(nomeCombinado) + 6;

    char *caminhoSvg = malloc(tamSvg);
    char *caminhoTxt = malloc(tamTxt);

    snprintf(caminhoSvg, tamSvg, "%s/%s.svg", dirSaida, nomeCombinado);
    snprintf(caminhoTxt, tamTxt, "%s/%s.txt", dirSaida, nomeCombinado);

    // Processamento conforme arquivos disponíveis
    if(caminhoVia == NULL && caminhoQry == NULL){
        printf("Processando apenas arquivo .geo!\n");
        processarArquivos(caminhoGeo, NULL, NULL, caminhoSvg, caminhoTxt);
    }else if (caminhoVia != NULL && caminhoQry == NULL){
        printf("Processando arquivo .geo e .via!\n");
        processarArquivos(caminhoGeo, caminhoVia, NULL, caminhoSvg, caminhoTxt);
    }else if(caminhoVia == NULL && caminhoQry != NULL){
        printf("Erro: Arquivo .via eh essencial nesse caso!\n");
    }else if(caminhoVia != NULL && caminhoQry != NULL) {
        printf("Processando arquivos .geo, .via e .qry!\n");
        processarArquivos(caminhoGeo, caminhoVia, caminhoQry, caminhoSvg, caminhoTxt);
    }

    // Liberação de memória
    free(caminhoGeo);
    free(caminhoQry);
    free(caminhoVia);
    free(caminhoSvg);
    free(caminhoTxt);
    free(nomeBaseGeo);
    if (nomeBaseQry) free(nomeBaseQry);
    free(nomeCombinado);

    desalocarParametros(parametros);

    printf("\nFIM DO PROGRAMA!\n");

    return 0;

}
