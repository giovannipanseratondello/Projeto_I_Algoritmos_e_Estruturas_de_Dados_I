#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include "item.h"
#include "../Item/item.h"
#define MAX_LENGTH 90000

typedef struct lista LISTA;

LISTA *lista_criar(void);

void lista_apagar(LISTA **lista);


bool lista_inserir_inicio(LISTA *lista, ITEM *item);

bool lista_inserir_fim(LISTA *lista, ITEM *item);

bool lista_inserir(LISTA *lista, ITEM *item, int posicao);


ITEM *lista_remover_inicio(LISTA *lista);

ITEM *lista_remover_fim(LISTA *lista);

ITEM *lista_remover(LISTA *lista, int posicao);


ITEM *lista_primeiro(LISTA *lista);

ITEM *lista_ultimo(LISTA *lista);

ITEM *lista_obter(LISTA *lista, int posicao);

bool lista_vazia(LISTA *lista);

int lista_tamanho(LISTA *lista);


void lista_imprimir(LISTA *lista);

void lista_buscar(PILHA *p, ITEM *item_ref, const char *campo, void (*processar)(ITEM *item, int resultado_comp));


#endif