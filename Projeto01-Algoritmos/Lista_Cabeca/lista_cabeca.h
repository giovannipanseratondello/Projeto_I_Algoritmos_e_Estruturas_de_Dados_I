#ifndef LISTA_CABECA_H
#define LISTA_CABECA_H

#include <stdbool.h>
#include "../Item/item.h"

typedef struct lista LISTA;

LISTA *lista_criar(void);
void lista_apagar(LISTA **lista);


bool lista_inserir_inicio(LISTA *lista, ITEM *item);

#endif /* LISTA_CABECA_H */