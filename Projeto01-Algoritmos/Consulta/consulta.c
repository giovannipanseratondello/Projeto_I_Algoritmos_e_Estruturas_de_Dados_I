#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "consulta.h"

struct filtro{
    char campo[64];
    char operador[4]; // "==", "!=", ">", "<", ">=", "<="
    char valor_str[128]; // guarda o terceiro parametro de FILTER
    double valor_num; // guarda o valor numerico de valor_str
};


struct similaridade{
    char campo[64];
    double valor_alvo;
    double tolerancia;
    double peso;
};

struct query{
   FILTRO filtros[32];
   int qnt_filtos;
   
   SIMILARIDADE similaridades[32];
   int qtd_sim;

   double accept;

   char order_campo[64];
   char order_tipo[8];
    
   char group_campo[64];
   
   char cross_campo1[64];
   char cross_campo2[64];
};
