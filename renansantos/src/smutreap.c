/* Importação das bibliotecas para a implementação da árvore.*/
#include "smutreap.h"
#include "fila.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*****************************************************************************************************/

struct Ancora{

    double x, y;

};

struct BoundingBox{

    double x, y, w, h;

};

struct NoArvore{
    
    struct Ancora ancora;
    Info informacao;
    DescritorTipoInfo descritor;
    int prioridade, qtdSelecionado;
    struct BoundingBox boundingBox;

    struct NoArvore *esquerdo, *direito;
    
};

struct Arvore{

    int hitcount, prioMax;
    double promotionRate, epsilon; 

    struct NoArvore *raiz;

};

typedef struct NoArvore NoArvore;
typedef struct Arvore Arvore;

/*****************************************************************************************************************/

SmuTreap newSmuTreap(int hitCount, double promotionRate, double epsilon, int prioMax){

    Arvore *arvore = (Arvore*)malloc(sizeof(Arvore));

    if(arvore == NULL){
        printf("Erro: Falha na alcocacao de memoria para a criacao da arvore!\n");
        return NULL;
    }

    arvore->hitcount = hitCount;
    arvore->promotionRate = promotionRate;
    arvore->epsilon = epsilon;
    arvore->prioMax = prioMax;
    arvore->raiz = NULL;
    
    return arvore;

}

/*****************************************************************************************************************/

// FUNÇÃO AUXILIAR
static Node rotacionarEsquerda(Node n){

    NoArvore *no = (NoArvore*)n;

    if(no->direito == NULL){
        printf("Erro: O no da arvore nao possui filho direito! Sem rotacao a esquerda!\n");
        return NULL;
    }

    NoArvore *auxiliar1 = no->direito;
    NoArvore *auxiliar2 = auxiliar1->esquerdo;

    auxiliar1->esquerdo = no;
    no->direito = auxiliar2;

    return auxiliar1;

}

// FUNÇÃO AUXILIAR
static Node rotacionarDireita(Node n){

    NoArvore *no = (NoArvore*)n;

    if(no->esquerdo == NULL){
        printf("Erro: O no da arvore nao possui filho esquerdo! Sem rotacao a direita!\n");
        return NULL;
    }

    NoArvore *auxiliar1 = no->esquerdo;
    NoArvore *auxiliar2 = auxiliar1->direito;

    auxiliar1->direito = no;
    no->esquerdo = auxiliar2;

    return auxiliar1;

}

// FUNÇÃO AUXILIAR
static Node criarNo(SmuTreap t, double x, double y, Info i, DescritorTipoInfo d, FCalculaBoundingBox fCalcBb){

    Arvore *arvore = (Arvore*)t;
    NoArvore *novoNo = (NoArvore*)malloc(sizeof(NoArvore));

    if(novoNo == NULL){
        printf("Erro: Falha na alocacao de memoria para insercao do no na arvore!\n");
        return NULL;
    }

    novoNo->ancora.x = x;
    novoNo->ancora.y = y;
    novoNo->informacao = i;
    novoNo->descritor = d;
    novoNo->prioridade = rand() % arvore->prioMax;
    novoNo->qtdSelecionado = 0;
    novoNo->esquerdo = NULL;
    novoNo->direito = NULL;

    double xBb = 0, yBb = 0, wBb = 0, hBb = 0;

    fCalcBb(i, &xBb, &yBb, &wBb, &hBb);

    novoNo->boundingBox.x = xBb;
    novoNo->boundingBox.y = yBb;
    novoNo->boundingBox.w = wBb;
    novoNo->boundingBox.h = hBb;

    return novoNo;

}

// FUNÇÃO AUXILIAR
static Node inserirNo(SmuTreap t, Node n, double x, double y, Info i, DescritorTipoInfo d, FCalculaBoundingBox fCalcBb){

    Arvore *arvore = (Arvore*)t;
    NoArvore *no = (NoArvore*)n;

    if(no == NULL){

        return criarNo(arvore, x, y, i, d, fCalcBb);

    }else{
        if(x < no->ancora.x || (fabs(x - no->ancora.x) < arvore->epsilon && y < no->ancora.y)){
            no->esquerdo = inserirNo(arvore, no->esquerdo, x, y, i, d, fCalcBb);

            if(no->esquerdo != NULL && no->esquerdo->prioridade > no->prioridade){
                no = rotacionarDireita(no);
            }
        }else{
            no->direito = inserirNo(arvore, no->direito, x, y, i, d, fCalcBb);

            if(no->direito != NULL && no->direito->prioridade > no->prioridade){
                no = rotacionarEsquerda(no);
            }
        }
    }

    return no;   

}

