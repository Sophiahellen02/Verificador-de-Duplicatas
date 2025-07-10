#include <stdlib.h>
#include <string.h>
#include "verificador.h"

int comparar_strings(const void *a, const void *b){
    char * const *sa = a;
    char * const *sb = b;
    return strcmp(*sa, *sb);
}

int verifica_ordenado(char **linhas, int n){
    qsort(linhas, n, sizeof(char *), comparar_strings);
    for (int i = 1; i < n; i++){
        if (strcmp(linhas[i - 1], linhas[i]) == 0){
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