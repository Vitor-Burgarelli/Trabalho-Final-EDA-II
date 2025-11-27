#include <stdio.h>
#include <stdlib.h>

typedef struct noB {
    int total;          // Numero atual de chaves
    int *chaves;        // Array de chaves
    struct noB **filhos;// Array de ponteiros para filhos
    struct noB *pai;    // Ponteiro para o pai
    int folha;          // 1 se for folha, 0 se nao
} NoB;

typedef struct {
    NoB *raiz;
    int ordem;          // t
    long long comparacoes;
} ArvoreB;

/* ------------------ CRIAÇÃO ------------------ */

NoB* criaNoB(ArvoreB *arvore) {
    int max_chaves = arvore->ordem * 2;
    NoB *no = (NoB*)malloc(sizeof(NoB));

    no->total = 0;
    no->pai = NULL;
    no->folha = 1;
    no->chaves = (int*)malloc(sizeof(int) * (max_chaves + 1));
    no->filhos = (NoB**)malloc(sizeof(NoB*) * (max_chaves + 2));

    for (int i = 0; i < max_chaves + 2; i++)
        no->filhos[i] = NULL;

    return no;
}

ArvoreB* criaArvoreB(int ordem) {
    ArvoreB *arvore = (ArvoreB*)malloc(sizeof(ArvoreB));
    arvore->ordem = ordem;
    arvore->raiz = criaNoB(arvore);
    arvore->comparacoes = 0;
    return arvore;
}

/* ------------------ PESQUISA ------------------ */

// Retorna o indice da primeira chave maior ou igual a k
int buscaBinariaB(ArvoreB* arv, NoB *no, int chave) {
    int inicio = 0, fim = no->total - 1, meio;
    int resposta = no->total;

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        if (no->chaves[meio] == chave) {
            return meio;
        }
        if (no->chaves[meio] > chave) {
            resposta = meio;
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
    }
    return resposta;
}

/* ------------------ AUXILIARES DE REMOÇÃO ------------------ */

void removerChaveDeNo(NoB *no, int idx) {
    for (int i = idx; i < no->total - 1; ++i) {
        no->chaves[i] = no->chaves[i+1];
    }
    no->total--;
}

void removerFilhoDeNo(NoB *no, int idx) {
    for (int i = idx; i < no->total + 1; ++i) {
        no->filhos[i] = no->filhos[i+1];
    }
}

/* ------------------ SPLIT E INSERÇÃO ------------------ */

void splitB(ArvoreB *arvore, NoB *no) {
    arvore->comparacoes++; // CONTA O SPLIT (Custo estrutural)

    int t = arvore->ordem;
    NoB *novo = criaNoB(arvore);
    novo->folha = no->folha;
    novo->total = t - 1;

    for (int j = 0; j < t - 1; j++) {
        novo->chaves[j] = no->chaves[j + t + 1];
    }

    if (!no->folha) {
        for (int j = 0; j < t; j++) {
            novo->filhos[j] = no->filhos[j + t + 1];
            if (novo->filhos[j]) novo->filhos[j]->pai = novo;
        }
    }

    int chaveMediana = no->chaves[t];
    no->total = t;

    if (no->pai == NULL) {
        NoB *pai = criaNoB(arvore);
        pai->folha = 0;
        pai->chaves[0] = chaveMediana;
        pai->filhos[0] = no;
        pai->filhos[1] = novo;
        pai->total = 1;

        no->pai = pai;
        novo->pai = pai;
        arvore->raiz = pai;
    } else {
        NoB *pai = no->pai;
        int i = buscaBinariaB(arvore, pai, chaveMediana);

        for (int j = pai->total; j > i; j--) {
            pai->chaves[j] = pai->chaves[j - 1];
            pai->filhos[j + 1] = pai->filhos[j];
        }

        pai->chaves[i] = chaveMediana;
        pai->filhos[i + 1] = novo;
        novo->pai = pai;
        pai->total++;

        if (pai->total == arvore->ordem * 2) {
            splitB(arvore, pai);
        }
    }
}

