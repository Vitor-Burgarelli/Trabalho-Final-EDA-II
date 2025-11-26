#include <stdio.h>
#include <stdlib.h>

typedef struct no {
    struct no* pai;
    struct no* esq;
    struct no* dir;
    int valor;
    int altura; // Otimizacao: guardar altura no no
} No;

typedef struct {
    No* raiz;
    long long comparacoes; // Contador global para a instancia
} ArvoreAVL;

No* criar_no(int valor) {
    No *no = (No*)malloc(sizeof(No));
    no->esq = NULL;
    no->dir = NULL;
    no->pai = NULL;
    no->valor = valor;
    no->altura = 1;
    return no;
}

int altura(No *no) {
    return (no == NULL) ? 0 : no->altura;
}

int maior(int a, int b) {
    return (a > b) ? a : b;
}

void atualizar_altura(No *no) {
    if (no) {
        no->altura = 1 + maior(altura(no->esq), altura(no->dir));
    }
}

int fb(No *no) {
    if (no == NULL) return 0;
    return altura(no->esq) - altura(no->dir);
}

/* Rotacoes com atualizacao de altura e pai */
No *rsd(ArvoreAVL* arv, No *y) {
    arv->comparacoes++; // Custo de rotação

    No *x = y->esq;
    No *T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    if (T2) T2->pai = y;
    x->pai = y->pai;
    y->pai = x;

    if (x->pai == NULL) arv->raiz = x;
    else if (y == x->pai->esq) x->pai->esq = x;
    else x->pai->dir = x;

    atualizar_altura(y);
    atualizar_altura(x);
    return x;
}

No *rse(ArvoreAVL* arv, No *x) {
    arv->comparacoes++; // Custo de rotação

    No *y = x->dir;
    No *T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    if (T2) T2->pai = x;
    y->pai = x->pai;
    x->pai = y;

    if (y->pai == NULL) arv->raiz = y;
    else if (x == y->pai->esq) y->pai->esq = y;
    else y->pai->dir = y;

    atualizar_altura(x);
    atualizar_altura(y);
    return y;
}

/* Balanceamento generico */
void balancear_avl(ArvoreAVL* arv, No* no) {
    while (no != NULL) {
        arv->comparacoes++; // Custo para verificar o fator de balanceamento

        atualizar_altura(no);
        int bal = fb(no);

        arv->comparacoes++;
        if (bal > 1) { // Esquerda pesada
            arv->comparacoes++; // Custo de decisão de rotação
            if (fb(no->esq) >= 0) {
                no = rsd(arv, no);
            } else {
                rse(arv, no->esq);
                no = rsd(arv, no);
            }
        } else {
            arv->comparacoes++;
            if (bal < -1) { // Direita pesada
                arv->comparacoes++; // Custo de decisão de rotação
                if (fb(no->dir) <= 0) {
                    no = rse(arv, no);
                } else {
                    rsd(arv, no->dir);
                    no = rse(arv, no);
                }
            }
        }
        no = no->pai;
    }
}

void inserir_avl(ArvoreAVL* arv, int valor) {
    if (arv->raiz == NULL) {
        arv->raiz = criar_no(valor);
        return;
    }

    No* atual = arv->raiz;
    No* pai = NULL;

    while (atual != NULL) {
        pai = atual;
        arv->comparacoes++; // Contando comparacao
        if (valor < atual->valor)
            atual = atual->esq;
        else
            atual = atual->dir;
    }

    No* novo = criar_no(valor);
    novo->pai = pai;
    
    // Nao contamos comparacao aqui pois ja sabemos onde inserir do loop acima
    if (valor < pai->valor) pai->esq = novo;
    else pai->dir = novo;

    balancear_avl(arv, novo->pai);
}

/* Funcao auxiliar para remocao: encontrar minimo */
No* min_valor_no(No* no, long long* comps) {
    No* atual = no;
    while (atual->esq != NULL) {
        (*comps)++;
        atual = atual->esq;
    }
    return atual;
}

/* Remocao AVL */
void remover_avl_no(ArvoreAVL* arv, No* raiz, int valor) {
    if (raiz == NULL) return;

    No* atual = raiz;
    while (atual != NULL) {
        arv->comparacoes++;
        if (valor == atual->valor) break;
        
        if (valor < atual->valor) atual = atual->esq;
        else atual = atual->dir;
    }

    if (atual == NULL) return; // Nao achou

    // Logica de remocao BST
    No* no_para_balancear = NULL;

    // Caso 1 e 2: 0 ou 1 filho
    if (atual->esq == NULL || atual->dir == NULL) {
        No* temp;
        if (atual->esq != NULL) {
            temp = atual->esq;
        } else {
            temp = atual->dir;
        }

        if (temp == NULL) { // Sem filhos
            if (atual == arv->raiz) {
                arv->raiz = NULL;
            } else {
                if (atual == atual->pai->esq) atual->pai->esq = NULL;
                else atual->pai->dir = NULL;
                no_para_balancear = atual->pai;
            }
            free(atual);
        } else { // 1 filho
            if (atual == arv->raiz) {
                arv->raiz = temp;
                temp->pai = NULL;
            } else {
                if (atual == atual->pai->esq) atual->pai->esq = temp;
                else atual->pai->dir = temp;
                temp->pai = atual->pai;
                no_para_balancear = atual->pai;
            }
            free(atual);
        }
    } else {
        // Caso 3: 2 filhos
        No* temp = min_valor_no(atual->dir, &arv->comparacoes);
        atual->valor = temp->valor;
        
        No* pai_temp = temp->pai;
        No* filho_temp = temp->dir; // Pode ser NULL
        
        if (filho_temp) filho_temp->pai = pai_temp;
        
        if (pai_temp == atual) {
            pai_temp->dir = filho_temp; // O pai é o próprio nó que teve valor substituido
            no_para_balancear = pai_temp;
        } else {
            pai_temp->esq = filho_temp;
            no_para_balancear = pai_temp;
        }
        free(temp);
    }

    if (no_para_balancear)
        balancear_avl(arv, no_para_balancear);
}
