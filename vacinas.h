#ifndef VACINAS_H
#define VACINAS_H

#define MAX_LOTES 1000
#define HASH_SIZE 1031

#include "vacinas.h"
#include "aplicacoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "project.h"

/**
 * @brief Estrutura que representa uma vacina.
 */
typedef struct Vacina {
    char nome[51];  ///< Nome da vacina.
    char lote[21];  ///< Código do lote da vacina.
    int dia, mes, ano;  ///< Data de validade da vacina.
    int doses;  ///< Número total de doses disponíveis.
    int aplicacoes;  ///< Número de doses aplicadas.
    struct Vacina *prox;  ///< Ponteiro para a próxima vacina na tabela hash.
} Vacina;

/**
 * @brief Inicia as hash tables das vacinas.
 */
void init_hash_table_vacinas();

/**
 * @brief Retorna a hash table das vacinas por lote.
 * @return Ponteiro para a tabela hash de vacinas por lote.
 */
Vacina **get_hash_vacinas();

void alterar_nome_utente(char *nome_original, char *novo_nome, int portugues);

/**
 * @brief Retorna a hash table das vacinas por nome.
 * @return Ponteiro para a tabela hash de vacinas por nome.
 */
Vacina **get_hash_vacinas_nome();

/**
 * @brief Verifica se a capacidade máxima de lotes foi atingida.
 * @param total_lotes Número total de lotes cadastrados.
 * @return 1 se a capacidade foi atingida, 0 caso contrário.
 */
int verificar_capacidade(int total_lotes);

/**
 * @brief Gera um índice de hash a partir de um nome.
 * @param name Nome a ser usado na geração do hash.
 * @return Índice na tabela hash.
 */
unsigned int hash(const char *name);

/**
 * @brief Verifica se uma data é válida.
 * @param dia Dia da data.
 * @param mes Mês da data.
 * @param ano Ano da data.
 * @return 1 se a data for válida, 0 caso contrário.
 */
int verificar_data(int dia, int mes, int ano);

/**
 * @brief Verifica se a quantidade de doses é válida.
 * @param doses Número de doses.
 * @return 1 se for válido, 0 caso contrário.
 */
int verificar_quantidade(int doses);

/**
 * @brief Verifica se um lote já está registado.
 * @param lote Código do lote a ser verificado.
 * @return 1 se já existir, 0 caso contrário.
 */
int verificar_duplicacao(char *lote);

/**
 * @brief Verifica se é um dígito hexadecimal maiúsculo.
 * @param c Caractere a ser verificado.
 * @return 1 se for um dígito hexadecimal maiúsculo, 0 caso contrário.
 */
int is_hexadecimal_uppercase(char c);

/**
 * @brief Verifica se um lote é válido.
 * @param lote Código do lote a ser verificado.
 * @return 1 se for válido, 0 caso contrário.
 */
int verificar_lote(char *lote);

/**
 * @brief Verifica se uma data fornecida é maior que a data atual.
 * @param data_atual Ponteiro para a estrutura de data atual.
 * @param dia Dia da data a ser comparada.
 * @param mes Mês da data a ser comparada.
 * @param ano Ano da data a ser comparada.
 * @return 1 se a data fornecida for maior que a data atual, 0 caso contrário.
 */
int verificar_data_maior(Data *data_atual, int dia, int mes, int ano);

/**
 * @brief Verifica se uma data fornecida é maior ou igual à data atual.
 * @param data_atual Ponteiro para a estrutura de data atual.
 * @param dia Dia da data a ser comparada.
 * @param mes Mês da data a ser comparada.
 * @param ano Ano da data a ser comparada.
 * @return 1 se a data fornecida for maior ou igual à data atual, 0 caso contrário.
 */
int verificar_data_maior_igual(Data *data_atual, int dia, int mes, int ano);

/**
 * @brief Verifica se um nome da vacina é válido.
 * @param nome Nome da vacina.
 * @return 1 se o nome for válido, 0 caso contrário.
 */
int verificar_nome(char *nome);

/**
 * @brief Adiciona um novo lote de vacinas ao sistema.
 * @param lote Código do lote.
 * @param dia Dia da validade.
 * @param mes Mês da validade.
 * @param ano Ano da validade.
 * @param doses Número de doses disponíveis.
 * @param nome Nome da vacina.
 * @param data_atual Data atual para verificação.
 * @param portugues Define se as mensagens devem estar em português.
 */
void adicionar_lote(char *lote, int dia, int mes, int ano, int doses, char *nome, Data *data_atual, int portugues);

/**
 * @brief Compara duas vacinas com base na data de validade e no código do lote.
 * @param va Ponteiro para a primeira vacina.
 * @param vb Ponteiro para a segunda vacina.
 * @return Valor negativo se va for menor que vb, positivo se maior, 0 se forem iguais.
 */
int comparar_vacinas(Vacina *va, Vacina *vb);

/**
 * @brief Função auxiliar para a ordenação das vacinas usando Merge Sort.
 * @param lista Lista de vacinas.
 * @param inicio Índice inicial.
 * @param meio Índice do meio.
 * @param fim Índice final.
 */
void merge(Vacina **lista, int inicio, int meio, int fim);

/**
 * @brief Implementa o algoritmo de ordenação Merge Sort para ordenar vacinas.
 * @param lista Lista de vacinas.
 * @param inicio Índice inicial.
 * @param fim Índice final.
 */
void merge_sort(Vacina **lista, int inicio, int fim);

/**
 * @brief Coleta todas as vacinas guardadas e ordena as por data de validade e lote.
 * @param tamanho Ponteiro para armazenar o número total de vacinas coletadas.
 * @return Um array de ponteiros para vacinas ordenadas.
 */
Vacina **coletar_vacinas_ordenadas(int *tamanho);

/**
 * @brief Lista todas as vacinas guardadas, ordenadas por validade.
 */
void listar_vacinas_ordenadas();

/**
 * @brief Lista todas as vacinas guardadas, aplicando filtros se necessário.
 * @param filtro Array de strings contendo os filtros de nome de vacina.
 * @param num_filtros Número de filtros aplicados.
 * @param portugues Define se as mensagens devem estar em português.
 */
void listar_vacinas_filtro(char **filtro, int num_filtros, int portugues);

/**
 * @brief Remove uma vacina do sistema com base no código do lote.
 * @param lote Código do lote da vacina a ser removida.
 * @param portugues Define se as mensagens devem estar em português.
 */
void remover_vacina(char *lote, int portugues);

#endif
