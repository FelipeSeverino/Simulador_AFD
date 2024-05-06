#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/aflib.h"
#include "include/graphviz/gvc.h"
#include "include/graphviz/cgraph.h"
#include "include/graphviz/cdt.h"

void simulacao();
void gerarGrafo(AF *af);

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

    while (1) { //Funcao programa
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

    // printf("\n------------\n");
    // printf("Estados: ");
    // ESTADO *currentEstado = af->estado;
    // while (currentEstado != NULL) {
    //     printf("%s", currentEstado->nome);
    //     currentEstado = currentEstado->next;
    // }
    // printf("\n-\n");
    // printf("------------");
    
    printf("ANALISE PRELIMINAR: ");
    int tipoAF = classificaAf(af);

    if (tipoAF == 1) {printf("AFD");}
    else if (tipoAF == 2) {printf("AFN");}
    else if (tipoAF == 3) {printf("AFN com movimento vazios");}

    // gerarGrafo(af);

}


typedef struct estado_node_map {
    ESTADO *estado;
    Agnode_t *node;
    struct estado_node_map *next;
} ENMap;

typedef struct en_mapHeader {
    ENMap *head;
} ENMapHeader;

void mapearEstadoNode(ESTADO *estado, Agnode_t *node, ENMapHeader *header) {
    ENMap *map = (ENMap*) malloc(sizeof(ENMap));
    map->estado = estado;
    map->node = node;
    map->next = NULL;

    if (header->head == NULL) {
        header->head = map;
    }
    else {
        ENMap *currentMap = header->head;
        while (currentMap->next != NULL) {
            currentMap = currentMap->next;
        } 
        currentMap->next = map;
    }
}

void gerarGrafo(AF *af) {
    GVC_t *gvc;
    gvc = gvContext();

    // Create a new graph
    Agraph_t *graph;
    graph = agopen("graph", Agdirected, NULL);

    // Add nodes and edges to the graph
    Agnode_t *node1, *node2;
    node1 = agnode(graph, "node1", 1);
    node2 = agnode(graph, "node2", 1);

    ESTADO *currentEstado = af->estado;
    ENMapHeader *enMap = (ENMapHeader*) malloc(sizeof(ENMapHeader));
    while (currentEstado != NULL) {
        Agnode_t *node = agnode(graph, currentEstado->nome, 1);
        mapearEstadoNode(currentEstado, node, enMap);

        currentEstado = currentEstado->next;
    }
    
    Agedge_t *edge;
    edge = agedge(graph, node1, node2, NULL, 1);

    // Set label on the edge
    agsafeset(edge, "label", "Edge Label", "");

    // Layout the graph
    gvLayout(gvc, graph, "dot");

    // Render the graph to a PNG file
    gvRenderFilename(gvc, graph, "png", "output.png");

    // Cleanup
    gvFreeLayout(gvc, graph);
    agclose(graph);
    gvFreeContext(gvc);
}