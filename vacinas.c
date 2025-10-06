#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vacinas.h"

Vacina *hash_vacinas[HASH_SIZE];
Vacina *hash_vacinas_nome[HASH_SIZE];

void init_hash_table_vacinas() {
    for (int i = 0; i < HASH_SIZE; i++) {
        hash_vacinas[i] = NULL;
        hash_vacinas_nome[i] = NULL;
    }
}

int verificar_capacidade(int total_lotes) {
    return total_lotes >= MAX_LOTES;
}

unsigned int hash(const char *name) {
    unsigned int h = 0;
    while (*name) {
        h = (h * 31) + (*name++);
    }
    return h % HASH_SIZE;
}

Vacina **get_hash_vacinas() {
    return hash_vacinas;
}

Vacina **get_hash_vacinas_nome() {
    return hash_vacinas_nome;
}

int verificar_quantidade(int doses) {
    return doses > 0;
}

int verificar_duplicacao(char *lote) {
    unsigned int index = hash(lote);
    Vacina *atual = hash_vacinas[index];
    while (atual) {
        if (strcmp(atual->lote, lote) == 0) {
            return 1;
        }
        atual = atual->prox;
    }
    return 0;
}

int is_hexadecimal_uppercase(char c) {
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F');
}

int verificar_lote(char *lote) {
    if (strlen(lote) > 20) return 0;
    for (int i = 0; lote[i] != '\0'; i++) {
        if (!is_hexadecimal_uppercase(lote[i])) return 0;
    }
    return 1;
}

int verificar_data_maior(Data *data_atual, int dia, int mes, int ano) {
    if (ano > data_atual->ano) {
        return 1;
    }
    if (ano == data_atual->ano && mes > data_atual->mes) {
        return 1;
    }
    if (ano == data_atual->ano && mes == data_atual->mes && dia >= data_atual->dia) {
        return 1;
    }
    return 0;
}

int verificar_data_maior_igual(Data *data_atual, int dia, int mes, int ano) {
    if (ano > data_atual->ano) {
        return 1;
    }
    if (ano == data_atual->ano && mes > data_atual->mes) {
        return 1;
    }
    if (ano == data_atual->ano && mes == data_atual->mes && dia > data_atual->dia) {
        return 1;
    }
    if(ano == data_atual->ano && mes == data_atual->mes && dia == data_atual->dia){
      return 1;
    }
    return 0;
}

int verificar_data(int dia, int mes, int ano) {
    if (ano < 2025) return 0;

    if (mes < 1 || mes > 12) return 0;

    if (dia < 1 || dia > 31) return 0;
    if (mes == 2 && (dia > 29 || (dia > 28 && (ano % 4 != 0 || (ano % 100 == 0 && ano % 400 != 0))))) return 0;
    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) return 0;

    return 1; 
}

int verificar_nome(char *nome) {
    if (strlen(nome) > 50) return 0;
    for (int i = 0; nome[i] != '\0'; i++) {
        char c = nome[i];
        if ((c == ' ') || (c == '\t') || (c == '\n') ||
            ((c >= '!' && c <= '/') && c != '-') ||
            (c >= ':' && c <= '@') ||
            (c >= '[' && c <= '`' && c!= '-') ||
            (c >= '{' && c <= '~')) {
            return 0;
        }
    }
    return 1;
}

