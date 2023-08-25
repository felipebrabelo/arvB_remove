

#include "ArvB.h"

//Retorna a posição da chave no nó se ela existe nele, do contrário indica a posição do filho em que ela
//deve estar.
int pos_chave(struct NO *no, int valor) {
    int i = 0;
    while (i < no->qtd_chaves && no->chaves[i] < valor) {
        i++;
    }
    return i;
}

//Retorna a chave antecessora na árvore da chave na posição correspondente a pos dentro do nó no.
int antecessor(struct NO *no, int pos) {
    if (!eFolha(no)) {
        struct NO *aux = no->filhos[pos];
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

//Retorna a chave sucessora na árvore da chave na posição correspondente a pos dentro do nó no.
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

//Função de remoção de uma chave dentro de um nó folha.
void folha_remove(struct NO *no, int pos) {
    for (int i = pos; i < no->qtd_chaves - 1; i++) {
        no->chaves[i] = no->chaves[i + 1];
    }
    no->qtd_chaves--;
}

//Função de concatena4 o filho esquerdo e direito da chave em pos do no_pai, resultando um nó que possui as chaves dos
// dois filhos, com a chave em pos de no_pai sendo a central desse novo nó.
bool concatenar_filhos(struct NO **no_pai, int pos) {
    bool flag_reducao = false; //Indica se ouve ou não redução da alutra da ávore.
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
    //Caso o nó pai possua apenas uma chave, a operação deixaria o nó pai vazio. Assim, seu filho concatenado toma seu lugar.
    if (no->qtd_chaves == 0) {
        *no_pai = filho_anterior;
        flag_reducao = true; //Quando isso ocorre, a árvore diminui de altura.
    }
    return flag_reducao;
}

//Função que redistribui uma chave para o filhos[pos] de nó de seu irmão esquerdo.
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

//Função que redistribui uma chave para o filhos[pos] de nó de seu irmão direito.
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

//Função que remove a chave de um nó interno.
void interno_remove(struct NO *no, int pos) {
    int aux;
    //Caso o filho esquerdo da chave a ser removida possua mais nós que o mínimo, seu antecessor toma seu lugar e é removido recursivamente da folha.
    if (no->filhos[pos]->qtd_chaves > min_ordem - 1) {
        aux = antecessor(no, pos);
        arvB_remove(&no->filhos[pos], aux);
        no->chaves[pos] = aux;
    } else if (no->filhos[pos + 1]->qtd_chaves > min_ordem - 1) {//Caso o filho direito da chabe a ser removida possua mais nós que o mínimo, seu sucessor toma seu luga e é removido recursivamente da árvore.

        aux = sucessor(no, pos);
        arvB_remove(&no->filhos[pos + 1], aux);
        no->chaves[pos] = aux;
    } else {//Caso ambos os filhos da chave a ser removida possuam apenas o mínimo de chaves, é necessário concanternar os dois e remover a chave recursivamente.
        aux = no->chaves[pos];
        bool concat_reducao = concatenar_filhos(&no, pos);
        if (concat_reducao) {//Caso a altura da árvore diminua, é necessário começar a remoção dentro do próprio nó.
            arvB_remove(&no, aux);
        } else {
            arvB_remove(&no->filhos[pos], aux);
        }
    }
}

//Função que remove uma chave da árvore enraizada em raiz.
int arvB_remove(ArvB *raiz, int valor) {
    bool concat_reducao;
    struct NO *aux = *raiz;
    int pos = pos_chave(aux, valor);
    if (pos < aux->qtd_chaves && aux->chaves[pos] == valor) { //Se a chave existe dentro do nó.
        if (eFolha(aux)) {//Se o nó for uma folha.
            folha_remove(aux, pos);
        } else {//Se o nó for interno
            interno_remove(aux, pos);
        }
        if (aux->qtd_chaves == 0) {//Caso o nó fique vazio após a remoção da chave, ele é liberado da memoria.
            if (!eFolha(aux)) {//Se o nó não for uma folha, as referências precisam ser corrigidas.
                *raiz = aux->filhos[0];
            }
            free(aux);
        }
        return 1;
    }
    //Caso a chave não exista dentro do nó,
    if (eFolha(aux)) {//Se a chave não existe no nó e ele é uma folha, a chave não existe na árvore.
        printf("Não existe a chave requisitada dentro da árvore.\n");
        return 0;
    }
    //Do contrário, caso o nó onde a chave deve estar possuir uma quantidade de chaves onde
    // caso uma for removida deixaria o nó com menos do mínimo possível.
    if (aux->filhos[pos]->qtd_chaves < min_ordem) {
        //Se o irmão esquerdo do nó possui uma quantidade maior que o mínimo, é feita uma redistribuição esquerda.
        if (pos > 0 && aux->filhos[pos - 1]->qtd_chaves > min_ordem - 1) {
            redistribuir_esquerdo(aux, pos);
            return arvB_remove(&aux->filhos[pos], valor);
        }
        //Se o irmão direito do nó possui uma quantidade maior que o mínimo, é feita uma redistribuição direita.
        if (pos < aux->qtd_chaves - 1 && aux->filhos[pos + 1]->qtd_chaves > min_ordem - 1) {
            redistribuir_direito(aux, pos);
            return arvB_remove(&aux->filhos[pos], valor);
        }
        //Do contrário, todos os irmãos possuem apenas o mínimo necessário por nó, assim sendo necessário uma concatenação.
        //Se o filho a ser concatenado não é o ultimo.
        if (pos < aux->qtd_chaves - 1 || pos == 0) {
            concat_reducao = concatenar_filhos(raiz, pos);
            if (concat_reducao) { //Se houve redução na altura da árvore, é necessário verificar se a chave não existe dentro do novo nó.
                return arvB_remove(&aux, valor);
            }
            return arvB_remove(&aux->filhos[pos], valor);
        }
        //Se o filho a ser concatenado é o ultimo.
        concat_reducao = concatenar_filhos(raiz, pos - 1);
        if (concat_reducao) {
            return arvB_remove(&aux, valor);//Se houve redução na altura da árvore, é necessário verificar se a chave não existe dentro do novo nó.
        }
        return arvB_remove(&aux->filhos[pos - 1], valor);
    }
    //Se o nó possui mais nós que o mínimo, é aplicada recursivamente a remoção sobre ele sem nenhum tratamento.
    return arvB_remove(&aux->filhos[pos], valor);
}

//Imprime todas as chaves de um nó.
void no_imprime(struct NO *no) {
    if (no != NULL) {
        for (int i = 0; i < no->qtd_chaves; i++) {
            printf("%d ", no->chaves[i]);
        }
        printf("\t\t");
    } else {
        printf("\n");
    }
}

//Imprime a transversal de nível da árvore B usando busca em largura.
void arvB_imprime(ArvB *raiz) {
    if (*raiz == NULL) {
        printf("Árvore vazia. \n");
    }
    Fila *q = cria_Fila();
    struct NO *fim_nivel = *raiz;
    insere_Fila(q, *raiz);
    while (!filaVazia(q)) {
        struct NO *no = remove_Fila(q);
        no_imprime(no);
        if (!eFolha(no)) {
            for (int i = 0; i <= no->qtd_chaves; i++) {
                insere_Fila(q, no->filhos[i]);
            }
        }
        if (no == fim_nivel) {
            printf("\n");
            fim_nivel = fim_nivel->filhos[fim_nivel->qtd_chaves];
        }
    }
    printf("\n\n");
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

//Função que indica se o nó é uma folha ou não.
bool eFolha(struct NO *no) {
    return (no->filhos[0] == NULL);
}

void arvB_destroi(ArvB *raiz) {
    struct NO *aux = *raiz;
    if (aux != NULL) {
        for (int i = 0; i < aux->qtd_chaves; i++) {
            arvB_destroi(&aux->filhos[i]);
        }
        free(aux);
    }
}



