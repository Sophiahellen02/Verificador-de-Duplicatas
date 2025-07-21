#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h> // para tolower

// Limites máximos para o número de strings e o tamanho de cada linha
#define TAM_MAX_LISTA 10000
#define TAM_MAX_LINHA 200

// Estrutura de um nó da tabela hash (lista encadeada)
typedef struct No {
    char *str;           // string armazenada
    int contagem;        // número de ocorrências (para contar duplicatas)
    struct No *prox;     // ponteiro para o próximo nó
} No;

// Estrutura da tabela hash
typedef struct TabelaHash {
    No **listas;         // vetor de ponteiros para listas de colisão
    int tamanho;         // tamanho total da tabela hash
} TabelaHash;

// Função de comparação de strings ignorando diferença entre maiúsculas e minúsculas
int strcmp_ci(const char *a, const char *b) {
    while (*a && *b) {
        char ca = tolower((unsigned char)*a);
        char cb = tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
        a++; b++;
    }
    return *a - *b;
}

// Função hash baseada no algoritmo djb2, adaptada para ser case-insensitive
unsigned int calcular_hash(const char *str, int tamanho) {
    unsigned int hash = 5381;
    while (*str) {
        hash = ((hash << 5) + hash) + tolower((unsigned char)*str);
        str++;
    }
    return hash % tamanho;
}

// Aloca e inicializa a tabela hash
TabelaHash *criar_tabela_hash(int tamanho) {
    TabelaHash *tabela = malloc(sizeof(TabelaHash));
    if (!tabela) {
        perror("Erro ao alocar tabela");
        exit(1);
    }
    tabela->tamanho = tamanho;
    tabela->listas = calloc(tamanho, sizeof(No *));
    if (!tabela->listas) {
        perror("Erro ao alocar listas");
        exit(1);
    }
    return tabela;
}

// Libera toda a memória da tabela hash e seus nós
void liberar_tabela_hash(TabelaHash *tabela) {
    for (int i = 0; i < tabela->tamanho; i++) {
        No *atual = tabela->listas[i];
        while (atual) {
            No *temp = atual;
            atual = atual->prox;
            free(temp->str);
            free(temp);
        }
    }
    free(tabela->listas);
    free(tabela);
}

// Insere uma string na tabela hash ou incrementa sua contagem se já existir
int inserir_tabela_hash(TabelaHash *tabela, const char *str) {
    unsigned int indice = calcular_hash(str, tabela->tamanho);
    No *atual = tabela->listas[indice];
    while (atual) {
        if (strcmp_ci(atual->str, str) == 0) {
            atual->contagem++; // duplicata encontrada
            return 1;
        }
        atual = atual->prox;
    }
    // Se não encontrou, cria novo nó e insere no início da lista
    No *novo = malloc(sizeof(No));
    novo->str = strdup(str);
    novo->contagem = 1;
    novo->prox = tabela->listas[indice];
    tabela->listas[indice] = novo;
    return 0;
}

// Exibe as duplicatas presentes na tabela hash
void imprime_duplicatas(TabelaHash *tabela) {
    int encontrou = 0;
    printf("\nDuplicatas encontradas:\n");
    for (int i = 0; i < tabela->tamanho; i++) {
        No *atual = tabela->listas[i];
        while (atual) {
            if (atual->contagem > 1) {
                printf("    %s: %d vezes\n", atual->str, atual->contagem);
                encontrou = 1;
            }
            atual = atual->prox;
        }
    }
    if (!encontrou) {
        printf("Nenhuma duplicata encontrada.\n");
    }
}

// Lê um arquivo CSV linha a linha e armazena cada linha como uma string em um vetor
char **carregar_csv(const char *nome_arquivo, int *n) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    char **linhas = malloc(TAM_MAX_LISTA * sizeof(char *));
    char buffer[TAM_MAX_LINHA];
    *n = 0;

    // Lê cada linha até o fim do arquivo ou até atingir o limite
    while (fgets(buffer, TAM_MAX_LINHA, arquivo) && *n < TAM_MAX_LISTA) {
        buffer[strcspn(buffer, "\r\n")] = 0; // remove quebras de linha
        linhas[*n] = strdup(buffer);
        (*n)++;
    }

    if (*n == TAM_MAX_LISTA) {
        printf("Aviso: arquivo parcialmente carregado (limite de %d linhas).\n", TAM_MAX_LISTA);
    }

    fclose(arquivo);
    return linhas;
}

// Libera memória de um vetor de strings
void liberar_listas(char **linhas, int n) {
    for (int i = 0; i < n; i++) {
        free(linhas[i]);
    }
    free(linhas);
}

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
    int encontrou = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (strcmp_ci(linhas[i], linhas[j]) == 0) {
                encontrou++;
                break; // ou continue, se quiser todas as repetições
            }
        }
    }
    return encontrou;
}

// Exibe o menu principal
void menu() {
    printf("\n============================\n");
    printf(" VERIFICADOR DE DUPLICATAS\n");
    printf("============================\n");
    printf("1. Inserir manualmente\n");
    printf("2. Importar CSV\n");
    printf("0. Sair\n");
    printf("============================\n");
    printf("Escolha: ");
}

