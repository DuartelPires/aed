#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aplicacoes.h"

Aplicacao *hash_aplicacoes[HASH_SIZE];

void init_hash_table_aplicacoes() {
    for (int i = 0; i < HASH_SIZE; i++) {
        hash_aplicacoes[i] = NULL;
    }
}

Aplicacao **get_hash_aplicacoes() {
    return hash_aplicacoes;
}

int comparar_aplicacoes(Aplicacao *a, Aplicacao *b) {
    return strcmp(a->nome_vacina, b->nome_vacina);
}

void aplicar_vacina(char *utente, char *nome_vacina, Data *data_atual, int portugues,int aplicacoes) {
    Vacina **hash_vacinas_nome = get_hash_vacinas_nome();
    Vacina **hash_vacinas = get_hash_vacinas();

    if (ja_vacinado(utente, nome_vacina, data_atual)) {
        print_erro("already vaccinated","já vacinado", portugues);
        return;
    }

    unsigned int index = hash(nome_vacina);
    Vacina *vacina = hash_vacinas_nome[index];
    Vacina *melhor_vacina = NULL;

    while (vacina) {
        if (strcmp(vacina->nome, nome_vacina) == 0 &&
            vacina->doses > 0 &&
            verificar_data_maior_igual(data_atual, vacina->dia, vacina->mes, vacina->ano)) {
            if (!melhor_vacina ||
                (vacina->ano < melhor_vacina->ano) ||
                (vacina->ano == melhor_vacina->ano && vacina->mes < melhor_vacina->mes) ||
                (vacina->ano == melhor_vacina->ano && vacina->mes == melhor_vacina->mes && vacina->dia < melhor_vacina->dia)) {
                melhor_vacina = vacina;
            }
        }
        vacina = vacina->prox;
    }

    if (!melhor_vacina) {
        print_erro("no stock", "esgotado", portugues);
        return;
    }

    unsigned int index_lote = hash(melhor_vacina->lote);
    Vacina *vacina_lote = hash_vacinas[index_lote];

    while (vacina_lote) {
        if (strcmp(vacina_lote->lote, melhor_vacina->lote) == 0) {
            vacina_lote->doses--;
            vacina_lote->aplicacoes++;
            break;
        }
        vacina_lote = vacina_lote->prox;
    }

    melhor_vacina->doses--;
    melhor_vacina->aplicacoes++;

    unsigned int index_utente = hash(utente);
    Aplicacao *nova = (Aplicacao *)malloc(sizeof(Aplicacao));
    if (!nova) {
        print_erro("no memory", "sem memória", portugues);
        return;
    }

    strcpy(nova->utente, utente);
    strcpy(nova->nome_vacina, nome_vacina);
    strcpy(nova->lote, melhor_vacina->lote);
    nova->dia = data_atual->dia;
    nova->mes = data_atual->mes;
    nova->ano = data_atual->ano;
    nova->aplicacao = aplicacoes;
    nova->prox = hash_aplicacoes[index_utente];
    hash_aplicacoes[index_utente] = nova;

    printf("%s\n", melhor_vacina->lote); 
}

void listar_aplicacoes(char *utente, int portugues) {
    unsigned int index = hash(utente);
    Aplicacao *atual = hash_aplicacoes[index];
    int numEncontrados = 0;

    Aplicacao *aplicacoes[100];
    while (atual) {
        if (strcmp(atual->utente, utente) == 0) {
            aplicacoes[numEncontrados++] = atual;
        }
        atual = atual->prox;
    }

    if (numEncontrados == 0) {
        printf("%s: %s\n", utente, portugues ? "utente inexistente" : "no such user");
        return;
    }

    for (int i = numEncontrados - 1; i >= 0; i--) {
        printf("%s %s %02d-%02d-%d\n", aplicacoes[i]->utente, aplicacoes[i]->lote,
               aplicacoes[i]->dia, aplicacoes[i]->mes, aplicacoes[i]->ano);
    }
}
/*
void listar_todas_aplicacoes() {
    Aplicacao **aplicacoes = malloc(MAX_LOTES * sizeof(Aplicacao *));
    int total_aplicacoes = 0;

    for (int i = 0; i < HASH_SIZE; i++) {
        Aplicacao *atual = hash_aplicacoes[i];
        while (atual) {
            aplicacoes[total_aplicacoes++] = atual;
            atual = atual->prox;
        }
    }

    for (int i = 0; i < total_aplicacoes; i++) {
        printf("%s %s %02d-%02d-%d\n", aplicacoes[i]->utente, aplicacoes[i]->lote,
               aplicacoes[i]->dia, aplicacoes[i]->mes, aplicacoes[i]->ano);
    }

    free(aplicacoes);
}

*/
int ja_vacinado(const char *utente, const char *vacina, Data *data_atual) {
    unsigned int index = hash(utente); 
    Aplicacao *atual = hash_aplicacoes[index];

    while (atual) {
        if (strcmp(atual->utente, utente) == 0 &&
            strcmp(atual->nome_vacina, vacina) == 0 &&
            atual->dia == data_atual->dia &&
            atual->mes == data_atual->mes &&
            atual->ano == data_atual->ano) {
            return 1;
        }
        atual = atual->prox;
    }
    return 0;
}

