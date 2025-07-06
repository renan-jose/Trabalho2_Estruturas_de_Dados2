/* Importação das bibliotecas. */
#include "arquivo.h" 
#include "tag.h"
#include "smutreap.h"
#include <stdio.h>
#include <stdlib.h>   
#include <string.h>     

/*****************************************************************************************************/

struct Parametros{

    char *diretorioEntrada, *arquivoGeo, *diretorioSaida, *arquivoQry; 
    int prioridadeMaxima, hitCount;
    double promotionRate; 

};

typedef struct Parametros Parametros;

/*****************************************************************************************************/

void gerarArquivoSvg(ArvoreGenerica t, char *nome){

    FILE* arquivoSvg = fopen(nome, "w");

    if(arquivoSvg == NULL){
        printf("Erro: Falha ao gerar o svg de saida!\n");
        return;
    }

    tagCabecalho(arquivoSvg);

    visitaProfundidadeSmuT(t, tagForma, arquivoSvg); /* Uso do percurso em profundidade para impressão da árvore em svg. */

    tagRodape(arquivoSvg);

    fclose(arquivoSvg);

}

/*****************************************************************************************************/

void gerarArquivoDot(ArvoreGenerica t, char* nome){

    printDotSmuTreap(t, nome);

}

/*****************************************************************************************************/

/* Função auxiliar */
static void escreverCsv(ArvoreGenerica t, NoGenerico n, FormaGeometricaGenerica f, double x, double y, void* aux){
    
    FILE* arquivo = (FILE*) aux;
    char tipo = buscarTipoFormaGeometrica(f);

    switch (tipo) {
        case 'r':
            fprintf(arquivo, "retangulo, %d, %lf, %lf, %lf, %lf, %s, %s\n",
                    buscarIdFormaGeometrica(f),
                    buscarCoordXFormaGeometrica(f),
                    buscarCoordYFormaGeometrica(f),
                    buscarLarguraRetangulo(buscarFormaGeometrica(f)),
                    buscarAlturaRetangulo(buscarFormaGeometrica(f)),
                    buscarCorBordaRetangulo(buscarFormaGeometrica(f)),
                    buscarCorPreenchimentoRetangulo(buscarFormaGeometrica(f)));
            break;

        case 'c':
            fprintf(arquivo, "circulo, %d, %lf, %lf, %lf, %s, %s\n",
                    buscarIdFormaGeometrica(f),
                    buscarCoordXFormaGeometrica(f),
                    buscarCoordYFormaGeometrica(f),
                    buscarRaioCirculo(buscarFormaGeometrica(f)),
                    buscarCorBordaCirculo(buscarFormaGeometrica(f)),
                    buscarCorPreenchimentoCirculo(buscarFormaGeometrica(f)));
            break;

        case 'l':
            fprintf(arquivo, "linha, %d, %lf, %lf, %lf, %lf, %s\n",
                    buscarIdFormaGeometrica(f),
                    buscarX1Linha(buscarFormaGeometrica(f)),
                    buscarY1Linha(buscarFormaGeometrica(f)),
                    buscarX2Linha(buscarFormaGeometrica(f)),
                    buscarY2Linha(buscarFormaGeometrica(f)),
                    buscarCorLinha(buscarFormaGeometrica(f)));
            break;

        case 't':
            fprintf(arquivo, "texto, %d, %lf, %lf, %s, %s, %c, \"%s\"\n",
                    buscarIdFormaGeometrica(f),
                    buscarCoordXFormaGeometrica(f),
                    buscarCoordYFormaGeometrica(f),
                    buscarCorBordaTexto(buscarFormaGeometrica(f)),
                    buscarCorPreenchimentoTexto(buscarFormaGeometrica(f)),
                    buscarPeriodoAncoraTexto(buscarFormaGeometrica(f)),
                    buscarTextoTexto(buscarFormaGeometrica(f)));
            break;
    }
}

/*****************************************************************************************************/

void gerarArquivoCsv(ArvoreGenerica t, char *nome){

    FILE* arquivoCsv = fopen(nome, "w");

    if (arquivoCsv == NULL) {
        printf("Erro: Falha ao gerar o csv de saida!\n");
        return;
    }

    visitaProfundidadeSmuT(t, escreverCsv, arquivoCsv);
    
    fclose(arquivoCsv);

}

