#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "hash.h"
#include "utils.h"
#include "verificador.h"
#include "io.h"

int main() {
    char **lista = NULL;
    int n = 0;
    int opcao;

    while (1) {
        char entrada_menu[10];
        int valido_menu = 0;
        do {
            menu();
            fgets(entrada_menu, sizeof(entrada_menu), stdin);
            entrada_menu[strcspn(entrada_menu, "\r\n")] = 0;
            valido_menu = 1;
            for (int i = 0; entrada_menu[i]; i++) {
                if (entrada_menu[i] < '0' || entrada_menu[i] > '9') {
                    valido_menu = 0;
                    break;
                }
            }
            if (!valido_menu) {
                printf("Por favor, insira apenas números (0, 1 ou 2).\n");
            }
        } while (!valido_menu);

        opcao = atoi(entrada_menu);
        if (opcao == 0) break;

        if (opcao == 1) {
            char entrada[20];
            int valido = 0;
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
                if (!valido) printf("Por favor, insira apenas números.\n");
            } while (!valido);
            n = atoi(entrada);
            if (n > TAM_MAX_LISTA) n = TAM_MAX_LISTA;
            lista = malloc(n * sizeof(char *));
            char buffer[TAM_MAX_LINHA];
            for (int i = 0; i < n; i++) {
                printf("    Insira a string %d: ", i + 1);
                fgets(buffer, TAM_MAX_LINHA, stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                lista[i] = strdup(buffer);
            }
        } else if (opcao == 2) {
            while (1) {
                char nome_arquivo[100];
                printf("\nInsira o nome do arquivo CSV: ");
                fgets(nome_arquivo, 100, stdin);
                nome_arquivo[strcspn(nome_arquivo, "\r\n")] = 0;
                lista = carregar_csv(nome_arquivo, &n);
                if (lista) {
                    printf("Arquivo carregado com %d entradas.\n", n);
                    break;
                } else {
                    printf("\n============================\n");
                    printf("1. Tentar novamente\n2. Sair\n");
                    printf("============================\n");
                    printf("Escolha: ");
                    int escolha = 0;
                    char entrada_escolha[10];
                    int valido_escolha = 0;
                    do {
                        fgets(entrada_escolha, sizeof(entrada_escolha), stdin);
                        entrada_escolha[strcspn(entrada_escolha, "\r\n")] = 0;
                        valido_escolha = 1;
                        for (int i = 0; entrada_escolha[i]; i++) {
                            if (entrada_escolha[i] < '0' || entrada_escolha[i] > '9') {
                                valido_escolha = 0;
                                break;
                            }
                        }
                        if (!valido_escolha) {
                            printf("Por favor, insira apenas números (1 ou 2): ");
                        }
                    } while (!valido_escolha);
                    escolha = atoi(entrada_escolha);
                    if (escolha == 2) {
                        lista = NULL;
                        n = 0;
                        printf("retornando ao menu...\n");
                        break;
                    }
                }
            }
            if (!lista || n == 0) continue;
        } else {
            printf("Opção inválida. Tente novamente.\n");
            continue;
        }

        clock_t inicio_hash = clock();
        TabelaHash *tabela = criar_tabela_hash(TAM_MAX_LISTA);
        for (int i = 0; i < n; i++) {
            inserir_tabela_hash(tabela, lista[i]);
        }
        clock_t fim_hash = clock();
        imprime_duplicatas(tabela);

        clock_t inicio_linear = clock();
        int resultado_linear = verifica_linear(lista, n);
        clock_t fim_linear = clock();

        char **copia = malloc(n * sizeof(char *));
        for (int i = 0; i < n; i++) {
            copia[i] = strdup(lista[i]);
        }
        clock_t inicio_ord = clock();
        int resultado_ord = verifica_por_ordenacao(copia, n);
        clock_t fim_ord = clock();
        liberar_listas(copia, n);

        double tempo_hash = (double)(fim_hash - inicio_hash) / CLOCKS_PER_SEC;
        double tempo_linear = (double)(fim_linear - inicio_linear) / CLOCKS_PER_SEC;
        double tempo_ord = (double)(fim_ord - inicio_ord) / CLOCKS_PER_SEC;

        printf("\nTempo para encontrar duplicadas:\n");
        printf("    tabela hash: %.9f segundos\n", tempo_hash);
        printf("    linear: %.9f segundos\n", tempo_linear);
        printf("    ordenação + comparação: %.9f segundos\n", tempo_ord);

        liberar_tabela_hash(tabela);
        liberar_listas(lista, n);
        lista = NULL;
        n = 0;
    }

    printf("\nPrograma encerrado.\n");
    return 0;
}