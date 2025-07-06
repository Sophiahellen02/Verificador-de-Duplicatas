#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_MAX_LISTA 10000
#define TAM_MAX_LINHA 200

typedef struct No{
    char *str;
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