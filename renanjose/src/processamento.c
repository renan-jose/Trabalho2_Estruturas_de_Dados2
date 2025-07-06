/* Importação das bibliotecas para a implementação do processamento do arquivo .qry. */
#include "processamento.h"
#include "smutreap.h"
#include "forma.h"
#include "arquivo.h"
#include "tag.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SELECOES 100

/*****************************************************************************************************/

/* Função de busca auxiliar usada para localizar uma forma pelo ID.
 * Parâmetros:
 * - SmuTreap: árvore de formas.
 * - Node: nó atual.
 * - Info: informação do nó (forma).
 * - double x, y: não utilizados aqui.
 * - void *aux: ponteiro para o ID buscado (int*).
 * Retorno:
 * - true se o ID for igual ao procurado, false caso contrário.
 */
static bool buscarPorId(SmuTreap t, Node n, Info i, double x, double y, void* aux){

    int idBuscado = *(int*)aux;
    int idAtual = buscarIdFormaGeometrica(i);

    if(idAtual == idBuscado){
        return true;
    }

    return false;

}

/*****************************************************************************************************/

/* Processa o comando 'selr' do arquivo .qry.
 * Seleciona as formas inteiramente contidas dentro do retângulo (x,y,w,h) e as identifica conjuntamente 
 * com o número n (0..99).
 * No arquivo .txt: reportar o identificador e o tipo da forma selecionada
 * No arquivo .svg: colocar uma pequena circunferência vermelha na âncora da forma selecionada. Desenha
 * o retângulo (x,y,w,h) em vermelho.
 * Parâmetros:
 * - ArvoreGenerica: árvore com as formas.
 * - char *: linha de comando do .qry.
 * - FILE *: arquivo .txt para logs.
 * - Lista: lista para anotações gráficas no SVG.
 * - Lista[]: vetor de seleções.
 */
static void processarSelr(ArvoreGenerica t, char *linha, FILE* txt, Lista anotacoesSvg, Lista selecoes[]){

    int n;
    double x, y, w, h;
    
    if(sscanf(linha, "selr %d %lf %lf %lf %lf", &n, &x, &y, &w, &h) != 5) {
        fprintf(stderr, "Erro ao ler parametros de selr!\n");
        return;
    }

    if(n < 0 || n >= MAX_SELECOES){
        fprintf(stderr, "selr %d: indice de seleção invalido!\n", n);
        return;
    }

    double x2 = x + w, y2 = y + h;

    // Libera seleção anterior se existir
    if(selecoes[n] != NULL){
        desalocarLista(selecoes[n]);
    }

    Lista resultados = inicializarLista();

    getInfosDentroRegiaoSmuT(t, x, y, x2, y2, formaDentroRegiao, resultados);

    int tamanho = buscarTamanhoLista(resultados), i;

    for(i = 0; i < tamanho; i++){
        NoGenerico no = buscarElementoLista(resultados, i);
        FormaGeometricaGenerica forma = getInfoSmuT(t, no);
        if(forma == NULL){
            fprintf(stderr, "Erro: forma nula retornada de getInfoSmuT.\n");
            continue;
        }

        char tipo = buscarTipoFormaGeometrica(forma);
        int id = buscarIdFormaGeometrica(forma);
        double ancX = buscarCoordXFormaGeometrica(forma);
        double ancY = buscarCoordYFormaGeometrica(forma);

        fprintf(txt, "[*] selr %d \nTipo: %c \nId: %d\n", n, tipo, id);

        FormaGeometricaGenerica circ = formularCirculo(0, ancX, ancY, 0.8, "red", "red");

        listar(anotacoesSvg, circ);
    }

    FormaGeometricaGenerica selecao = formularRetangulo(0, x, y, w, h, "red", "none");
    listar(anotacoesSvg, selecao);

    // Salva a seleção no vetor global
    selecoes[n] = resultados;

}

/*****************************************************************************************************/

/* Processa o comando 'seli' do arquivo .qry.
 * Similar a selr, porém, seleciona uma única forma, cuja âncora é a coordenada (x,y).
 * Parâmetros:
 * - ArvoreGenerica: árvore com as formas.
 * - char *: linha de comando do .qry.
 * - FILE *: arquivo .txt para logs.
 * - FILE *: arquivo .svg para marcação gráfica.
 * - Lista[]: vetor de seleções.
 */
