#include <stdbool.h>

#ifndef ITEM_H
#define ITEM_H

typedef struct item ITEM;

// cria o item e atribui valores Nulos ao seus campos
ITEM *item_criar(); 

// libera a memoria do item
void item_apagar(ITEM **item); 

//imprime o item no csv "saida" no mesmo formato do csv padrao
void item_imprimir_csv(ITEM *item, FILE *saida);  

//compara um "campo" de Item_A e Item_B ---> retorna -1 se B > A, 0 se A = B e 1 se A > B 
int item_comparar(ITEM *item_A, ITEM *item_B, char *campo);



#endif