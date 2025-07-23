/* Importação das bibliotecas para a implementação das tags SVG */
#include "tag.h"

/*****************************************************************************************************/

void tagRetangulo(FILE* arquivo, FormaGeometricaGenerica forma){

    fprintf(arquivo, "<rect x='%lf' y='%lf' width='%lf' height='%lf' stroke='%s' fill='%s'/>\n",
            buscarCoordXFormaGeometrica(forma),
            buscarCoordYFormaGeometrica(forma),
            buscarLarguraRetangulo(buscarFormaGeometrica(forma)),
            buscarAlturaRetangulo(buscarFormaGeometrica(forma)),
            buscarCorBordaRetangulo(buscarFormaGeometrica(forma)),
            buscarCorPreenchimentoRetangulo(buscarFormaGeometrica(forma)));

}

/*****************************************************************************************************/
/*
void tagLinha(FILE *arquivo, FormaGeometricaGenerica forma){

    fprintf(arquivo, "<line x1='%lf' y1='%lf' x2='%lf' y2='%lf' stroke='%s'/>\n",
            buscarX1Linha(buscarFormaGeometrica(forma)),
            buscarY1Linha(buscarFormaGeometrica(forma)),
            buscarX2Linha(buscarFormaGeometrica(forma)),
            buscarY2Linha(buscarFormaGeometrica(forma)),
            buscarCorLinha(buscarFormaGeometrica(forma)));

}*/

/*****************************************************************************************************/

void tagCabecalho(FILE* arquivo){

    if(arquivo == NULL){
        printf("Erro: Nao foi possivel inserir a tag do cabecalho no arquivo svg!\n");
        return;
    }

    // Primeira linha obrigatória para arquivos SVG bem formados
    fprintf(arquivo, "<?xml version='1.0' encoding='UTF-8'?>\n");
    // Tag de abertura correta do SVG
    fprintf(arquivo, "<svg xmlns='http://www.w3.org/2000/svg' version='1.1' "
    "width='100%%' height='100%%' viewBox='0 0 4000 12000'>\n");

}

/*****************************************************************************************************/

void tagRodape(FILE* arquivo){

    if(arquivo == NULL){
        printf("Erro: Nao foi possivel inserir a tag do rodape no arquivo svg!\n");
        return;
    }

    fprintf(arquivo, "</svg>\n");
    
}

/*****************************************************************************************************/

void tagForma(GrafoGenerico g, NoGenerico n, FormaGeometricaGenerica f, double x, double y, void* aux){

    FILE* arquivo = (FILE*)aux;
    FormaGeometricaGenerica forma = (FormaGeometricaGenerica)f;
    char tipo = buscarTipoFormaGeometrica(forma);

    if(arquivo == NULL){
        printf("Erro: Falha na insercao das tags SVG no arquivo!\n");
        return;
    }

    switch(tipo){
        case 'r':
            tagRetangulo(arquivo, forma);
            break;
        /*
        case 'l':
            tagLinha(arquivo, forma);
            break;*/

        default:
            printf("Aviso: Tipo de forma '%c' desconhecido.\n", tipo);
            break;
    }
    
}

/*****************************************************************************************************/