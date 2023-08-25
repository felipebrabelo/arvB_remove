#include <stdio.h>
#include "ArvB.h"

int posicao_alfabeto(char letra){
    return (letra-'a'+1);
}

int main() {

    struct NO *noraizp = (struct NO*) malloc(sizeof(struct NO));
    noraizp->qtd_chaves = 1;
    // noraizp->folha = false;
    noraizp->chaves[0] = posicao_alfabeto('p');
    struct NO *no1 = (struct NO*) malloc(sizeof(struct NO));
    no1->qtd_chaves = 3;
   // no1->folha = false;
    no1->chaves[0] = posicao_alfabeto('c');
    no1->chaves[1] = posicao_alfabeto('g');
    no1->chaves[2] = posicao_alfabeto('m');
    noraizp->filhos[0] = no1;
    struct NO *no2 = (struct NO*) malloc(sizeof(struct NO));
    no2->qtd_chaves = 2;
  //  no2->folha = false;
    no2->chaves[0] = posicao_alfabeto('t');
    no2->chaves[1] = posicao_alfabeto('x');
    noraizp->filhos[1] = no2;
    struct NO *no3 = (struct NO*) malloc(sizeof (struct NO));
    no3->qtd_chaves = 2;
   // no3->folha = true;
    no3->chaves[0] = posicao_alfabeto('a');
    no3->chaves[1] = posicao_alfabeto('b');
    no1->filhos[0] = no3;
    struct NO *no4 = (struct NO*) malloc(sizeof (struct NO));
    no4->qtd_chaves = 3;
   // no4->folha = true;
    no4->chaves[0] = posicao_alfabeto('d');
    no4->chaves[1] = posicao_alfabeto('e');
    no4->chaves[2] = posicao_alfabeto('f');
    no1->filhos[1] = no4;
    struct NO *no5 = (struct NO*) malloc(sizeof (struct NO));
    no5->qtd_chaves = 3;
    //no5->folha = true;
    no5->chaves[0] = posicao_alfabeto('j');
    no5->chaves[1] = posicao_alfabeto('k');
    no5->chaves[2] = posicao_alfabeto('l');
    no1->filhos[2] = no5;
    struct NO *no6 = (struct NO*) malloc(sizeof (struct NO));
    no6->qtd_chaves = 2;
   // no6->folha = true;
    no6->chaves[0] = posicao_alfabeto('n');
    no6->chaves[1] = posicao_alfabeto('o');
    no1->filhos[3] = no6;
    struct NO *no7 = (struct NO*) malloc(sizeof (struct NO));
    no7->qtd_chaves = 3;
   // no7->folha = true;
    no7->chaves[0] = posicao_alfabeto('q');
    no7->chaves[1] = posicao_alfabeto('r');
    no7->chaves[2] = posicao_alfabeto('s');
    no2->filhos[0] = no7;
    struct NO *no8 = (struct NO*) malloc(sizeof (struct NO));
    no8->qtd_chaves = 2;
   // no8->folha = true;
    no8->chaves[0] = posicao_alfabeto('u');
    no8->chaves[1] = posicao_alfabeto('v');
    no2->filhos[1] = no8;
    struct NO *no9 = (struct NO*) malloc(sizeof (struct NO));
    no9->qtd_chaves = 2;
   // no9->folha = true;
    no9->chaves[0] = posicao_alfabeto('y');
    no9->chaves[1] = posicao_alfabeto('z');
    no2->filhos[2] = no9;
    ArvB *raiz = &noraizp;
    arvB_imprime(raiz);
    arvB_remove(raiz, posicao_alfabeto('f'));
    arvB_imprime(raiz);
    arvB_remove(raiz, posicao_alfabeto('m'));
    arvB_imprime(raiz);
    arvB_remove(raiz, posicao_alfabeto('g'));
    arvB_imprime(raiz);
    //concatenar_filhos(raiz,0);
    arvB_remove(raiz, posicao_alfabeto('d'));
    arvB_imprime(raiz);
    arvB_remove(raiz, posicao_alfabeto('b'));
    arvB_imprime(raiz);
    arvB_destroi(raiz);
    //arvB_imprime(raiz);

}
