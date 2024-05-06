#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef AFLIB_H
#define AFLIB_H

typedef struct transicao {
    char symbol;
    char *q_from;
    char *q_to;

    struct transicao * next;
} TRANSICAO;

TRANSICAO* criarTransicao(char symbol, char *from, char *to) {
    TRANSICAO *novaTransicao = (TRANSICAO*) malloc(sizeof(TRANSICAO));
    novaTransicao->q_from = (char*) malloc(sizeof(char*) * 5);
    novaTransicao->q_to = (char*) malloc(sizeof(char*) * 5);

    if (novaTransicao == NULL) {
        printf("Erro ao criar transicao!! (Mem alloc)\n");
        exit(1);
    }

    novaTransicao->symbol = symbol;
    strcpy(novaTransicao->q_from, from);
    strcpy(novaTransicao->q_to, to);
    novaTransicao->next = NULL;

    return novaTransicao;
}


typedef struct estado {
    int inicial; // 0 = false, 1 = true
    int final; // 0 = false, 1 = true
    char *nome;

    struct estado * next;
} ESTADO;

ESTADO* criarEstado(char *nome, int inicial, int final) {
    ESTADO *novoEstado = (ESTADO*) malloc(sizeof(ESTADO));
    novoEstado->nome = (char*) malloc(sizeof(char) * 5);

    if (novoEstado == NULL) {
        printf("Erro ao criar estado!! (Mem alloc)\n");
        exit(1);
    }

    strcpy(novoEstado->nome, nome);
    novoEstado->inicial = inicial;
    novoEstado->final = final;
    novoEstado->next = NULL;

    return novoEstado;
}



typedef struct af {
    char *estado_inicial;
    ESTADO *estado;
    TRANSICAO *transicao;
} AF;

AF* criarAF() {
    AF *novoAF = (AF*) malloc(sizeof(AF));

    if (novoAF == NULL) {
        printf("Erro ao criar AF!! (Mem alloc)\n");
        exit(1);
    }

    novoAF->estado = NULL;
    novoAF->estado_inicial = NULL;
    novoAF->transicao = NULL;

    return novoAF;
}


int comparaTransicoes(const TRANSICAO *t1, const TRANSICAO *t2) {
    if (t1 == NULL || t2 == NULL) {
        return 0;
    }

    if (strcmp(t1->q_from, t2->q_from) == 0 && t1->symbol == t2->symbol) {
        if (strcmp(t1->q_to, t2->q_to) != 0) {
            return 1;
        }
    }
    return 0;
}

int classificaAf(AF *af) { //1 = AFD, 2 = AFN, 3 = AFN vazio
    TRANSICAO *transicoes = af->transicao;

    TRANSICAO *t1 = transicoes;
    TRANSICAO *t2;
    int class = 1;

    while (t1 != NULL) {
        if (t1->symbol == '$') {return 3;}

        t2 = t1->next;
        while (t2 != NULL) {
            if (comparaTransicoes(t1, t2)) {
                class = 2;
            }
            t2 = t2->next;
        }
        t1 = t1->next;
    }

    return class;
}


void inserirEstado(ESTADO *estado, AF *af) {
    if (af == NULL) {
        printf("AF invalido!! \n");
        return;
    }

    if (af->estado == NULL) {
        af->estado = estado;
    }
    else {
        ESTADO *currentEstado = af->estado;
        while (currentEstado->next != NULL) {
            currentEstado = currentEstado->next;
        }
        currentEstado->next = estado;
    }
}

void inserirTransicao(TRANSICAO *transicao, AF *af) {
    if (af == NULL) {
        printf("AF invalido!! \n");
        return;
    }

    if (af->transicao == NULL) {
        af->transicao = transicao;
    }
    else {
        TRANSICAO *currentTransicao = af->transicao;
        while (currentTransicao->next != NULL) {
            currentTransicao = currentTransicao->next;
        }
        currentTransicao->next = transicao;
    }
}

int existeEstado(char *estado, AF *af) {
    if (af == NULL) {
        return 0;
    }

    ESTADO *currentState = af->estado;

    while (currentState != NULL) {
        if (strcmp(currentState->nome, estado) == 0) {
            return 1;
        }

        currentState = currentState->next;
    }

    return 0;
}

#endif