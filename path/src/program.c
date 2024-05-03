#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/aflib.h"

void simulacao();

int main() {
    while (1) {
        printf("---------Menu---------\n1 - Iniciar simulacao\n2 - Encerrar o programa");
        
        int opcao = 0;
        scanf("%d", &opcao);

        if (opcao == 2) {
            printf("Programa finalizado!");
            break;
        }
        else if (opcao == 1) {
            simulacao();
        }
        else {
            printf("Opcao nao identificada!");
        }
    }

    return 0;
}

void simulacao() {
    char estados[40][5];
    char alfabeto[20][1];

    //Leitura dos estados
    int opcao = 0;

    printf("1 - Criar novo estado");
    printf("2 - Próximo passo");
    while (opcao != 2) {
        break;
    }

}