#include <stdio.h>
#include <stdlib.h>
#include "lista_cabeca.h"

typedef struct no NO;

struct no {
    ITEM *item;
    NO *proximo;
    NO *anterior;
};

struct lista {
    NO *cabeca; // Nó sentinela que não armazena dado de ITEM
    int tamanho;
};

/* Helper interno para alocar um novo nó */
static NO *no_criar(ITEM *item) {
    NO *novo = (NO *) malloc(sizeof(NO));
    if (novo != NULL) {
        novo->item = item;
        novo->proximo = NULL;
        novo->anterior = NULL;
    }
    return novo;
}

LISTA *lista_criar(void) {
    LISTA *lista = (LISTA *) malloc(sizeof(LISTA));
    if (lista == NULL) return NULL;

    // Aloca o nó cabeça/sentinela real exigido pelo projeto
    lista->cabeca = (NO *) malloc(sizeof(NO));
    if (lista->cabeca == NULL) {
        free(lista);
        return NULL;
    }

    // Em uma lista duplamente encadeada circular/sentinela,
    // o nó cabeça aponta para si mesmo quando vazia.
    lista->cabeca->item = NULL;
    lista->cabeca->proximo = lista->cabeca;
    lista->cabeca->anterior = lista->cabeca;
    lista->tamanho = 0;

    return lista;
}

void lista_apagar(LISTA **lista) {
    if (lista == NULL || *lista == NULL) return;

    NO *atual = (*lista)->cabeca->proximo;
    while (atual != (*lista)->cabeca) {
        NO *temp = atual;
        atual = atual->proximo;
        
        if (temp->item != NULL) {
            item_destruir(&(temp->item));
        }
        free(temp);
    }

    // Libera o nó cabeça real e a estrutura da lista
    free((*lista)->cabeca);
    free(*lista);
    *lista = NULL;
}

bool lista_vazia(LISTA *lista) {
    if (lista == NULL || lista->cabeca == NULL) return true;
    return (lista->tamanho == 0);
}

int lista_tamanho(LISTA *lista) {
    if (lista == NULL) return 0;
    return lista->tamanho;
}

bool lista_inserir_fim(LISTA *lista, ITEM *item) {
    if (lista == NULL || item == NULL) return false;

    NO *novo = no_criar(item);
    if (novo == NULL) return false;

    NO *ultimo = lista->cabeca->anterior;

    novo->proximo = lista->cabeca;
    novo->anterior = ultimo;
    ultimo->proximo = novo;
    lista->cabeca->anterior = novo;

    lista->tamanho++;
    return true;
}

bool lista_inserir_inicio(LISTA *lista, ITEM *item) {
    if (lista == NULL || item == NULL) return false;

    NO *novo = no_criar(item);
    if (novo == NULL) return false;

    NO *primeiro = lista->cabeca->proximo;

    novo->proximo = primeiro;
    novo->anterior = lista->cabeca;
    lista->cabeca->proximo = novo;
    primeiro->anterior = novo;

    lista->tamanho++;
    return true;
}

bool lista_inserir(LISTA *lista, ITEM *item, int posicao) {
    if (lista == NULL || item == NULL || posicao < 0 || posicao > lista->tamanho) {
        return false;
    }

    if (posicao == 0) return lista_inserir_inicio(lista, item);
    if (posicao == lista->tamanho) return lista_inserir_fim(lista, item);

    NO *atual = lista->cabeca->proximo;
    for (int i = 0; i < posicao; i++) {
        atual = atual->proximo;
    }

    NO *novo = no_criar(item);
    if (novo == NULL) return false;

    novo->proximo = atual;
    novo->anterior = atual->anterior;
    atual->anterior->proximo = novo;
    atual->anterior = novo;

    lista->tamanho++;
    return true;
}

ITEM *lista_remover_inicio(LISTA *lista) {
    if (lista_vazia(lista)) return NULL;

    NO *remover = lista->cabeca->proximo;
    ITEM *item = remover->item;

    lista->cabeca->proximo = remover->proximo;
    remover->proximo->anterior = lista->cabeca;

    free(remover);
    lista->tamanho--;

    return item;
}

ITEM *lista_remover_fim(LISTA *lista) {
    if (lista_vazia(lista)) return NULL;

    NO *remover = lista->cabeca->anterior;
    ITEM *item = remover->item;

    remover->anterior->proximo = lista->cabeca;
    lista->cabeca->anterior = remover->anterior;

    free(remover);
    lista->tamanho--;

    return item;
}

ITEM *lista_remover(LISTA *lista, int posicao) {
    if (lista_vazia(lista) || posicao < 0 || posicao >= lista->tamanho) {
        return NULL;
    }

    if (posicao == 0) return lista_remover_inicio(lista);
    if (posicao == lista->tamanho - 1) return lista_remover_fim(lista);

    NO *remover = lista->cabeca->proximo;
    for (int i = 0; i < posicao; i++) {
        remover = remover->proximo;
    }

    ITEM *item = remover->item;
    remover->anterior->proximo = remover->proximo;
    remover->proximo->anterior = remover->anterior;

    free(remover);
    lista->tamanho--;

    return item;
}

ITEM *lista_primeiro(LISTA *lista) {
    if (lista_vazia(lista)) return NULL;
    return lista->cabeca->proximo->item;
}

ITEM *lista_ultimo(LISTA *lista) {
    if (lista_vazia(lista)) return NULL;
    return lista->cabeca->anterior->item;
}

ITEM *lista_obter(LISTA *lista, int posicao) {
    if (lista_vazia(lista) || posicao < 0 || posicao >= lista->tamanho) {
        return NULL;
    }

    NO *atual = lista->cabeca->proximo;
    for (int i = 0; i < posicao; i++) {
        atual = atual->proximo;
    }

    return atual->item;
}

