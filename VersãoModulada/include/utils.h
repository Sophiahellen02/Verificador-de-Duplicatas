#ifndef UTILS_H
#define UTILS_H

#define TAM_MAX_LISTA 10000     // Limite máximo para o número de strings
#define TAM_MAX_LINHA 200       // Limite máximo para o tamanho de cada linha

char **carregar_csv(const char *nome_arquivo, int *n); // Lê um arquivo CSV linha a linha e armazena cada linha como uma string em um vetor
void liberar_listas(char **linhas, int n);            // Libera memória de um vetor de strings

#endif