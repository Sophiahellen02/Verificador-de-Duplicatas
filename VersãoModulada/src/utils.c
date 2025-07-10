#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

char **carregar_csv(const char *nome_arquivo, int *n){
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo){
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    char **linhas = malloc(TAM_MAX_LISTA * sizeof(char *));
    char buffer[TAM_MAX_LINHA];
    *n = 0;

    while (fgets(buffer, TAM_MAX_LINHA, arquivo) && *n < TAM_MAX_LISTA){
        buffer[strcspn(buffer, "\r\n")] = 0;
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