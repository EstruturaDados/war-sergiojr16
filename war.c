#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// --- Banco de Missões ---
const char* LISTA_MISSOES[] = {
    "Dominar todo o mapa",             
    "Eliminar o exercito Azul",        
    "Conquistar o Territorio 1",     
    "Acumular 20 tropas totais",       
    "Ter um grande exercito (10+ em um local)" 
};
const int TOTAL_MISSOES = 5;

// --- Sumário das Funções ---
Territorio* alocarMapa(int qtd);
void inicializarTerritorios(Territorio* mapa, int qtd);
void liberarMemoria(Territorio* mapa, char* missao);
void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa, int qtd);
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int qtd, char* corJogador);
void exibirMissao(char* missao); 
void faseDeAtaque(Territorio* mapa, int qtd, char* corJogador);
void atacar(Territorio* atacante, Territorio* defensor);

// --- Função Principal ---
int main() {
    Territorio* mapa = NULL;
    char* missaoJogador = NULL;
    char corJogador[10];
    int qtdTerritorios;
    int opcao = 0;
    int vitoria = 0;

    srand(time(NULL));

    printf("=== WAR SIMULATOR: MISSÕES ESTRATÉGICAS ===\n");
    
    // 1. Definição do Jogador
    printf("Escolha a cor do seu exército (ex: Vermelho, Azul): ");
    scanf(" %[^\n]", corJogador);

    printf("Quantos territórios estarão em jogo? ");
    scanf("%d", &qtdTerritorios);

    // 2. Alocação Dinâmica (Mapa e Missão)
    mapa = alocarMapa(qtdTerritorios);
    missaoJogador = (char*) malloc(100 * sizeof(char)); // Aloca espaço para texto da missão

    if (mapa == NULL || missaoJogador == NULL) {
        printf("Erro crítico: Falha na alocação de memória.\n");
        return 1;
    }

    // 3. Inicialização e Sorteio
    inicializarTerritorios(mapa, qtdTerritorios);
    atribuirMissao(missaoJogador, LISTA_MISSOES, TOTAL_MISSOES);

    // Exibe a missão apenas uma vez no início
    exibirMissao(missaoJogador);

    // 4. Loop do Jogo
    do {
        exibirMapa(mapa, qtdTerritorios);
        
        // Verificação da Missão
        if (verificarMissao(missaoJogador, mapa, qtdTerritorios, corJogador)) {
            vitoria = 1;
            break; // Sai do loop se ganhou
        }

        exibirMenuPrincipal();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, qtdTerritorios, corJogador);
                break;
            case 2:
                printf("Você desistiu da guerra.\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
        
        // Limpeza de buffer
        printf("\nPressione Enter para continuar...");
        getchar(); getchar(); 
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

    } while (opcao != 2);

    // 5. Encerramento
    if (vitoria) {
        printf("\n============================================\n");
        printf("       PARABÉNS! MISSÃO CUMPRIDA!           \n");
        printf("   Objetivo: %s\n", missaoJogador);
        printf("   Vitória do Exército %s!\n", corJogador);
        printf("============================================\n");
    }

    liberarMemoria(mapa, missaoJogador);
    return 0;
}

// --- Implementação das Funções ---

Territorio* alocarMapa(int qtd) {
    return (Territorio*) calloc(qtd, sizeof(Territorio));
}