static int strcasecmp(char *s1, char *s2){

    while (*s1 && *s2){
        int c1 = (*s1 >= 'A' && *s1 <= 'Z') ? *s1 + 32 : *s1;
        int c2 = (*s2 >= 'A' && *s2 <= 'Z') ? *s2 + 32 : *s2;
        if(c1 != c2){
            return c1 - c2;
        }
        s1++;
        s2++;
    }

    return *s1 - *s2;

}

/*****************************************************************************************************/

static int arquivo_existe(char *caminho){
    
    FILE *arquivo = fopen(caminho, "r");

    if(arquivo != NULL){
        fclose(arquivo);
        return 1;
    }

    return 0;

}

/*****************************************************************************************************/

static int verificarExtensao(char *arquivo, char *extensao_esperada){
    
    size_t len_arquivo = strlen(arquivo);
    size_t len_extensao = strlen(extensao_esperada);
    
    // Arquivo deve ser maior que a extensão
    if(len_arquivo <= len_extensao){
        return 0;
    }

    // Verificar se termina com a extensão esperada (case insensitive)
    char *extensao_arquivo = arquivo + len_arquivo - len_extensao;

    return (strcasecmp(extensao_arquivo, extensao_esperada) == 0);

}

/*****************************************************************************************************/

static char* gerarCaminho(char *dir, char *arquivo){
    
    size_t len_dir = strlen(dir);
    size_t len_arq = strlen(arquivo);
    size_t len_total = len_dir + len_arq + 2;
    
    char *caminho = (char*)malloc(len_total);

    if(caminho == NULL){
        return NULL;
    }

    // Verificar se dir já termina com '/'
    if(len_dir > 0 && dir[len_dir-1] == '/'){
        snprintf(caminho, len_total, "%s%s", dir, arquivo);
    }else{
        snprintf(caminho, len_total, "%s/%s", dir, arquivo);
    }
    
    return caminho;

}

/*****************************************************************************************************/

static char* extrairNomeBase(char *arquivo){
    
    char *nome = arquivo;
    char *p = arquivo;

    while (*p) {
        if (*p == '/' || *p == '\\') {
            nome = p + 1;
        }
        p++;
    }
    
    char *ponto = strrchr(nome, '.');
    size_t len = ponto ? (size_t)(ponto - nome) : strlen(nome);
    char *resultado = malloc(len + 1);

    if(resultado != NULL){
        strncpy(resultado, nome, len);
        resultado[len] = '\0';
    }
    
    return resultado;
}

/*****************************************************************************************************/