// Função principal
int main() {
    char **lista = NULL;
    int n = 0;
    int opcao;

    while (1) {
        char entrada_menu[10];
        int valido_menu = 0;
        // Entrada segura e verificada do menu
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
            // Inserção manual de strings
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

            if (lista != NULL && n > 0) {
                liberar_listas(lista, n);
                lista = NULL;
                n = 0;
            }

            lista = malloc(n * sizeof(char *));
            char buffer[TAM_MAX_LINHA];

            for (int i = 0; i < n; i++) {
                printf("    Insira a string %d ('sair' para cancelar): ", i + 1);
                fgets(buffer, TAM_MAX_LINHA, stdin);
                buffer[strcspn(buffer, "\n")] = 0;

                if (strcmp(buffer, "sair") == 0) {
                    printf("\nOperação cancelada. Retornando ao menu...\n");
                    for (int j = 0; j < i; j++) free(lista[j]);
                    free(lista);
                    lista = NULL;
                    n = 0;
                    break;
                }

                lista[i] = strdup(buffer);
            }

            if (lista == NULL) continue;

        } else if (opcao == 2) {
            // Importa strings de arquivo CSV
            while (1) {
                char nome_arquivo[100];
                printf("\nInsira o nome do arquivo CSV: ");
                fgets(nome_arquivo, 100, stdin);
                nome_arquivo[strcspn(nome_arquivo, "\r\n")] = 0;

                if (lista != NULL && n > 0) {
                    liberar_listas(lista, n);
                    lista = NULL;
                    n = 0;
                }

                lista = carregar_csv(nome_arquivo, &n);

                if (lista) {
                    printf("Arquivo carregado com %d entradas.\n", n);
                    break;
                } else {
                    // Caso erro, permite nova tentativa
                    printf("\n============================\n");
                    printf("1. Tentar novamente\n");
                    printf("2. Sair\n");
                    printf("============================\n");

                    int escolha = 0;
                    char entrada_escolha[10];
                    int valido_escolha = 0;

                    do {
                        printf("Escolha: ");
                        fgets(entrada_escolha, sizeof(entrada_escolha), stdin);
                        entrada_escolha[strcspn(entrada_escolha, "\r\n")] = 0;

                        valido_escolha = 1;
                        for (int i = 0; entrada_escolha[i]; i++) {
                            if (entrada_escolha[i] < '0' || entrada_escolha[i] > '9') {
                                valido_escolha = 0;
                                break;
                            }
                        }

                        if (valido_escolha) {
                            escolha = atoi(entrada_escolha);
                            if (escolha != 1 && escolha != 2) {
                                valido_escolha = 0;
                                printf("\nOpção inválida. Digite 1 ou 2.\n");
                            }
                        } else {
                            printf("\nPor favor, insira apenas números (1 ou 2).\n");
                        }

                    } while (!valido_escolha);

                    if (escolha == 2) {
                        lista = NULL;
                        n = 0;
                        printf("Retornando ao menu...\n");
                        break;
                    }
                }
            }

            if (!lista || n == 0) continue;

        } else {
            printf("Opção inválida. Tente novamente.\n");
            continue;
        }

        // Verificação de duplicatas por tabela hash
        clock_t inicio_hash = clock();
        TabelaHash *tabela = criar_tabela_hash(TAM_MAX_LISTA);
        for (int i = 0; i < n; i++) {
            inserir_tabela_hash(tabela, lista[i]);
        }
        clock_t fim_hash = clock();
        imprime_duplicatas(tabela);

        // Verificação linear
        clock_t inicio_linear = clock();
        int resultado_linear = verifica_linear(lista, n);
        clock_t fim_linear = clock();

        // Verificação por ordenação + comparação
        char **copia = malloc(n * sizeof(char *));
        for (int i = 0; i < n; i++) {
            copia[i] = strdup(lista[i]);
        }
        clock_t inicio_ord = clock();
        int resultado_ord = verifica_por_ordenacao(copia, n);
        clock_t fim_ord = clock();
        liberar_listas(copia, n);

        // Mostra os tempos de execução
        double tempo_hash = (double)(fim_hash - inicio_hash) / CLOCKS_PER_SEC;
        double tempo_linear = (double)(fim_linear - inicio_linear) / CLOCKS_PER_SEC;
        double tempo_ord = (double)(fim_ord - inicio_ord) / CLOCKS_PER_SEC;

        printf("\nTempo para encontrar duplicatas:\n");
        printf("    tabela hash: %.9f segundos\n", tempo_hash);
        printf("    linear: %.9f segundos\n", tempo_linear);
        printf("    ordenação + comparação: %.9f segundos\n", tempo_ord);

        // Libera memória e reseta estado
        liberar_tabela_hash(tabela);
        liberar_listas(lista, n);
        lista = NULL;
        n = 0;
    }

    printf("\nPrograma encerrado.\n");
    return 0;
}