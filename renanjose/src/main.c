/* Este é o programa principal que integra e executa todas as etapas do sistema de manipulação de formas geométricas.
 *
 * Ele interpreta os argumentos da linha de comando, identificando os arquivos de entrada (.geo e .qry), os diretórios 
 * de entrada e saída, e os parâmetros de configuração da SmuTreap (como taxa de promoção, contagem de hits e epsilon).
 *
 * A partir disso, o programa realiza as seguintes ações:
 * - Lê os dados do arquivo .geo e os armazena em uma árvore do tipo SmuTreap.
 * - Gera um arquivo SVG com o conteúdo inicial da árvore.
 * - Se o arquivo .qry for fornecido, ele é lido e suas instruções são processadas sobre a árvore, incluindo consultas, 
 *   alterações, inserções e remoções de formas.
 * - Arquivos de saída (.svg, .txt, .csv e .dot) são gerados com os resultados das operações realizadas. */

/* Importação das bibliotecas para a implementação da função principal do programa. */
#include "armazenamento.h"
#include "processamento.h"
#include "smutreap.h"
#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*****************************************************************************************************/

int main(int argc, char *argv[]){

    srand(time(NULL)); // Inicializa gerador de números aleatórios

    // Criar e processar os parâmetros
    ParametrosGenericos parametros = criarParametros();

    if(!processarArgumentos(parametros, argc, argv)){
        desalocarParametros(parametros);
        return 1;
    }

    // Obter caminhos de entrada
    char* caminhoGeo = buscarCaminhoCompletoGeo(parametros);
    char* caminhoQry = buscarCaminhoCompletoQry(parametros); 

    // Obter caminhos de saída
    char* caminhoSvgGeo = buscarCaminhoSvgBase(parametros);
    char* caminhoSvgQry = buscarCaminhoSvgConsulta(parametros); 
    char* caminhoTxt = buscarCaminhoTxtConsulta(parametros); 
    char* caminhoCsv = NULL;
    char* caminhoDotGeo = buscarCaminhoDotBase(parametros);
    char* caminhoDotQry = buscarCaminhoDotConsulta(parametros);

    // Criar o caminho do CSV (sempre existe, independente do .qry)
    char* nomeBaseGeo = buscarNomeBaseGeo(parametros);

    if(temArquivoQry(parametros)){
        char* nomeBaseQry = buscarNomeBaseQry(parametros);
        size_t len = strlen(buscarDiretorioSaida(parametros)) + strlen(nomeBaseGeo) + strlen(nomeBaseQry) + 10;
        caminhoCsv = malloc(len);
        snprintf(caminhoCsv, len, "%s/%s-%s.csv", buscarDiretorioSaida(parametros), nomeBaseGeo, nomeBaseQry);
        free(nomeBaseQry);
    }else{
        size_t len = strlen(buscarDiretorioSaida(parametros)) + strlen(nomeBaseGeo) + 10;
        caminhoCsv = malloc(len);
        snprintf(caminhoCsv, len, "%s/%s.csv", buscarDiretorioSaida(parametros), nomeBaseGeo);
    }

    free(nomeBaseGeo);
    
    // Criar a SmuTreap
    SmuTreap arvore = newSmuTreap(buscarHitCount(parametros), buscarPromotionRate(parametros), 0.000001, buscarPrioridadeMax(parametros));

    // Processar o .geo
    armazenarFormas(arvore, caminhoGeo);
    gerarArquivoSvg(arvore, caminhoSvgGeo);

    if(temArquivoQry(parametros)){
        processarFormas(arvore, caminhoQry, caminhoSvgQry, caminhoTxt, caminhoCsv);

        // Gera apenas o .dot final (após o .qry)
        if(caminhoDotQry){
            gerarArquivoDot(arvore, caminhoDotQry);
        }
    }else{
        gerarArquivoCsv(arvore, caminhoCsv);

        // Só gera o .dot se NÃO tiver qry (ou seja, só .geo)
        gerarArquivoDot(arvore, caminhoDotGeo);
    }

    // Libera memória
    killSmuTreap(arvore);

    free(caminhoGeo);
    free(caminhoQry);
    free(caminhoSvgGeo);
    free(caminhoSvgQry);
    free(caminhoTxt);
    free(caminhoCsv);
    free(caminhoDotGeo);
    free(caminhoDotQry);

    desalocarParametros(parametros);

    printf("\nFIM! Acho que agora deu certo (espero)!\n");

    return 0;
    
}
