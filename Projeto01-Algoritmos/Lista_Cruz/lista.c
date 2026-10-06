#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_cruz.h"

typedef struct no_cruz NO_CRUZ;
typedef struct cabeca_eixo CABECA_EIXO;

/* Nó de dados da lista cruzada */
struct no_cruz {
    ITEM *item;                     // Elemento armazenado (compartilhado)
    char chave_linha[64];           // Identificador do eixo horizontal (ex: "CRITICAL")
    char chave_coluna[64];          // Identificador do eixo vertical (ex: "2024")
    
    NO_CRUZ *prox_linha;            // Ponteiro para o próximo nó na mesma linha
    NO_CRUZ *prox_coluna;           // Ponteiro para o próximo nó na mesma coluna
};

/* Cabeça de lista para gerenciar o início de cada Linha/Coluna */
struct cabeca_eixo {
    char chave[64];
    NO_CRUZ *primeiro;
    CABECA_EIXO *proximo;
};

/* Estrutura principal da Lista Cruzada */
struct lista_cruz {
    CABECA_EIXO *linhas;           // Cabeça da lista de linhas
    CABECA_EIXO *colunas;          // Cabeça da lista de colunas
    NO_CRUZ *lista_linear;         // Encadeamento sequencial simples (para carga via leitor)
    int tamanho;
};

/* Helper privado para alocar o nó cruzado */
static NO_CRUZ *no_cruz_criar(ITEM *item, const char *linha, const char *coluna) {
    NO_CRUZ *novo = (NO_CRUZ *) malloc(sizeof(NO_CRUZ));
    if (novo != NULL) {
        novo->item = item;
        
        if (linha != NULL) {
            strncpy(novo->chave_linha, linha, sizeof(novo->chave_linha) - 1);
            novo->chave_linha[sizeof(novo->chave_linha) - 1] = '\0';
        } else {
            novo->chave_linha[0] = '\0';
        }

        if (coluna != NULL) {
            strncpy(novo->chave_coluna, coluna, sizeof(novo->chave_coluna) - 1);
            novo->chave_coluna[sizeof(novo->chave_coluna) - 1] = '\0';
        } else {
            novo->chave_coluna[0] = '\0';
        }

        novo->prox_linha = NULL;
        novo->prox_coluna = NULL;
    }
    return novo;
}

/* Helper para buscar ou criar uma cabeça de eixo (linha ou coluna) */
static CABECA_EIXO *obter_ou_criar_eixo(CABECA_EIXO **lista_eixo, const char *chave) {
    CABECA_EIXO *atual = *lista_eixo;
    CABECA_EIXO *anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->chave, chave) == 0) {
            return atual;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    CABECA_EIXO *novo = (CABECA_EIXO *) malloc(sizeof(CABECA_EIXO));
    if (novo == NULL) return NULL;

    strncpy(novo->chave, chave, sizeof(novo->chave) - 1);
    novo->chave[sizeof(novo->chave) - 1] = '\0';
    novo->primeiro = NULL;
    novo->proximo = NULL;

    if (anterior == NULL) {
        *lista_eixo = novo;
    } else {
        anterior->proximo = novo;
    }

    return novo;
}

LISTA_CRUZ *lista_cruz_criar(void) {
    LISTA_CRUZ *lista = (LISTA_CRUZ *) malloc(sizeof(LISTA_CRUZ));
    if (lista == NULL) return NULL;

    lista->linhas = NULL;
    lista->colunas = NULL;
    lista->lista_linear = NULL;
    lista->tamanho = 0;

    return lista;
}

