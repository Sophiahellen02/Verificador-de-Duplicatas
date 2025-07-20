#include <stdlib.h>
#include <string.h>
#include "verificador.h"
#include "utils.h"

// Função de comparação de strings para qsort
int comparar_strings(const void *a, const void *b) {
    const char * const *sa = a;
    const char * const *sb = b;
    return strcmp_ci(*sa, *sb);
}

// Método 1: Verifica duplicatas ordenando e comparando sequencialmente
int verifica_por_ordenacao(char **linhas, int n) {
    qsort(linhas, n, sizeof(char *), comparar_strings);
    for (int i = 1; i < n; i++) {
        if (strcmp_ci(linhas[i - 1], linhas[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Método 2: Verificação bruta com duplo loop (comparação de todos com todos)
int verifica_linear(char **linhas, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (strcmp_ci(linhas[i], linhas[j]) == 0) {
                return 1;
            }
        }
    }
    return 0;
}