#ifndef _SMUTREAP_H__
#define _SMUTREAP_H__

#include <stdbool.h>
#include "lista.h"

/*
 * Uma SmuTreap é uma treap espacial com promoção dinâmica de nós mais usados.
 * Assim como qualquer Treap, a SmuTreap associa a cada nó inserido na árvore 
 * uma prioriodade. A SmuTreap permite que esta prioridade seja alterada
 * posteriormente, por dois meios:
 * (a) diretamente pelas operações promoteNodiSmuT e demoteNodiSmuT;
 * (b) indiretamente pela frequência com que o nó é selecionado.
 *
 * Em relação ao item (b), dois parâmetros, informados na criação da árvore, são 
 * considerados: hitCount, promotionRate. O primeiro (hitCount) é a quantidade 
 * de vezes que o nó deve ser selecionado para que tenha sua prioridade aumentada. 
 * O segundo é o fator multiplicativo usado para calcular a nova prioridade do nó.
 *
 * A cada nó r da Árvore é associado um ponto de 2 dimensões (r.x e r.y), denominado 
 * âncora, uma informação relacionada tal ponto e um "descritor" do tipo da informação. 
 * O significado dos valores de tal descritor é definido pela aplicação. A âncora 
 * determina a relação de ordem entre os elementos armazenados na árvore. Sejam dois 
 * nós da árvore: s e t. 
 * s < t: se (s.x < t.x) OU (s.x = t.x E s.y < t.y)
 *
 * A SmuTreap implementa 2 percursos: em largura e em profundidade. Tais percursos 
 * recebem como parâmetro uma função que processa a informação associada ao nó visitado.
 *
 * Também sao providas operações de seleção espacial:
 * A PRIMEIRA seleciona os nós da árvore cujas âncoras sao internas a uma região retangular.
 * A SEGUNDA seleciona os nós da árvore cujas informações associadas podem ser consideradas 
 * internas a uma região retangular. Esta operação recebe por parâmetro uma função que
 * decide se tal informação é considerada (ou não) interna.
 * A TERCEIRA, similarmente, seleciona os nós da árvore cujo um dado ponto é interno à 
 * informação associada ao nó.
 *
 * A SmuTreap é uma estrutura que se rearranja dinamicamente visando tornar consultas mais 
 * eficientes, a saber:
 *
 * (a) Cada nó da árvore mantém o bounding box (mínimo) que engloba as âncoras de suas 
 * sub-árvores. Este bounding box é usado para otimizar as operações de busca espacial.
 * (b) Nós mais selecionados são posicionados mais próximos a raiz. 
 *
 * Quando for necessario comparar a igualdade de coordenada (por exemplo, comparar a âncora 
 * de um nó com um outro ponto), aceita-se uma pequena discrepância entre a coordenada da 
 * âncora (anc.x, anc.y) e o ponto (x, y) de epsilon unidades. 
 * Ou seja, distancia(anc,x,y) < epsilon.
 *
 * Módulo a ser implementado para o trabalho 1 da disciplina "Estrutura de Dados 2",
 * do curso de Bacharelado em Ciência da Computação, da Universidade Estadual de Londrina, 
 * no primeiro semestre de 2025. Obviamente, é expressamente proibido usar ferramentas de 
 * geração automática de código para fazer a implementção deste módulo. Qualquer dúvida ou 
 * inconsistência, favor reportar rapidamente para que correções possam ser rapidamente efetuadas.
 */

typedef void* SmuTreap;
typedef void* Nodi;
typedef void* Info;
typedef int DescritorTipoInfo;

typedef bool (*FdentroDeRegiao)(SmuTreap t, Nodi n, Info i, double x1, double y1, double x2, double y2);
/*
 * Uma função deste tipo deve retornar verdadeiro se a informacao i está
 * "dentro" da região retangular delimitada pelos pontos opostos (x1,y1) e (x2,y2).
 * Retorna falso, caso contrário.
 */

typedef bool (*FpontoInternoAInfo)(SmuTreap t, Nodi n, Info i, double x, double y);
/*
 * Uma função deste tipo deve retornar verdadeiro se o ponto (x,y)
 * deva ser considerado "interno" à informação i.
 */

typedef void (*FvisitaNo)(SmuTreap t, Nodi n, Info i, double x, double y, void* aux);
/*
 * Processa a informação i associada a um nó da árvore, cuja âncora
 * é o ponto (x,y). O parâmetro aux aponta para conjunto de dados
 * (provavelmente um registro) que sao compartilhados entre as
 * sucessivas invocações a esta função.
 */

typedef void (*FCalculaBoundingBox)(Info i, double *x, double *y, double *w, double *h);
/*
 * Calcula o bounding box da informação i.
 * Atribui a x,y,w,h, respectivamente, a âncora do retângulo (x,y), a largura e altura
 * do retângulo.
 */