static void processarSeli(ArvoreGenerica t, char *linha, FILE* txt, FILE* svg, Lista selecoes[]){

    int n;
    double x, y;

    if(sscanf(linha, "seli %d %lf %lf", &n, &x, &y) != 3){
        fprintf(stderr, "Erro ao ler parametros de seli!\n");
        return;
    }

    if(n < 0 || n >= MAX_SELECOES){
        fprintf(stderr, "seli: indice de seleção invalido (%d)!\n", n);
        return;
    }

    // Remove seleção anterior, se existir
    if(selecoes[n]){
        desalocarLista(selecoes[n]);
        selecoes[n] = NULL;
    }

    // Obtem o nó da forma cuja âncora é (x, y)
    NoGenerico no = getNodeSmuT((SmuTreap)t, x, y);

    if(no == NULL){
        fprintf(txt, "seli: Nenhuma forma encontrada na coordenada (%.2lf, %.2lf)\n", x, y);
        return;
    }

    FormaGeometricaGenerica forma = getInfoSmuT((SmuTreap)t, no);

    if(forma == NULL){
        return; 
    }

    // Armazena na nova seleção
    selecoes[n] = inicializarLista();
    listar(selecoes[n], no);

    // Log e destaque
    fprintf(txt, "[*] seli \nTipo: %c \nId: %d selecionado para selecao %d\n", buscarTipoFormaGeometrica(forma), buscarIdFormaGeometrica(forma), n);

    double ancX = buscarCoordXFormaGeometrica(forma);
    double ancY = buscarCoordYFormaGeometrica(forma);

    FormaGeometricaGenerica circ = formularCirculo(0, ancX, ancY, 0.8, "red", "red");
    tagCirculo(svg, circ);

    free(circ);

}

/*****************************************************************************************************/

/* Processa o comando 'disp', do arquivo .qry.
 * Dispara as ogivas da seleção n na direção dada pela linha de identificador id. A distância depende da área
 * da forma disparada. Destrói as formas para as quais a âncora da forma disparada é interna à forma atingida 
 * (para mais detalhes, veja seção específica). As ogivas são destruídas.
 * No arquivo .txt: reportar identificador, tipo da forma disparada, a posição inicial, a posição final, a distância
 * de lançamento e o identificador, tipo e posição das respectivas formas atingidas.
 * No arquivo .svg: colocar um pequeno # no local da explosão da ogiva e um pequeno x nas âncoras das formas destruídas.
 * Parâmetros:
 * - ArvoreGenerica: árvore com as formas.
 * - char *: linha de comando do .qry.
 * - FILE *: arquivo .txt para logs.
 * - FILE *: arquivo .svg para desenhar disparos e efeitos.
 * - Lista[]: vetor de seleções.
 */
