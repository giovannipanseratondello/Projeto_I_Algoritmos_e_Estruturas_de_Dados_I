#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_gen.h"

typedef enum {
    NO_ATOMO,   // Contém um dado individual (ITEM *)
    NO_SUBLISTA // Contém um ponteiro para outra LISTA_GEN (Sublista/Grupo)
} TipoNo;

typedef struct no_gen NO_GEN;

struct no_gen {
    TipoNo tipo;
    char chave_grupo[64]; // Nome/Valor do atributo de agrupamento (ex: "CRITICAL", "2024")
    
    union {
        ITEM *item;           // Usado se tipo == NO_ATOMO
        LISTA_GEN *sublista;  // Usado se tipo == NO_SUBLISTA
    } info;

    NO_GEN *proximo;
};

struct lista_gen {
    NO_GEN *cabeca; // Nó sentinela para simplificar encadeamento
    char chave_lista[64]; // Identificação da sublista atual
    int tamanho;
};

/* Helper privado para alocação de nó atômico (ITEM) */
static NO_GEN *no_gen_criar_atomo(ITEM *item) {
    NO_GEN *novo = (NO_GEN *) malloc(sizeof(NO_GEN));
    if (novo != NULL) {
        novo->tipo = NO_ATOMO;
        novo->chave_grupo[0] = '\0';
        novo->info.item = item;
        novo->proximo = NULL;
    }
    return novo;
}

/* Helper privado para alocação de nó de sublista */
static NO_GEN *no_gen_criar_sublista(const char *chave_grupo, LISTA_GEN *sublista) {
    NO_GEN *novo = (NO_GEN *) malloc(sizeof(NO_GEN));
    if (novo != NULL) {
        novo->tipo = NO_SUBLISTA;
        if (chave_grupo != NULL) {
            strncpy(novo->chave_grupo, chave_grupo, sizeof(novo->chave_grupo) - 1);
            novo->chave_grupo[sizeof(novo->chave_grupo) - 1] = '\0';
        } else {
            novo->chave_grupo[0] = '\0';
        }
        novo->info.sublista = sublista;
        novo->proximo = NULL;
    }
    return novo;
}

LISTA_GEN *lista_gen_criar(void) {
    LISTA_GEN *lista = (LISTA_GEN *) malloc(sizeof(LISTA_GEN));
    if (lista == NULL) return NULL;

    lista->cabeca = (NO_GEN *) malloc(sizeof(NO_GEN));
    if (lista->cabeca == NULL) {
        free(lista);
        return NULL;
    }

    lista->cabeca->tipo = NO_ATOMO;
    lista->cabeca->info.item = NULL;
    lista->cabeca->proximo = NULL;
    lista->chave_lista[0] = '\0';
    lista->tamanho = 0;

    return lista;
}

void lista_gen_apagar(LISTA_GEN **lista) {
    if (lista == NULL || *lista == NULL) return;

    NO_GEN *atual = (*lista)->cabeca->proximo;
    while (atual != NULL) {
        NO_GEN *temp = atual;
        atual = atual->proximo;

        if (temp->tipo == NO_ATOMO) {
            if (temp->info.item != NULL) {
                item_destruir(&(temp->info.item));
            }
        } else if (temp->tipo == NO_SUBLISTA) {
            // Chamada recursiva para desalocar a sublista inteira
            lista_gen_apagar(&(temp->info.sublista));
        }

        free(temp);
    }

    free((*lista)->cabeca);
    free(*lista);
    *lista = NULL;
}

bool lista_gen_vazia(LISTA_GEN *lista) {
    if (lista == NULL || lista->cabeca == NULL) return true;
    return (lista->tamanho == 0);
}

int lista_gen_tamanho(LISTA_GEN *lista) {
    if (lista == NULL) return 0;
    return lista->tamanho;
}

