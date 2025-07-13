#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

// Função de comparação case-insensitive
int strcmp_ci(const char *a, const char *b) {
    while (*a && *b) {
        char ca = *a, cb = *b;
        if (ca >= 'A' && ca <= 'Z') ca += 32;
        if (cb >= 'A' && cb <= 'Z') cb += 32;
        if (ca != cb) return ca - cb;
        a++; b++;
    }
    return *a - *b;
}

// Hash case-insensitive
unsigned int calcular_hash(const char *str, int tamanho){
    unsigned int hash = 5381;
    while (*str){
        char c = *str;
        if (c >= 'A' && c <= 'Z') c += 32;
        hash = ((hash << 5) + hash) + (unsigned char)c;
        str++;
    }
    return hash % tamanho;
}

TabelaHash *criar_tabela_hash(int tamanho){
    TabelaHash *tabela = malloc(sizeof(TabelaHash));
    tabela->tamanho = tamanho;
    tabela->listas = calloc(tamanho, sizeof(No *));
    return tabela;
}

void liberar_tabela_hash(TabelaHash *tabela){
    for(int i = 0; i < tabela->tamanho; i++){
        No *atual = tabela->listas[i];
        while (atual){
            No *temp = atual;
            atual = atual->prox;
            free(temp->str);
            free(temp);
        }
    }
    free(tabela->listas);
    free(tabela);
}

int inserir_tabela_hash(TabelaHash *tabela, const char *str){
    unsigned int indice = calcular_hash(str, tabela->tamanho);
    No *atual = tabela->listas[indice];

    while (atual){
        if (strcmp_ci(atual->str, str) == 0){
            atual->contagem++;
            return 1;
        }
        atual = atual->prox;
    }

    No *novo = malloc(sizeof(No));
    novo->str = strdup(str);
    novo->contagem = 1;
    novo->prox = tabela->listas[indice];
    tabela->listas[indice] = novo;
    return 0;
}

void imprime_duplicatas(TabelaHash *tabela){
    int encontrou = 0;
    printf("\nDuplicatas encontradas:\n");
    for (int i = 0; i < tabela->tamanho; i++){
        No *atual = tabela->listas[i];
        while (atual){
            if (atual->contagem > 1){
                printf("    %s: %d vezes\n", atual->str, atual->contagem);
                encontrou = 1;
            }
            atual = atual->prox;
        }
    }
    if(!encontrou){
        printf("Nenhuma duplicata encontrada.\n");
    }
}