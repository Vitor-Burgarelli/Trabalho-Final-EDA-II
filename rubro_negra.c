#include <stdio.h>
#include <stdlib.h>

typedef enum { Vermelho, Preto } Cor;

typedef struct noRB {
    struct noRB* pai;
    struct noRB* esq;
    struct noRB* dir;
    Cor cor;
    int valor;
} NoRB;

typedef struct {
    NoRB* raiz;
    NoRB* nulo; 
    long long comparacoes; 
} ArvoreRB;

/* ==================== CRIAÇÃO ==================== */

ArvoreRB* criar_arvore_rb() {
    ArvoreRB* arvore = (ArvoreRB*)malloc(sizeof(ArvoreRB));
    arvore->nulo = (NoRB*)malloc(sizeof(NoRB));
    arvore->nulo->cor = Preto;
    arvore->nulo->pai = NULL; 
    arvore->nulo->esq = NULL;
    arvore->nulo->dir = NULL;
    arvore->raiz = arvore->nulo;
    arvore->comparacoes = 0;
    return arvore;
}

NoRB* criar_no_rb(ArvoreRB* arvore, int valor) {
    NoRB* novo = (NoRB*)malloc(sizeof(NoRB));
    novo->valor = valor;
    novo->pai = arvore->nulo;
    novo->esq = arvore->nulo;
    novo->dir = arvore->nulo;
    novo->cor = Vermelho;
    return novo;
}

/* ==================== ROTAÇÕES ==================== */

void rotacionarEsqRB(ArvoreRB* arvore, NoRB* x) {
    arvore->comparacoes++; // Custo de rotação

    NoRB* y = x->dir;
    x->dir = y->esq;

    if (y->esq != arvore->nulo)
        y->esq->pai = x;

    y->pai = x->pai;

    if (x->pai == arvore->nulo) 
        arvore->raiz = y;
    else if (x == x->pai->esq)
        x->pai->esq = y;
    else
        x->pai->dir = y;

    y->esq = x;
    x->pai = y;
}

void rotacionarDirRB(ArvoreRB* arvore, NoRB* y) {
    arvore->comparacoes++; // Custo de rotação

    NoRB* x = y->esq;
    y->esq = x->dir;

    if (x->dir != arvore->nulo)
        x->dir->pai = y;

    x->pai = y->pai;

    if (y->pai == arvore->nulo)
        arvore->raiz = x;
    else if (y == y->pai->dir)
        y->pai->dir = x;
    else
        y->pai->esq = x;

    x->dir = y;
    y->pai = x;
}

/* ==================== INSERÇÃO ==================== */

void balancear_insercao(ArvoreRB* arvore, NoRB* z) {
    while (z->pai->cor == Vermelho) {
        arvore->comparacoes++; // Custo do loop/verificação

        if (z->pai == z->pai->pai->esq) {
            NoRB* y = z->pai->pai->dir;

            arvore->comparacoes++;
            if (y->cor == Vermelho) {
                z->pai->cor = Preto;
                y->cor = Preto;
                z->pai->pai->cor = Vermelho;
                z = z->pai->pai;
            } else {
                if (z == z->pai->dir) {
                    arvore->comparacoes++;
                    z = z->pai;
                    rotacionarEsqRB(arvore, z);
                }
                z->pai->cor = Preto;
                z->pai->pai->cor = Vermelho;
                rotacionarDirRB(arvore, z->pai->pai);
            }
        } else {
            NoRB* y = z->pai->pai->esq;
            if (y->cor == Vermelho) {
                z->pai->cor = Preto;
                y->cor = Preto;
                z->pai->pai->cor = Vermelho;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esq) {
                    arvore->comparacoes++;
                    z = z->pai;
                    rotacionarDirRB(arvore, z);
                }
                z->pai->cor = Preto;
                z->pai->pai->cor = Vermelho;
                rotacionarEsqRB(arvore, z->pai->pai);
            }
        }
    }
    arvore->comparacoes++;
    arvore->raiz->cor = Preto;
}

void inserir_rb(ArvoreRB* arvore, int valor) {
    NoRB* z = criar_no_rb(arvore, valor);
    NoRB* y = arvore->nulo;
    NoRB* x = arvore->raiz;

    while (x != arvore->nulo) {
        y = x;
        arvore->comparacoes++; 
        if (z->valor < x->valor)
            x = x->esq;
        else
            x = x->dir;
    }

    z->pai = y;
    if (y == arvore->nulo)
        arvore->raiz = z;
    else if (z->valor < y->valor)
        y->esq = z;
    else
        y->dir = z;

    balancear_insercao(arvore, z);
}

