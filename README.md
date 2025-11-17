# Trabalho-Final-EDA-II

## Análise de Complexidade Algorítmica: Árvores AVL, Rubro-Negra e B

Este documento descreve o escopo do trabalho de análise da complexidade algorítmica das operações de adição e remoção em diferentes estruturas de dados de árvores auto-balanceáveis.

**Data de Entrega:** 28/11/2024

### 1. Objetivo

O objetivo deste trabalho consiste em analisar e comparar o esforço computacional (complexidade) das operações de **adição** e **remoção** de nós, incluindo o respectivo balanceamento, nas seguintes estruturas:

1. Árvore AVL
2. Árvore Rubro-Negra (Red-Black)
3. Árvore B (com ordens `t=1`, `t=5` e `t=10`)

### 2. Metodologia

A análise será realizada através da execução de experimentos em C, medindo o esforço computacional para popular e esvaziar as árvores.

* **Conjunto de Dados:** O tamanho do conjunto de chaves (inteiros) irá variar de 1 a 10.000.
* **Caso de Teste:** Será avaliado o caso médio, utilizando chaves geradas aleatoriamente (sugestão: `rand()` em C).
* **Validade Estatística:** Para cada tamanho `N` (de 1 a 10.000), o experimento será executado 10 vezes (10 amostras) com conjuntos de chaves aleatórias diferentes. O resultado final para o tamanho `N` será a média aritmética do esforço computacional obtido nas 10 amostras.

### 3. Entregáveis

O resultado final do trabalho compreende dois itens principais:

#### 3.1. Relatório Técnico

O relatório deve conter:

1. **Gráfico de Adição:** Um gráfico de linha (Eixo X: Tamanho do conjunto [1-10k], Eixo Y: Esforço computacional) contendo 5 linhas, representando a operação de adição para:
   * AVL
   * Rubro-Negra
   * B-Tree (t=1)
   * B-Tree (t=5)
   * B-Tree (t=10)

2. **Gráfico de Remoção:** Um gráfico de linha similar ao anterior, mas para a operação de remoção.
3. **Discussão Final:** Uma análise e discussão dos resultados observados nos gráficos, comparando o desempenho das estruturas.

#### 3.2. Códigos-Fonte

Todos os códigos-fonte desenvolvidos em C para a implementação das árvores e para a realização dos experimentos.

---

### 4. Checklist de Tarefas

* [ ] **Definição:** Definir a métrica exata para "esforço computacional" (Ex: número de comparações, número de rotações/operações de balanceamento, tempo de execução, ou uma combinação).
* [ ] **Implementação:** Árvore AVL (Adição, Remoção, Balanceamento).
* [ ] **Implementação:** Árvore Rubro-Negra (Adição, Remoção, Balanceamento).
* [ ] **Implementação:** Árvore B (Adição, Remoção, Balanceamento) de forma genérica para suportar diferentes ordens (t).
* [ ] **Testes Unitários:** Validar as implementações de cada árvore com casos simples.
* [ ] **Script de Experimento:** Criar o programa principal (em C) que executa a metodologia:
    * [ ] Loop de N (1 a 10.000).
    * [ ] Loop de Amostras (10 execuções).
    * [ ] Gerador de chaves aleatórias (`rand()`).
    * [ ] Medição e coleta de dados (para adição e remoção).
    * [ ] Cálculo da média das amostras.
* [ ] **Coleta de Dados:** Executar o experimento e salvar os resultados (ex: em um arquivo .csv).
* [ ] **Geração de Gráficos:** Utilizar uma ferramenta (Excel, Python/Matplotlib, Gnuplot) para gerar os dois gráficos solicitados.
* [ ] **Redação:** Escrever o Relatório Técnico.
    * [ ] Introdução e Metodologia.
    * [ ] Inserção dos Gráficos.
    * [ ] Redação da Discussão Final.
* [ ] **Revisão:** Revisar o relatório e organizar os códigos-fonte para entrega.
* [ ] **Entrega:** Submeter o Relatório e os Códigos-Fonte até 28/11/2024.
