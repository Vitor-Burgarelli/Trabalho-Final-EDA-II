#include <stdio.h>
#include <stdlib.h>

typedef struct no {
    int total;
    int *chaves;
    struct no **filhos;
    struct no *pai;
    int folha;
} No;

typedef struct arvoreB {
    No *raiz;
    int ordem;
} ArvoreB;

/* ------------------ CRIAÇÃO ------------------ */

No* criaNo(ArvoreB *arvore) {
    int max = arvore->ordem * 2;
    No *no = malloc(sizeof(No));
    
    no->total = 0;
    no->pai = NULL;
    no->folha = 1;

    no->chaves = malloc(sizeof(int) * (max + 1));
    no->filhos = malloc(sizeof(No*) * (max + 2));

    for (int i = 0; i < max + 2; i++)
        no->filhos[i] = NULL;

    return no;
}

ArvoreB* criaArvore(int ordem) {
    ArvoreB *arvore = malloc(sizeof(ArvoreB));
    arvore->ordem = ordem;
    arvore->raiz = criaNo(arvore);
    return arvore;
}

/* ------------------ PESQUISA BINÁRIA ------------------ */

int pesquisaBinaria(No *no, int chave) {
    int inicio = 0, fim = no->total - 1, meio;
    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        if (no->chaves[meio] == chave)
            return meio;
        else if (no->chaves[meio] > chave)
            fim = meio - 1;
        else
            inicio = meio + 1;
    }
    return inicio;
}

int localizaChave(ArvoreB *arvore, int chave) {
    No *no = arvore->raiz;
    while (no != NULL) {
        int i = pesquisaBinaria(no, chave);
        if (i < no->total && no->chaves[i] == chave)
            return 1;
        no = no->filhos[i];
    }
    return 0;
}

/* ------------------ SPLIT DO NÓ ------------------ */

void split(ArvoreB *arvore, No *no) {
    int meio = arvore->ordem;
    No *novo = criaNo(arvore);
    novo->folha = no->folha;

    // mover metade das chaves para novo nó
    for (int i = 0; i < arvore->ordem; i++)
        novo->chaves[i] = no->chaves[i + meio + 1];

    if (!no->folha) {
        for (int i = 0; i < arvore->ordem + 1; i++) {
            novo->filhos[i] = no->filhos[i + meio + 1];
            if (novo->filhos[i]) novo->filhos[i]->pai = novo;
        }
    }

    novo->total = arvore->ordem;
    no->total = meio;

    if (no->pai == NULL) {
        No *pai = criaNo(arvore);
        pai->folha = 0;
        pai->chaves[0] = no->chaves[meio];
        pai->filhos[0] = no;
        pai->filhos[1] = novo;
        pai->total = 1;
        no->pai = pai;
        novo->pai = pai;
        arvore->raiz = pai;
    } else {
        No *pai = no->pai;
        int i = pesquisaBinaria(pai, no->chaves[meio]);
        for (int j = pai->total; j > i; j--) {
            pai->chaves[j] = pai->chaves[j - 1];
            pai->filhos[j + 1] = pai->filhos[j];
        }
        pai->chaves[i] = no->chaves[meio];
        pai->filhos[i + 1] = novo;
        novo->pai = pai;
        pai->total++;

        if (pai->total > arvore->ordem * 2)
            split(arvore, pai);
    }
}

/* ------------------ INSERÇÃO ------------------ */

void insereNoNaoCheio(ArvoreB *arvore, No *no, int chave) {
    int i = pesquisaBinaria(no, chave);

    if (no->folha) {
        for (int j = no->total; j > i; j--)
            no->chaves[j] = no->chaves[j - 1];

        no->chaves[i] = chave;
        no->total++;
    } else {
        No *filho = no->filhos[i];
        if (filho->total == arvore->ordem * 2) {
            split(arvore, filho);
            if (chave > no->chaves[i]) i++;
        }
        insereNoNaoCheio(arvore, no->filhos[i], chave);
    }
}

void inserir(ArvoreB *arvore, int chave) {
    No *raiz = arvore->raiz;
    if (raiz->total == arvore->ordem * 2) {
        split(arvore, raiz);
        raiz = arvore->raiz;
    }
    insereNoNaoCheio(arvore, raiz, chave);
}

/* ------------------ IMPRESSÃO ------------------ */

void imprimeArvore(No *no, int nivel) {
    if (no != NULL) {
        for (int i = no->total; i >= 0; i--) {
            imprimeArvore(no->filhos[i], nivel + 1);
            if (i < no->total) {
                for (int j = 0; j < nivel; j++) printf("    ");
                printf("%d\n", no->chaves[i]);
            }
        }
    }
}

/* ------------------ MAIN ------------------ */

int main() {
    ArvoreB *arvore = criaArvore(2); // ordem 2 = B-tree 2-4

    int valores[] = {10, 20, 5, 6, 12, 30, 7, 17};
    for (int i = 0; i < 8; i++) {
        inserir(arvore, valores[i]);
        printf("\nApós inserir %d:\n", valores[i]);
        imprimeArvore(arvore->raiz, 0);
    }

    return 0;
}
