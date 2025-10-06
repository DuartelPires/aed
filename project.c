/* iaed25 - ist1109342 - project */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vacinas.h"
#include "aplicacoes.h"
#include "data.h"

void print_erro(const char *msg, const char *msg_pt, int portugues) {
    printf("%s\n", portugues ? msg_pt : msg);
}

void print_data_atual(Data *data) {
    printf("%02d-%02d-%d\n", data->dia, data->mes, data->ano);
}

void atualizar_data(Data *data, int novo_dia, int novo_mes, int novo_ano, int portugues) {
    if (!verificar_data(novo_dia, novo_mes, novo_ano) || !verificar_data_maior_igual(data, novo_dia, novo_mes, novo_ano)) {
        print_erro("invalid date", "data inv\u00e1lida", portugues);
        return;
    }
    data->dia = novo_dia;
    data->mes = novo_mes;
    data->ano = novo_ano;
    print_data_atual(data);
}

void libertar_memoria() {
    Vacina **hash_vacinas = get_hash_vacinas();
    Vacina **hash_vacinas_nome = get_hash_vacinas_nome();
    Aplicacao **hash_aplicacoes = get_hash_aplicacoes();

    for (int i = 0; i < HASH_SIZE; i++) {
        Vacina *atual = hash_vacinas[i];
        while (atual) {
            Vacina *temp = atual;
            atual = atual->prox;
            free(temp);
        }
        hash_vacinas[i] = NULL;

        Vacina *atual_nome = hash_vacinas_nome[i];
        while (atual_nome) {
            Vacina *temp_nome = atual_nome;
            atual_nome = atual_nome->prox;
            free(temp_nome);
        }
        hash_vacinas_nome[i] = NULL;

        Aplicacao *atual_aplicacao = hash_aplicacoes[i];
        while (atual_aplicacao) {
            Aplicacao *temp_aplicacao = atual_aplicacao;
            atual_aplicacao = atual_aplicacao->prox;
            free(temp_aplicacao);
        }
        hash_aplicacoes[i] = NULL;
    }
}

int main(int argc, char *argv[]) {
  int portugues = 0;
  if(argc > 1 && strcmp(argv[1], "pt") == 0){
     portugues = 1;
  }
  Data data_atual = {1,1,2025};
  char comando;
  init_hash_table_vacinas();
  init_hash_table_aplicacoes();
  int aplicacoes=1;

  while (scanf(" %c", &comando) == 1) {
    if (comando == 'q') {
          break;
      }else if (comando == 'm') {
        char nome_original[201], novo_nome[201];
        if (scanf(" \"%200[^\"]\" \"%200[^\"]\"", nome_original, novo_nome) != 2) {
            if (scanf(" %200s %200s", nome_original, novo_nome) != 2) {
                printf("Input inválido\n");
                continue;
            }
        }
        alterar_nome_utente(nome_original, novo_nome, portugues);
    } else if (comando == 'c') {
          char lote[55], nome[100];
          int dia, mes, ano, doses;
          if (scanf(" %55s %d-%d-%d %d %100[^\n]", lote, &dia, &mes, &ano, &doses, nome) != 6) {
              continue;
          }
          adicionar_lote(lote, dia, mes, ano, doses, nome, &data_atual, portugues);
      } else if (comando == 'l') {
          char linha[500];
          char *filtro[50];
          int num_filtros = 0;
          if (fgets(linha, sizeof(linha), stdin)) {
            char *token = strtok(linha, " \n");
            while (token) {
              filtro[num_filtros++] = token;
              token = strtok(NULL, " \n");
            }
          }
          if (num_filtros == 0) {
            listar_vacinas_ordenadas();
          } else {
            listar_vacinas_filtro(filtro, num_filtros,portugues);
          }
      } else if (comando == 'a') {
          char utente[201], nome_vacina[51];
          if (scanf(" \"%200[^\"]\" %50s", utente, nome_vacina) != 2) {
            if (scanf(" %200s %50s", utente, nome_vacina) != 2) {
              printf("Input inválido\n");
            continue;
            }
          }
          aplicar_vacina(utente, nome_vacina, &data_atual, portugues, aplicacoes);
          aplicacoes++;
      }else if (comando == 'u') {
        char utente[201];
        char linha[500];

          if (fgets(linha, sizeof(linha), stdin)) {
              if (sscanf(linha, " \"%200[^\"]\"", utente) == 1) {
                  listar_aplicacoes(utente, portugues);
              }
              else if (sscanf(linha, " %200s", utente) == 1) {
                  listar_aplicacoes(utente, portugues);
              } else {
                  listar_todas_aplicacoes();
              }
          }      
        } else if (comando == 't') {
          char linha[50];
          int novo_dia, novo_mes, novo_ano;
          if (fgets(linha, sizeof(linha), stdin)) {
              if (sscanf(linha, "%d-%d-%d", &novo_dia, &novo_mes, &novo_ano) == 3) {
                  atualizar_data(&data_atual, novo_dia, novo_mes, novo_ano, portugues);
              } else {
                  print_data_atual(&data_atual);
              }
          } else {
              print_data_atual(&data_atual);
          }
      }else if (comando == 'r') {
        char lote[21];
        if (scanf(" %20s", lote) != 1) {
          printf("Invalid input\n");
        continue;
        }
        remover_vacina(lote, portugues);
      }else if (comando == 'd') {
        char utente[201];
        char data[15] = {0};
        char lote[21] = {0};

        if (scanf(" \"%200[^\"]\"", utente) != 1) {
            if (scanf(" %200s", utente) != 1) {
                printf("Input inválido\n");
                continue;
            }
        }

        char linha[500];
        if (fgets(linha, sizeof(linha), stdin)) {
            char *token = strtok(linha, " \n");
            if (token && sscanf(token, "%d-%d-%d", &data_atual.dia, &data_atual.mes, &data_atual.ano) == 3) {
                strcpy(data, token);
                token = strtok(NULL, " \n");
            }
            if (token) {
                strcpy(lote, token);
            }
        }

        if (strlen(data) == 0 && strlen(lote) == 0) {
            remove_aplicacoes(utente, NULL, NULL, &data_atual, portugues);
        } else if (strlen(data) > 0 && strlen(lote) == 0) {
            remove_aplicacoes(utente, data, NULL, &data_atual, portugues);
        } else if (strlen(data) > 0 && strlen(lote) > 0) {
            remove_aplicacoes(utente, data, lote, &data_atual, portugues);
        }
      }
    }
  libertar_memoria();
  return 0;
}


