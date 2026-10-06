#ifndef LISTA_GEN_H
#define LISTA_GEN_H

#include <stdbool.h>
#include "../Item/item.h"

typedef struct lista_gen LISTA_GEN;

/* ============================================================
   OPERAÇÕES BÁSICAS
   ============================================================ */

/* Cria uma lista generalizada vazia */
LISTA_GEN *lista_gen_criar(void);

/* Apaga e desaloca toda a estrutura recursivamente */
void lista_gen_apagar(LISTA_GEN **lista);

/* ============================================================
   INSERÇÃO
   ============================================================ */

/* Inserção simples no fim da lista principal (uso padrão no reader) */
bool lista_gen_inserir(LISTA_GEN *lista, ITEM *item);

/* Inserção organizada por chave/atributo (para operações de GROUP) */
bool lista_gen_inserir_grupo(LISTA_GEN *lista, const char *chave_grupo, ITEM *item);

/* Inserção de uma sublista como nó filho */
bool lista_gen_inserir_sublista(LISTA_GEN *lista_pai, const char *chave_grupo, LISTA_GEN *sublista);

/* ============================================================
   ACESSO E CONSULTA
   ============================================================ */

/* Retorna a sublista correspondente a uma chave de grupo */
LISTA_GEN *lista_gen_obter_sublista(LISTA_GEN *lista, const char *chave_grupo);

/* Retorna a chave do grupo armazenada no nó atual (se for nó de sublista) */
const char *lista_gen_obter_chave(LISTA_GEN *lista);

/* ============================================================
   INFORMAÇÕES
   ============================================================ */

bool lista_gen_vazia(LISTA_GEN *lista);
int lista_gen_tamanho(LISTA_GEN *lista);

/* ============================================================
   AUXILIAR
   ============================================================ */

/* Imprime a estrutura hierárquica e recursiva da lista generalizada */
void lista_gen_imprimir(LISTA_GEN *lista);

#endif /* LISTA_GEN_H */