/* ==================== REMOÇÃO ==================== */

void transplant(ArvoreRB* arvore, NoRB* u, NoRB* v) {
    if (u->pai == arvore->nulo)
        arvore->raiz = v;
    else if (u == u->pai->esq)
        u->pai->esq = v;
    else
        u->pai->dir = v;
    v->pai = u->pai;
}

NoRB* minimum(ArvoreRB* arvore, NoRB* x) {
    while (x->esq != arvore->nulo) {
        arvore->comparacoes++; 
        x = x->esq;
    }
    return x;
}

void balancear_remocao(ArvoreRB* arvore, NoRB* x) {
    while (x != arvore->raiz && x->cor == Preto) {
        arvore->comparacoes++;

        if (x == x->pai->esq) {
            NoRB* w = x->pai->dir;

            // Caso 1
            arvore->comparacoes++;
            if (w->cor == Vermelho) {
                w->cor = Preto;
                x->pai->cor = Vermelho;
                rotacionarEsqRB(arvore, x->pai);
                w = x->pai->dir;
            }

           // Caso 2
            arvore->comparacoes++;
            if (w->esq->cor == Preto && w->dir->cor == Preto) {
                w->cor = Vermelho;
                x = x->pai;
            } else {

                // Caso 3
                arvore->comparacoes++;
                if (w->dir->cor == Preto) {
                    w->esq->cor = Preto;
                    w->cor = Vermelho;
                    rotacionarDirRB(arvore, w);
                    w = x->pai->dir;
                }
                // Caso 4
                w->cor = x->pai->cor;
                x->pai->cor = Preto;
                w->dir->cor = Preto;
                rotacionarEsqRB(arvore, x->pai);
                x = arvore->raiz;
            }
        } else { // Simétrico
            NoRB* w = x->pai->esq;

            arvore->comparacoes++;
            if (w->cor == Vermelho) {
                w->cor = Preto;
                x->pai->cor = Vermelho;
                rotacionarDirRB(arvore, x->pai);
                w = x->pai->esq;
            }

            arvore->comparacoes++;
            if (w->dir->cor == Preto && w->esq->cor == Preto) {
                w->cor = Vermelho;
                x = x->pai;
            } else {
                arvore->comparacoes++;
                if (w->esq->cor == Preto) {
                    w->dir->cor = Preto;
                    w->cor = Vermelho;
                    rotacionarEsqRB(arvore, w);
                    w = x->pai->esq;
                }
                w->cor = x->pai->cor;
                x->pai->cor = Preto;
                w->esq->cor = Preto;
                rotacionarDirRB(arvore, x->pai);
                x = arvore->raiz;
            }
        }
    }
    x->cor = Preto;
}

void remover_rb(ArvoreRB* arvore, int valor) {
    NoRB* z = arvore->raiz;
    while (z != arvore->nulo) {
        arvore->comparacoes++; 
        if (valor == z->valor) break;
        if (valor < z->valor) z = z->esq;
        else z = z->dir;
    }
    if (z == arvore->nulo) return; 

    NoRB* y = z;
    NoRB* x;
    Cor cor_original_y = y->cor;

    if (z->esq == arvore->nulo) {
        x = z->dir;
        transplant(arvore, z, z->dir);
    } else if (z->dir == arvore->nulo) {
        x = z->esq;
        transplant(arvore, z, z->esq);
    } else {
        y = minimum(arvore, z->dir);
        cor_original_y = y->cor;
        x = y->dir;
        
        if (y->pai == z) {
            x->pai = y;
        } else {
            transplant(arvore, y, y->dir);
            y->dir = z->dir;
            y->dir->pai = y;
        }
        
        transplant(arvore, z, y);
        y->esq = z->esq;
        y->esq->pai = y;
        y->cor = z->cor;
    }

    if (cor_original_y == Preto)
        balancear_remocao(arvore, x);
        
    free(z);
}

void liberar_rb(ArvoreRB* arvore, NoRB* no) {
    if (no != arvore->nulo) {
        liberar_rb(arvore, no->esq);
        liberar_rb(arvore, no->dir);
        free(no);
    }
}
