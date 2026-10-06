#include "lista_enc.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct no
{
   ITEM *item;
   struct no *next;
   struct no *prior;
} NO;

struct lista
{
    int size;
    NO *begin;
    NO *end;
};

LISTA *lista_criar(void){
   LISTA *l = malloc(sizeof(LISTA));
    if(l == NULL) return NULL;

    l->begin = NULL;
    l->end = NULL;
    l->size = 0;

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

bool lista_inserir_inicio(LISTA *lista, ITEM *item){
   if(lista == NULL || item == NULL) return false;

   NO *new = malloc(sizeof(NO));
   if(new == NULL) return false;
   new->item = item;

   if(lista->begin == NULL) lista->end = new;
   else lista->begin->prior = new;

   new->next = lista->begin;
   new->prior = NULL;
   lista->begin = new;

   lista->size++;
   return true;
}

bool lista_inserir_fim(LISTA *lista, ITEM *item){
   if(lista == NULL || item == NULL) return false;

   NO *new = malloc(sizeof(NO));
   if(new == NULL) return false;
   new->item = item;

   if(lista->end == NULL) lista->begin = new;
   else lista->end->next = new;

   new->prior = lista->end;
   new->next = NULL;
   lista->end = new;

   lista->size++;
   return true;
}

bool lista_inserir(LISTA *lista, ITEM *item, int posicao){
   if(lista == NULL || item == NULL || posicao < 0 || posicao > lista->size) return false;

   if(posicao == 0) return lista_inserir_inicio(lista, item);
   if(posicao == lista->size) return lista_inserir_fim(lista, item);

   NO *new = malloc(sizeof(NO));
   if(new == NULL) return false;
   new->item = item;

   NO *aux;
   if(posicao >= lista->size / 2){
      aux = lista->end;
      for(int i = lista->size - 1; i != posicao; i--) aux = aux->prior;
   }
   else{
      aux = lista->begin;
      for(int i = 0; i != posicao; i++) aux = aux->next;
   }

   new->prior = aux->prior;
   aux->prior->next = new;
   aux->prior = new;
   new->next = aux;
   
   lista->size++;
   return true;
}

ITEM *lista_remover_inicio(LISTA *lista){
   if(lista == NULL || lista_vazia(lista)) return NULL;

   NO *remove = lista->begin;
   ITEM *item = remove->item;

   lista->begin = remove->next;

   if(lista->begin == NULL){
      lista->end = NULL;
   } 
   else lista->begin->prior = NULL;
   
   lista->size--;
   free(remove);
   return item;
}

ITEM *lista_remover_fim(LISTA *lista){
   if(lista == NULL || lista_vazia(lista)) return NULL;

   NO *remove = lista->end;
   ITEM *item = remove->item;

   lista->end = remove->prior;

   if(lista->end == NULL) lista->begin = NULL;
   else lista->end->next = NULL;

   lista->size--;
   free(remove);
   return item;
}

ITEM *lista_remover(LISTA *lista, int posicao){
   if(lista == NULL || lista_vazia(lista) || posicao < 0 || posicao >= lista->size) return NULL;

   if(posicao == 0) return lista_remover_inicio(lista);
   if(posicao == lista->size - 1) return lista_remover_fim(lista);

   NO *aux;
   if(posicao >= lista->size / 2){
      aux = lista->end;
      for(int i = lista->size - 1; i != posicao; i--) aux = aux->prior;
   }
   else{
      aux = lista->begin;
      for(int i = 0; i != posicao; i++) aux = aux->next;
   }


   aux->prior->next = aux->next;
   aux->next->prior = aux->prior;

   ITEM *item = aux->item;
   free(aux);
   lista->size--;
   return item;
}

bool lista_vazia(LISTA *lista){
    if(lista == NULL || lista->size == 0) return true;
    return false;
}

int lista_tamanho(LISTA *lista){
    if(lista == NULL) return -1;
    return lista->size;
}

