#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "hash.h"
#include "utils.h"
#include "verificador.h"
#include "io.h"

int main(){
    char **lista = NULL;
    int n = 0;
    int opcao;

    while(1){
        menu();
        scanf("%d%*c", &opcao);

        if(opcao == 0){
            break;
        }

        if(opcao == 1){
            printf("\nQuantas strings deseja inserir? ");
            scanf("%d%*c", &n);
            if(n > TAM_MAX_LISTA) n = TAM_MAX_LISTA;
            lista = malloc(n * sizeof(char *));
            char buffer[TAM_MAX_LINHA];
            for(int i = 0; i < n; i++){
                printf("    Insira a string %d: ", i + 1);
                fgets(buffer, TAM_MAX_LINHA, stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                lista[i] = strdup(buffer);
            }
        } else if(opcao == 2){
            char nome_arquivo[100];
            printf("\nInsira o nome do arquivo CSV: ");
            fgets(nome_arquivo, 100, stdin);
            nome_arquivo[strcspn(nome_arquivo, "\r\n")] = 0;
            lista = carregar_csv(nome_arquivo, &n);
            if (!lista) continue;
            printf("Arquivo carregado com %d entradas.\n", n);
        } else{
            printf("Opção inválida. Tente novamente.\n");
            continue;
        }

        if (!lista || n == 0) {
            printf("Nenhuma entrada para processar.\n");
            continue;
        }

        TabelaHash *tabela = criar_tabela_hash(TAM_MAX_LISTA);

        clock_t inicio = clock();
        for(int i = 0; i < n; i++){
            inserir_tabela_hash(tabela, lista[i]);
        }
        clock_t fim = clock();
        double tempo_construcao = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("Construção da tabela hash concluída em %.6f segundos.\n", tempo_construcao);

        clock_t inicio_dup = clock();
        imprime_duplicatas(tabela);
        clock_t fim_dup = clock();
        double tempo_hash = (double)(fim_dup - inicio_dup) / CLOCKS_PER_SEC;

        clock_t inicio_linear = clock();
        int resultado_linear = verifica_linear(lista, n);
        clock_t fim_linear = clock();
        double tempo_linear = (double)(fim_linear - inicio_linear) / CLOCKS_PER_SEC;

        char **copia = malloc(n * sizeof(char *));
        for(int i = 0; i < n; i++) {
            copia[i] = strdup(lista[i]);
        }
        clock_t inicio_ord = clock();
        int resultado_ord = verifica_ordenado(copia, n);
        clock_t fim_ord = clock();
        double tempo_ord = (double)(fim_ord - inicio_ord) / CLOCKS_PER_SEC;
        liberar_listas(copia, n);

        printf("\nTempo para encontrar duplicadas:\n");
        printf("    hash: %.6f segundos\n", tempo_hash);
        printf("    linear: %.6f segundos\n", tempo_linear);
        printf("    ordenação + comparação: %.6f segundos\n", tempo_ord);

        liberar_tabela_hash(tabela);
        liberar_listas(lista, n);
        lista = NULL;
        n = 0;
    }

    printf("\nPrograma encerrado.\n");
    return 0;
}