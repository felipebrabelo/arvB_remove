//
// Created by Felipe Rabelo on 14/08/23.
//

#include "ArvB.h"


int pos_chave(struct NO *no, int valor) {
    int i = 0;
    while (i < no->qtd_chaves && no->chaves[i] < valor) {
        i++;
    }
    return i;
}

int antecessor(struct NO *no, int pos) {
    if (!eFolha(no)) {
        struct NO *aux = no->filhos[pos]; //subárvore esquerda
        while (!eFolha(aux)) {
            aux = aux->filhos[aux->qtd_chaves - 1];
        }
        return aux->chaves[aux->qtd_chaves - 1];
    } else {
        if (pos != 0) {
            return no->chaves[pos - 1];
        } else {
            return no->chaves[pos];
        }
    }
}

int sucessor(struct NO *no, int pos) {
    if (!eFolha(no)) {
        struct NO *aux = no->filhos[pos + 1];
        while (!eFolha(aux)) {
            aux = aux->filhos[0];
        }
        return aux->chaves[0];
    } else {
        if (pos != no->qtd_chaves - 1) {
            return no->chaves[pos + 1];
        } else {
            return no->chaves[pos];
        }
    }
}

void folha_remove(struct NO *no, int pos) {
    for (int i = pos; i < no->qtd_chaves - 1; i++) {
        no->chaves[i] = no->chaves[i + 1];
    }
    no->qtd_chaves--;
}

void concatenar_filhos(struct NO **no_pai, int pos) {
    struct NO *no = *(no_pai);
    struct NO *filho_anterior = no->filhos[pos];
    struct NO *filho_posterior = no->filhos[pos + 1];
    filho_anterior->chaves[filho_anterior->qtd_chaves] = no->chaves[pos];
    filho_anterior->qtd_chaves++;
    for (int i = 0; i < filho_posterior->qtd_chaves; i++) {
        filho_anterior->chaves[filho_anterior->qtd_chaves + i] = filho_posterior->chaves[i];
    }
    if (!eFolha(filho_anterior)) {
        for (int i = 0; i <= filho_posterior->qtd_chaves; i++) {
            filho_anterior->filhos[filho_anterior->qtd_chaves + i] = filho_posterior->filhos[i];
        }
    }
    filho_anterior->qtd_chaves += filho_posterior->qtd_chaves;
    for (int i = pos; i < no->qtd_chaves - 1; i++) {
        no->chaves[i] = no->chaves[i + 1];
        no->filhos[i + 1] = no->filhos[i + 2];
    }
    no->qtd_chaves--;
    free(filho_posterior);
    if (no->qtd_chaves == 0) {
        *no_pai = filho_anterior;
    }
}

void redistribuir_esquerdo(struct NO *no, int pos) {
    struct NO *filho_atual = no->filhos[pos];
    struct NO *irmao_anterior = no->filhos[pos - 1];
    for (int i = filho_atual->qtd_chaves - 1; i >= 0; i--) {
        filho_atual->chaves[i + 1] = filho_atual->chaves[i];
    }
    if (!eFolha(filho_atual)) {
        for (int i = filho_atual->qtd_chaves; i >= 0; i++) {
            filho_atual->filhos[i + 1] = filho_atual->filhos[i];
        }
    }
    filho_atual->qtd_chaves++;
    filho_atual->chaves[0] = no->chaves[pos - 1];
    if (!eFolha(filho_atual)) {
        filho_atual->filhos[0] = irmao_anterior->filhos[irmao_anterior->qtd_chaves - 1];
    }
    no->chaves[pos - 1] = irmao_anterior->chaves[irmao_anterior->qtd_chaves - 1];
    irmao_anterior->qtd_chaves--;
}

void redistribuir_direito(struct NO *no, int pos) {
    struct NO *filho_atual = no->filhos[pos];
    struct NO *irmao_posterior = no->filhos[pos + 1];
    filho_atual->chaves[filho_atual->qtd_chaves] = no->chaves[pos];
    filho_atual->qtd_chaves++;
    if (!eFolha(filho_atual)) {
        filho_atual->filhos[filho_atual->qtd_chaves] = irmao_posterior->filhos[0];
    }
    no->chaves[pos] = irmao_posterior->chaves[0];
    for (int i = 1; i < irmao_posterior->qtd_chaves; i++) {
        irmao_posterior->chaves[i - 1] = irmao_posterior->chaves[i];
    }
    if (!eFolha(irmao_posterior)) {
        for (int i = 0; i <= irmao_posterior->qtd_chaves; i++) {
            irmao_posterior->filhos[i - 1] = irmao_posterior->filhos[i];
        }
    }
    irmao_posterior->qtd_chaves--;
}

