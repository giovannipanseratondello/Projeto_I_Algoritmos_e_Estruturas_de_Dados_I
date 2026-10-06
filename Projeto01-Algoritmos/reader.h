#ifndef READER_H
#define READER_H

#include <stdbool.h>
#include "item.h"

typedef struct reader READER;

//Lê e ignora a primeira linha (cabeçalho) do arquivo CSV.
void reader_pular_cabecalho(FILE *fp);

//Lê uma linha do arquivo CSV de asteroides, faz o parse de todos os 20 campos e retorna um ponteiro para ITEM alocado dinamicamente. Retorna NULL ao atingir o Fim de Arquivo (EOF) ou em caso de erro de leitura.
ITEM* reader_ler_item(FILE *fp);

#endif