void insereNaoCheioB(ArvoreB *arvore, NoB *no, int chave) {
    arvore->comparacoes++; // CONTA VISITA AO NÓ (Custo de descida)

    int i = no->total - 1;

    if (no->folha) {
        while (i >= 0) {
            if (no->chaves[i] > chave) {
                no->chaves[i + 1] = no->chaves[i];
                i--;
            } else {
                break;
            }
        }
        no->chaves[i + 1] = chave;
        no->total++;
    } else {
        while (i >= 0) {
            if (no->chaves[i] > chave) i--;
            else break;
        }
        i++;

        if (no->filhos[i]->total == arvore->ordem * 2) {
            splitB(arvore, no->filhos[i]);
            if (chave > no->chaves[i]) i++;
        }
        insereNaoCheioB(arvore, no->filhos[i], chave);
    }
}

void inserir_b(ArvoreB *arvore, int chave) {
    NoB *raiz = arvore->raiz;
    if (raiz->total == arvore->ordem * 2) {
        splitB(arvore, raiz);
        insereNaoCheioB(arvore, arvore->raiz, chave);
    } else {
        insereNaoCheioB(arvore, raiz, chave);
    }
}

/* ------------------ REMOÇÃO ------------------ */

void removerDeNo(ArvoreB *arv, NoB *no, int chave);
void mergeB(ArvoreB *arv, NoB *no, int idx);
void preencher(ArvoreB *arv, NoB *no, int idx);
int getPredecessor(ArvoreB *arv, NoB *no, int idx);
int getSucessor(ArvoreB *arv, NoB *no, int idx);

void borrowFromPrev(ArvoreB *arv, NoB *no, int idx) {
    arv->comparacoes++; // Conta custo estrutural
    NoB *child = no->filhos[idx];
    NoB *sibling = no->filhos[idx - 1];

    for (int i = child->total - 1; i >= 0; --i)
        child->chaves[i+1] = child->chaves[i];

    if (!child->folha) {
        for(int i = child->total; i >= 0; --i)
            child->filhos[i+1] = child->filhos[i];
    }

    child->chaves[0] = no->chaves[idx-1];

    if(!child->folha)
        child->filhos[0] = sibling->filhos[sibling->total];

    if(child->filhos[0]) child->filhos[0]->pai = child;

    no->chaves[idx-1] = sibling->chaves[sibling->total-1];

    child->total += 1;
    sibling->total -= 1;
}

void borrowFromNext(ArvoreB *arv, NoB *no, int idx) {
    arv->comparacoes++; // Conta custo estrutural
    NoB *child = no->filhos[idx];
    NoB *sibling = no->filhos[idx + 1];

    child->chaves[child->total] = no->chaves[idx];

    if (!(child->folha))
        child->filhos[child->total + 1] = sibling->filhos[0];

    if(child->filhos[child->total + 1])
        child->filhos[child->total + 1]->pai = child;

    no->chaves[idx] = sibling->chaves[0];

    for (int i = 1; i < sibling->total; ++i)
        sibling->chaves[i-1] = sibling->chaves[i];

    if (!sibling->folha) {
        for(int i = 1; i <= sibling->total; ++i)
            sibling->filhos[i-1] = sibling->filhos[i];
    }

    child->total += 1;
    sibling->total -= 1;
}

void mergeB(ArvoreB *arv, NoB *no, int idx) {
    arv->comparacoes++; // Conta custo estrutural (fusão)
    NoB *child = no->filhos[idx];
    NoB *sibling = no->filhos[idx+1];
    int t = arv->ordem;

    child->chaves[t] = no->chaves[idx];
    int mid = child->total;
    child->chaves[mid] = no->chaves[idx];

    for (int i=0; i<sibling->total; ++i)
        child->chaves[i + mid + 1] = sibling->chaves[i];

    if (!child->folha) {
        for(int i=0; i<=sibling->total; ++i) {
            child->filhos[i + mid + 1] = sibling->filhos[i];
            if(child->filhos[i + mid + 1]) child->filhos[i + mid + 1]->pai = child;
        }
    }

    for (int i=idx+1; i<no->total; ++i)
        no->chaves[i-1] = no->chaves[i];

    for (int i=idx+2; i<=no->total; ++i)
        no->filhos[i-1] = no->filhos[i];

    child->total += sibling->total + 1;
    no->total--;

    free(sibling->chaves);
    free(sibling->filhos);
    free(sibling);
}