// FUNÇÃO AUXILIAR
static void modificarBoundingBox(Node n){

    NoArvore *no = (NoArvore*)n;

    if(no == NULL){
        return;
    }

    struct BoundingBox bb = no->boundingBox;

    if(no->esquerdo != NULL){
        struct BoundingBox bb1 = no->esquerdo->boundingBox;
        double x = fmin(bb.x, bb1.x);
        double y = fmin(bb.y, bb1.y);
        double w = fmax(bb.x + bb.w, bb1.x + bb1.w);
        double h = fmax(bb.y + bb.h, bb1.y + bb1.h);
    
        bb.x = x;
        bb.y = y;
        bb.w = w - x;
        bb.h = h - y;
    }

    if(no->direito != NULL){
        struct BoundingBox bb2 = no->direito->boundingBox;
        double x = fmin(bb.x, bb2.x);
        double y = fmin(bb.y, bb2.y);
        double w = fmax(bb.x + bb.w, bb2.x + bb2.w);
        double h = fmax(bb.y + bb.h, bb2.y + bb2.h);
    
        bb.x = x;
        bb.y = y;
        bb.w = w - x;
        bb.h = h - y;
    }

    no->boundingBox = bb;

}

// FUNÇÃO AUXILIAR
static void atualizarBoundingBoxes(Node n){

    NoArvore *no = (NoArvore*)n;

    if(no == NULL){
        return;
    }

    atualizarBoundingBoxes(no->esquerdo);
    atualizarBoundingBoxes(no->direito);
    modificarBoundingBox(no);

}

Node insertSmuT(SmuTreap t, double x, double y, Info i, DescritorTipoInfo d, FCalculaBoundingBox fCalcBb){

    Arvore *arvore = (Arvore*)t;

    arvore->raiz = inserirNo(arvore, arvore->raiz, x, y, i, d, fCalcBb);

    Node n = getNodeSmuT(t, x, y);

    NoArvore *novoNo = (NoArvore*)n;

    if(novoNo != NULL){
        novoNo->qtdSelecionado--;
    }

    atualizarBoundingBoxes(arvore->raiz);

    return n;

}

/*****************************************************************************************************************/

// FUNCÃO AUXILIAR
static double calcularDistanciaAncoraPonto(Node n, double x, double y){

    NoArvore *no = (NoArvore*)n;

    double distancia = sqrt(pow(no->ancora.x - x, 2) + pow(no->ancora.y - y, 2));

    return distancia;

}

// FUNÇÃO AUXILIAR
static Node buscarNo(SmuTreap t, Node n, double x, double y){

    Arvore *arvore = (Arvore*)t;
    NoArvore *no = (NoArvore*)n;

    if(no == NULL){
        return NULL;
    }

    if(calcularDistanciaAncoraPonto(no, x, y) < arvore->epsilon){
        if(no->qtdSelecionado == arvore->hitcount){
            promoteNodeSmuT(arvore, no, arvore->promotionRate);
            no->qtdSelecionado = 0;
        }else{
            no->qtdSelecionado++;
        }
        return no;
    }

    if(x < no->ancora.x || (fabs(x - no->ancora.x) < arvore->epsilon && y < no->ancora.y)){
        return buscarNo(arvore, no->esquerdo, x, y);
    }else{
        return buscarNo(arvore, no->direito, x, y);
    }

}

Node getNodeSmuT(SmuTreap t, double x, double y){

    Arvore *arvore = (Arvore*)t;
    Node busca = buscarNo(arvore, arvore->raiz, x, y);

    if(busca == NULL){
        return NULL;
    }

    return busca;

}  

/*****************************************************************************************************************/

DescritorTipoInfo getTypeInfoSmuT(SmuTreap t, Node n){

    Arvore *arvore = (Arvore*)t;
    NoArvore *no = (NoArvore*)n;
 
    return no->descritor;

}

/*****************************************************************************************************************/

void promoteNodeSmuT(SmuTreap t, Node n, double promotionRate){

    Arvore *arvore = (Arvore*)t;
    NoArvore *no = (NoArvore*)n;

    arvore->promotionRate = promotionRate;
    no->prioridade = no->prioridade * arvore->promotionRate;
    
}

/*****************************************************************************************************************/

