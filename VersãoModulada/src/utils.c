#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

// Lê um arquivo CSV linha a linha e armazena cada linha como uma string em um vetor
char **carregar_csv(const char *nome_arquivo, int *n) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    char **linhas = malloc(TAM_MAX_LISTA * sizeof(char *));
    char buffer[TAM_MAX_LINHA];
    *n = 0;

    // Lê cada linha até o fim do arquivo ou até atingir o limite
    while (fgets(buffer, TAM_MAX_LINHA, arquivo) && *n < TAM_MAX_LISTA) {
        buffer[strcspn(buffer, "\r\n")] = 0; // remove quebras de linha
        linhas[*n] = strdup(buffer);
        (*n)++;
    }

    if (*n == TAM_MAX_LISTA) {
        printf("Aviso: arquivo parcialmente carregado (limite de %d linhas).\n", TAM_MAX_LISTA);
    }

    fclose(arquivo);
    return linhas;
}

// Libera memória de um vetor de strings
void liberar_listas(char **linhas, int n) {
    for (int i = 0; i < n; i++) {
        free(linhas[i]);
    }
    free(linhas);
}