void lista_cruz_apagar(LISTA_CRUZ **lista) {
    if (lista == NULL || *lista == NULL) return;

    // Libera todos os nós e itens através da lista sequencial/linear
    NO_CRUZ *atual = (*lista)->lista_linear;
    while (atual != NULL) {
        NO_CRUZ *temp = atual;
        atual = atual->prox_linha; // Usa o ponteiro linear para avançar

        if (temp->item != NULL) {
            item_destruir(&(temp->item));
        }
        free(temp);
    }

    // Libera os cabeçalhos de linhas
    CABECA_EIXO *eixo_lin = (*lista)->linhas;
    while (eixo_lin != NULL) {
        CABECA_EIXO *temp = eixo_lin;
        eixo_lin = eixo_lin->proximo;
        free(temp);
    }

    // Libera os cabeçalhos de colunas
    CABECA_EIXO *eixo_col = (*lista)->colunas;
    while (eixo_col != NULL) {
        CABECA_EIXO *temp = eixo_col;
        eixo_col = eixo_col->proximo;
        free(temp);
    }

    free(*lista);
    *lista = NULL;
}

bool lista_cruz_vazia(LISTA_CRUZ *lista) {
    if (lista == NULL) return true;
    return (lista->tamanho == 0);
}

int lista_cruz_tamanho(LISTA_CRUZ *lista) {
    if (lista == NULL) return 0;
    return lista->tamanho;
}

/* Inserção Sequencial Simples (Atende à chamada genérica do ler_arquivo) */
bool lista_cruz_inserir(LISTA_CRUZ *lista, ITEM *item) {
    if (lista == NULL || item == NULL) return false;

    NO_CRUZ *novo = no_cruz_criar(item, "GERAL", "GERAL");
    if (novo == NULL) return false;

    if (lista->lista_linear == NULL) {
        lista->lista_linear = novo;
    } else {
        NO_CRUZ *atual = lista->lista_linear;
        while (atual->prox_linha != NULL) {
            atual = atual->prox_linha;
        }
        atual->prox_linha = novo;
    }

    lista->tamanho++;
    return true;
}

/* Inserção Cruzada por Eixos/Atributos (Para operações de CROSS) */
bool lista_cruz_inserir_cruzado(LISTA_CRUZ *lista, const char *chave_linha, const char *chave_coluna, ITEM *item) {
    if (lista == NULL || chave_linha == NULL || chave_coluna == NULL || item == NULL) {
        return false;
    }

    NO_CRUZ *novo = no_cruz_criar(item, chave_linha, chave_coluna);
    if (novo == NULL) return false;

    // 1. Obter/Criar os eixos correspondentes
    CABECA_EIXO *eixo_lin = obter_ou_criar_eixo(&(lista->linhas), chave_linha);
    CABECA_EIXO *eixo_col = obter_ou_criar_eixo(&(lista->colunas), chave_coluna);

    if (eixo_lin == NULL || eixo_col == NULL) {
        free(novo);
        return false;
    }

    // 2. Encadear na Linha (Eixo Horizontal)
    if (eixo_lin->primeiro == NULL) {
        eixo_lin->primeiro = novo;
    } else {
        NO_CRUZ *p = eixo_lin->primeiro;
        while (p->prox_linha != NULL) {
            p = p->prox_linha;
        }
        p->prox_linha = novo;
    }

    // 3. Encadear na Coluna (Eixo Vertical)
    if (eixo_col->primeiro == NULL) {
        eixo_col->primeiro = novo;
    } else {
        NO_CRUZ *p = eixo_col->primeiro;
        while (p->prox_coluna != NULL) {
            p = p->prox_coluna;
        }
        p->prox_coluna = novo;
    }

    // 4. Manter na lista linear para controle e destruição segura
    if (lista->lista_linear == NULL) {
        lista->lista_linear = novo;
    } else {
        NO_CRUZ *p = lista->lista_linear;
        while (p->prox_linha != NULL) {
            p = p->prox_linha;
        }
        p->prox_linha = novo;
    }

    lista->tamanho++;
    return true;
}

void lista_cruz_imprimir(LISTA_CRUZ *lista) {
    if (lista_cruz_vazia(lista)) return;

    printf("=== VISUALIZAÇÃO POR LINHAS ===\n");
    CABECA_EIXO *lin = lista->linhas;
    while (lin != NULL) {
        printf("Linha [%s]:\n", lin->chave);
        NO_CRUZ *curr = lin->primeiro;
        while (curr != NULL) {
            printf("  -> Coluna [%s] | ITEM\n", curr->chave_coluna);
            curr = curr->prox_linha;
        }
        lin = lin->proximo;
    }
}