// FUNÇÃO AUXILIAR
static Node removerNo(SmuTreap t, Node n1, Node n2, double epsilon){

    Arvore *arvore = (Arvore*)t;
    NoArvore *no = (NoArvore*)n1;
    NoArvore *alvo = (NoArvore*)n2;

    if(no == NULL){
        return NULL;
    }

    if(calcularDistanciaAncoraPonto(no, alvo->ancora.x, alvo->ancora.y) < epsilon){
        if(no->esquerdo == NULL && no->direito == NULL){
            free(no);
            return NULL;
        }else{
            if(no->esquerdo != NULL && (no->direito == NULL || no->esquerdo->prioridade > no->direito->prioridade)){
                no = rotacionarDireita(no);
                no->direito = removerNo(arvore, no->direito, alvo, epsilon);
            }else{
                no = rotacionarEsquerda(no);
                no->esquerdo = removerNo(arvore, no->esquerdo, alvo, epsilon);
            }
        }   
    }else{
        if(alvo->ancora.x < no->ancora.x || (fabs(alvo->ancora.x - no->ancora.x) < epsilon && alvo->ancora.y < no->ancora.y)){
            no->esquerdo = removerNo(arvore, no->esquerdo, alvo, epsilon);
        }else{
            no->direito = removerNo(arvore, no->direito, alvo, epsilon);
        }
    }

    atualizarBoundingBoxes(no);

    return no;

}

void removeNoSmuT(SmuTreap t, Node n){

    Arvore *arvore = (Arvore*)t;
    NoArvore *no = (NoArvore*)n;
    NoArvore *noARemover = getNodeSmuT(arvore, no->ancora.x, no->ancora.y);

    if(noARemover != NULL){
        arvore->raiz = removerNo(arvore, arvore->raiz, noARemover, arvore->epsilon);
    }

}

/*****************************************************************************************************************/

Info getInfoSmuT(SmuTreap t, Node n){

    Arvore *arvore = (Arvore*)t;
    NoArvore *no = (NoArvore*)n;

    return no->informacao; 

}

/*****************************************************************************************************************/

Info getBoundingBoxSmuT(SmuTreap t, Node n, double *x, double *y, double *w, double *h){

    Arvore *arvore = (Arvore*)t;
    NoArvore *no = (NoArvore*)n;

    *x = no->boundingBox.x;
    *y = no->boundingBox.y;
    *w = no->boundingBox.w;
    *h = no->boundingBox.h;

    return no->informacao;

}

/*****************************************************************************************************************/

// FUNÇÃO AUXILIAR
bool interseccionaBoundingBox(struct BoundingBox bb, double x1, double y1, double x2, double y2, double epsilon){

    double xmin = fmin(x1, x2) - epsilon;
    double xmax = fmax(x1, x2) + epsilon;
    double ymin = fmin(y1, y2) - epsilon;
    double ymax = fmax(y1, y2) + epsilon;

    double bbxmax = bb.x + bb.w;
    double bbymax = bb.y + bb.h;

    return !(bb.x > xmax || bbxmax < xmin || bb.y > ymax || bbymax < ymin);

}

// FUNÇÃO AUXILIAR
void inserirNoAncoraDentro(SmuTreap t, Node n, double x1, double y1, double x2, double y2, Lista *L){

    Arvore *arvore = (Arvore*)t;
    NoArvore *no = (NoArvore*)n;

    if(no == NULL){
        return;
    }

    double xMaior = fmax(x1, x2);
    double xMenor = fmin(x1, x2);
    double yMaior = fmax(y1, y2);
    double yMenor = fmin(y1, y2);

    if((no->ancora.x >= xMenor && no->ancora.x <= xMaior) && (no->ancora.y >= yMenor && no->ancora.y <= yMaior)){
        listar(L, no);
    }

    double epsilon = getEpsilonSmuTreap(t);
    
    if(no->esquerdo != NULL && interseccionaBoundingBox(no->esquerdo->boundingBox, x1, y1, x2, y2, epsilon)){
        inserirNoAncoraDentro(arvore, no->esquerdo, x1, y1, x2, y2, L);
    }

    if(no->direito != NULL && interseccionaBoundingBox(no->direito->boundingBox, x1, y1, x2, y2, epsilon)){
        inserirNoAncoraDentro(arvore, no->direito, x1, y1, x2, y2, L);
    }

}