static void processarDisp(ArvoreGenerica t, char *linha, FILE* txt, FILE* svg, Lista selecoes[]){

    int idLinha, n;

    if(sscanf(linha, "disp %d %d", &idLinha, &n) != 2){
        fprintf(stderr, "Erro ao ler parametros do comando disp!\n");
        return;
    }

    fprintf(txt, "[*] disp %d %d\n", idLinha, n);

    if(n < 0 || n >= MAX_SELECOES || selecoes[n] == NULL){
        fprintf(txt, "  Selecao %d invalida ou inexistente.\n", n);
        return;
    }

    Node linhaNode = procuraNoSmuT((SmuTreap)t, buscarPorId, &idLinha);

    if(linhaNode == NULL){
        fprintf(txt, "  Linha com id %d nao encontrada.\n", idLinha);
        return;
    }

    FormaGeometricaGenerica formaLinha = getInfoSmuT((SmuTreap)t, linhaNode);

    if(formaLinha == NULL || buscarTipoFormaGeometrica(formaLinha) != 'l') {
        fprintf(txt, "  ID %d não corresponde a uma linha.\n", idLinha);
        return;
    }

    // Calcula bounding box da linha
    double x, y, w, h;
    calcularBbLinha(buscarFormaGeometrica(formaLinha), &x, &y, &w, &h);

    double x1 = x;
    double y1 = y;
    double x2 = x + w;
    double y2 = y + h;

    double dx = x2 - x1;
    double dy = y2 - y1;
    double magnitude = sqrt(dx * dx + dy * dy);

    if(magnitude == 0){
        fprintf(txt, "  Linha %d com direção nula.\n", idLinha);
        return;
    }

    dx /= magnitude;
    dy /= magnitude;

    int tam = buscarTamanhoLista(selecoes[n]), i;

    for(i = 0; i < tam; i++){
        Node ogivaNode = buscarElementoLista(selecoes[n], i);

        if(ogivaNode == NULL){
            continue;
        } 

        FormaGeometricaGenerica ogiva = getInfoSmuT((SmuTreap)t, ogivaNode);

        if(ogiva == NULL){
            continue;
        }

        double cx, cy, w, h;

        getBoundingBoxSmuT((SmuTreap)t, ogivaNode, &cx, &cy, &w, &h);

        double area = w * h;
        double dist = sqrt(area);

        double fx = cx;
        double fy = cy;
        double tx = fx + dx * dist;
        double ty = fy + dy * dist;

        FormaGeometricaGenerica disparo = formularLinha(-1, fx, fy, tx, ty, "red");

        if(disparo != NULL){
            tagLinha(svg, disparo);
        } 

        FormaGeometricaGenerica marca = formularTexto(-1, fx, fy, "red", "red", 'i', "#", "sans", "normal", "12px");

        if(marca != NULL){
            tagTexto(svg, marca);
        }

        Lista atingidas = inicializarLista();

        getInfosAtingidoPontoSmuT((SmuTreap)t, tx, ty, pontoInternoforma, atingidas);

        int qnt = buscarTamanhoLista(atingidas), j;

        for(j = 0; j < qnt; j++){
            Node alvo = buscarElementoLista(atingidas, j);
            FormaGeometricaGenerica f = getInfoSmuT((SmuTreap)t, alvo);

            if(f != NULL || f == ogiva){
                continue;
            }

            int fid = buscarIdFormaGeometrica(f);
            char tipoF = buscarTipoFormaGeometrica(f);
            double ax = buscarCoordXFormaGeometrica(f);
            double ay = buscarCoordYFormaGeometrica(f);

            fprintf(txt, "  Atingida: \nTipo: %c \nId: %d \nAncora:(%.2lf, %.2lf)\n", tipoF, fid, ax, ay);

            FormaGeometricaGenerica marcaHit = formularTexto(-1, ax, ay, "red", "red", 'i', "x", "sans", "normal", "10px");
            if(marcaHit != NULL){
                tagTexto(svg, marcaHit);
            }

            removeNoSmuT((SmuTreap)t, alvo);

        }

        desalocarLista(atingidas);

    }
    
}

/*****************************************************************************************************/

/* Processa o comando 'transp', do arquivo .qry.
 * Move a forma de identificador id para a posição (x,y).
 * No arquivo .txt: reportar tipo e posição original da forma.
 * Parâmetros:
 * - ArvoreGenerica: árvore com as formas.
 * - char *: linha de comando do .qry.
 * - FILE *: arquivo .txt para logs.
 * - FILE *: arquivo .svg para desenhar a translação.
 */