void adicionar_lote(char *lote, int dia, int mes, int ano, int doses, char *nome, Data *data_atual, int portugues) {
    static int total_lotes = 0;

    if (verificar_capacidade(total_lotes)) {
        print_erro("too many vaccines", "muitas vacinas", portugues);
        return;
    }

    Vacina *novo = (Vacina *)malloc(sizeof(Vacina));
    if (!novo) {
        print_erro("no memory", "sem memória", portugues);
        return;
    }
    
    if (verificar_duplicacao(lote)) {
        free(novo);
        print_erro("duplicate batch number", "número de lote duplicado", portugues);
        return;
    }

    if (!verificar_nome(nome)) {
        free(novo);
        print_erro("invalid character in vaccine name", "nome inválido", portugues);
        return;
    }

    if (!verificar_data(dia, mes, ano) || !verificar_data_maior(data_atual, dia, mes, ano)) {
        free(novo);
        print_erro("invalid date", "data inválida", portugues);
        return;
    }

    if (!verificar_lote(lote)) {
        free(novo);
        print_erro("invalid batch", "lote inválido", portugues);
        return;
    }

    if (!verificar_quantidade(doses)) {
        free(novo);
        print_erro("invalid quantity", "quantidade inválida", portugues);
        return;
    }

    strcpy(novo->nome, nome);
    strcpy(novo->lote, lote);
    novo->dia = dia;
    novo->mes = mes;
    novo->ano = ano;
    novo->doses = doses;
    novo->aplicacoes = 0;
    novo->prox = NULL;

    unsigned int index_lote = hash(lote);
    novo->prox = hash_vacinas[index_lote];
    hash_vacinas[index_lote] = novo;

    Vacina *copy = (Vacina *)malloc(sizeof(Vacina));
    if (!copy) {
        hash_vacinas[index_lote] = novo->prox;
        free(novo);
        print_erro("no memory", "sem memória", portugues);
        return;
    }

    *copy = *novo;
    copy->prox = NULL;

    unsigned int index_nome = hash(nome);
    copy->prox = hash_vacinas_nome[index_nome];
    hash_vacinas_nome[index_nome] = copy;

    total_lotes++;
    printf("%s\n", lote);
}

int comparar_vacinas(Vacina *va, Vacina *vb) {
    if (va->ano != vb->ano) return va->ano - vb->ano;
    if (va->mes != vb->mes) return va->mes - vb->mes;
    if (va->dia != vb->dia) return va->dia - vb->dia;
    return strcmp(va->lote, vb->lote);
}

void merge(Vacina **lista, int inicio, int meio, int fim) {
    int tamanho_esquerda = meio - inicio + 1;
    int tamanho_direita = fim - meio;

    Vacina **esquerda = malloc(tamanho_esquerda * sizeof(Vacina *));
    Vacina **direita = malloc(tamanho_direita * sizeof(Vacina *));

    for (int i = 0; i < tamanho_esquerda; i++)
        esquerda[i] = lista[inicio + i];
    for (int j = 0; j < tamanho_direita; j++)
        direita[j] = lista[meio + 1 + j];

    int i = 0, j = 0, k = inicio;

    while (i < tamanho_esquerda && j < tamanho_direita) {
        if (comparar_vacinas(esquerda[i], direita[j]) <= 0) {
            lista[k] = esquerda[i];
            i++;
        } else {
            lista[k] = direita[j];
            j++;
        }
        k++;
    }

    while (i < tamanho_esquerda) {
        lista[k] = esquerda[i];
        i++;
        k++;
    }
    while (j < tamanho_direita) {
        lista[k] = direita[j];
        j++;
        k++;
    }

    free(esquerda);
    free(direita);
}

void merge_sort(Vacina **lista, int inicio, int fim) {
    if (inicio < fim) {
        int meio = inicio + (fim - inicio) / 2;

        merge_sort(lista, inicio, meio);
        merge_sort(lista, meio + 1, fim);

        merge(lista, inicio, meio, fim);
    }
}

Vacina **coletar_vacinas_ordenadas(int *tamanho) {
    Vacina **lista = malloc(MAX_LOTES * sizeof(Vacina *));
    *tamanho = 0;

    for (int i = 0; i < HASH_SIZE; i++) {
        Vacina *atual = hash_vacinas[i];
        while (atual) {
            lista[(*tamanho)++] = atual;
            atual = atual->prox;
        }
    }

  merge_sort(lista, 0, *tamanho - 1); 
  return lista;
}