bool getNodesDentroRegiaoSmuT(SmuTreap t, double x1, double y1, double x2, double y2, Lista L){

    Arvore *arvore = (Arvore*)t;

    if(arvore == NULL || arvore->raiz == NULL){
        return false;
    }

    inserirNoAncoraDentro(arvore, arvore->raiz, x1, y1, x2, y2, L);

    if(listaVazia(L) == true){
        return false;
    }else{
        return true;
    }

}

/*****************************************************************************************************************/

// FUNÇÃO AUXILIAR
void inserirNoInfoDentro(SmuTreap t, Node n, double x1, double y1, double x2, double y2, FdentroDeRegiao f, Lista *L){

    Arvore *arvore = (Arvore*)t;
    NoArvore *no = (NoArvore*)n;

    if(no == NULL){
        return;
    }

    if(f(arvore, no, no->informacao, x1, y1, x2, y2) == true){
        listar(L, no);
    }

    double epsilon = getEpsilonSmuTreap(t);

    if(no->esquerdo != NULL && interseccionaBoundingBox(no->esquerdo->boundingBox, x1, y1, x2, y2, epsilon)){
        inserirNoInfoDentro(arvore, no->esquerdo, x1, y1, x2, y2, f, L);
    }

    if(no->direito != NULL && interseccionaBoundingBox(no->direito->boundingBox, x1, y1, x2, y2, epsilon)){
        inserirNoInfoDentro(arvore, no->direito, x1, y1, x2, y2, f, L);
    }
    
}

bool getInfosDentroRegiaoSmuT(SmuTreap t, double x1, double y1, double x2, double y2, FdentroDeRegiao f, Lista L){

    Arvore *arvore = (Arvore*)t;

    if(arvore == NULL || arvore->raiz == NULL){
        return false;
    }

    inserirNoInfoDentro(arvore, arvore->raiz, x1, y1, x2, y2, f, L);

    if(listaVazia(L) == true){
        return false;
    }else{
        return true;
    }

}

/*****************************************************************************************************************/

// FUNÇÃO AUXILIAR
void inserirNoPontoDentro(SmuTreap t, Node n, double x, double y, FpontoInternoAInfo f, Lista *L){

    Arvore *arvore = (Arvore*)t;
    NoArvore *no = (NoArvore*)n;

    if(no == NULL){
        return;
    }

    if(f(arvore, no, no->informacao, x, y) == true){
        listar(L, no);
    }
 
    double epsilon = getEpsilonSmuTreap(t);

    if(no->esquerdo != NULL && interseccionaBoundingBox(no->esquerdo->boundingBox, x, y, x, y, epsilon)){
        inserirNoPontoDentro(arvore, no->esquerdo, x, y, f, L);
    }

    if(no->direito != NULL && interseccionaBoundingBox(no->direito->boundingBox, x, y, x, y, epsilon)){
        inserirNoPontoDentro(arvore, no->direito, x, y, f, L);
    }

}

bool getInfosAtingidoPontoSmuT(SmuTreap t, double x, double y, FpontoInternoAInfo f, Lista L){

    Arvore *arvore = (Arvore*)t;

    if(arvore == NULL || arvore->raiz == NULL){
        return false;
    }

    inserirNoPontoDentro(arvore, arvore->raiz, x , y, f, L);

    if(listaVazia(L) == true){
        return false;
    }else{
        return true;
    }

}

/*****************************************************************************************************************/

// FUNÇÃO AUXILIAR
void percorrerPreOrdem(SmuTreap t, Node n, FvisitaNo f, void* aux){

    Arvore *arvore = (Arvore*)t;
    NoArvore *no = (NoArvore*)n;

    if(no == NULL){
        return;
    }

    f(arvore, no, no->informacao, no->ancora.x, no->ancora.y, aux);

    percorrerPreOrdem(arvore, no->esquerdo, f, aux);

    percorrerPreOrdem(arvore, no->direito, f, aux);

}

void visitaProfundidadeSmuT(SmuTreap t, FvisitaNo f, void *aux){

    Arvore *arvore = (Arvore*)t;

    percorrerPreOrdem(arvore, arvore->raiz, f, aux);
    
}

/*****************************************************************************************************************/