static void processarTransp(ArvoreGenerica t, char *linha, FILE* txt, FILE* svg){
    
    int id;
    double novoX, novoY;

    if(sscanf(linha, "transp %d %lf %lf", &id, &novoX, &novoY) != 3){
        fprintf(stderr, "Erro ao ler parâmetros de transp.\n");
        return;
    }

    Node no = procuraNoSmuT((SmuTreap)t, buscarPorId, &id);

    if(no == NULL){
        fprintf(txt, "transp %d: forma nao encontrada\n", id);
        return;
    }

    FormaGeometricaGenerica forma = getInfoSmuT((SmuTreap)t, no);
    if(forma == NULL){
        fprintf(stderr, "Erro: forma nula retornada de getInfoSmuT.\n");
        return;
    }

    char tipo = buscarTipoFormaGeometrica(forma);
    double xOrig = buscarCoordXFormaGeometrica(forma);
    double yOrig = buscarCoordYFormaGeometrica(forma);

    fprintf(txt, "[*] transp %d \nTipo: %c: \nOrigem: (%.2lf, %.2lf) \nDestino:(%.2lf, %.2lf)\n",
            id, tipo, xOrig, yOrig, novoX, novoY); // escrever melhor aqui

    // Desenha linha tracejada vermelha entre origem e destino
    FormaGeometricaGenerica linhaVermelha = formularLinha(0, xOrig, yOrig, novoX, novoY, "red");
    tagLinha(svg, linhaVermelha);

    free(linhaVermelha);
   
    // Atualiza posição
    removeNoSmuT((SmuTreap)t, no);
    mudarCoordXFormaGeometrica(forma, novoX);
    mudarCoordYFormaGeometrica(forma, novoY);
    insertSmuT((SmuTreap)t, novoX, novoY, forma, tipo, calcularBbFormaGeometrica);

    tagForma(t, NULL, forma, novoX, novoY, svg);

}

/*****************************************************************************************************/

/* Processa o comando 'cln', do arquivo .qry.
 * Clona as formas da seleção n. Os clones são transladados por dx, dy. O clone terá as cores de
 * borda e preenchimento trocadas.
 * Parâmetros:
 * - ArvoreGenerica: árvore com as formas.
 * - char *: linha de comando do .qry.
 * - FILE *: arquivo .txt para logs.
 * - FILE *: arquivo .svg para desenhar os clones.
 * - Lista[]: vetor de seleções.
 */
static void processarCln(ArvoreGenerica t, char *linha, FILE* txt, FILE* svg, Lista selecoes[]){

    int n;
    double dx, dy;

    if(sscanf(linha, "cln %d %lf %lf", &n, &dx, &dy) != 3){
        fprintf(stderr, "Erro ao ler parâmetros do comando cln.\n");
        return;
    }

    if(n < 0 || n >= MAX_SELECOES || selecoes[n] == NULL){
        fprintf(txt, "cln %d: seleção inválida ou inexistente.\n", n);
        return;
    }

    int tam = buscarTamanhoLista(selecoes[n]), i;

    for(i = 0; i < tam; i++){

        Node originalNode = buscarElementoLista(selecoes[n], i);

        if(originalNode == NULL){
            continue;
        }

        FormaGeometricaGenerica original = getInfoSmuT((SmuTreap)t, originalNode);

        if(original == NULL){
            continue;
        }

        // Pegando cores invertidas
        char* corb = buscarCorPreenchimentoFormaGeometrica(original);
        char* corp = buscarCorBordaFormaGeometrica(original);

        FormaGeometricaGenerica clone = clonarForma(original, dx, dy, corb, corp);

        if(clone == NULL){
            continue;
        }
        
        double x = buscarCoordXFormaGeometrica(clone);
        double y = buscarCoordYFormaGeometrica(clone);
        char tipo = buscarTipoFormaGeometrica(clone);

        DescritorTipoInfo descritor = tipoFormaParaDescritor(tipo);

        insertSmuT(t, x, y, clone, descritor, calcularBbFormaGeometrica);

    }

}

/*****************************************************************************************************/

/* Processa o comando 'spy', do arquivo .qry.
 * No arquivo .txt: Reporta dados sobre as formas inteiramente contidas dentro do retângulo de identificador 
 * id ou exatamente na coordenada do texto de identificador id.
 * Parâmetros:
 * - ArvoreGenerica: árvore com as formas.
 * - char *: linha de comando do .qry.
 * - FILE *: arquivo .txt para logs.
 * - FILE *: arquivo .svg para marcação visual.
 * - Lista: lista de anotações para o SVG.
 */
