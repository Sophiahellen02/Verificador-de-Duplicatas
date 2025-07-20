#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

// Função de comparação de strings ignorando diferença entre maiúsculas e minúsculas
int strcmp_ci(const char *a, const char *b) {
    while (*a && *b) {
        char ca = tolower((unsigned char)*a);
        char cb = tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
        a++; b++;
    }
    return *a - *b;
}

// Função hash baseada no algoritmo djb2, adaptada para ser case-insensitive
unsigned int calcular_hash(const char *str, int tamanho) {
    unsigned int hash = 5381;
    while (*str) {
        hash = ((hash << 5) + hash) + tolower((unsigned char)*str);
        str++;
    }
    return hash % tamanho;
}

// Aloca e inicializa a tabela hash
TabelaHash *criar_tabela_hash(int tamanho) {
    TabelaHash *tabela = malloc(sizeof(TabelaHash));
    if (!tabela) {
        perror("Erro ao alocar tabela");
        exit(1);
    }
    tabela->tamanho = tamanho;
    tabela->listas = calloc(tamanho, sizeof(No *));
    if (!tabela->listas) {
        perror("Erro ao alocar listas");
        exit(1);
    }
    return tabela;
}

// Libera toda a memória da tabela hash e seus nós
void liberar_tabela_hash(TabelaHash *tabela) {
    for (int i = 0; i < tabela->tamanho; i++) {
        No *atual = tabela->listas[i];
        while (atual) {
            No *temp = atual;
            atual = atual->prox;
            free(temp->str);
            free(temp);
        }
    }
    free(tabela->listas);
    free(tabela);
}

// Insere uma string na tabela hash ou incrementa sua contagem se já existir
int inserir_tabela_hash(TabelaHash *tabela, const char *str) {
    unsigned int indice = calcular_hash(str, tabela->tamanho);
    No *atual = tabela->listas[indice];
    while (atual) {
        if (strcmp_ci(atual->str, str) == 0) {
            atual->contagem++; // duplicata encontrada
            return 1;
        }
        atual = atual->prox;
    }
    // Se não encontrou, cria novo nó e insere no início da lista
    No *novo = malloc(sizeof(No));
    novo->str = strdup(str);
    novo->contagem = 1;
    novo->prox = tabela->listas[indice];
    tabela->listas[indice] = novo;
    return 0;
}

// Exibe as duplicatas presentes na tabela hash
void imprime_duplicatas(TabelaHash *tabela) {
    int encontrou = 0;
    printf("\nDuplicatas encontradas:\n");
    for (int i = 0; i < tabela->tamanho; i++) {
        No *atual = tabela->listas[i];
        while (atual) {
            if (atual->contagem > 1) {
                printf("    %s: %d vezes\n", atual->str, atual->contagem);
                encontrou = 1;
            }
            atual = atual->prox;
        }
    }
    if (!encontrou) {
        printf("Nenhuma duplicata encontrada.\n");
    }
}