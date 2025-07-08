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

    while (atual){
        if (strcmp(atual->str, str) == 0){
            atual->contagem++;
            return 1;
        }
        atual = atual->prox;
    }

    No *novo = malloc(sizeof(No));
    novo->str = strdup(str);
    novo->contagem = 1;
    novo->prox = tabela->listas[indice];
    tabela->listas[indice] = novo;
    return 0;
}

void imprime_duplicatas(TabelaHash *tabela){
    int encontrou = 0;
    printf("Duplicatas encontradas:\n");
    for (int i = 0; i < tabela->tamanho; i++){
        No *atual = tabela->listas[i];
        while (atual){
            if (atual->contagem > 1){
                printf("%s: %d vezes\n", atual->str, atual->contagem);
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

    char **linhas = malloc (TAM_MAX_LISTA * sizeof(char *));
    char buffer[TAM_MAX_LINHA];
    *n = 0;

    while (fgets(buffer, TAM_MAX_LINHA, arquivo) && *n < TAM_MAX_LISTA){
        buffer[strcspn(buffer, "\n")] = 0;
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
    return strcmp(*sa, *sb);
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

// Substitui medir_tempo para usar clock()
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
            printf("Quantas strings deseja inserir? ");
            scanf("%d%*c", &n);
            if(n > TAM_MAX_LISTA) n = TAM_MAX_LISTA;
            lista = malloc(n * sizeof(char *));
            char buffer[TAM_MAX_LINHA];
            for(int i = 0; i < n; i++){
                printf("Insira a string %d: ", i + 1);
                fgets(buffer, TAM_MAX_LINHA, stdin);
                buffer[strcspn(buffer, "\n")] = 0; // Remove newline
                lista[i] = strdup(buffer);
            }
        } else if(opcao == 2){
            char nome_arquivo[100];
            printf("Insira o nome do arquivo CSV: ");
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
        double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("Inserção concluída em %.6f segundos.\n", tempo);

        imprime_duplicatas(tabela);

        liberar_tabela_hash(tabela);
        liberar_listas(lista, n);
        lista = NULL;
        n = 0;
    }

    printf("Programa encerrado.\n");
    return 0;
}