void listar_vacinas_ordenadas() {
    int tamanho;
    Vacina **lista = coletar_vacinas_ordenadas(&tamanho);

    for (int i = 0; i < tamanho; i++) {
      Vacina *v = lista[i];
      printf("%s %s %02d-%02d-%d %d %d\n", v->nome, v->lote, v->dia, v->mes, v->ano, v->doses, v->aplicacoes);
    }

    free(lista);
}

void listar_vacinas_filtro(char **filtro, int num_filtros, int portugues) {
    if (num_filtros == 0) {
        int encontrou = 0;
        for (int i = 0; i < HASH_SIZE; i++) {
            Vacina *atual = hash_vacinas[i];
            while (atual) {
                printf("%s %s %02d-%02d-%d %d %d\n", atual->nome, atual->lote, atual->dia,
                       atual->mes, atual->ano, atual->doses, atual->aplicacoes);
                encontrou = 1;
                atual = atual->prox;
            }
        }
        if (!encontrou) {
            print_erro("no such vaccine", "vacina inexistente" , portugues);
        }
        return;
    }

    for (int i = 0; i < num_filtros; i++) {
        int lote_encontrado = 0;
        for (int j = 0; j < HASH_SIZE; j++) {
            Vacina *atual = hash_vacinas[j];
            while (atual) {
                if (strcmp(atual->nome, filtro[i]) == 0) {
                    printf("%s %s %02d-%02d-%d %d %d\n", atual->nome, atual->lote, atual->dia,
                           atual->mes, atual->ano, atual->doses, atual->aplicacoes);
                    lote_encontrado = 1;
                }
                atual = atual->prox;
            }
        }
        if (!lote_encontrado) {
          printf("%s: %s\n", filtro[i], portugues ? "vacina inexistente" : "no such vaccine");
        }
    }
}


void remover_vacina(char *lote, int portugues) {
    unsigned int index = hash(lote);
    Vacina *atual = hash_vacinas[index];
    Vacina *anterior = NULL;

    while (atual) {
        if (strcmp(atual->lote, lote) == 0) {
            printf("%d\n", atual->aplicacoes);

            if (anterior) {
                anterior->prox = atual->prox;
            } else {
                hash_vacinas[index] = atual->prox;
            }

            unsigned int index_nome = hash(atual->nome);
            Vacina *atual_nome = hash_vacinas_nome[index_nome];
            Vacina *anterior_nome = NULL;

            while (atual_nome) {
                if (strcmp(atual_nome->lote, lote) == 0) {
                    if (anterior_nome) {
                        anterior_nome->prox = atual_nome->prox;
                    } else {
                        hash_vacinas_nome[index_nome] = atual_nome->prox;
                    }

                    free(atual_nome);
                    break;
                }
                anterior_nome = atual_nome;
                atual_nome = atual_nome->prox;
            }

            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }
    printf("%s: %s\n", lote, portugues ? "lote inexistente" : "no such batch");
}


void alterar_nome_utente(char *nome_original, char *novo_nome, int portugues) {
    Aplicacao **hash_aplicacoes = get_hash_aplicacoes();
    unsigned int index_original = hash(nome_original);
    unsigned int index_novo = hash(novo_nome);

    if (hash_aplicacoes[index_novo]) {
        printf("%s: %s\n", novo_nome, portugues ? "utente já existe" : "user already exists");
        return;
    }

    Aplicacao *atual = hash_aplicacoes[index_original];
    int encontrado = 0;

    while (atual) {
        if (strcmp(atual->utente, nome_original) == 0) {
            encontrado = 1;
            strcpy(atual->utente, novo_nome);
        }
        atual = atual->prox;
    }

    if (!encontrado) {
        printf("%s: %s\n", nome_original, portugues ? "utente inexistente" : "no such user");
        return;
    }

    printf("%s\n", novo_nome);
}

