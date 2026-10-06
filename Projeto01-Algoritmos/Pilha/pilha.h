#ifndef PILHA_H
#define PILHA_H

#include <stdbool.h>
#include "../Item/item.h"

typedef struct pilha PILHA;

/* Criação e destruição */
PILHA *pilha_criar();
void pilha_apagar(PILHA **p);

/* Operações da pilha */
bool pilha_empilhar(PILHA *p, ITEM *item);
ITEM *pilha_desempilhar(PILHA *p);
ITEM *pilha_topo(PILHA *p);

/* Informações */
bool pilha_vazia(PILHA *p);
int pilha_tamanho(PILHA *p);

/* Auxiliar */
void pilha_imprimir(PILHA *p);
void pilha_inverter(PILHA *p);
void pilha_buscar(PILHA *p, ITEM *item_ref, const char *campo, void (*processar)(ITEM *item, int resultado_comp));

#endif