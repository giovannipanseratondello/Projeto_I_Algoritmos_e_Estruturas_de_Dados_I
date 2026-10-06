#ifndef LISTA_CRUZ_H
#define LISTA_CRUZ_H

#include <stdbool.h>
#include "../Item/item.h"

typedef struct lista_cruz LISTA_CRUZ;

/* ============================================================
   OPERAÇÕES BÁSICAS
   ============================================================ */

/* Cria uma lista cruzada vazia */
LISTA_CRUZ *lista_cruz_criar(void);

/* Apaga e desaloca a estrutura e os itens compartilhados */
void lista_cruz_apagar(LISTA_CRUZ **lista);

/* ============================================================
   INSERÇÃO
   ============================================================ */

/* Inserção sequencial simples na lista principal (compatível com o ler_arquivo) */
bool lista_cruz_inserir(LISTA_CRUZ *lista, ITEM *item);

/* Inserção em uma posição cruzada específica (Matriz Esparsa / Eixos X e Y) */
bool lista_cruz_inserir_cruzado(LISTA_CRUZ *lista, const char *chave_linha, const char *chave_coluna, ITEM *item);

/* ============================================================
   INFORMAÇÕES E CONSULTA
   ============================================================ */

bool lista_cruz_vazia(LISTA_CRUZ *lista);
int lista_cruz_tamanho(LISTA_CRUZ *lista);

/* ============================================================
   AUXILIAR
   ============================================================ */

/* Imprime a estrutura nos dois eixos de navegação */
void lista_cruz_imprimir(LISTA_CRUZ *lista);

#endif /* LISTA_CRUZ_H */