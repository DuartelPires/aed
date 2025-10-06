#ifndef APLICACOES_H
#define APLICACOES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vacinas.h"
#include "data.h"

#define HASH_SIZE 1031

typedef struct Aplicacao {
    char utente[201];
    char nome_vacina[51];
    char lote[21];
    int dia, mes, ano;
    int aplicacao;
    struct Aplicacao *prox;
} Aplicacao;

void mostar_aplicacoes_ordem_cronologica(int aplicacoes);


/**
 * @brief Inicia a hash table das aplicações.
 */
void init_hash_table_aplicacoes();

/**
 * @brief Retorna a hash table das aplicações.
 */
Aplicacao **get_hash_aplicacoes();

/**
 * @brief Compara duas aplicações da vacina.
 *
 * @param a Aplicação 1.
 * @param b Aplicação 2.
 * @return Valor negativo se a < b, 0 se a == b, valor positivo se a > b.
 */
int comparar_aplicacoes(Aplicacao *a, Aplicacao *b);

/**
 * @brief Aplica uma vacina a um utente.
 *
 * @param utente Nome do utente.
 * @param nome_vacina Nome da vacina.
 * @param data_atual Data atual.
 * @param portugues Define se as mensagens devem ser exibidas em português (1) ou inglês (0).
 */
void aplicar_vacina(char *utente, char *nome_vacina, Data *data_atual, int portugues, int aplicacoes);

/**
 * @brief Lista todas as aplicações da vacina num determinado utente.
 *
 * @param utente Nome do utente.
 * @param portugues Define se as mensagens devem ser exibidas em português (1) ou inglês (0).
 */
void listar_aplicacoes(char *utente, int portugues);

/**
 * @brief Lista todas as aplicações registadas.
 */
void listar_todas_aplicacoes();

/**
 * @brief Verifica se um utente já foi vacinado com uma determinada vacina na data atual.
 *
 * @param utente Nome do utente.
 * @param vacina Nome da vacina.
 * @param data_atual Data atual.
 * @return 1 se o utente já foi vacinado, 0 caso contrário.
 */
int ja_vacinado(const char *utente, const char *vacina, Data *data_atual);

/**
 * @brief Remove aplicações de vacina de um utente, opcionalmente filtrando por data e lote.
 *
 * @param utente Nome do utente.
 * @param data_aplicacao Data da aplicação no formato "DD-MM-YYYY" (opcional).
 * @param lote Código do lote (opcional).
 * @param data_atual Data atual.
 * @param portugues Define se s mensagens devem ser exibidas em português (1) ou inglês (0).
 */
void remove_aplicacoes(const char *utente, const char *data_aplicacao, const char *lote, Data *data_atual, int portugues);

#endif
