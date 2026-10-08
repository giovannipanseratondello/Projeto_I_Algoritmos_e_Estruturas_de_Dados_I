#ifndef LISTA_CRUZ_H
#define LISTA_CRUZ_H

#include <stdbool.h>
#include "../Item/item.h"

typedef struct lista_cruz LISTA_CRUZ;
LISTA_CRUZ *lista_cruz_criar(void);
void lista_cruz_apagar(LISTA_CRUZ **lista);
bool lista_cruz_inserir(LISTA_CRUZ *lista, ITEM *item);
bool lista_cruz_inserir_cruzado(LISTA_CRUZ *lista, const char *chave_linha, const char *chave_coluna, ITEM *item);

#endif /* LISTA_CRUZ_H */