void preencher(ArvoreB *arv, NoB *no, int idx) {
    int t = arv->ordem;

    if (idx != 0 && no->filhos[idx-1]->total >= t + 1)
        borrowFromPrev(arv, no, idx);
    else if (idx != no->total && no->filhos[idx+1]->total >= t + 1)
        borrowFromNext(arv, no, idx);
    else {
        if (idx != no->total)
            mergeB(arv, no, idx);
        else
            mergeB(arv, no, idx-1);
    }
}

int getPredecessor(ArvoreB *arv, NoB *no, int idx) {
    NoB *cur = no->filhos[idx];
    while (!cur->folha)
        cur = cur->filhos[cur->total];
    return cur->chaves[cur->total-1];
}

int getSucessor(ArvoreB *arv, NoB *no, int idx) {
    NoB *cur = no->filhos[idx+1];
    while (!cur->folha)
        cur = cur->filhos[0];
    return cur->chaves[0];
}

void removerDeNo(ArvoreB *arv, NoB *no, int chave) {
    arv->comparacoes++; // CONTA VISITA AO NÓ NA REMOÇÃO

    int idx = buscaBinariaB(arv, no, chave);

    if (idx < no->total && no->chaves[idx] == chave) {
        if (no->folha) {
            removerChaveDeNo(no, idx);
        } else {
            int k = no->chaves[idx];

            if (no->filhos[idx]->total >= arv->ordem + 1) {
                int pred = getPredecessor(arv, no, idx);
                no->chaves[idx] = pred;
                removerDeNo(arv, no->filhos[idx], pred);
            }
            else if (no->filhos[idx+1]->total >= arv->ordem + 1) {
                int succ = getSucessor(arv, no, idx);
                no->chaves[idx] = succ;
                removerDeNo(arv, no->filhos[idx+1], succ);
            }
            else {
                mergeB(arv, no, idx);
                removerDeNo(arv, no->filhos[idx], k);
            }
        }
    } else {
        if (no->folha) {
            return;
        }

        int flag = (idx == no->total) ? 1 : 0;

        if (no->filhos[idx]->total < arv->ordem + 1) {
            preencher(arv, no, idx);
        }

        if (flag && idx > no->total)
            removerDeNo(arv, no->filhos[idx-1], chave);
        else
            removerDeNo(arv, no->filhos[idx], chave);
    }
}

void remover_b(ArvoreB *arvore, int chave) {
    if (arvore->raiz->total == 0) return;

    removerDeNo(arvore, arvore->raiz, chave);

    if (arvore->raiz->total == 0) {
        NoB *tmp = arvore->raiz;
        if (arvore->raiz->folha)
            arvore->raiz = NULL;
        else
            arvore->raiz = arvore->raiz->filhos[0];

        if(arvore->raiz) arvore->raiz->pai = NULL;

        free(tmp->chaves);
        free(tmp->filhos);
        free(tmp);

        if (arvore->raiz == NULL) {
            arvore->raiz = criaNoB(arvore);
        }
    }
}

/* ------------------ LIBERAR ------------------ */

void liberar_b_rec(NoB *no) {
    if (no) {
        if (!no->folha) {
            for (int i = 0; i <= no->total; i++)
                liberar_b_rec(no->filhos[i]);
        }
        free(no->chaves);
        free(no->filhos);
        free(no);
    }
}

void liberar_b(ArvoreB *arv) {
    if (arv) {
        liberar_b_rec(arv->raiz);
        free(arv);
    }
}
