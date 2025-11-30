#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MaxTerri 5

struct war {
    char nome[30];
    char cor[10];
    int tropas;
};

typedef struct war Territorio;

void limparbuff() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função de Ataque 1 dado para cada lado
void atacar(Territorio* atacante, Territorio* defensor) {
    int dado_atacante = rand() % 6 + 1;
    int dado_defensor = rand() % 6 + 1;

    printf("\n--- Resultado do Ataque ---\n");
    printf("%s rolou: %d\n", atacante->nome, dado_atacante);
    printf("%s rolou: %d\n", defensor->nome, dado_defensor);

    if (dado_atacante > dado_defensor) {
        defensor->tropas--;
        printf("%s perdeu 1 tropa!\n", defensor->nome);
    } else if (dado_defensor > dado_atacante) {
        atacante->tropas--;
        printf("%s perdeu 1 tropa!\n", atacante->nome);
    } else {
        printf("Empate! Ninguém perde tropas.\n");
    }

    printf("\nTropas restantes:\n");
    printf("%s: %d tropas\n", atacante->nome, atacante->tropas);
    printf("%s: %d tropas\n", defensor->nome, defensor->tropas);
}   //deu certo por mais que eu nao tenha entendido muito bem a logica de ataque 

int main() {
    Territorio territorios[MaxTerri];
    int opcao = -1;
    int escolha_atacante, escolha_defensor;

    srand(time(NULL));  // Inicializa os dados

    printf("======================================\n");
    printf("          Bem-vindo a WAR\n");
    printf("======================================\n");

    while (opcao != 0) {
        printf("\n1. Usar territórios prontos\n");
        printf("2. Cadastrar novos territórios\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparbuff();

        switch (opcao) {
        case 1:
            // Territórios prontos
            strcpy(territorios[0].nome, "Brasil");
            strcpy(territorios[0].cor, "Verde");
            territorios[0].tropas = 1000;

            strcpy(territorios[1].nome, "Argentina");
            strcpy(territorios[1].cor, "Azul");
            territorios[1].tropas = 800;

            strcpy(territorios[2].nome, "Chile");
            strcpy(territorios[2].cor, "Vermelho");
            territorios[2].tropas = 600;

            strcpy(territorios[3].nome, "Colombia");
            strcpy(territorios[3].cor, "Amarelo");
            territorios[3].tropas = 700;

            strcpy(territorios[4].nome, "Peru");
            strcpy(territorios[4].cor, "Roxo");
            territorios[4].tropas = 500;

            printf("\n--- Territórios Disponíveis ---\n");
            for (int i = 0; i < MaxTerri; i++) {
                printf("\nTerritório %d\n", i + 1);
                printf("Nome: %s\n", territorios[i].nome);
                printf("Cor: %s\n", territorios[i].cor);
                printf("Tropas: %d\n", territorios[i].tropas);
            }

            printf("\nEscolha o número do território atacante (1-5): ");
            scanf("%d", &escolha_atacante);
            printf("Escolha o número do território defensor (1-5): ");
            scanf("%d", &escolha_defensor);
            limparbuff();

            escolha_atacante--;
            escolha_defensor--;

            if (escolha_atacante < 0 || escolha_atacante >= MaxTerri ||
                escolha_defensor < 0 || escolha_defensor >= MaxTerri ||
                escolha_atacante == escolha_defensor) {
                printf("Escolhas inválidas!\n");
                break;
            }

            atacar(&territorios[escolha_atacante], &territorios[escolha_defensor]);

            break;

        case 2:
            // Cadastrar  territórios
            printf("\nCadastro dos 5 territórios:\n");

            for (int i = 0; i < MaxTerri; i++) {
                printf("\nNome do território %d: ", i + 1);
                fgets(territorios[i].nome, 30, stdin);
                territorios[i].nome[strcspn(territorios[i].nome, "\n")] = 0;

                printf("Cor do exército: ");
                fgets(territorios[i].cor, 10, stdin);
                territorios[i].cor[strcspn(territorios[i].cor, "\n")] = 0;

                printf("Número de tropas: ");
                scanf("%d", &territorios[i].tropas);
                limparbuff();
            }

            printf("\n--- Territórios Cadastrados ---\n");
            for (int i = 0; i < MaxTerri; i++) {
                printf("\nTerritório %d:\n", i + 1);
                printf("Nome: %s\n", territorios[i].nome);
                printf("Cor: %s\n", territorios[i].cor);
                printf("Tropas: %d\n", territorios[i].tropas);
            }
            break;

        case 0:
            printf("\nSaindo do jogo...\n");
            break;

        default:
            printf("Opção inválida!\n");
        }
    }
// perdi 3 anos de vida nisso aqui me passe
    return 0;
}