typedef bool (*FsearchNo)(SmuTreap t, Nodi n, Info i, double x, double y, void* aux);
/*
 * Verifica se a informaçao i associada a um nó da árvore, cuja âncora é o ponto (x,y),
 * é a informaçao procurada. Retorna verdadeiro, em caso afirmativo; falso, caso contrário. 
 * O parâmetro aux aponta para conjunto de dados (provavelmente um registro) que são 
 * compartilhados entre as sucessivas invocações a esta função, incluindo (provavelmente)
 * uma chave de busca.
 */

SmuTreap newSmuTreap(int hitCount, double promotionRate, double epsilon, int prioMax);
/*
 * Retorna uma SmuTreap vazia.
 * hitCount >= 1; promotionRate > 0.0, tipicamente é um número positivo maior 
 * que 1.0 (por exemplo, 1.10 significa aumentar em 10%). 
 * Valores na faixa (0.0,1.0) sao possíveis, mas diminuirão a prioridade
 * do nó.
 */

Nodi insertSmuT(SmuTreap t, double x, double y, Info i, DescritorTipoInfo d, FCalculaBoundingBox fCalcBb);
/*
 * Insere a informação i, associada à âncora (x,y) na árvore t.
 * d é um valor (definido pela aplicação) que identifica, caso existam várias
 * categorias, um categoria específica da informação i.
 * fCalcBb calcula o bounding box da informação i.
 * Retorna um indicador para o nó inserido.
 */

Nodi getNodiSmuT(SmuTreap t, double x, double y);
/* 
 * Retorna o nó cuja âncora seja o ponto (x,y), admitida a discrepância
 * epsilon definida na criação da árvore. Retorna NULL caso não tenha 
 * encontrado o nó.
 */

DescritorTipoInfo getTypeInfoSmuT(SmuTreap t, Nodi n);
/* 
 * Retorna o tipo da informação associada ao nó n.
 */

void promoteNodiSmuT(SmuTreap t, Nodi n, double promotionRate);
/*
 * Aumenta a prioridade do nó n pelo fator promotionRate.
 */

void removeNoSmuT(SmuTreap t, Nodi n);
/*
 * Remove o nó n da árvore. O nó n deve ser um nó valido.
 */

bool getNodisDentroRegiaoSmuT(SmuTreap t, double x1, double y1, double x2, double y2, Lista L);
/* 
 * Insere na lista L os nós (Nodi) da árvore t cujas âncoras estao dentro da região 
 * delimitada pelos pontos (x1,y1) e (x2,y2). Retorna falso, caso não existam nós 
 * dentro da região; verdadeiro, caso contrário.
 */

bool getInfosDentroRegiaoSmuT(SmuTreap t, double x1, double y1, double x2, double y2, FdentroDeRegiao f, Lista L);
/* 
 * Insere na lista L os nós cujas respectivas informações associadas estão inteiramente 
 * dentro da região delimitada pelos pontos (x1,y1) e (x2,y2). A função f é usada para 
 * determinar se uma  informação armazenada na árvore está dentro da região.
 * Retorna falso caso não existam informações internas; verdadeiro, caso contrário.
 */

bool getInfosAtingidoPontoSmuT(SmuTreap t, double x, double y, FpontoInternoAInfo f, Lista L);
/* 
 * Insere na lista L os nós para os quais o ponto (x,y) possa ser considerado interno às 
 * informações associadas ao nó. A funcao f é invocada para determinar se o ponto (x,y) 
 * é interno a uma informação específica. Retorna falso caso não existam informações 
 * internas; verdadeiro, caso contrário.
 */

Info getInfoSmuT(SmuTreap t, Nodi n);
/* 
 * Retorna a informação associada ao nó n.
 */

Info getBoundingBoxSmuT(SmuTreap t, Nodi n, double *x, double *y, double *w, double *h);
/* 
 * Retorna o bounding box associado ao nó n 
 */

void visitaProfundidadeSmuT(SmuTreap t, FvisitaNo f, void* aux);
/*
 * Percorre a árvore em profundidade. Invoca a função f em cada nó visitado.
 * O apontador aux é parametro em cada invocação de f; assim alguns dados 
 * podem ser compartilhados entre as diversas invocações de f.
 */

void visitaLarguraSmuT(SmuTreap t, FvisitaNo f, void* aux);
/*
 * Similar a visitaProfundidadeSmuT, porém, faz o percurso em largura.
 */ 

Nodi procuraNoSmuT(SmuTreap t, FsearchNo f, void* aux);
/*
 * Procura o nó da árvore que contenha um dado específico. Visita cada nó 
 * da árvore e invoca a função f. A função f retornará verdadeiro se o nó 
 * contém o dado procurado. Neste caso, retorna o nó encontrado. Caso a busca 
 * falhe, retorna NULL.
 */

bool printDotSmuTreap(SmuTreap t, char *fn);
/* 
 * Gera representação da árvore no arquivo fn, usando a Dot Language
 * (ver https://graphviz.org/). Retorna falso, caso o arquivo não possa
 * ser criado (para escrita); true, caso contrario)
*/

void killSmuTreap(SmuTreap t);
/*
 * Libera a memória usada pela árvore t.
 */

double getEpsilonSmuTreap(SmuTreap t);
/*
 * Retorna o valor de epsilon da árvore t. 
 */

#endif