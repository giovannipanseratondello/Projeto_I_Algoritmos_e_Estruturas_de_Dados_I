#include "../Item/item.h"

#ifndef CONSULTA_H
#define CONSULTA_H

typedef struct filtro FILTRO;
typedef struct similaridade SIMILARIDADE;
typedef struct query QUERY;

QUERY *query_carregar( char *arquivo);
void query_destruir( QUERY **query);
bool query_avaliar_item(QUERY *q, ITEM *item);
void processar(ITEM *item, void *contexto);

#endif