static int processar_argumentos_interno(int argc, char *argv[], Parametros *p) {
    
    memset(p, 0, sizeof(Parametros));
    p->prioridadeMaxima = 10000;
    p->hitCount = 3;
    p->promotionRate = 1.10;
    
    if(argc < 5){ 
        fprintf(stderr, "Erro: Argumentos insuficientes!\n");
        return 0;
    }
    
    for(int i = 1; i < argc; i++){
        if (strcmp(argv[i], "-e") == 0) {
            if(i + 1 >= argc){
                fprintf(stderr, "Erro: -e antecede um argumento!\n");
                return 0;
            }

            p->diretorioEntrada = malloc(strlen(argv[i + 1]) + 1);

            if(!p->diretorioEntrada){
                fprintf(stderr, "Erro: Falha na alocacao de memoria para o diretorio de entrada!\n");
                return 0;
            }

            strcpy(p->diretorioEntrada, argv[i + 1]);
            i++; 
        }
        else if(strcmp(argv[i], "-f") == 0){
            if(i + 1 >= argc){
                fprintf(stderr, "Erro: -f antecede um argumento!\n");
                return 0;
            }
            
          
            if(!verificarExtensao(argv[i + 1], ".geo")){
                fprintf(stderr, "Erro: Arquivo deve ter extensao .geo!\n");
                fprintf(stderr, "Arquivo fornecido: %s\n", argv[i + 1]);
                return 0;
            }
            
            p->arquivoGeo = malloc(strlen(argv[i + 1]) + 1);

            if(!p->arquivoGeo){
                fprintf(stderr, "Erro: Falha na alocacao de memoria para o arquivo .geo!\n");
                return 0;
            }

            strcpy(p->arquivoGeo, argv[i + 1]);
            i++; 
        }
        else if(strcmp(argv[i], "-o") == 0){
            if(i + 1 >= argc){
                fprintf(stderr, "Erro: -o antecede um argumento!\n");
                return 0;
            }

            p->diretorioSaida = malloc(strlen(argv[i + 1]) + 1);

            if(!p->diretorioSaida){
                fprintf(stderr, "Erro: Falha na alocacao de memoria para o diretorio de saida!\n");
                return 0;
            }

            strcpy(p->diretorioSaida, argv[i + 1]);
            i++; 
        }
        else if(strcmp(argv[i], "-q") == 0){
            if(i + 1 >= argc){
                fprintf(stderr, "Erro: -q antecede um argumento!\n");
                return 0;
            }
            
            if(!verificarExtensao(argv[i + 1], ".qry")){
                fprintf(stderr, "Erro: Arquivo de consulta deve ter extensao .qry\n");
                fprintf(stderr, "Arquivo fornecido: %s\n", argv[i + 1]);
                return 0;
            }
            
            p->arquivoQry = malloc(strlen(argv[i + 1]) + 1);

            if(!p->arquivoQry){
                fprintf(stderr, "Erro: Falha na alocacao de memoria para o arquivo .qry!\n");
                return 0;
            }

            strcpy(p->arquivoQry, argv[i + 1]);
            i++; 
        }
        else if(strcmp(argv[i], "-p") == 0){
            if (i + 1 >= argc) {
                fprintf(stderr, "Erro: -p antecede um numero inteiro!\n");
                return 0;
            }

            int valor = atoi(argv[i + 1]);

            if(valor <= 0){
                fprintf(stderr, "Erro: -p deve ser um numero inteiro positivo!\n");
                return 0;
            }

            p->prioridadeMaxima = valor;
            i++;
        }
        else if(strcmp(argv[i], "-hc") == 0){
            if(i + 1 >= argc){
                fprintf(stderr, "Erro: -hc antecede um numero inteiro!\n");
                return 0;
            }

            int valor = atoi(argv[i + 1]);

            if(valor <= 0){
                fprintf(stderr, "Erro: -hc deve ser um numero inteiro positivo!\n");
                return 0;
            }

            p->hitCount = valor;
            i++;
        }
        else if(strcmp(argv[i], "-pr") == 0){
            if(i + 1 >= argc){
                fprintf(stderr, "Erro: -pr antecede um numero decimal\n");
                return 0;
            }

            double valor = atof(argv[i + 1]);

            if (valor <= 0.0) {
                fprintf(stderr, "Erro: -pr deve ser um numero decimal positivo!\n");
                return 0;
            }

            p->promotionRate = valor;
            i++;
        }
        else{
            fprintf(stderr, "Erro: Argumento desconhecido '%s'\n", argv[i]);
            return 0;
        }
    }
    
    return 1;

}

/*****************************************************************************************************/

static int confirmarParametros(Parametros *p){

    if(p->arquivoGeo == NULL){
        fprintf(stderr, "Erro: Parametro -f (arquivo .geo) e obrigatorio\n");
        return 0;
    }
    
    if(p->diretorioSaida == NULL){
        fprintf(stderr, "Erro: Parametro -o (diretório de saida) e obrigatorio\n");
        return 0;
    }
    
    if(p->diretorioEntrada == NULL){
        p->diretorioEntrada = malloc(2);
        if(p->diretorioEntrada == NULL){
            fprintf(stderr, "Erro: Falha na alocação de memória\n");
            return 0;
        }
        strcpy(p->diretorioEntrada, ".");
    }
    
    char *caminho_geo = gerarCaminho(p->diretorioEntrada, p->arquivoGeo);

    if(!caminho_geo){
        fprintf(stderr, "Erro: Falha ao criar caminho do arquivo .geo!\n");
        return 0;
    }
    
    if(!arquivo_existe(caminho_geo)){
        fprintf(stderr, "Erro: Arquivo .geo '%s' não encontrado\n", caminho_geo);
        free(caminho_geo);
        return 0;
    }

    free(caminho_geo);
    
    if (p->arquivoQry){
        char *caminho_qry = gerarCaminho(p->diretorioEntrada, p->arquivoQry);
        if(!caminho_qry){
            fprintf(stderr, "Erro: Falha ao criar caminho do arquivo .qry\n");
            return 0;
        }
        
        if(!arquivo_existe(caminho_qry)){
            fprintf(stderr, "Erro: Arquivo de consultas .qry '%s' não encontrado\n", caminho_qry);
            free(caminho_qry);
            return 0;
        }
        free(caminho_qry);
    }
    
    char *teste_saida = gerarCaminho(p->diretorioSaida, "test.tmp");
    if (!teste_saida) {
        fprintf(stderr, "Erro: Falha ao criar caminho de teste!\n");
        return 0;
    }
    
    FILE *fp = fopen(teste_saida, "w");

    if(!fp){
        fprintf(stderr, "Erro: Diretorio de saida '%s' não existe ou não e gravavel!\n", p->diretorioSaida);
        free(teste_saida);
        return 0;
    }

    fclose(fp);
    remove(teste_saida);
    free(teste_saida);
    
    return 1;

}