static void processarSpy(ArvoreGenerica t, char *linha, FILE *txt, FILE *svg, Lista anotacoesSvg){
    int id;

    if(sscanf(linha, "spy %d", &id) != 1){
        fprintf(stderr, "Erro ao ler parametros do comando spy.\n");
        return;
    }

    fprintf(txt, "[*] spy %d\n", id);

    Node alvo = procuraNoSmuT((SmuTreap)t, buscarPorId, &id);

    if(alvo == NULL){
        fprintf(txt, "  Forma com id %d nao encontrada.\n", id);
        return;
    }

    FormaGeometricaGenerica forma = getInfoSmuT((SmuTreap)t, alvo);

    if(forma == NULL){
        fprintf(txt, "  Erro ao obter a forma com id %d.\n", id);
        return;
    }

    char tipo = buscarTipoFormaGeometrica(forma);
    Lista resultado = inicializarLista();

    if(tipo == 'r'){
        double x = buscarCoordXFormaGeometrica(forma);
        double y = buscarCoordYFormaGeometrica(forma);
        double w = buscarLarguraFormaGeometrica(forma);
        double h = buscarAlturaFormaGeometrica(forma);

        getInfosDentroRegiaoSmuT((SmuTreap)t, x, y, x + w, y + h, formaDentroRegiao, resultado);

        // Círculo de destaque com raio limitado
        double cx = x + w / 2.0;
        double cy = y + h / 2.0;
        double raio = 5.0;

        FormaGeometricaGenerica destaque = formularCirculo(-1, cx, cy, raio, "red", "none");

        if(destaque != NULL){
            listar(anotacoesSvg, destaque);
        }

    }else if (tipo == 't'){

        double x = buscarCoordXFormaGeometrica(forma);
        double y = buscarCoordYFormaGeometrica(forma);

        getInfosAtingidoPontoSmuT((SmuTreap)t, x, y, pontoInternoforma, resultado);

        // Círculo pequeno para marcar texto
        FormaGeometricaGenerica destaque = formularCirculo(-1, x, y, 5.0, "red", "none");

        if(destaque != NULL){
            listar(anotacoesSvg, destaque);
        }

    }else{
        fprintf(txt, "  Tipo da forma com id %d nao suportado pelo comando spy.\n", id);
        desalocarLista(resultado);
        return;
    }

    // Relatório textual — agora acessando Info via getInfoSmuT(Node)
    int tam = buscarTamanhoLista(resultado), i;

    for(i = 0; i < tam; i++){
        Node no = buscarElementoLista(resultado, i);

        FormaGeometricaGenerica f = getInfoSmuT((SmuTreap)t, no);

        if(f == NULL){
            continue;
        }

        int fid = buscarIdFormaGeometrica(f);

        if(fid == -1){
            continue;  
        }

        char tipoF = buscarTipoFormaGeometrica(f);
        double fx = buscarCoordXFormaGeometrica(f);
        double fy = buscarCoordYFormaGeometrica(f);

        fprintf(txt, "Tipo: %c \nId: %d \nAncora: (%.2lf, %.2lf)\n", tipoF, fid, fx, fy);
    }

    desalocarLista(resultado);

}

/*****************************************************************************************************/

/* Processa o comando 'cmflg', do arquivo.qry.
 * Semelhante ao comando spy, porém, muda a cor da borda para cb, preenchimento cp e largura da borda
 * para w px.
 * Parâmetros:
 * - ArvoreGenerica: árvore com as formas.
 * - char *: linha de comando do .qry.
 * - FILE *: arquivo .txt para logs.
 * - FILE *: arquivo .svg para anotações visuais.
 */
static void processarCmflg(ArvoreGenerica t, char* linha, FILE* txt, FILE* svg){

    int id;
    char corBorda[32], corPreench[32];
    double largura;  

    if(sscanf(linha, "cmflg %d %s %s %lf", &id, corBorda, corPreench, &largura) != 4){
        fprintf(stderr, "Erro ao ler parametros do comando cmflg.\n");
        return;
    }

    fprintf(txt, "[*] cmflg %d\n", id);

    Node alvo = procuraNoSmuT((SmuTreap)t, buscarPorId, &id);

    if(alvo == NULL){
        fprintf(txt, "  Forma com id %d nao encontrada.\n", id);
        return;
    }

    FormaGeometricaGenerica forma = getInfoSmuT((SmuTreap)t, alvo);
    if(forma == NULL){
        fprintf(txt, "  Erro ao acessar forma com id %d.\n", id);
        return;
    }

    char tipo = buscarTipoFormaGeometrica(forma);

    if(tipo == 'r'){
        mudarCorBordaRetangulo(buscarFormaGeometrica(forma), corBorda);
        mudarCorPreenchimentoRetangulo(buscarFormaGeometrica(forma), corPreench);
    }else if (tipo == 'c'){
        mudarCorBordaCirculo(buscarFormaGeometrica(forma), corBorda);
        mudarCorPreenchimentoCirculo(buscarFormaGeometrica(forma), corPreench);
    }else if (tipo == 'l'){
        mudarCorLinha(buscarFormaGeometrica(forma), corBorda);
    }else if(tipo == 't'){
        mudarCorBordaTexto(buscarFormaGeometrica(forma), corBorda);
        mudarCorPreenchimentoTexto(buscarFormaGeometrica(forma), corPreench);
    }else{
        fprintf(txt, "  Tipo da forma com id %d desconhecido.\n", id);
        return;
    }

}

