#ifndef PROJECT_H
#define PROJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vacinas.h"
#include "aplicacoes.h"
#include "data.h"

/**
 * @brief Exibe uma mensagem de erro na língua apropriada.
 *
 * @param msg Mensagem em inglês.
 * @param msg_pt Mensagem em português.
 * @param portugues Indica se o idioma deve ser português (1) ou inglês (0).
 */
void print_erro(const char *msg, const char *msg_pt, int portugues);

/**
 * @brief Imprime a data atual no formato DD-MM-YYYY.
 *
 * @param data Ponteiro para a estrutura Data contendo a data atual.
 */
void print_data_atual(Data *data);

/**
 * @brief Atualiza a data atual do sistema.
 *
 * @param data Ponteiro para a estrutura Data a ser atualizada.
 * @param novo_dia Novo dia a ser definido.
 * @param novo_mes Novo mês a ser definido.
 * @param novo_ano Novo ano a ser definido.
 * @param portugues Indica se as mensagens devem ser exibidas em português (1) ou inglês (0).
 */
void atualizar_data(Data *data, int novo_dia, int novo_mes, int novo_ano, int portugues);

/**
 * @brief Liberta toda a memória alocada dinamicamente.
 */
void libertar_memoria();

#endif
