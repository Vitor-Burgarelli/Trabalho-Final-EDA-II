#include <stdio.h>
#include <stdlib.h>

typedef enum { Vermelho, Preto } Cor;

typedef struct no {
    struct no* pai;
    struct no* esq;
    struct no* dir;
    Cor cor;
    int valor;
} No;

typedef struct {
    No* raiz;
    No* nulo;
} Arvore;

/* ==================== CRIAÇÃO DE NÓS E DA ÁRVORE ==================== */

No* criar_no(Arvore* arvore, int valor) {
    No* novo = (No*)malloc(sizeof(No));
    novo->valor = valor;
    novo->pai = arvore->nulo;
    novo->esq = arvore->nulo;
    novo->dir = arvore->nulo;
    novo->cor = Vermelho; // regra: nós começam vermelhos
    return novo;
}

Arvore* criar_arvore() {
    Arvore* arvore = (Arvore*)malloc(sizeof(Arvore));
    arvore->nulo = (No*)malloc(sizeof(No));
    arvore->nulo->cor = Preto;
    arvore->nulo->pai = NULL;
    arvore->nulo->esq = NULL;
    arvore->nulo->dir = NULL;
    arvore->raiz = arvore->nulo;
    return arvore;
}

/* ==================== ROTAÇÕES ==================== */

void rotacionarEsq(Arvore* arvore, No* no) {
    No* direita = no->dir;
    no->dir = direita->esq;

    if (direita->esq != arvore->nulo)
        direita->esq->pai = no;

    direita->pai = no->pai;

    if (no->pai == arvore->nulo)
        arvore->raiz = direita;
    else if (no == no->pai->esq)
        no->pai->esq = direita;
    else
        no->pai->dir = direita;

    direita->esq = no;
    no->pai = direita;
}

void rotacionarDir(Arvore* arvore, No* no) {
    No* esquerda = no->esq;
    no->esq = esquerda->dir;

    if (esquerda->dir != arvore->nulo)
        esquerda->dir->pai = no;

    esquerda->pai = no->pai;

    if (no->pai == arvore->nulo)
        arvore->raiz = esquerda;
    else if (no == no->pai->dir)
        no->pai->dir = esquerda;
    else
        no->pai->esq = esquerda;

    esquerda->dir = no;
    no->pai = esquerda;
}

/* ==================== BALANCEAMENTO APÓS INSERÇÃO ==================== */

void balancear(Arvore* arvore, No* no) {
    while (no->pai->cor == Vermelho) {
        if (no->pai == no->pai->pai->esq) {
            No* tio = no->pai->pai->dir;

            if (tio->cor == Vermelho) {  // Caso 1: tio vermelho
                no->pai->cor = Preto;
                tio->cor = Preto;
                no->pai->pai->cor = Vermelho;
                no = no->pai->pai;
            } else {
                if (no == no->pai->dir) { // Caso 2/3
                    no = no->pai;
                    rotacionarEsq(arvore, no);
                }
                no->pai->cor = Preto;
                no->pai->pai->cor = Vermelho;
                rotacionarDir(arvore, no->pai->pai);
            }

        } else {
            No* tio = no->pai->pai->esq;

            if (tio->cor == Vermelho) {
                no->pai->cor = Preto;
                tio->cor = Preto;
                no->pai->pai->cor = Vermelho;
                no = no->pai->pai;
            } else {
                if (no == no->pai->esq) {
                    no = no->pai;
                    rotacionarDir(arvore, no);
                }
                no->pai->cor = Preto;
                no->pai->pai->cor = Vermelho;
                rotacionarEsq(arvore, no->pai->pai);
            }
        }
    }
    arvore->raiz->cor = Preto; // regra 2: raiz sempre é preta
}

/* ==================== INSERÇÃO ==================== */

void inserir(Arvore* arvore, int valor) {
    No* novo = criar_no(arvore, valor);
    No* y = arvore->nulo;
    No* x = arvore->raiz;

    while (x != arvore->nulo) {
        y = x;
        if (novo->valor < x->valor)
            x = x->esq;
        else
            x = x->dir;
    }

    novo->pai = y;

    if (y == arvore->nulo)
        arvore->raiz = novo;
    else if (novo->valor < y->valor)
        y->esq = novo;
    else
        y->dir = novo;

    balancear(arvore, novo);
}

/* ==================== FUNÇÕES DE IMPRESSÃO ==================== */

void imprimir_inorder(Arvore* arvore, No* no) {
    if (no != arvore->nulo) {
        imprimir_inorder(arvore, no->esq);
        printf("%d(%c) ", no->valor, no->cor == Vermelho ? 'R' : 'P');
        imprimir_inorder(arvore, no->dir);
    }
}

void imprimir_estrutura(Arvore* arvore, No* no, int nivel) {
    if (no != arvore->nulo) {
        imprimir_estrutura(arvore, no->dir, nivel + 1);
        for (int i = 0; i < nivel; i++) printf("    ");
        printf("%d(%c)\n", no->valor, no->cor == Vermelho ? 'R':'P');
        imprimir_estrutura(arvore, no->esq, nivel + 1);
    }
}

/* ==================== MAIN PARA TESTE ==================== */

int main() {
    Arvore* arvore = criar_arvore();

    int valores[] = {4, 2, 1, 3, 8, 6, 5, 7, 9};
    int n = sizeof(valores)/sizeof(valores[0]);

    for (int i = 0; i < n; i++) {
        inserir(arvore, valores[i]);
        printf("\nInseriu %d:\n", valores[i]);
        imprimir_estrutura(arvore, arvore->raiz, 0);
        printf("--------------------------------------------------\n");
    }

    printf("\nInorder final:\n");
    imprimir_inorder(arvore, arvore->raiz);
    printf("\n");

    return 0;
}