/*****************************************************************************************************/

ParametrosGenericos criarParametros(){

    Parametros *novosParametros = (Parametros*)malloc(sizeof(Parametros));

    if(novosParametros != NULL){
        memset(novosParametros, 0, sizeof(Parametros));
    }

    return novosParametros;

}

/*****************************************************************************************************/

int processarArgumentos(ParametrosGenericos p, int argc, char *argv[]){
    
    Parametros *parametros = (Parametros*)p;
    
    if(!processar_argumentos_interno(argc, argv, parametros)){
        return 0;
    }
    
    if(!confirmarParametros(parametros)){
        return 0;
    }
    
    return 1;

}

/*****************************************************************************************************/

void desalocarParametros(ParametrosGenericos p){
    
    Parametros *parametros = (Parametros*)p;
    
    free(parametros->diretorioEntrada);
    free(parametros->diretorioSaida);
    free(parametros->arquivoGeo);
    free(parametros->arquivoQry);
    free(parametros);

}

/*****************************************************************************************************/

char *buscarDiretorioEntrada(ParametrosGenericos p){
    
    Parametros *parametros = (Parametros*)p;

    return parametros->diretorioEntrada;
    
}

/*****************************************************************************************************/

char *buscarArquivoGeo(ParametrosGenericos p){

    Parametros *parametros = (Parametros*)p;

    return parametros->arquivoGeo;

}

/*****************************************************************************************************/

char *buscarDiretorioSaida(ParametrosGenericos p){
    
    Parametros *parametros = (Parametros*)p;

    return parametros->diretorioSaida;

}

/*****************************************************************************************************/

char *buscarArquivoQry(ParametrosGenericos p){
    
    Parametros *parametros = (Parametros*)p;

    return parametros->arquivoQry;

}

/*****************************************************************************************************/

int temArquivoQry(ParametrosGenericos p){

    Parametros *parametros = (Parametros*)p;

    if(parametros->arquivoQry != NULL){
        return 1;
    }else{
        return 0;
    }

}

/*****************************************************************************************************/

int buscarPrioridadeMax(ParametrosGenericos p){

    Parametros *parametros = (Parametros*)p;

    if(parametros == NULL){
        return 10000;
    }

    return parametros->prioridadeMaxima;

}

/*****************************************************************************************************/

int buscarHitCount(ParametrosGenericos p){

    Parametros *parametros = (Parametros*)p;

    if(parametros == NULL){
        return 3;
    }

    return parametros->hitCount;

}

/*****************************************************************************************************/

double buscarPromotionRate(ParametrosGenericos p){

    Parametros *parametros = (Parametros*)p;

    if(parametros == NULL){
        return 1.10;
    }

    return parametros->promotionRate;

}

/*****************************************************************************************************/

char *buscarCaminhoCompletoGeo(ParametrosGenericos p){

    Parametros *parametros = (Parametros*)p;

    return gerarCaminho(parametros->diretorioEntrada, parametros->arquivoGeo);

}

/*****************************************************************************************************/

char *buscarCaminhoCompletoQry(ParametrosGenericos p){
 
    Parametros *parametros = (Parametros*)p;

    if(parametros->arquivoQry == NULL){
        return NULL;
    }

    return gerarCaminho(parametros->diretorioEntrada, parametros->arquivoQry);

}

/*****************************************************************************************************/

char *buscarNomeBaseGeo(ParametrosGenericos p){
    
    Parametros *parametros = (Parametros*)p;
    
    return extrairNomeBase(parametros->arquivoGeo);

}

/*****************************************************************************************************/

