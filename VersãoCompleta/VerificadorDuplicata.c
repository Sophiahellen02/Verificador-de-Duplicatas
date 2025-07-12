#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_MAX_LISTA 10000
#define TAM_MAX_LINHA 200

typedef struct No{
    char *str;
    int contagem;
    struct No *prox;
} No;

typedef struct TabelaHash{
    No **listas;
    int tamanho;
} TabelaHash;

unsigned int calcular_hash(const char *str, int tamanho){
    unsigned int hash = 5381;
    while (*str){
        hash = ((hash << 5) + hash) + (unsigned char)(*str);
        str++;
    }
    return hash % tamanho;
}

TabelaHash *criar_tabela_hash(int tamanho){
    TabelaHash *tabela = malloc(sizeof(TabelaHash));
    tabela->tamanho = tamanho;
    tabela->listas = calloc(tamanho, sizeof(No *));
    return tabela;
}

void liberar_tabela_hash(TabelaHash *tabela){
    for (int i = 0; i < tabela->tamanho; i++){
        No *atual = tabela->listas[i];
        while (atual){
            No *temp = atual;
            atual = atual->prox;
            free(temp->str);
            free(temp);
        }
    }
    free(tabela->listas);
    free(tabela);
}

int inserir_tabela_hash(TabelaHash *tabela, const char *str){
    unsigned int indice = calcular_hash(str, tabela->tamanho);
    No *atual = tabela->listas[indice];

    No *novo = malloc(sizeof(No));
    novo->str = strdup(str);
    novo->contagem = 1;
    novo->prox = tabela->listas[indice];
    tabela->listas[indice] = novo;
    return 0;
}

void imprime_duplicatas(TabelaHash *tabela){
    int encontrou = 0;
    printf("\nDuplicatas encontradas:\n");
    for (int i = 0; i < tabela->tamanho; i++){
        No *atual = tabela->listas[i];
        while (atual){
            if (atual->contagem > 1){
                printf("    %s: %d vezes\n", atual->str, atual->contagem);
                encontrou = 1;
            }
            atual = atual->prox;
        }
    }
    if(!encontrou){
        printf("Nenhuma duplicata encontrada.\n");
    }
}

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

int comparar_strings(const void *a, const void *b){
    char * const *sa = a;
    char * const *sb = b;
}

int verifica_ordenado(char **linhas, int n){
    qsort(linhas, n, sizeof(char *), comparar_strings);
    for (int i = 1; i < n; i++){
        if (strcmp(linhas[i-1], linhas[i]) == 0){
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

void medir_tempo(const char *nome, int (*func)(char **, int), char **linhas, int n){
    clock_t inicio = clock();
    int resultado = func(linhas, n);
    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("%s: %s (%.6f segundos)\n", nome, resultado ? "Duplicados encontrados" : "Nenhum duplicado", tempo);
    printf("Tempo de execução: %.6f segundos\n", tempo);
}

void menu(){
    printf("\n============================\n");
    printf(" VERIFICADOR DE DUPLICATAS\n");
    printf("============================\n");
    printf("1. Inserir manualmente\n");
    printf("2. Importar CSV\n");
    printf("0. Sair\n");
    printf("============================\n");
    printf("Escolha: ");
}

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
                    printf("============================\n");
                    printf("1. Tentar novamente\n2. Sair\n");
                    printf("============================\n");
                    printf("Escolha: ");
                    int escolha = 0;
                    scanf("%d%*c", &escolha);
                    if (escolha == 2) {
                        lista = NULL;
                        n = 0;
                        printf("retornando ao menu...\n");
                        break;
                    }
                }
            }

            if (!lista || n == 0) {
                continue;
            }
        } else {
            printf("Opção inválida. Tente novamente.\n");
            continue;
        }

        TabelaHash *tabela = criar_tabela_hash(TAM_MAX_LISTA);

        clock_t inicio = clock();
        for(int i = 0; i < n; i++){
            inserir_tabela_hash(tabela, lista[i]);
        }
        clock_t fim = clock();

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

        printf("\nTempo para encontrar duplicadas (comparação):\n");
        printf("    hash: %.9f segundos\n", tempo_hash);
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