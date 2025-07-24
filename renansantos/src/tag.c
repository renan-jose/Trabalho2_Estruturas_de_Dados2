/* Importação das bibliotecas para a implementação das tags SVG */
#include "tag.h"

////////////////////////////////////////////////////////////////////////////////////////

void tagRetangulo(FILE* arquivo, FormaGeometricaGenerica forma){

    fprintf(arquivo, "<rect x='%lf' y='%lf' width='%lf' height='%lf' stroke='%s' fill='%s'/>\n",
            buscarCoordXFormaGeometrica(forma),
            buscarCoordYFormaGeometrica(forma),
            buscarLarguraRetangulo(buscarFormaGeometrica(forma)),
            buscarAlturaRetangulo(buscarFormaGeometrica(forma)),
            buscarCorBordaRetangulo(buscarFormaGeometrica(forma)),
            buscarCorPreenchimentoRetangulo(buscarFormaGeometrica(forma)));

}

////////////////////////////////////////////////////////////////////////////////////////

void tagCabecalho(FILE* arquivo){

    if(arquivo == NULL){
        printf("Erro: Nao foi possivel inserir a tag do cabecalho no arquivo svg!\n");
        return;
    }

    // Primeira linha obrigatória para arquivos SVG bem formados
    fprintf(arquivo, "<?xml version='1.0' encoding='UTF-8'?>\n");
    // Tag de abertura correta do SVG
    fprintf(arquivo, "<svg xmlns='http://www.w3.org/2000/svg' version='1.1' "
    "width='100%%' height='200%%' viewBox='0 0 4000 12000'>\n");

}

////////////////////////////////////////////////////////////////////////////////////////

void tagRodape(FILE* arquivo){

    if(arquivo == NULL){
        printf("Erro: Nao foi possivel inserir a tag do rodape no arquivo svg!\n");
        return;
    }

    fprintf(arquivo, "</svg>\n");
    
}

////////////////////////////////////////////////////////////////////////////////////////
