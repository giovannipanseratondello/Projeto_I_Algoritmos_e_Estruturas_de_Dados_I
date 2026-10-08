#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include "../Item/item.h"

typedef struct lista LISTA;


LISTA *lista_criar(bool ordenada);

void lista_apagar(LISTA **lista);

bool lista_inserir_inicio(LISTA *lista, ITEM *item);

void lista_buscar(LISTA *l, void (*processar)(ITEM *item, void *), void *contexto);

#endif