#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/aflib.h"
#include "include/graphviz/gvc.h"
#include "include/graphviz/cgraph.h"
#include "include/graphviz/cdt.h"

void simulacao();
// void gerarGrafo(AF *af);
void generateAutomatonDot(AF *af, const char *filename);

int main() {
    while (1) {
        printf("---------Menu---------\n1 - Iniciar simulacao\n2 - Encerrar o programa\ncomando >> ");
        
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
    AF *af = criarAF();

    //Leitura dos estados
    int opcao = 0;
    int nEstados = 0;
    while (1) { //Criacao de estados
        printf("\n\n1 - Criar novo estado\n2 - Proximo passo\n");
        printf("comando >> ");
        scanf("%d", &opcao);
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        if (opcao == 2 && nEstados < 1) {
            printf("Crie ao menos um estado!");
            continue;
        }
        if (opcao == 2) {
            break;
        }

        char nome[5];
        printf("Digite o nome do estado: \n");
        fgets(nome, 5, stdin);

        int inicial = 0;
        int final = 0;

        if (af->estado_inicial == NULL) {
            printf("Eh inicial? (0 ou 1) ");
            scanf("%d", &inicial);

            if (inicial == 1) {
                af->estado_inicial = (char*) malloc(sizeof(char) * 5);
                strcpy(af->estado_inicial, nome);
            }
        }


        printf("Eh final? (0 ou 1) ");
        scanf("%d", &final);

        ESTADO *novoEstado = criarEstado(nome, inicial, final);

        inserirEstado(novoEstado, af);
        nEstados++;
    }

    while (1) { //Inpuit funcao programa
        printf("\n\n1 - Criar nova transicao\n2 - Proximo passo\n");
        printf("comando >> ");

        int opcao = 0;
        scanf("%d", &opcao);
        if (opcao == 2) {break;}

        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        //---
        printf("Do estado: ");
        char doEstado[5];
        fgets(doEstado, 5, stdin);

        int existe = 0;
        existe = existeEstado(doEstado, af);
        if (existe == 0) {
            printf("Estado invalido");
            continue;
        }
        //---

        printf("Para o estado: ");
        char paraEstado[5];
        fgets(paraEstado, 5, stdin);

        existe = 0;
        existe = existeEstado(paraEstado, af);
        if (existe == 0) {
            printf("Estado invalido");
            continue;
        }
        //--

        printf("Simbolo: ");
        char symbol = '\n';
        scanf("%c", &symbol);

        TRANSICAO *transicao = criarTransicao(symbol, doEstado, paraEstado);
        inserirTransicao(transicao, af);
    }
    
    printf("ANALISE PRELIMINAR: ");
    int tipoAF = classificaAf(af);

    if (tipoAF == 1) {printf("AFD\n");}
    else if (tipoAF == 2) {printf("AFN\n");}
    else if (tipoAF == 3) {printf("AFN com movimento vazios\n");}

    while (1) {
        printf("tamanho da palavra: ");
        int tamanho;
        if (scanf("%d", &tamanho) != 1) {
            printf("Input invalido\n");
            exit(1);
        }

        
        printf("Digite uma palavra: ");

        
        char *palavra = (char*)malloc(sizeof(char) * (tamanho + 1));
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        if (palavra == NULL) {
            printf("Erro mem alloc.\n");
            exit(1);
        }
        if (fgets(palavra, tamanho + 1, stdin) == NULL) {
            printf("Erro palavra.\n");
            free(palavra);
            exit(1);
        }
        palavra[strcspn(palavra, "\n")] = '\0';

        for (int i = 0; i <= tamanho; i++) {
            char *subword = (char*)malloc(sizeof(char) * (i + 1));
            if (subword == NULL) {
                printf("Erro mem alloc.\n");
                free(palavra);
                exit(1);
            }
            strncpy(subword, palavra, i);
            subword[i] = '\0';

            int aceita = aceitaPalavra(subword, i, af);

            if (i == 0) {
                free(subword);
                subword = strdup("(Palavra vazia)");
            }

            if (aceita == 1) {
                printf("%s => aceita\n", subword);
            } else if (aceita == 0) {
                printf("%s => rejeitada\n", subword);
            }

            free(subword);
        }

        free(palavra);

        int opcao = 0;
        printf("1 - Continuar\n2 - Finalizar Simulacao e gerar grafo visual\n");
        scanf("%d", &opcao);

        while ((c = getchar()) != '\n' && c != EOF);

        if (opcao == 2) {
            break;
        }

    }

    // gerarGrafo(af);
    generateAutomatonDot(af, "teste.dot");

}



void generateAutomatonDot(AF *af, const char *filename) {
    GVC_t *gvc;
    Agraph_t *graph;
    FILE *fp;

    gvc = gvContext();

    graph = agopen("automaton", Agdirected, NULL);

    ESTADO *currentState = af->estado;
    while (currentState != NULL) {
        Agnode_t *node = agnode(graph, currentState->nome, 1);
        if (currentState->final == 1) {
            agset(node, "shape", "doublecircle");
            agset(node, "color", "blue");
        } else {
            agset(node, "shape", "circle");
        }
        currentState = currentState->next;
    }

    TRANSICAO *currentTransition = af->transicao;
    while (currentTransition != NULL) {
        agedge(graph, agnode(graph, currentTransition->q_from, 0), agnode(graph, currentTransition->q_to, 0), NULL, 1);
        currentTransition = currentTransition->next;
    }

    Agnode_t *initialNode = agnode(graph, af->estado_inicial, 1);
    agset(initialNode, "shape", "none");

    gvLayout(gvc, graph, "dot");

    fp = fopen(filename, "w");
    agwrite(graph, fp);
    fclose(fp);

    gvRenderFilename(gvc, graph, "png", "automaton.png");

    gvFreeLayout(gvc, graph);
    agclose(graph);
    gvFreeContext(gvc);
}
