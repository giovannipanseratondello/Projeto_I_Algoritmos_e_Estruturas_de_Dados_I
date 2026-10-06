#include "lista_ord.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct no{
    ITEM *item;
    struct no *next;
} NO;

struct lista
{
    int size;
    NO *begin;
    NO *end;
    bool ordered;
};



/* ============================================================
   OPERAÇÕES BÁSICAS
   ============================================================ */

LISTA *lista_criar(bool ordenada){
    LISTA *l = malloc(sizeof(LISTA));
    if(l == NULL) return NULL;

    l->begin = NULL;
    l->end = NULL;
    l->size = 0;
    l->ordered = ordenada;
    return l;
}

void lista_apagar(LISTA **lista){
    if(lista == NULL || *lista == NULL) return;

    ITEM *item;
    while(!lista_vazia(*lista)){
        item = lista_remover_inicio(*lista);
        item_apagar(&item);
    }
    free(*lista);
    *lista = NULL;
    return;
}

static bool lista_inserir_ordenada(LISTA *l, ITEM *item){
    NO *new = malloc(sizeof(NO));
    if(new == NULL) return false;
    new->item = item;

    NO *aux = l->begin, *prior = NULL; 
    while(aux != NULL && item_get_chave(aux->item) < item_get_chave(new->item)){
        prior = aux;
        aux = aux->next;
    }
    // Tratam tanto o caso de lista vazia quanto, inserir no fim e quando o item deve ser o primeiro da lista
    if(aux == NULL){ 
        l->end = new;
    }
    if(prior == NULL){
        l->begin = new;
    }
    else prior->next = new;
    new->next = aux;
    l->size++;
    return true;
}

/* ============================================================
   INSERÇÃO
   ============================================================ */

bool lista_inserir_inicio(LISTA *lista, ITEM *item){
    if(lista == NULL || item == NULL) return false;

    if(lista->ordered == true) return lista_inserir_ordenada(lista, item);

    NO *new = malloc(sizeof(NO));
    if(new == NULL) return false;
    new->item = item;

    if(lista->begin == NULL) lista->end = new;

    new->next = lista->begin;
    lista->begin = new;
    lista->size++;
    return true;
}

bool lista_inserir_fim(LISTA *lista, ITEM *item){
    if(lista == NULL || item == NULL) return false;

    if(lista->ordered == true) return lista_inserir_ordenada(lista, item);

    NO *new = malloc(sizeof(NO));
    if(new == NULL) return false;
    new->item = item;

    if(lista->end == NULL) lista->begin = new;
    else lista->end->next = new;

    new->next = NULL;
    lista->end = new;

    lista->size++;
    return true;
}

bool lista_inserir(LISTA *lista, ITEM *item, int posicao){
    if(lista == NULL || item == NULL || posicao < 0 || posicao > lista->size) return false;

    if(lista->ordered == true) return lista_inserir_ordenada(lista, item);

    if(posicao == 0) return lista_inserir_inicio(lista, item);
    if(posicao == lista->size) return lista_inserir_fim(lista, item);

    NO *new = malloc(sizeof(NO));
    if(new == NULL) return false;
    new->item = item;

    NO *aux = lista->begin, *prior = NULL; // Prior nunca será NULL, pois para ele ser, posição tem que ser igual a 0 e ela nunca será nessa parte do código!
    for(int i = 0; i != posicao; i++){ // Posição nunca será igual a  0 ou ao tamanho da lista pois ele já teria caído nos if´s acima. 
        prior = aux;
        aux = aux->next;
    }
    
    prior->next = new;
    new->next = aux;

    lista->size++;
    return true;
}


/* ============================================================
   REMOÇÃO
   ============================================================ */

ITEM *lista_remover_inicio(LISTA *lista){
    if(lista == NULL || lista_vazia(lista)) return NULL;

    NO *remove = lista->begin;
    ITEM *item = remove->item;

    if(remove->next == NULL) lista->end = NULL;
    
    lista->begin = remove->next;

    free(remove);
    lista->size--;
    return item;
}

ITEM *lista_remover_fim(LISTA *lista){
    if(lista == NULL || lista_vazia(lista)) return NULL;

    NO *remove = lista->end;
    ITEM *item = remove->item;

    if(lista->size == 1){
        lista->begin = NULL;
        lista->end = NULL;
    } 
    else{
        NO *aux = lista->begin;

        while(aux->next != lista->end) aux = aux->next;

        aux->next = NULL;
        lista->end = aux;
    }

    free(remove);
    lista->size--;
    return item;
}

ITEM *lista_remover(LISTA *lista, int posicao){
    if(lista == NULL || lista_vazia(lista) || posicao < 0 || posicao >= lista->size) return NULL;

    if(posicao == 0) return lista_remover_inicio(lista);
    if(posicao == lista->size - 1) return lista_remover_fim(lista);

    NO *aux = lista->begin, *prior = NULL; // Prior nunca será NULL, pois para ele ser, posição tem que ser igual a 0 e ela nunca será nessa parte do código!
    for(int i = 0; i != posicao; i++){ // Posição nunca será igual a  0 ou ao tamanho da lista pois ele já teria caído nos if´s acima. 
        prior = aux;
        aux = aux->next;
    }

    prior->next = aux->next;
    ITEM *item = aux->item;

    free(aux);
    lista->size--;
    return item;
    }


/* ============================================================
   ACESSO
   ============================================================ */

ITEM *lista_primeiro(LISTA *lista){
    if(lista == NULL || lista_vazia(lista)) return NULL;

    return lista->begin->item;
}

ITEM *lista_ultimo(LISTA *lista){
    if(lista == NULL || lista_vazia(lista)) return NULL;

    return lista->end->item;
}

ITEM *lista_obter(LISTA *lista, int posicao){
    if(lista == NULL || lista_vazia(lista) || posicao < 0 || posicao >= lista->size) return NULL;

    if(posicao == 0) return lista_primeiro(lista);
    if(posicao == lista->size - 1) return lista_ultimo(lista);

    NO *aux = lista->begin;
    for(int i = 0; aux != NULL && i != posicao; i++) aux = aux->next;

    if(aux == NULL) return NULL;
    
    return aux->item;
}

ITEM *lista_buscar(LISTA *lista, int chave){
    if(lista == NULL || lista_vazia(lista)) return NULL;

    NO *aux = lista->begin;
    if(lista->ordered == true){
        for(; aux != NULL && item_get_chave(aux->item) < chave;) aux = aux->next;
    }
    else for(; aux != NULL && item_get_chave(aux->item) != chave;) aux = aux->next;

    if(aux == NULL || item_get_chave(aux->item) != chave) return NULL;
    
    return aux->item;
}


/* ============================================================
   INFORMAÇÕES
   ============================================================ */

bool lista_vazia(LISTA *lista){
    if(lista == NULL || lista->size == 0) return true;
    return false;
}

int lista_tamanho(LISTA *lista){
    if(lista == NULL) return -1;
    return lista->size;
}


/* ============================================================
   AUXILIAR
   ============================================================ */

void lista_imprimir(LISTA *lista){
    if(lista == NULL || lista_vazia(lista)) return;

    NO *aux = lista->begin;
    printf("Inicio -> ");
    while(aux != NULL){
        printf("(%d) ", item_get_chave(aux->item));
        aux = aux->next;
    }
    printf("<- Fim\n");
}

