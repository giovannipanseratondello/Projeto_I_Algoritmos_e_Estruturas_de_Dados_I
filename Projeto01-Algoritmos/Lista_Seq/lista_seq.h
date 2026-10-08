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

void lista_buscar(LISTA *l, void (*processar)(ITEM *item, void *), void *contexto);

#endif