/*****************************************************************************************************/

/* Processa o comando 'blow', que simula uma explosão na âncora de uma forma, removendo formas atingidas.
 * A ogiva cujo identificar é id explode. A explosão produz os mesmos efeitos da explosão do comando disp.
 * No arquivo .txt: reporta os atributos da figura removida e os atributos das figuras atingidas.
 * Parâmetros:
 * - ArvoreGenerica: árvore com as formas.
 * - char *: linha de comando do .qry.
 * - FILE *: arquivo .txt para logs.
 * - FILE *: arquivo .svg para anotações visuais.
 * - Lista: lista de anotações para o SVG.
 */
static void processarBlow(ArvoreGenerica t, char* linha, FILE* txt, FILE* svg, Lista anotacoesSvg){

    int id;

    if(sscanf(linha, "blow %d", &id) != 1){
        fprintf(stderr, "Erro ao ler parametros do comando blow.\n");
        return;
    }

    fprintf(txt, "[*] blow %d\n", id);

    Node ogivaNode = procuraNoSmuT((SmuTreap)t, buscarPorId, &id);
    if(ogivaNode == NULL){
        fprintf(txt, "  Ogiva com id %d nao encontrada.\n", id);
        return;
    }

    FormaGeometricaGenerica ogiva = getInfoSmuT((SmuTreap)t, ogivaNode);
    if(ogiva == NULL){
        fprintf(txt, "  Erro ao acessar a ogiva com id %d.\n", id);
        return;
    }

    // Obtem âncora da ogiva
    double x, y, w, h;
    getBoundingBoxSmuT((SmuTreap)t, ogivaNode, &x, &y, &w, &h);

    char tipo = buscarTipoFormaGeometrica(ogiva);
    double area = w * h;
    double dist = sqrt(area);  // distância de disparo baseada na área

    // Calcula retângulo de alcance da explosão
    double x1 = x - dist;
    double y1 = y - dist;
    double x2 = x + dist;
    double y2 = y + dist;

    Lista atingidas = inicializarLista();
    getInfosAtingidoPontoSmuT((SmuTreap)t, x, y, pontoInternoforma, atingidas);

    int tam = buscarTamanhoLista(atingidas);
    for (int i = 0; i < tam; i++) {
        Node alvo = buscarElementoLista(atingidas, i);

        FormaGeometricaGenerica forma = getInfoSmuT((SmuTreap)t, alvo);

        if(forma == NULL || forma == ogiva){
            continue;
        }

        int fid = buscarIdFormaGeometrica(forma);
        char tipoF = buscarTipoFormaGeometrica(forma);
        double fx = buscarCoordXFormaGeometrica(forma);
        double fy = buscarCoordYFormaGeometrica(forma);

        fprintf(txt, "Atingida: \nTipo %c \nId: %d \nAncora: (%.2lf, %.2lf)\n", tipoF, fid, fx, fy);

        // Anotação visual: 'x' na âncora da forma atingida
        FormaGeometricaGenerica marca = formularTexto(-1, fx, fy, "red", "red", 'i', "x", "sans", "normal", "10px");

        if(marca != NULL){
            listar(anotacoesSvg, marca);
        }

        removeNoSmuT((SmuTreap)t, alvo);
    }

    // Marca local da explosão com #
    FormaGeometricaGenerica marca = formularTexto(-1, x, y, "red", "red", 'i', "#", "sans", "normal", "12px");

    if(marca != NULL){
        listar(anotacoesSvg, marca);
    }

    // Descreve ogiva destruída
    fprintf(txt, "Destruida: \nTipo %c \nId: %d \nAncora: (%.2lf, %.2lf) \nDistancia: %.2lf\n", tipo, id, x, y, dist);
    removeNoSmuT((SmuTreap)t, ogivaNode);

    desalocarLista(atingidas);
}



