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

void tagCabecalho(FILE* arquivo) {
    if (arquivo == NULL) {
        printf("Erro: Nao foi possivel inserir a tag do cabecalho no arquivo svg!\n");
        return;
    }

    fprintf(arquivo, "<?xml version='1.0' encoding='UTF-8'?>\n");
    fprintf(arquivo,
    "<svg xmlns='http://www.w3.org/2000/svg' version='1.1' width='200' height='200' viewBox='0 0 2000 2000'>\n"
    "<defs>\n"
    "  <marker id='arrow' markerWidth='10' markerHeight='7' refX='10' refY='3.5' orient='auto'>\n"
    "    <polygon points='0 0, 10 3.5, 0 7' fill='black'/>\n"
    "  </marker>\n"
    "</defs>\n");

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