// FUNÇÃO AUXILIAR
void percorrerEmNivel(SmuTreap t, Node n, FvisitaNo f, void* aux){

    Arvore *arvore = (Arvore*)t;
    NoArvore *no = (NoArvore*)n;

    if(no == NULL){
        return;
    }

    FilaGenerica F = criarFila();

    enfileirar(F, arvore->raiz);

    while(filaVazia(F) == false){
        no = desenfileirar(F);
        if(no == NULL){
            break;
        }

        f(arvore, no, no->informacao, no->ancora.x, no->ancora.y, aux);

        if(no->esquerdo != NULL){
            enfileirar(F, no->esquerdo);
        }

        if(no->direito != NULL){
            enfileirar(F, no->direito);
        }
    }

    desalocarFila(F);

}

void visitaLarguraSmuT(SmuTreap t, FvisitaNo f, void *aux){

    Arvore *arvore = (Arvore*)t;

    percorrerEmNivel(arvore, arvore->raiz, f, aux); 
    
}

/*****************************************************************************************************************/

// FUNÇÃO AUXILIAR
Node procurarNo(SmuTreap t, Node n, FsearchNo f, void* aux) {
    Arvore *arvore = (Arvore*)t;
    NoArvore *no = (NoArvore*)n;

    if (no == NULL) {
        return NULL;
    }

    // Testa o nó atual
    if (f(arvore, no, no->informacao, no->ancora.x, no->ancora.y, aux)) {
        return no;
    }

    // Busca na subárvore esquerda
    Node resEsq = procurarNo(arvore, no->esquerdo, f, aux);
    if (resEsq != NULL) {
        return resEsq;
    }

    // Busca na subárvore direita
    Node resDir = procurarNo(arvore, no->direito, f, aux);
    return resDir;
}

Node procuraNoSmuT(SmuTreap t, FsearchNo f, void *aux){

    Arvore *arvore = (Arvore*)t;

    return procurarNo(arvore, arvore->raiz, f, aux);

}

/*****************************************************************************************************************/

// FUNÇÃO AUXILIAR
char *definirCorNo(Node n){

    NoArvore *no = (NoArvore*)n;

    switch(no->descritor){
        case 1:
            return "green";
            
        case 2:
            return "yellow";
        
        case 3:
            return "cyan";

        case 4:
            return "lightpink";

        default:
            return "white";
    }

}

// FUNÇÃO AUXILIAR
void imprimirConexoesDot(SmuTreap t, Node n, FILE *arquivo){

    Arvore *arvore = (Arvore*)t;
    NoArvore *no = (NoArvore*)n;

    if(no == NULL){
        return;
    }

    char *cor = definirCorNo(no);

    fprintf(arquivo, "  \"%p\" [label=\"(%.2f, %.2f)\", style=filled, fillcolor=%s];\n", no, no->ancora.x, no->ancora.y, cor);

    if(no->esquerdo != NULL){
        fprintf(arquivo, "  \"%p\" -> \"%p\" [label=\"Esquerda\"];\n", no, no->esquerdo);
        imprimirConexoesDot(t, no->esquerdo, arquivo);
    }

    if(no->direito != NULL){
        fprintf(arquivo, "  \"%p\" -> \"%p\" [label=\"Direita\"];\n", no, no->direito);
        imprimirConexoesDot(t, no->direito, arquivo);
    }

}

bool printDotSmuTreap(SmuTreap t, char *fn){

    Arvore *arvore = (Arvore*)t;

    FILE *arquivo = fopen(fn, "w");

    if(arquivo == NULL){
        printf("Falha na abertura do arquivo para impressao da arvore!\n");
        return false;
    }

    if(arvore->raiz == NULL){
        return false;
    }
    
    fprintf(arquivo, "digraph SmuTreap{\n");
    fprintf(arquivo, "\tnode [shape=record];\n");
    imprimirConexoesDot(arvore, arvore->raiz, arquivo);
    fprintf(arquivo, "}\n");

    fclose(arquivo);

    return true;

}

/*****************************************************************************************************************/

// FUNÇÃO AUXILIAR
void liberarMemoriaNo(Node n){

    NoArvore *no = (NoArvore*)n;

    if(no == NULL){
        return;
    }

    if(no->esquerdo != NULL){
        liberarMemoriaNo(no->esquerdo);
    }
    
    if(no->direito != NULL){
        liberarMemoriaNo(no->direito);
    }

    free(no);

}

void killSmuTreap(SmuTreap t){

    Arvore *arvore = (Arvore*)t;

    if(arvore->raiz != NULL){
        liberarMemoriaNo(arvore->raiz);
    }

    free(arvore);

}

/*****************************************************************************************************************/

double getEpsilonSmuTreap(SmuTreap t){

    Arvore *arvore = (Arvore*)t;

    return arvore->epsilon;
    
}