/* Inserção sequencial simples no fim da lista (Compatível com ler_arquivo) */
bool lista_gen_inserir(LISTA_GEN *lista, ITEM *item) {
    if (lista == NULL || item == NULL) return false;

    NO_GEN *novo = no_gen_criar_atomo(item);
    if (novo == NULL) return false;

    NO_GEN *atual = lista->cabeca;
    while (atual->proximo != NULL) {
        atual = atual->proximo;
    }

    atual->proximo = novo;
    lista->tamanho++;
    return true;
}

/* Inserção agrupada: Procura uma sublista com a chave ou cria uma nova sublista */
bool lista_gen_inserir_grupo(LISTA_GEN *lista, const char *chave_grupo, ITEM *item) {
    if (lista == NULL || chave_grupo == NULL || item == NULL) return false;

    NO_GEN *atual = lista->cabeca->proximo;
    NO_GEN *no_grupo = NULL;

    // Busca se já existe um grupo com essa chave
    while (atual != NULL) {
        if (atual->tipo == NO_SUBLISTA && strcmp(atual->chave_grupo, chave_grupo) == 0) {
            no_grupo = atual;
            break;
        }
        atual = atual->proximo;
    }

    // Se o grupo/sublista não existe, cria um novo
    if (no_grupo == NULL) {
        LISTA_GEN *nova_sublista = lista_gen_criar();
        if (nova_sublista == NULL) return false;

        strncpy(nova_sublista->chave_lista, chave_grupo, sizeof(nova_sublista->chave_lista) - 1);
        
        no_grupo = no_gen_criar_sublista(chave_grupo, nova_sublista);
        if (no_grupo == NULL) {
            lista_gen_apagar(&nova_sublista);
            return false;
        }

        // Insere o nó de sublista na lista principal
        NO_GEN *p = lista->cabeca;
        while (p->proximo != NULL) {
            p = p->proximo;
        }
        p->proximo = no_grupo;
        lista->tamanho++;
    }

    // Insere o item atômico dentro da sublista correspondente
    return lista_gen_inserir(no_grupo->info.sublista, item);
}

bool lista_gen_inserir_sublista(LISTA_GEN *lista_pai, const char *chave_grupo, LISTA_GEN *sublista) {
    if (lista_pai == NULL || sublista == NULL) return false;

    NO_GEN *novo = no_gen_criar_sublista(chave_grupo, sublista);
    if (novo == NULL) return false;

    NO_GEN *atual = lista_pai->cabeca;
    while (atual->proximo != NULL) {
        atual = atual->proximo;
    }

    atual->proximo = novo;
    lista_pai->tamanho++;
    return true;
}

LISTA_GEN *lista_gen_obter_sublista(LISTA_GEN *lista, const char *chave_grupo) {
    if (lista_gen_vazia(lista) || chave_grupo == NULL) return NULL;

    NO_GEN *atual = lista->cabeca->proximo;
    while (atual != NULL) {
        if (atual->tipo == NO_SUBLISTA && strcmp(atual->chave_grupo, chave_grupo) == 0) {
            return atual->info.sublista;
        }
        atual = atual->proximo;
    }

    return NULL;
}

const char *lista_gen_obter_chave(LISTA_GEN *lista) {
    if (lista == NULL) return "";
    return lista->chave_lista;
}

/* Helper interno recursivo para impressão hierárquica com indentação */
static void imprimir_recursivo(LISTA_GEN *lista, int nivel) {
    if (lista_gen_vazia(lista)) return;

    NO_GEN *atual = lista->cabeca->proximo;
    while (atual != NULL) {
        for (int i = 0; i < nivel; i++) printf("  ");

        if (atual->tipo == NO_ATOMO) {
            printf("[ITEM] -> ");
            // Se houver uma função de impressão no item.h:
            // item_imprimir(atual->info.item);
            printf("\n");
        } else if (atual->tipo == NO_SUBLISTA) {
            printf("[GRUPO: %s]\n", atual->chave_grupo);
            imprimir_recursivo(atual->info.sublista, nivel + 1);
        }
        atual = atual->proximo;
    }
}

void lista_gen_imprimir(LISTA_GEN *lista) {
    imprimir_recursivo(lista, 0);
}