void inicializarTerritorios(Territorio* mapa, int qtd) {
    printf("\n--- CONFIGURAÇÃO DO MAPA ---\n");
    for (int i = 0; i < qtd; i++) {
        printf("Território %d -> Nome: ", i + 1);
        scanf(" %[^\n]", mapa[i].nome);
        printf("   Cor: ");
        scanf(" %[^\n]", mapa[i].cor);
        printf("   Tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

// Sorteia e copia a missão para a memória alocada do jogador
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes) {
    int r = rand() % totalMissoes;
    strcpy(destino, missoes[r]); 
}

// Exibe a missão (Interface)
void exibirMissao(char* missao) {
    printf("\n################################################\n");
    printf(" SUAS ORDENS, COMANDANTE: %s\n", missao);
    printf("################################################\n");
    printf("Pressione Enter para iniciar...");
    getchar(); getchar();
}

// Verificação de vitória
int verificarMissao(char* missao, Territorio* mapa, int qtd, char* corJogador) {
    // Lógica 1: Dominar todo o mapa
    if (strcmp(missao, "Dominar todo o mapa") == 0) {
        for (int i = 0; i < qtd; i++) {
            if (strcmp(mapa[i].cor, corJogador) != 0) return 0; // Se um não for meu, falha
        }
        return 1; // Sucesso
    }
    
    // Lógica 2: Eliminar exército Azul
    if (strcmp(missao, "Eliminar o exercito Azul") == 0) {
        // Se eu sou azul, essa missão é impossível (ou suicídio), mas segue a lógica:
        for (int i = 0; i < qtd; i++) {
            if (strcmp(mapa[i].cor, "Azul") == 0) return 0; // Ainda existe azul
        }
        return 1; // Não existe mais azul
    }

    // Lógica 3: Conquistar Território 1
    if (strcmp(missao, "Conquistar o Territorio 1") == 0) {
        if (qtd > 0 && strcmp(mapa[0].cor, corJogador) == 0) return 1;
        return 0;
    }

    // Lógica 4: Acumular tropas
    if (strcmp(missao, "Acumular 20 tropas totais") == 0) {
        int total = 0;
        for (int i = 0; i < qtd; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) total += mapa[i].tropas;
        }
        return (total >= 20);
    }

    // Lógica 5: Grande Exército
    if (strstr(missao, "10+") != NULL) { // Busca substring para identificar a missão
        for (int i = 0; i < qtd; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas >= 10) return 1;
        }
    }

    return 0;
}

void exibirMenuPrincipal() {
    printf("\n1. Atacar\n2. Sair\nEscolha: ");
}

void exibirMapa(const Territorio* mapa, int qtd) {
    printf("\n%-20s | %-10s | %s\n", "NOME", "COR", "TROPAS");
    printf("------------------------------------------\n");
    for (int i = 0; i < qtd; i++) {
        printf("%-20s | %-10s | %d\n", mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

void faseDeAtaque(Territorio* mapa, int qtd, char* corJogador) {
    int idAtk, idDef;
    printf("\nAtacante (ID): "); scanf("%d", &idAtk);
    printf("Defensor (ID): "); scanf("%d", &idDef);
    
    int idxAtk = idAtk - 1;
    int idxDef = idDef - 1;

    // Impedir que ataque a si mesmo
    if (idxAtk >= 0 && idxAtk < qtd && idxDef >= 0 && idxDef < qtd) {
        if (strcmp(mapa[idxAtk].cor, corJogador) != 0) {
            printf("Erro: Você só pode atacar com territórios da sua cor (%s)!\n", corJogador);
        } else {
            atacar(&mapa[idxAtk], &mapa[idxDef]);
        }
    } else {
        printf("IDs inválidos.\n");
    }
}

void atacar(Territorio* atacante, Territorio* defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Não pode atacar aliados!\n");
        return;
    }
    if (atacante->tropas <= 1) {
        printf("Tropas insuficientes (precisa > 1).\n");
        return;
    }

    int dAtk = (rand() % 6) + 1;
    int dDef = (rand() % 6) + 1;
    printf("\nBatalha! Dado Atk [%d] vs Def [%d] -> ", dAtk, dDef);

    if (dAtk > dDef) {
        printf("VITÓRIA!\n");
        strcpy(defensor->cor, atacante->cor);
        int movidas = atacante->tropas / 2;
        defensor->tropas = movidas;
        atacante->tropas -= movidas;
    } else {
        printf("DERROTA!\n");
        if (atacante->tropas > 0) atacante->tropas--;
    }
}

// Libera toda a memória dinâmica (Vetor de structs e String da missão)
void liberarMemoria(Territorio* mapa, char* missao) {
    if (mapa != NULL) free(mapa);
    if (missao != NULL) free(missao);
}