#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "io.h"

void menu(void){
    printf("\n============================\n");
    printf(" VERIFICADOR DE DUPLICATAS\n");
    printf("============================\n");
    printf("1. Inserir manualmente\n");
    printf("2. Importar CSV\n");
    printf("0. Sair\n");
    printf("============================\n");
    printf("Escolha: ");
}

int ler_quantidade_strings(void) {
    char entrada[32];
    int valido = 0;
    int n = 0;
    do {
        printf("\nQuantas strings deseja inserir? ");
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\r\n")] = 0;
        valido = 1;
        for (int i = 0; entrada[i]; i++) {
            if (entrada[i] < '0' || entrada[i] > '9') {
                valido = 0;
                break;
            }
        }
        if (!valido) {
            printf("Por favor, insira apenas números.\n");
        }
    } while (!valido);
    n = atoi(entrada);
    return n;
}