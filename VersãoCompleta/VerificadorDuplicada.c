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

void liberar_listas(char **linhas, int n){
    for (int i = 0; i < n; i++){
        free(linhas[i]);
    }
    free(linhas);
}

int comparar_strings(const void *a, const void *b){
    char * const *sa = a;
    char * const *sb = b;
    return strcmp(*sa, *sb);
}

int verifica_ordenado(char **linhas, int n){
    qsort(linhas, n, sizeof(char *), comparar_strings);
    for (int i = 1; i < n; i++){
        if (strcmp(linhas[i], linhas[i + 1]) == 0){
            return 1;
        }
    }
    return 0;
}

int verifica_linear(char **linhas, int n){
    for(int i = 0; i < n - 1; i++){
        for(int j = i + 1; j < n; j++){
            if (strcmp(linhas[i], linhas[j]) == 0){
                return 1;
            }
        }
    }
    return 0;
}

void medir_tempo(const char *nome, int (*func)(char **, int), char **linhas, int n){
    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    int resultado = func(linhas, n);
    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    printf("%s: %s (%.6f segundos)\n", nome, resultado ? "Duplicados encontrados" : "Nenhum duplicado", tempo);
    printf("Tempo de execução: %.6f segundos\n", tempo);
}
void menu(){
    printf("\n============================\n");
    printf(" VERIFICADOR DE DUPLICATAS\n");
    printf("============================\n");
    printf("1. Inserir manualmente\n");
    printf("2. Importar CSV\n");
    printf("0. Sair\n");
    printf("============================\n");
    printf("Escolha: ");
}