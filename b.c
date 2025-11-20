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
    int resposta = no->total; // Se nao achar maior, retorna total (posicao do filho a direita)

    while (inicio <= fim) {
        arv->comparacoes++; 
        meio = (inicio + fim) / 2;
        
        arv->comparacoes++; // Comparacao da chave
        if (no->chaves[meio] == chave) {
            return meio; 
        }
        
        arv->comparacoes++; // Comparacao de magnitude
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

// Remove a chave na posicao idx de um no (apenas shift array)
void removerChaveDeNo(NoB *no, int idx) {
    for (int i = idx; i < no->total - 1; ++i) {
        no->chaves[i] = no->chaves[i+1];
    }
    no->total--;
}

// Remove o filho na posicao idx (apenas shift array)
void removerFilhoDeNo(NoB *no, int idx) {
    for (int i = idx; i < no->total + 1; ++i) { // total+1 é o numero de filhos atual
        no->filhos[i] = no->filhos[i+1];
    }
}

/* ------------------ SPLIT E INSERÇÃO ------------------ */

void splitB(ArvoreB *arvore, NoB *no) {
    int t = arvore->ordem;
    // O no esta cheio (2*t chaves). Vamos dividir.
    // Novo no armazenara as chaves t+1 a 2*t.
    // A chave mediana (indice t) sobe.

    NoB *novo = criaNoB(arvore);
    novo->folha = no->folha;
    novo->total = t - 1; // Se max era 2*t, sobra t-1 pro novo, t-1 pro velho, 1 sobe.
    
    // Ajuste para logica do seu codigo original (max = ordem*2)
    // Se ordem=2 (max 4), split ocorre quando tem 5? Ou quando enche 4?
    // Vamos assumir que split ocorre quando tem 2*t chaves.
    // Mediana é index t.
    
    // Copia chaves para o novo no
    for (int j = 0; j < t - 1; j++) { // copia chaves finais
        novo->chaves[j] = no->chaves[j + t + 1];
    }

    // Copia filhos se nao for folha
    if (!no->folha) {
        for (int j = 0; j < t; j++) {
            novo->filhos[j] = no->filhos[j + t + 1];
            if (novo->filhos[j]) novo->filhos[j]->pai = novo;
        }
    }

    // A chave mediana que vai subir
    int chaveMediana = no->chaves[t];

    no->total = t; // Reduz tamanho do no original

    // Inserir no pai
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
        // Achar posicao no pai
        int i = buscaBinariaB(arvore, pai, chaveMediana);
        
        // Shift no pai para abrir espaco
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
    int i = no->total - 1;

    if (no->folha) {
        // Achar posicao e fazer shift
        while (i >= 0) {
            arvore->comparacoes++;
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
        // Achar filho correto
        while (i >= 0) {
            arvore->comparacoes++;
            if (no->chaves[i] > chave) i--;
            else break;
        }
        i++;
        
        if (no->filhos[i]->total == arvore->ordem * 2) {
            splitB(arvore, no->filhos[i]);
            arvore->comparacoes++;
            if (chave > no->chaves[i]) i++;
        }
        insereNaoCheioB(arvore, no->filhos[i], chave);
    }
}

void inserir_b(ArvoreB *arvore, int chave) {
    NoB *raiz = arvore->raiz;
    if (raiz->total == arvore->ordem * 2) {
        splitB(arvore, raiz);
        // Apos split, a raiz muda (foi criada nova raiz acima)
        // Porem a funcao split ja atualiza arvore->raiz
        insereNaoCheioB(arvore, arvore->raiz, chave);
    } else {
        insereNaoCheioB(arvore, raiz, chave);
    }
}

/* ------------------ REMOÇÃO (Lógica Complexa) ------------------ */

// Assinaturas
void removerDeNo(ArvoreB *arv, NoB *no, int chave);
void mergeB(ArvoreB *arv, NoB *no, int idx);
void preencher(ArvoreB *arv, NoB *no, int idx);
int getPredecessor(ArvoreB *arv, NoB *no, int idx);
int getSucessor(ArvoreB *arv, NoB *no, int idx);

// Pega emprestado do irmao anterior
void borrowFromPrev(ArvoreB *arv, NoB *no, int idx) {
    NoB *child = no->filhos[idx];
    NoB *sibling = no->filhos[idx - 1];

    // Shift filho p/ direita
    for (int i = child->total - 1; i >= 0; --i)
        child->chaves[i+1] = child->chaves[i];

    if (!child->folha) {
        for(int i = child->total; i >= 0; --i)
            child->filhos[i+1] = child->filhos[i];
    }

    child->chaves[0] = no->chaves[idx-1]; // Pai desce
    
    if(!child->folha)
        child->filhos[0] = sibling->filhos[sibling->total];

    if(child->filhos[0]) child->filhos[0]->pai = child;

    no->chaves[idx-1] = sibling->chaves[sibling->total-1]; // Irmao sobe

    child->total += 1;
    sibling->total -= 1;
}

// Pega emprestado do irmao seguinte
void borrowFromNext(ArvoreB *arv, NoB *no, int idx) {
    NoB *child = no->filhos[idx];
    NoB *sibling = no->filhos[idx + 1];

    child->chaves[child->total] = no->chaves[idx]; // Pai desce

    if (!(child->folha))
        child->filhos[child->total + 1] = sibling->filhos[0];
    
    if(child->filhos[child->total + 1]) 
        child->filhos[child->total + 1]->pai = child;

    no->chaves[idx] = sibling->chaves[0]; // Irmao sobe

    for (int i = 1; i < sibling->total; ++i)
        sibling->chaves[i-1] = sibling->chaves[i];

    if (!sibling->folha) {
        for(int i = 1; i <= sibling->total; ++i)
            sibling->filhos[i-1] = sibling->filhos[i];
    }

    child->total += 1;
    sibling->total -= 1;
}

// Funde filho[idx] com filho[idx+1]
void mergeB(ArvoreB *arv, NoB *no, int idx) {
    NoB *child = no->filhos[idx];
    NoB *sibling = no->filhos[idx+1];
    int t = arv->ordem;

    // Desce a chave do pai
    child->chaves[t] = no->chaves[idx]; 
    // seu codigo original usa t como parametro, entao child tinha t chaves?
    // Aqui ajustamos para juntar tudo em 'child'
    // Child tem t-1 chaves (minimo -1, motivo do merge)
    // Sibling tem t chaves (minimo) ou t-1
    // Vamos usar 'total' para ser generico
    
    // Como é "merge", estamos assumindo que ambos sao pequenos e cabem juntos.
    int mid = child->total;
    child->chaves[mid] = no->chaves[idx];
    
    // Copia chaves do irmao
    for (int i=0; i<sibling->total; ++i)
        child->chaves[i + mid + 1] = sibling->chaves[i];

    // Copia filhos do irmao
    if (!child->folha) {
        for(int i=0; i<=sibling->total; ++i) {
            child->filhos[i + mid + 1] = sibling->filhos[i];
            if(child->filhos[i + mid + 1]) child->filhos[i + mid + 1]->pai = child;
        }
    }

    // Shift no pai para remover a chave que desceu e o ponteiro do irmao
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

// Garante que o filho tenha chaves suficientes antes de descer
void preencher(ArvoreB *arv, NoB *no, int idx) {
    int t = arv->ordem; 
    // Minimo chaves = t (definicao usada no codigo original para split em 2*t)
    
    // Se filho idx tem menos de t chaves (underflow iminente)
    if (idx != 0 && no->filhos[idx-1]->total >= t + 1) // sua logica: min é t
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
    int idx = buscaBinariaB(arv, no, chave);
    
    // A chave está neste nó
    if (idx < no->total && no->chaves[idx] == chave) {
        if (no->folha) {
            removerChaveDeNo(no, idx);
        } else {
            int k = no->chaves[idx];
            
            // Caso 2a: Filho a esquerda tem chaves suficientes?
            if (no->filhos[idx]->total >= arv->ordem + 1) { // aqui ajustavel dependendo se min = t ou t-1
                int pred = getPredecessor(arv, no, idx);
                no->chaves[idx] = pred;
                removerDeNo(arv, no->filhos[idx], pred);
            }
            // Caso 2b: Filho a direita tem?
            else if (no->filhos[idx+1]->total >= arv->ordem + 1) {
                int succ = getSucessor(arv, no, idx);
                no->chaves[idx] = succ;
                removerDeNo(arv, no->filhos[idx+1], succ);
            }
            // Caso 2c: Merge
            else {
                mergeB(arv, no, idx);
                removerDeNo(arv, no->filhos[idx], k);
            }
        }
    } else {
        // A chave nao esta aqui, deve estar nos filhos
        if (no->folha) {
            return; // Chave nao existe na arvore
        }

        int flag = (idx == no->total) ? 1 : 0;

        if (no->filhos[idx]->total < arv->ordem + 1) { // Proactive fix: se filho é "magro"
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

    // Se raiz ficou vazia apos merge
    if (arvore->raiz->total == 0) {
        NoB *tmp = arvore->raiz;
        if (arvore->raiz->folha)
            arvore->raiz = NULL; // Arvore vazia (cuidado na reimplementacao para nao dar segfault)
        else
            arvore->raiz = arvore->raiz->filhos[0];
        
        if(arvore->raiz) arvore->raiz->pai = NULL;
        
        free(tmp->chaves);
        free(tmp->filhos);
        free(tmp);
        
        // Se a arvore ficou vazia, recriamos uma raiz limpa para nao quebrar insercoes futuras
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