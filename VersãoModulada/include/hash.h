#ifndef HASH_H
#define HASH_H

typedef struct No{
    char *str;
    int contagem;
    struct No *prox;
} No;

typedef struct TabelaHash{
    No **listas;
    int tamanho;
} TabelaHash;

TabelaHash *criar_tabela_hash(int tamanho);
void liberar_tabela_hash(TabelaHash *tabela);
int inserir_tabela_hash(TabelaHash *tabela, const char *str);
void imprime_duplicatas(TabelaHash *tabela);

#endif