char *buscarNomeBaseQry(ParametrosGenericos p){
    
    Parametros *parametros = (Parametros*)p;

    if(parametros->arquivoQry == NULL){
        return NULL;
    }

    return extrairNomeBase(parametros->arquivoQry);

}

/*****************************************************************************************************/

char *buscarCaminhoSvgBase(ParametrosGenericos p){
    
    Parametros *parametros = (Parametros*)p;
    
    char *nome_base = extrairNomeBase(parametros->arquivoGeo);

    if(nome_base == NULL){
        return NULL;
    }
    
    size_t len = strlen(parametros->diretorioSaida) + strlen(nome_base) + 10;

    char *caminho = (char*)malloc(len);

    if(caminho != NULL){
        snprintf(caminho, len, "%s/%s.svg", parametros->diretorioSaida, nome_base);
    }
    
    free(nome_base);

    return caminho;

}

/*****************************************************************************************************/

char *buscarCaminhoSvgConsulta(ParametrosGenericos p){

    Parametros *parametros = (Parametros*)p;

    if(parametros->arquivoQry == NULL){
        return NULL;
    }
    
    char *nome_base_geo = extrairNomeBase(parametros->arquivoGeo);
    char *nome_base_qry = extrairNomeBase(parametros->arquivoQry);
    
    if(!nome_base_geo || !nome_base_qry){
        free(nome_base_geo);
        free(nome_base_qry);
        return NULL;
    }
    
    size_t len = strlen(parametros->diretorioSaida) + strlen(nome_base_geo) + strlen(nome_base_qry) + 15;
    char *caminho = (char*)malloc(len);

    if(caminho != NULL){
        snprintf(caminho, len, "%s/%s-%s.svg", parametros->diretorioSaida, nome_base_geo, nome_base_qry);
    }
    
    free(nome_base_geo);
    free(nome_base_qry);

    return caminho;

}

/*****************************************************************************************************/

char *buscarCaminhoTxtConsulta(ParametrosGenericos p){
   
    Parametros *parametros = (Parametros*)p;

    if(parametros->arquivoQry == NULL){
        return NULL;
    }
    
    char *nome_base_geo = extrairNomeBase(parametros->arquivoGeo);
    char *nome_base_qry = extrairNomeBase(parametros->arquivoQry);
    
    if(!nome_base_geo || !nome_base_qry){
        free(nome_base_geo);
        free(nome_base_qry);
        return NULL;
    }
    
    size_t len = strlen(parametros->diretorioSaida) + strlen(nome_base_geo) + strlen(nome_base_qry) + 15;
    char *caminho = malloc(len);

    if(caminho != NULL){
        snprintf(caminho, len, "%s/%s-%s.txt", parametros->diretorioSaida, nome_base_geo, nome_base_qry);
    }
    
    free(nome_base_geo);
    free(nome_base_qry);

    return caminho;

}

/*****************************************************************************************************/

char *buscarCaminhoDotBase(ParametrosGenericos p){

    Parametros *parametros = (Parametros*)p;
    
    char *nome_base = extrairNomeBase(parametros->arquivoGeo);

    if(nome_base == NULL){
        return NULL;
    }

    size_t len = strlen(parametros->diretorioSaida) + strlen(nome_base) + 10;
    char *caminho = malloc(len);

    if(caminho != NULL){
        snprintf(caminho, len, "%s/%s.dot", parametros->diretorioSaida, nome_base);
    }
    
    free(nome_base);

    return caminho;

}

/*****************************************************************************************************/

char *buscarCaminhoDotConsulta(ParametrosGenericos p){
    
    Parametros *parametros = (Parametros*)p;

    if(parametros->arquivoQry == NULL){
        return NULL;
    }

    char *nome_base_geo = extrairNomeBase(parametros->arquivoGeo);
    char *nome_base_qry = extrairNomeBase(parametros->arquivoQry);
    
    if(!nome_base_geo || !nome_base_qry){
        return NULL; 
    }
    
    size_t len = strlen(parametros->diretorioSaida) + strlen(nome_base_geo) + strlen(nome_base_qry) + 15;
    char *caminho = malloc(len);

    if(caminho != NULL){
        snprintf(caminho, len, "%s/%s-%s.dot", parametros->diretorioSaida, nome_base_geo, nome_base_qry);
    }
    
    free(nome_base_geo);
    free(nome_base_qry);

    return caminho;

}

/*****************************************************************************************************/