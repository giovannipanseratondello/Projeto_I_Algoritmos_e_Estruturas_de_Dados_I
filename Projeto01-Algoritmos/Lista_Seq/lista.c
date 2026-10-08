#include "lista_seq.h"
#include <stdlib.h>
#include <stdio.h>

struct lista
{
    ITEM *itens[MAX_LENGTH];
    int end; // Último item da lista
    int size; // Próximo item
};

LISTA *lista_criar(void){
    LISTA *l = malloc(sizeof(LISTA));
    if(l == NULL) return NULL;

    for(int i = 0; i < MAX_LENGTH; i++) l->itens[i] = NULL;

    l->size = 0;
    l->end = -1; // Permite que ao inserir o primeiro elemento, ele referencie corretamente
    return l;
}

void lista_apagar(LISTA **lista){
    if(lista == NULL || *lista == NULL) return;

    ITEM *item;
    while(!lista_vazia(*lista)){
        item = lista_remover_fim(*lista);
        item_apagar(&item);
    }
    free(*lista);
    *lista = NULL;
}

bool lista_inserir_inicio(LISTA *lista, ITEM *item){
    if(lista == NULL || item == NULL || lista_tamanho(lista) == MAX_LENGTH) return false;

    if(!lista_vazia(lista)){
        for(int i = lista->size; i > 0; i--){
            lista->itens[i] = lista->itens[i - 1];
        }
    }
    lista->itens[0] = item;
    lista->end++;
    lista->size++;
    return true;
}

bool lista_inserir_fim(LISTA *lista, ITEM *item){
    if(lista == NULL || item == NULL || lista_tamanho(lista) == MAX_LENGTH) return false;

    lista->itens[lista->size] = item;
    lista->end++;
    lista->size++;
    return true;
}

bool lista_inserir(LISTA *lista, ITEM *item, int posicao){
    if(lista == NULL || item == NULL || lista_tamanho(lista) == MAX_LENGTH || posicao < 0 || posicao > lista->size) return false;

    if(posicao == 0) return lista_inserir_inicio(lista, item);
    if(posicao == lista->size) return lista_inserir_fim(lista, item);

    for(int i = lista->size; i > posicao; i--){
        lista->itens[i] = lista->itens[i - 1];
    }
    lista->itens[posicao] = item;
    lista->end++;
    lista->size++;
    return true;
}

ITEM *lista_remover_inicio(LISTA *lista){
    if(lista == NULL || lista_vazia(lista)) return NULL;

    ITEM *remove = lista->itens[0];
    if(lista->size != 1){
        for(int i = 0; i < lista->size; i++){
            lista->itens[i] = lista->itens[i + 1];
        }
    }
    else lista->itens[0] = NULL;
    lista->end--;
    lista->size--;
    return remove;
}

ITEM *lista_remover_fim(LISTA *lista){
    if(lista == NULL || lista_vazia(lista)) return NULL;

    ITEM *remove = lista->itens[lista->end];
    lista->itens[lista->end] = NULL;
    lista->end--;
    lista->size--;
    return remove;
}

ITEM *lista_remover(LISTA *lista, int posicao){
    if(lista == NULL || posicao < 0 || posicao >= lista->size) return NULL;

    if(posicao == 0) return lista_remover_inicio(lista);
    if(posicao == lista->size - 1) return lista_remover_fim(lista);

    ITEM *remove = lista->itens[posicao];
    for(int i = posicao; i < lista->size - 1; i++){
        lista->itens[i] = lista->itens[i + 1];
    }

    lista->end--;
    lista->size--;
    lista->itens[lista->size] = NULL;
    return remove;
}

bool lista_vazia(LISTA *lista){
    if(lista == NULL || lista->size == 0) return true;
    return false;
}

int lista_tamanho(LISTA *lista){
    if(lista == NULL) return -1;
    return lista->size;
}


void lista_buscar(LISTA *l, void (*processar)(ITEM *item, void *), void *contexto) {
    if (l == NULL || lista_vazia(l) || processar == NULL) {
        return;
    }

    int i = 0;
    while (i < lista_tamanho(l)) {
        
        // Dispara a função callback enviando o item atual e o resultado da comparação (-1, 0, ou 1)
        processar(l->itens[i], contexto);
        
        i++;
        
    }
}
