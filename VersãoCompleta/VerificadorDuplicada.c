#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_MAX_LISTA 10000
#define TAM_MAX_LINHA 200

typedef struct No{
    char *str;
    struct No *prox;
} No;

typedef struct TabelaHash{
    No **listas;
    int tamanho;
} TabelaHash;

unsigned int calcular_hash(const char *str, int tamanho){
    unsigned int hash = 5381;

    while (*str){
        hash = ((hash << 5) + hash) + (unsigned char)(*str);
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
    for (int i = 0; i < tabela->tamanho; i++){
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
    unsigned int indece = calcular_hash(str, tabela->tamanho);
    No *atual = tabela->listas[indece];

    while (atual){
        if (strcmp(atual->str, str) == 0){
            return 1;
        }
        atual = atual->prox;
    }

    No *novo = malloc(sizeof(No));
    novo->str = strdup(str);
    novo->prox = tabela->listas[indece];
    tabela->listas[indece] = novo;
    return 0;
}

char **carregar_csv(const char *nome_arquivo, int *n){
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo){
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    char **linhas = malloc (TAM_MAX_LISTA * sizeof(char *));
    char buffer[TAM_MAX_LINHA];
    *n = 0;

    while (fgets(buffer, TAM_MAX_LINHA, arquivo) && *n < TAM_MAX_LINHA){
        buffer[strcspn(buffer, "\n")] = 0;
        linhas[*n] = strdup(buffer);
        (*n)++;
    }
    fclose(arquivo);
    return linhas;
}