void interno_remove(struct NO *no, int pos) {
    int aux;
    if (no->filhos[pos]->qtd_chaves > min_ordem - 1) {
        aux = antecessor(no, pos);
        arvB_remove(&no->filhos[pos], aux);
        no->chaves[pos] = aux;
    } else if (no->filhos[pos + 1]->qtd_chaves > min_ordem - 1) {
        aux = sucessor(no, pos);
        arvB_remove(&no->filhos[pos], aux);
        no->chaves[pos] = aux;
    } else {
        aux = no->chaves[pos];
        concatenar_filhos(&no, pos);
        arvB_remove(&no->filhos[pos], aux);
    }
}

int arvB_remove(ArvB *raiz, int valor) {
    struct NO *aux = *raiz;
    int pos = pos_chave(aux, valor);
    if (pos < aux->qtd_chaves && aux->chaves[pos] == valor) {
        if (eFolha(aux)) {
            folha_remove(aux, pos);
        } else {
            interno_remove(aux, pos);
        }
        if(aux->qtd_chaves == 0){
            free(aux);
        }
        return 1;
    }
    if (eFolha(aux)) {
        printf("Não existe a chave requisitada dentro da árvore.\n");
        return 0;
    }
    if (aux->filhos[pos]->qtd_chaves < min_ordem) {
        if (pos > 0 && aux->filhos[pos - 1]->qtd_chaves > min_ordem - 1) {
            redistribuir_esquerdo(aux, pos);
            return arvB_remove(&aux->filhos[pos], valor);
        }
        if (pos < aux->qtd_chaves - 1 && aux->filhos[pos + 1]->qtd_chaves > min_ordem - 1) {
            redistribuir_direito(aux, pos);
            return arvB_remove(&aux->filhos[pos], valor);
        }
        if (pos < aux->qtd_chaves - 1 || aux->qtd_chaves == 1) {
            concatenar_filhos(raiz, pos);
            return arvB_remove(&aux->filhos[pos], valor);
        }
        concatenar_filhos(raiz, pos - 1);
        return arvB_remove(&aux->filhos[pos - 1], valor);
    }
    return arvB_remove(&aux->filhos[pos], valor);
}

void no_imprime(struct NO *no) {
    for (int i = 0; i < no->qtd_chaves; i++) {
        printf("%d ", no->chaves[i]);
    }
    printf("\t\t");
}

void arvB_imprime(ArvB *raiz) {
    if (*raiz == NULL) {
        printf("Árvore vazia. \n");
    }
    Fila *q = cria_Fila();
    insere_Fila(q, *raiz);
    while (!filaVazia(q)) {
        struct NO *no = remove_Fila(q);
        no_imprime(no);
        if (!eFolha(no)) {
            for (int i = 0; i <= no->qtd_chaves; i++) {
                insere_Fila(q, no->filhos[i]);
            }
        }
    }
    printf("\n");
    liberaFila(q);
}

Fila *cria_Fila() {
    Fila *q;
    q = (Fila *) malloc(sizeof(Fila));
    q->inicio = NULL;
    q->fim = NULL;
    q->tam = 0;
    return q;
}

void insere_Fila(Fila *q, struct NO *n) {
    Celula *nova;
    nova = (Celula *) malloc(sizeof(Celula));
    nova->conteudo = n;
    nova->prox = NULL;
    if (q->fim == NULL) {
        q->inicio = nova;
        q->fim = nova;
    } else {
        q->fim->prox = nova;
        q->fim = nova;
    }
    q->tam++;
}

struct NO *remove_Fila(Fila *q) {
    struct NO *no;
    Celula *removida;
    removida = q->inicio;
    no = removida->conteudo;
    q->inicio = removida->prox;
    if (q->inicio == NULL) {
        q->fim = NULL;
    }
    free(removida);
    q->tam--;
    return no;
}

bool filaVazia(Fila *q) {
    return (q->fim == NULL);
}

Fila *liberaFila(Fila *q) {
    Celula *p, *morta;
    p = q->inicio;
    while (p != NULL) {
        morta = p;
        p = p->prox;
        free(morta);
    }
    free(q);
    return NULL;
}


bool eFolha(struct NO *no){
    return (no->filhos[0] == NULL);
}

void arvB_destroi(ArvB *raiz){
    struct NO *aux = *raiz;
    if(aux != NULL){
        for(int i = 0; i<aux->qtd_chaves; i++){
            arvB_destroi(&aux->filhos[i]);
        }
        free(aux);
    }
}