#ifndef UTILS_H
#define UTILS_H

#define TAM_MAX_LISTA 10000
#define TAM_MAX_LINHA 200

char **carregar_csv(const char *nome_arquivo, int *n);
void liberar_listas(char **linhas, int n);

#endif