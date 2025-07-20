#ifndef VERIFICADOR_H
#define VERIFICADOR_H

int comparar_strings(const void *a, const void *b);      // Função de comparação de strings para qsort
int verifica_por_ordenacao(char **linhas, int n);            // Método 1: Verifica duplicatas ordenando e comparando sequencialmente
int verifica_linear(char **linhas, int n);             // Método 2: Verificação bruta com duplo loop (comparação de todos com todos)

#endif