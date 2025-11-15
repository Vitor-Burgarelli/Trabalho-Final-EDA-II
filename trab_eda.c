#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "avl.c" 
#include "b.c" 
#include "rubro_negra.c" 

void teste_avl(int tamanho, int num_amostras) {
    long long total_comps_ins = 0;
    long long total_comps_rem = 0;

    for (int k = 0; k < num_amostras; k++) {
        ArvoreAVL avl = {NULL, 0};
        int *chaves = malloc(tamanho * sizeof(int));
        
        // Gerar chaves aleatorias
        for (int i = 0; i < tamanho; i++) {
            chaves[i] = rand();
        }

        // 1. Teste de Inserção
        avl.comparacoes = 0;
        for (int i = 0; i < tamanho; i++) {
            inserir_avl(&avl, chaves[i]);
        }
        total_comps_ins += avl.comparacoes;

        // 2. Teste de Remoção (Removemos tudo para testar pior caso/médio completo)
        avl.comparacoes = 0;
        for (int i = 0; i < tamanho; i++) {
            remover_avl_no(&avl, avl.raiz, chaves[i]);
        }
        total_comps_rem += avl.comparacoes;

        free(chaves);
        // (Idealmente faria uma funcao para liberar a arvore se sobrasse algo)
    }

    printf("AVL,%d,%.2f,%.2f\n", 
           tamanho, 
           (double)total_comps_ins / num_amostras, 
           (double)total_comps_rem / num_amostras);
}

int main() {
    srand(time(NULL));
    
    printf("Algoritmo,Tamanho,Media_Comparacoes_Insercao,Media_Comparacoes_Remocao\n");

    // Loop de tamanho de 100 em 100 ate 10000 (para o grafico nao ficar gigante)
    for (int n = 100; n <= 10000; n += 100) {
        teste_avl(n, 10); // 10 amostras
        // teste_rubro_negra(n, 10);
        // teste_arvore_b(n, 10, 1);
        // teste_arvore_b(n, 10, 5);
        // teste_arvore_b(n, 10, 10);
    }
    return 0;
}