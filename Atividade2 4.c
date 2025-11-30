/*
  WAR - versão com sistema de missões
  Anotando pra nao me perder depois:
  Requisitos pra atividade :
  - vetor de missões (5 opções)
  - atribuirMissao: sorteia missão e copia para destino (destino deve ser memória alocada)
  - missões dos jogadores armazenadas com malloc
  - verificarMissao: lógica simples para 5 tipos de missão
  - exibirMissao (por valor), atribuirMissao (recebe referencia para memória alocada)
  - atacar: usa rand() (1-6), valida ataque (somente contra cor diferente), se atacante vencer:
            transfere cor e metade das tropas do atacante para defensor; caso contrário atacante perde 1 tropa
  - exibirMapa, liberarMemoria, main, modularização
  - srand(time(NULL)), free() ao final
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MaxTerri 5
#define MAX_MISSAO_LEN 128

/* Estrutura do Território */
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

/* Protótipos das do sistema funções  */
void limparbuff(void);
void exibirMapa(Territorio* mapa, int tamanho);
void atacar(Territorio* atacante, Territorio* defensor);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, const char* corJogador, Territorio* mapa, int tamanho);
void exibirMissao(const char* missao); // por valor (recebe ponteiro, mas intenção: somente exibir)
void liberarMemoria(Territorio* mapa, char** missoesJogadores, int numJogadores);

/* Implementação das funções */

void limparbuff(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Imprime qual sera o mapa atual */
void exibirMapa(Territorio* mapa, int tamanho) {
    printf("\n--- MAPA ATUAL ---\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%d) %s | Cor: %s | Tropas: %d\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

/*
  Função de ataque:
  - gera um dado (1-6) para atacante e defensor
  - somente permite ataque se cores forem diferentes (inimigo)
  - se atacante vencer: defensor muda para cor do atacante e recebe metade das tropas do atacante (movidas)
    -- metade arredondada para baixo; atacante perde essa metade
  - se defensor vencer ou empatar: atacante perde 1 tropa (mínimo 0)
*/
void atacar(Territorio* atacante, Territorio* defensor) {
    if (atacante == NULL || defensor == NULL) return;

    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Ataque inválido: os territórios pertencem à mesma cor.\n");
        return;
    }

    if (atacante->tropas <= 0) {
        printf("%s não tem tropas suficientes para atacar.\n", atacante->nome);
        return;
    }

    int dado_atacante = rand() % 6 + 1;
    int dado_defensor = rand() % 6 + 1;

    printf("\n--- Rolagens ---\n");
    printf("%s (atacante) rolou: %d\n", atacante->nome, dado_atacante);
    printf("%s (defensor)  rolou: %d\n", defensor->nome, dado_defensor);

    if (dado_atacante > dado_defensor) {
        /* Atacante vence: transfere cor e metade das tropas para defensor */
        int movidas = atacante->tropas / 2; /* metade do exército atacante */
        if (movidas <= 0) movidas = 1; /* garante ao menos 1 movida se atacante tiver tropas */
        printf("%s venceu! Transferindo cor e %d tropas para %s.\n", atacante->nome, movidas, defensor->nome);
        /* atualiza defensor */
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = movidas;
        /* atualiza atacante */
        atacante->tropas -= movidas;
        if (atacante->tropas < 0) atacante->tropas = 0;
    } else {
        /* Defensor empata ou vence: atacante perde 1 tropa */
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("%s defendeu! %s perde 1 tropa.\n", defensor->nome, atacante->nome);
    }

    printf("Tropas agora: %s = %d | %s = %d\n",
           atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);
}

/*
  atribuirMissao: sorteia uma missão do vetor 'missoes' e copia para 'destino'.
  'destino' deve ter memória alocada (malloc/calloc) com espaço suficiente.
*/
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    if (destino == NULL || missoes == NULL || totalMissoes <= 0) return;
    int idx = rand() % totalMissoes;
    strcpy(destino, missoes[idx]);
}

/*
  verificarMissao:
  - recebe a string da missão e a cor do jogador (para verificar propriedade)
  - lógica simples baseada em palavras-chave:
    * "Conquistar 3 territorios seguidos" -> existe sequência de 3 territories consecutivos com corJogador?
    * "Eliminar todas as tropas da cor Vermelho" -> não existe território com cor "Vermelho" e tropas>0
    * "Ter mais de 3000 tropas" -> soma tropas de territórios com corJogador > 3000
    * "Conquistar Brasil" -> verificar se 'Brasil' tem cor igual à corJogador
    * "Controlar 4 territorios" -> contar territorios com corJogador >= 4
  - Retorna 1 se cumprida, 0 caso contrário.
*/
int verificarMissao(char* missao, const char* corJogador, Territorio* mapa, int tamanho) {
    if (missao == NULL || corJogador == NULL || mapa == NULL) return 0;

    /* Conquistar 3 territorios seguidos */
    if (strstr(missao, "Conquistar 3 territorios seguidos") != NULL) {
        int consec = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                consec++;
                if (consec >= 3) return 1;
            } else consec = 0;
        }
        return 0;
    }

    /* Eliminar todas as tropas da cor Vermelho (case-insensitive parcial) */
    if (strstr(missao, "Eliminar todas as tropas da cor Vermelho") != NULL ||
        strstr(missao, "Eliminar todas as tropas da cor vermelha") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcasecmp(mapa[i].cor, "Vermelho") == 0 || strcasecmp(mapa[i].cor, "Vermelha") == 0) {
                if (mapa[i].tropas > 0) return 0; /* ainda existe tropa vermelha */
            }
        }
        return 1;
    }

    /* Ter mais de 3000 tropas no total */
    if (strstr(missao, "Ter mais de 3000 tropas") != NULL) {
        int soma = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) soma += mapa[i].tropas;
        }
        return (soma > 3000) ? 1 : 0;
    }

    /* Conquistar um territorio específico, ex: "Conquistar Brasil" */
    if (strstr(missao, "Conquistar ") == missao || strstr(missao, "Conquistar") != NULL) {
        /* tenta extrair nome após "Conquistar " */
        const char* chave = "Conquistar ";
        char alvo[30] = {0};
        char *pos = strstr(missao, chave);
        if (pos) {
            pos += strlen(chave);
            /* copiar restante da string como nome do território */
            strncpy(alvo, pos, sizeof(alvo)-1);
            /* remover possíveis espaços finais/novas linhas */
            alvo[strcspn(alvo, "\r\n")] = 0;
            /* comparar com mapa */
            for (int i = 0; i < tamanho; i++) {
                if (strcasecmp(mapa[i].nome, alvo) == 0) {
                    if (strcmp(mapa[i].cor, corJogador) == 0) return 1;
                    else return 0;
                }
            }
        }
    }

    /* Controlar N territorios (ex: "Controlar 4 territorios") */
    if (strstr(missao, "Controlar") != NULL) {
        /* procura número na string */
        int need = 0;
        const char* p = missao;
        while (*p) {
            if (*p >= '0' && *p <= '9') {
                need = atoi(p);
                break;
            }
            p++;
        }
        if (need > 0) {
            int cont = 0;
            for (int i = 0; i < tamanho; i++) {
                if (strcmp(mapa[i].cor, corJogador) == 0) cont++;
            }
            return (cont >= need) ? 1 : 0;
        }
    }

    /* caso não identificado, retorna 0 (não cumprida) */
    return 0;
}

