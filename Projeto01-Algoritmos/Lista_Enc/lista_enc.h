#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include "../Item/item.h"

typedef struct lista LISTA;


LISTA *lista_criar(void);

void lista_apagar(LISTA **lista);

bool lista_inserir_inicio(LISTA *lista, ITEM *item);

bool lista_inserir_fim(LISTA *lista, ITEM *item);

bool lista_inserir(LISTA *lista, ITEM *item, int posicao);

ITEM *lista_remover_inicio(LISTA *lista);

ITEM *lista_remover_fim(LISTA *lista);

ITEM *lista_remover(LISTA *lista, int posicao);

bool lista_vazia(LISTA *lista);

int lista_tamanho(LISTA *lista);


void lista_buscar(LISTA *l, void (*processar)(ITEM *item, void *), void *contexto);

#endif