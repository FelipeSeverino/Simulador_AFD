#ifndef AFLIB_H
#define AFLIB_H

typedef struct transicao {
    char symbol;
    char q_from[5];
    char q_to[5];

    TRANSICAO next;
} TRANSICAO;


typedef struct estado {
    int final; // 0 = false, 1 = true
    char nome[5];

    ESTADO next;
} ESTADO;

typedef struct af {
    char estado_inicial[5];
    ESTADO estado;
    TRANSICAO transicao;
} AF;

#endif