/* Exibe a missão (passagem por valor: recebe ponteiro, mas trata apenas como leitura) */
void exibirMissao(const char* missao) {
    if (missao == NULL) return;
    printf("\n--- Sua Missao ---\n%s\n", missao);
}

/* Libera memória alocada dinamicamente */
void liberarMemoria(Territorio* mapa, char** missoesJogadores, int numJogadores) {
    if (mapa != NULL) free(mapa);
    if (missoesJogadores != NULL) {
        for (int i = 0; i < numJogadores; i++) {
            if (missoesJogadores[i] != NULL) free(missoesJogadores[i]);
        }
        free(missoesJogadores);
    }
}

/* MAIN: fluxo do jogo com 2 jogadores (padrão) */
int main(void) {
    srand((unsigned int)time(NULL));

    /* Missões possíveis (pelo menos 5) */
    char* missoesDisponiveis[] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor Vermelho",
        "Ter mais de 3000 tropas",
        "Conquistar Brasil",
        "Controlar 4 territorios"
    };
    int totalMissoes = sizeof(missoesDisponiveis) / sizeof(missoesDisponiveis[0]);

    /* Aloca dinamicamente o mapa de territórios (conforme requisito) */
    Territorio* mapa = (Territorio*) calloc(MaxTerri, sizeof(Territorio));
    if (mapa == NULL) {
        fprintf(stderr, "Erro ao alocar memoria para o mapa.\n");
        return 1;
    }

    /* Preenche mapa padrão (pode ser alterado) */
    strcpy(mapa[0].nome, "Brasil");    strcpy(mapa[0].cor, "Verde");    mapa[0].tropas = 1000;
    strcpy(mapa[1].nome, "Argentina"); strcpy(mapa[1].cor, "Azul");     mapa[1].tropas = 800;
    strcpy(mapa[2].nome, "Chile");     strcpy(mapa[2].cor, "Vermelho"); mapa[2].tropas = 600;
    strcpy(mapa[3].nome, "Colombia");  strcpy(mapa[3].cor, "Amarelo");  mapa[3].tropas = 700;
    strcpy(mapa[4].nome, "Peru");      strcpy(mapa[4].cor, "Roxo");     mapa[4].tropas = 500;

    /* Configuração de jogadores (2 jogadores para este exemplo) */
    int numJogadores = 2;
    char** missoesJogadores = (char**) malloc(numJogadores * sizeof(char*));
    char nomes[2][30];
    char cores[2][10];

    if (missoesJogadores == NULL) {
        fprintf(stderr, "Erro ao alocar memoria para missoes dos jogadores.\n");
        free(mapa);
        return 1;
    }

    /* Aloca espaço para cada missão de jogador (armazenamento dinamico) */
    for (int i = 0; i < numJogadores; i++) {
        missoesJogadores[i] = (char*) malloc(MAX_MISSAO_LEN);
        if (missoesJogadores[i] == NULL) {
            fprintf(stderr, "Erro ao alocar memoria para a missao do jogador %d.\n", i+1);
            /* libera o que já foi alocado antes de sair */
            for (int j = 0; j < i; j++) free(missoesJogadores[j]);
            free(missoesJogadores);
            free(mapa);
            return 1;
        }
    }

    /* Leitura de nomes e cores dos jogadores */
    for (int i = 0; i < numJogadores; i++) {
        printf("Digite o nome do jogador %d: ", i + 1);
        fgets(nomes[i], sizeof(nomes[i]), stdin);
        nomes[i][strcspn(nomes[i], "\n")] = 0;

        printf("Digite a cor do jogador %d (ex: Verde, Azul, Vermelho): ", i + 1);
        fgets(cores[i], sizeof(cores[i]), stdin);
        cores[i][strcspn(cores[i], "\n")] = 0;
    }

    /* Sorteio e atribuição das missoes para cada jogador (uma vez no início) */
    for (int i = 0; i < numJogadores; i++) {
        atribuirMissao(missoesJogadores[i], missoesDisponiveis, totalMissoes);
        printf("\nJogador: %s | Cor: %s\n", nomes[i], cores[i]);
        /* Exibir a missão apenas uma vez no início (requisito) */
        exibirMissao(missoesJogadores[i]);
    }

    /* Loop principal do jogo: cada iteração é um 'turno' simples */
    int opcao = -1;
    while (1) {
        exibirMapa(mapa, MaxTerri);

        /* Verificar missões silenciosamente ao final de cada turno - mas também fazemos verificação aqui
           se já houve cumprimento antes de pedir ações. */
        for (int i = 0; i < numJogadores; i++) {
            if (verificarMissao(missoesJogadores[i], cores[i], mapa, MaxTerri)) {
                printf("\n--- MISSÃO CUMPRIDA ---\nJogador %s (cor %s) cumpriu a missao:\n%s\n",
                       nomes[i], cores[i], missoesJogadores[i]);
                printf("VENCEDOR: %s\n", nomes[i]);
                liberarMemoria(mapa, missoesJogadores, numJogadores);
                return 0;
            }
        }

        /* Menu simples do turno */
        printf("\nEscolha uma acao:\n");
        printf("1 - Atacar (o jogador escolhe atacante e defensor)\n");
        printf("0 - Sair do jogo\n");
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1) {
            limparbuff();
            printf("Entrada invalida.\n");
            continue;
        }
        limparbuff();

        if (opcao == 0) {
            printf("Saindo do jogo.\n");
            break;
        } else if (opcao == 1) {
            int idx_atacante, idx_defensor;
            printf("Escolha o indice do territorio atacante (1-%d): ", MaxTerri);
            if (scanf("%d", &idx_atacante) != 1) { limparbuff(); printf("Entrada invalida.\n"); continue; }
            limparbuff();
            printf("Escolha o indice do territorio defensor (1-%d): ", MaxTerri);
            if (scanf("%d", &idx_defensor) != 1) { limparbuff(); printf("Entrada invalida.\n"); continue; }
            limparbuff();

            idx_atacante--; idx_defensor--;
            if (idx_atacante < 0 || idx_atacante >= MaxTerri || idx_defensor < 0 || idx_defensor >= MaxTerri) {
                printf("Indices invalidos.\n");
                continue;
            }
            /* valida que atacante é realmente da cor do jogador que o escolheu:
               vamos pedir ao usuário a cor do jogador que está atacando (simplificação) */
            char corDoJogador[10];
            printf("Informe a cor do jogador que esta atacando (ex: Verde): ");
            fgets(corDoJogador, sizeof(corDoJogador), stdin);
            corDoJogador[strcspn(corDoJogador, "\n")] = 0;

            /* valida se o territorio atacante pertence ao jogador informado */
            if (strcmp(mapa[idx_atacante].cor, corDoJogador) != 0) {
                printf("Erro: o territorio atacante nao pertence a essa cor.\n");
                continue;
            }

            /* valida que defensor é inimigo */
            if (strcmp(mapa[idx_atacante].cor, mapa[idx_defensor].cor) == 0) {
                printf("Erro: nao e possivel atacar territorio da mesma cor.\n");
                continue;
            }

            /* executa ataque */
            atacar(&mapa[idx_atacante], &mapa[idx_defensor]);

            /* ao final do turno, verificar as missoes */
        } else {
            printf("Opcao invalida.\n");
        }
    }

    /* libera tudo antes de sair */
    liberarMemoria(mapa, missoesJogadores, numJogadores);
    return 0;
    //Nota pra alguem que estiver lendo nao sei como fiz isso funcionar mas ta ai E CRL COMO ISSO FICO TAO COMPLEXO E GRANDE
}
