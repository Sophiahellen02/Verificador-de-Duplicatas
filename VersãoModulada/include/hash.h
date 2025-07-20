#ifndef HASH_H
#define HASH_H

#include <stddef.h>

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

TabelaHash *criar_tabela_hash(int tamanho);            // Aloca e inicializa a tabela hash  
void liberar_tabela_hash(TabelaHash *tabela);         // Libera toda a memória da tabela hash e seus nós
int inserir_tabela_hash(TabelaHash *tabela, const char *str); // Insere uma string na tabela hash, contando duplicatas
void imprime_duplicatas(TabelaHash *tabela);          // Exibe as duplicatas presentes na tabela hash
int strcmp_ci(const char *a, const char *b);         // Função de comparação de strings ignorando diferença entre maiúsculas e minúsculas

#endif