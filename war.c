#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// --- Sumário das Funções ---
Territorio* alocarMapa(int qtd);
void inicializarTerritorios(Territorio* mapa, int qtd);
void liberarMemoria(Territorio* mapa);
void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa, int qtd);
void faseDeAtaque(Territorio* mapa, int qtd);
void simularAtaque(Territorio* atacante, Territorio* defensor);

// --- Função Principal ---
int main() {
    Territorio* mapa = NULL;
    int qtdTerritorios;
    int opcao = 0;

    srand(time(NULL));

    printf("=== CONFIGURAÇÃO DO MAPA ===\n");
    printf("Quantos territórios estarão em jogo? ");
    scanf("%d", &qtdTerritorios);

    mapa = alocarMapa(qtdTerritorios);
    if (mapa == NULL) {
        printf("Erro crítico: Falha na alocação de memória.\n");
        return 1;
    }

    // 2. Cadastro Inicial
    inicializarTerritorios(mapa, qtdTerritorios);

    // 3. Loop do Jogo
    do {
        exibirMapa(mapa, qtdTerritorios);
        exibirMenuPrincipal();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, qtdTerritorios);
                break;
            case 2:
                printf("Encerrando a simulação...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
        
        printf("\nPressione Enter para continuar...");
        getchar(); getchar(); 

    } while (opcao != 2);

    // 4. Liberação de Memória
    liberarMemoria(mapa);
    printf("Memória liberada. Jogo finalizado com sucesso.\n");

    return 0;
}

// --- Implementação das Funções ---

// Aloca dinamicamente a memória para o vetor de territórios usando calloc
Territorio* alocarMapa(int qtd) {
    // calloc inicializa a memória com zero, o que é mais seguro
    Territorio* t = (Territorio*) calloc(qtd, sizeof(Territorio));
    return t;
}

// Preenche os dados iniciais de cada território
void inicializarTerritorios(Territorio* mapa, int qtd) {
    printf("\n--- CADASTRO DOS TERRITÓRIOS ---\n");
    for (int i = 0; i < qtd; i++) {
        printf("Território %d:\n", i + 1);
        
        printf(" Nome: ");
        scanf(" %[^\n]", mapa[i].nome); 
        
        printf(" Cor do Exército: ");
        scanf(" %[^\n]", mapa[i].cor);
        
        printf(" Quantidade de Tropas: ");
        scanf("%d", &mapa[i].tropas);
        printf("-----------------------------\n");
    }
}

// Liberar memória
void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
    }
}

// Imprime o menu de ações
void exibirMenuPrincipal() {
    printf("\n=== MENU DE AÇÕES ===\n");
    printf("1. Atacar Território\n");
    printf("2. Sair do Jogo\n");
    printf("Escolha uma opção: ");
}

// Mostra o estado atual de todos os territórios 
void exibirMapa(const Territorio* mapa, int qtd) {
    printf("\n%-4s | %-20s | %-10s | %s\n", "ID", "NOME", "COR", "TROPAS");
    printf("------------------------------------------------------\n");
    for (int i = 0; i < qtd; i++) {
        printf("%-4d | %-20s | %-10s | %d\n", 
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("------------------------------------------------------\n");
}

// Gerencia a interface de ataque
void faseDeAtaque(Territorio* mapa, int qtd) {
    int idAtk, idDef;

    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Informe o ID do território ATACANTE: ");
    scanf("%d", &idAtk);
    
    printf("Informe o ID do território DEFENSOR: ");
    scanf("%d", &idDef);

    // Validação de índices (ajuste de 1-based para 0-based)
    int idxAtk = idAtk - 1;
    int idxDef = idDef - 1;

    if (idxAtk < 0 || idxAtk >= qtd || idxDef < 0 || idxDef >= qtd) {
        printf("Erro: IDs de território inválidos!\n");
        return;
    }

    // Passagem por referência (ponteiros) para a função lógica
    simularAtaque(&mapa[idxAtk], &mapa[idxDef]);
}

// Executa a lógica da batalha (Regras de Negócio)
void simularAtaque(Territorio* atacante, Territorio* defensor) {
    // Impedir que ataque a si mesmo
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\n[!] Ação bloqueada: Você não pode atacar um território aliado (%s)!\n", atacante->cor);
        return;
    }

    // Impedir que ataque sem tropas
    if (atacante->tropas <= 1) {
        printf("\n[!] Ação bloqueada: %s não tem tropas suficientes para atacar (Mínimo >= 1).\n", atacante->nome);
        return;
    }

    printf("\nIniciando batalha: %s (Atq) vs %s (Def)...\n", atacante->nome, defensor->nome);

    // Simular batalha (1 a 6)
    int dadoAtk = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;

    printf("Dados rolados -> Atacante: [%d] | Defensor: [%d]\n", dadoAtk, dadoDef);

    // Resultado batalha
    if (dadoAtk > dadoDef) {
        printf(">>> VITÓRIA DO ATACANTE! <<<\n");
        printf("O território %s foi conquistado pelo exército %s.\n", defensor->nome, atacante->cor);

        strcpy(defensor->cor, atacante->cor); 
        
        int tropasMovidas = atacante->tropas / 2;
        defensor->tropas = tropasMovidas;
        atacante->tropas -= tropasMovidas; // A outra metade fica

    } else {
        printf(">>> DEFESA BEM SUCEDIDA! <<<\n");
        printf("O atacante perdeu a batalha e sofreu baixas.\n");
        
        // Penalidade por derrota
        if (atacante->tropas > 0) {
            atacante->tropas -= 1; // Perde 1 tropa
        }
    }
}