void remove_aplicacoes(const char *utente, const char *data_aplicacao, const char *lote, Data *data_atual, int portugues) {
    int dia, mes, ano;

    if (data_aplicacao != NULL && sscanf(data_aplicacao, "%d-%d-%d", &dia, &mes, &ano) == 3) {
        if (!verificar_data(dia, mes, ano)) {
            printf("%s\n", portugues ? "data inválida" : "invalid date");
            return;
        }

        if (!verificar_data_maior_igual(data_atual, dia, mes, ano)) {
            printf("%s\n", portugues ? "data inválida" : "invalid date");
            return;
        }
    }

    unsigned int index_utente = hash(utente);
    Aplicacao *atual = hash_aplicacoes[index_utente];
    Aplicacao *anterior = NULL;
    int aplicacoes_removidas = 0;
    int aplicacoes_na_data = 0;

    while (atual) {
        int deve_remover = 0;

        if (strcmp(atual->utente, utente) == 0) {
            if (data_aplicacao == NULL) {
                deve_remover = 1;
            } else if (atual->dia == dia && atual->mes == mes && atual->ano == ano) {
                aplicacoes_na_data = 1;

                if (lote == NULL || strcmp(atual->lote, lote) == 0) {
                    deve_remover = 1;
                }
            }
        }

        if (deve_remover) {
            aplicacoes_removidas++;

            if (anterior) {
                anterior->prox = atual->prox;
            } else {
                hash_aplicacoes[index_utente] = atual->prox;
            }

            Aplicacao *temp = atual;
            atual = atual->prox;
            free(temp);
        } else {
            anterior = atual;
            atual = atual->prox;
        }
    }

    if (data_aplicacao != NULL && aplicacoes_na_data == 0) {
        printf("%s\n", portugues ? "data inválida" : "invalid date");
        return;
    }

    if (aplicacoes_removidas == 0) {
        if (data_aplicacao != NULL && lote != NULL) {
            printf("%s: %s\n", lote, portugues ? "lote inexistente" : "no such batch");
        } else {
            printf("%s: %s\n", utente, portugues ? "utente inexistente" : "no such user");
        }
    } else {
        printf("%d\n", aplicacoes_removidas);
    }
}


void mostar_aplicacoes_ordem_cronologica(int aplicacoes){
  int existe = 0;
  int tamanho = 0;
  Aplicacao **lista = malloc(MAX_LOTES * sizeof(Aplicacao*));
  for(int i = 0; i < aplicacoes; i++){ 
    for(int i = 0; i < MAX_LOTES; i++){
      Aplicacao *atual = hash_aplicacoes[i];

      while(atual){
        if( atual->aplicacao == aplicacoes){
          lista[tamanho] = atual;
          tamanho++;
          existe = 1;
        }
        atual = atual->prox;
      }
    }
  }

  if(!existe){
    printf("no data\n");
  }

  for(int i = tamanho; i > 0; i--){
    printf("%s %s %02d-%02d-%d\n");
  }

  free(lista);
}

void listar_todas_aplicacoes() {
    Aplicacao **aplicacoes = malloc(MAX_LOTES * sizeof(Aplicacao *));
    int total_aplicacoes = 0;

    for (int i = 0; i < HASH_SIZE; i++) {
        Aplicacao *atual = hash_aplicacoes[i];
        while (atual) {
            aplicacoes[total_aplicacoes++] = atual;
            atual = atual->prox;
        }
    }

    for (int i = 0; i < total_aplicacoes - 1; i++) {
        for (int j = i + 1; j < total_aplicacoes; j++) {
            if (aplicacoes[i]->aplicacao < aplicacoes[j]->aplicacao) { // Now sorting in reverse order
                Aplicacao *temp = aplicacoes[i];
                aplicacoes[i] = aplicacoes[j];
                aplicacoes[j] = temp;
            }
        }
    }

    for (int i = 0; i < total_aplicacoes; i++) {
        printf("%s %s %02d-%02d-%d\n", aplicacoes[i]->utente, aplicacoes[i]->lote,
               aplicacoes[i]->dia, aplicacoes[i]->mes, aplicacoes[i]->ano);
    }

    free(aplicacoes);
}


