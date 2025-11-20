#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "avl.c" 
#include "b.c" 
#include "rubro_negra.c" 

#define PASSO 100 
#define AMOSTRAS 10

typedef struct {
    double ins_avl, rem_avl;
    double ins_rb, rem_rb;
    double ins_b1, rem_b1;
    double ins_b5, rem_b5;
    double ins_b10, rem_b10;
} Resultado;

void get_media_avl(int n, int amostras, double *m_ins, double *m_rem) {
    long long total_ins = 0, total_rem = 0;
    for(int k=0; k<amostras; k++) {
        ArvoreAVL avl = {NULL, 0};
        int *chaves = malloc(n * sizeof(int));
        for(int i=0; i<n; i++) chaves[i] = rand();

        avl.comparacoes = 0;
        for(int i=0; i<n; i++) inserir_avl(&avl, chaves[i]);
        total_ins += avl.comparacoes;

        avl.comparacoes = 0;
        for(int i=0; i<n; i++) remover_avl_no(&avl, avl.raiz, chaves[i]);
        total_rem += avl.comparacoes;

        free(chaves);
    }
    *m_ins = (double)total_ins / amostras;
    *m_rem = (double)total_rem / amostras;
}

void get_media_rb(int n, int amostras, double *m_ins, double *m_rem) {
    long long total_ins = 0, total_rem = 0;
    for(int k=0; k<amostras; k++) {
        ArvoreRB* rb = criar_arvore_rb();
        int *chaves = malloc(n * sizeof(int));
        for(int i=0; i<n; i++) chaves[i] = rand();

        rb->comparacoes = 0;
        for(int i=0; i<n; i++) inserir_rb(rb, chaves[i]);
        total_ins += rb->comparacoes;

        rb->comparacoes = 0;
        for(int i=0; i<n; i++) remover_rb(rb, chaves[i]);
        total_rem += rb->comparacoes;

        liberar_rb(rb, rb->raiz);
        free(rb->nulo);
        free(rb);
        free(chaves);
    }
    *m_ins = (double)total_ins / amostras;
    *m_rem = (double)total_rem / amostras;
}

void get_media_b(int n, int amostras, int ordem, double *m_ins, double *m_rem) {
    long long total_ins = 0, total_rem = 0;
    for(int k=0; k<amostras; k++) {
        ArvoreB* b = criaArvoreB(ordem);
        int *chaves = malloc(n * sizeof(int));
        for(int i=0; i<n; i++) chaves[i] = rand();

        b->comparacoes = 0;
        for(int i=0; i<n; i++) inserir_b(b, chaves[i]);
        total_ins += b->comparacoes;

        b->comparacoes = 0;
        for(int i=0; i<n; i++) remover_b(b, chaves[i]);
        total_rem += b->comparacoes;

        liberar_b(b);
        free(chaves);
    }
    *m_ins = (double)total_ins / amostras;
    *m_rem = (double)total_rem / amostras;
}

int main() {
    srand(time(NULL));

    // Cabeçalho do CSV
    printf("N;AVL_Ins;AVL_Rem;RB_Ins;RB_Rem;B1_Ins;B1_Rem;B5_Ins;B5_Rem;B10_Ins;B10_Rem\n");

    // Logica do Loop: Garante N=1, depois segue o PASSO
    for (int n = 1; n <= 10000; ) {
        Resultado r;

        get_media_avl(n, AMOSTRAS, &r.ins_avl, &r.rem_avl);
        get_media_rb(n, AMOSTRAS, &r.ins_rb, &r.rem_rb);
        get_media_b(n, AMOSTRAS, 1,  &r.ins_b1,  &r.rem_b1);
        get_media_b(n, AMOSTRAS, 5,  &r.ins_b5,  &r.rem_b5);
        get_media_b(n, AMOSTRAS, 10, &r.ins_b10, &r.rem_b10);

        printf("%d;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f\n",
               n,
               r.ins_avl, r.rem_avl,
               r.ins_rb,  r.rem_rb,
               r.ins_b1,  r.rem_b1,
               r.ins_b5,  r.rem_b5,
               r.ins_b10, r.rem_b10);
        
        // Controle do incremento para garantir que começa em 1 e depois alinha com o passo
        if (n == 1) n = PASSO; 
        else n += PASSO;
    }

    return 0;
}