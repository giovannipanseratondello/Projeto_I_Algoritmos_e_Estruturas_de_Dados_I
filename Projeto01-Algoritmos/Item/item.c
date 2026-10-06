#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "item.h"

struct item{
    // Identificação e texto
    char designation[64];            // Designação oficial (ex: "2020 BX12")
    char fullname[128];              // Nome completo
    char close_approach_date[32];    // Data de aproximação formatada em texto

    // Métricas e datas
    int year;                        // Ano do evento
    int month;                       // Mês do evento
    double distance_au;              // Distância em Unidades Astronômicas
    double velocity_km_s;            // Velocidade em km/s
    double absolute_magnitude;       // Magnitude absoluta (H)
    long days_until_approach;        // Contagem regressiva em dias

    // Flags de tempo
    bool is_past_event;              // Já passou?
    bool is_future_event;            // Ainda vai acontecer?

    // Avaliação de risco e pânico
    double risk_score;               // Nota de risco calculada
    int panic_level;                 // Nível de alerta/pânico
    char threat_category[64];        // Categoria de ameaça (ex: "MONITOR", "CRITICAL")
    char panic_verdict[256];         // Descrição do veredito

    // Dados opcionais do programa Sentry
    bool on_sentry_list;             // Está na lista de monitoramento?
    double sentry_impact_prob;       // Probabilidade de impacto
    double sentry_torino_scale;      // Escala Torino
    double sentry_palermo_scale;     // Escala Palermo
    double sentry_diameter_km;       // Diâmetro estimado (km)

    // Campo auxiliar para as buscas
    double calculated_similarity;    // Pontuação final calculada pelas consultas (Query)
};

ITEM *item_criar(void) {
    ITEM *item = (ITEM*) malloc(sizeof(ITEM));
    if (item == NULL) {
        return NULL;
    }

    // Inicializa strings vazias
    item->designation[0] = '\0';
    item->fullname[0] = '\0';
    item->close_approach_date[0] = '\0';
    item->threat_category[0] = '\0';
    item->panic_verdict[0] = '\0';

    // Inicializa valores numéricos e booleanos
    item->year = 0;
    item->month = 0;
    item->distance_au = 0.0;
    item->velocity_km_s = 0.0;
    item->absolute_magnitude = 0.0;
    item->days_until_approach = 0;

    item->is_past_event = false;
    item->is_future_event = false;

    item->risk_score = 0.0;
    item->panic_level = 0;

    item->on_sentry_list = false;
    item->sentry_impact_prob = 0.0;
    item->sentry_torino_scale = 0.0;
    item->sentry_palermo_scale = 0.0;
    item->sentry_diameter_km = 0.0;

    item->calculated_similarity = 0.0;

    return item;
}

void item_apagar(ITEM **item){
    if ( item == NULL || *item == NULL ){
        return;
    }
    free(*item);
    *item = NULL;
}

void item_imprimir_csv(ITEM *item, FILE *saida) {
    if (item == NULL || saida == NULL) return;
    fprintf(saida, "%s,%s,%s,%d,%d,%.6f,%.6f,%.6f,%ld,%s,%s,%.6f,%d,%s,%s,%s,%.6f,%.6f,%.6f,%.6f\n",
        item->designation,
        item->fullname,
        item->close_approach_date,
        item->year,
        item->month,
        item->distance_au,
        item->velocity_km_s,
        item->absolute_magnitude,
        item->days_until_approach,
        item->is_past_event ? "true" : "false",
        item->is_future_event ? "true" : "false",
        item->risk_score,
        item->panic_level,
        item->threat_category,
        item->panic_verdict,
        item->on_sentry_list ? "true" : "false",
        item->sentry_impact_prob,
        item->sentry_torino_scale,
        item->sentry_palermo_scale,
        item->sentry_diameter_km
    );
}

int item_comparar(ITEM *item_A, ITEM *item_B, char *campo){
    if (item_A == NULL || item_B == NULL || campo == NULL){
        return 0;
    }
    if (strcmp(campo, "calculated_similarity") == 0 || strcmp(campo, "SIM") == 0) {
        if (item_A->calculated_similarity < item_B->calculated_similarity) return -1;
        if (item_A->calculated_similarity > item_B->calculated_similarity) return 1;
        return 0;
    }
    if (strcmp(campo, "risk_score") == 0) {
        if (item_A->risk_score < item_B->risk_score) return -1;
        if (item_A->risk_score > item_B->risk_score) return 1;
        return 0;
    }
    if (strcmp(campo, "distance_au") == 0) {
        if (item_A->distance_au < item_B->distance_au) return -1;
        if (item_A->distance_au > item_B->distance_au) return 1;
        return 0;
    }
    if (strcmp(campo, "velocity_km_s") == 0) {
        if (item_A->velocity_km_s < item_B->velocity_km_s) return -1;
        if (item_A->velocity_km_s > item_B->velocity_km_s) return 1;
        return 0;
    }

    // --- Comparação por Campos Inteiros ---
    if (strcmp(campo, "year") == 0) {
        return item_A->year - item_B->year;
    }
    if (strcmp(campo, "month") == 0) {
        return item_A->month - item_B->month;
    }
    if (strcmp(campo, "panic_level") == 0) {
        return item_A->panic_level - item_B->panic_level;
    }

    // --- Comparação por Campos de Texto (String) ---
    if (strcmp(campo, "designation") == 0) {
        return strcmp(item_A->designation, item_B->designation);
    }
    if (strcmp(campo, "threat_category") == 0) {
        return strcmp(item_A->threat_category, item_B->threat_category);
    }

    return 0;

}