/*****************************************************************************************************/

/* Função principal que processa todos os comandos do arquivo .qry.
 * Parâmetros:
 * - ArvoreGenerica: árvore de formas.
 * - char *: caminho completo do arquivo .qry.
 * - char *: caminho completo do arquivo .svg de saída (após o .qry).
 * - char *: caminho completo do arquivo .txt de saída.
 * - char *: caminho completo do arquivo .csv de saída.
 */
void processarFormas(ArvoreGenerica t, char *caminhoQry, char *caminhoSvg, char *caminhoTxt, char *caminhoCsv) {

    FILE *arquivoQry = fopen(caminhoQry, "r");

    if(arquivoQry == NULL){
        printf("Erro: Nao foi possível abrir o arquivo .qry: %s\n", caminhoQry);
        return;
    }

    FILE *arquivoTxt = fopen(caminhoTxt, "w");

    if(arquivoTxt == NULL){
        printf("Erro: Nao foi possível criar o arquivo .txt: %s\n", caminhoTxt);
        fclose(arquivoQry);
        return;
    }

    FILE *arquivoSvg = fopen(caminhoSvg, "w");

    if(arquivoSvg == NULL){
        printf("Erro: Nao foi possível criar o arquivo .svg (qry): %s\n", caminhoSvg);
        fclose(arquivoQry);
        fclose(arquivoTxt);
        return;
    }

    // Começa o SVG
    tagCabecalho(arquivoSvg);

    Lista anotacoesSvg = inicializarLista();
    Lista selecoes[MAX_SELECOES] = {NULL};

    char linha[512];

    while (fgets(linha, sizeof(linha), arquivoQry)){
        if(linha[0] == '#' || linha[0] == '\n'){
            continue;
        }

        if(strncmp(linha, "selr", 4) == 0){
            processarSelr(t, linha, arquivoTxt, anotacoesSvg, selecoes);
        } else if (strncmp(linha, "seli", 4) == 0){
            processarSeli(t, linha, arquivoTxt, arquivoSvg, selecoes);
        }else if(strncmp(linha, "disp", 4) == 0){
            processarDisp(t, linha, arquivoTxt, arquivoSvg, selecoes);
        }else if(strncmp(linha, "transp", 6) == 0){
            processarTransp(t, linha, arquivoTxt, arquivoSvg);
        }else if(strncmp(linha, "cln", 3) == 0){
            processarCln(t, linha, arquivoTxt, arquivoSvg, selecoes);
        } else if(strncmp(linha, "spy", 3) == 0){ 
            processarSpy(t, linha, arquivoTxt, arquivoSvg, anotacoesSvg);
        }else if(strncmp(linha, "cmflg", 5) == 0){
            processarCmflg(t, linha, arquivoTxt, arquivoSvg);
        } else if (strncmp(linha, "blow", 4) == 0){
            processarBlow(t, linha, arquivoTxt, arquivoSvg, anotacoesSvg);
        }

    }

    visitaProfundidadeSmuT(t, tagForma, arquivoSvg);

    int tam = buscarTamanhoLista(anotacoesSvg), i;

    for (i = 0; i < tam; i++) {
        FormaGeometricaGenerica f = buscarElementoLista(anotacoesSvg, i);
        tagForma(t, NULL, f, 0, 0, arquivoSvg);
    }

    tagRodape(arquivoSvg);
    desalocarLista(anotacoesSvg);

    for (int i = 0; i < MAX_SELECOES; i++) {
        if (selecoes[i] != NULL) {
            desalocarLista(selecoes[i]);
        }
    }

    fclose(arquivoQry);
    fclose(arquivoTxt);
    fclose(arquivoSvg);

    // Gera CSV ao final
    gerarArquivoCsv(t, caminhoCsv);

}