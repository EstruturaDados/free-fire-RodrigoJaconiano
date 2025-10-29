#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ITENS 10
#define MAX_NOME 50
#define MAX_TIPO 30

typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
    int prioridade; 
} Item;

typedef enum {
    CRIT_NOME = 1,
    CRIT_TIPO = 2,
    CRIT_PRIORIDADE = 3
} CriterioOrdenacao;

Item mochila[MAX_ITENS];
int numItens = 0;
bool ordenadaPorNome = false;

void limparTela() {
    for (int i = 0; i < 30; ++i) puts("");
}

void lerLinha(char *buffer, int size) {
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
}

void listarItens() {
    limparTela();
    printf("==== ITENS NA MOCHILA (%d/%d) ====\n", numItens, MAX_ITENS);
    if (numItens == 0) {
        puts("Mochila vazia.");
        return;
    }
    printf("%-3s | %-30s | %-15s | %-9s | %-9s\n", "ID", "NOME", "TIPO", "QUANT", "PRIORIDADE");
    puts("----+--------------------------------+-----------------+----------+----------");
    for (int i = 0; i < numItens; ++i) {
        printf("%-3d | %-30s | %-15s | %-9d | %-9d\n",
               i+1,
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade,
               mochila[i].prioridade);
    }
}

void inserirItem() {
    if (numItens >= MAX_ITENS) {
        puts("Mochila cheia. Remova um item antes de inserir.");
        return;
    }
    Item novo;
    printf("Nome: ");
    lerLinha(novo.nome, MAX_NOME);
    if (strlen(novo.nome) == 0) {
        puts("Nome inválido. Inserção cancelada.");
        return;
    }
    printf("Tipo: ");
    lerLinha(novo.tipo, MAX_TIPO);
    if (strlen(novo.tipo) == 0) strncpy(novo.tipo, "Desconhecido", MAX_TIPO-1);

    printf("Quantidade (inteiro): ");
    if (scanf("%d", &novo.quantidade) != 1) {
        while (getchar() != '\n');
        puts("Quantidade inválida. Inserção cancelada.");
        return;
    }
    if (novo.quantidade < 1) novo.quantidade = 1;
    printf("Prioridade (1-5): ");
    if (scanf("%d", &novo.prioridade) != 1) {
        while (getchar() != '\n');
        puts("Prioridade inválida. Inserção cancelada.");
        return;
    }
    if (novo.prioridade < 1) novo.prioridade = 1;
    if (novo.prioridade > 5) novo.prioridade = 5;

    while (getchar() != '\n');

    mochila[numItens++] = novo;
    ordenadaPorNome = false;
    puts("Item inserido com sucesso.");
}

void removerItem() {
    if (numItens == 0) {
        puts("Mochila vazia. Nada para remover.");
        return;
    }
    char nomeBusca[MAX_NOME];
    printf("Nome do item a remover: ");
    lerLinha(nomeBusca, MAX_NOME);
    if (strlen(nomeBusca) == 0) {
        puts("Nome inválido.");
        return;
    }

    int pos = -1;
    for (int i = 0; i < numItens; ++i) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            pos = i;
            break;
        }
    }
    if (pos == -1) {
        puts("Item não encontrado.");
        return;
    }
    for (int i = pos; i < numItens - 1; ++i) {
        mochila[i] = mochila[i+1];
    }
    numItens--;
    puts("Item removido com sucesso.");
}

void insertionSort(CriterioOrdenacao criterio, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < numItens; ++i) {
        Item chave = mochila[i];
        int j = i - 1;
        bool deveDeslocar = false;

        while (j >= 0) {
            (*comparacoes)++;
            if (criterio == CRIT_NOME) {
                if (strcmp(mochila[j].nome, chave.nome) > 0) deveDeslocar = true;
                else deveDeslocar = false;
            } else if (criterio == CRIT_TIPO) {
                if (strcmp(mochila[j].tipo, chave.tipo) > 0) deveDeslocar = true;
                else deveDeslocar = false;
            } else { // CRIT_PRIORIDADE (maior prioridade primeiro)
                if (mochila[j].prioridade < chave.prioridade) deveDeslocar = true;
                else if (mochila[j].prioridade > chave.prioridade) deveDeslocar = false;
                else {
                    // se prioridades iguais, desempata por nome asc
                    if (strcmp(mochila[j].nome, chave.nome) > 0) deveDeslocar = true;
                    else deveDeslocar = false;
                }
            }

            if (!deveDeslocar) break;
            mochila[j+1] = mochila[j];
            j--;
        }
        mochila[j+1] = chave;
    }

    ordenadaPorNome = (criterio == CRIT_NOME);
}

void menuDeOrdenacao() {
    if (numItens <= 1) {
        puts("Poucos itens. Ordenação desnecessária.");
        return;
    }
    printf("Escolha o critério de ordenação:\n");
    printf("1 - Nome (alfabético asc)\n");
    printf("2 - Tipo (alfabético asc)\n");
    printf("3 - Prioridade (5 -> 1)\n");
    printf("Opção: ");
    int opt;
    if (scanf("%d", &opt) != 1) {
        while (getchar() != '\n');
        puts("Opção inválida.");
        return;
    }
    while (getchar() != '\n');
    if (opt < 1 || opt > 3) {
        puts("Opção inválida.");
        return;
    }

    long comparacoes = 0;
    insertionSort((CriterioOrdenacao)opt, &comparacoes);

    puts("Ordenação concluída.");
    printf("Comparações realizadas durante a ordenação: %ld\n", comparacoes);
}

void buscaBinariaPorNome() {
    if (!ordenadaPorNome) {
        puts("Busca binária requer que a mochila esteja ordenada por nome. Use a opção de ordenação (critério: Nome).");
        return;
    }
    if (numItens == 0) {
        puts("Mochila vazia.");
        return;
    }
    char alvo[MAX_NOME];
    printf("Nome a buscar (busca binária): ");
    lerLinha(alvo, MAX_NOME);
    if (strlen(alvo) == 0) {
        puts("Nome inválido.");
        return;
    }

    int low = 0, high = numItens - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(mochila[mid].nome, alvo);
        if (cmp == 0) {
            printf("Item encontrado na posição %d:\n", mid + 1);
            printf("Nome: %s\nTipo: %s\nQuantidade: %d\nPrioridade: %d\n",
                   mochila[mid].nome,
                   mochila[mid].tipo,
                   mochila[mid].quantidade,
                   mochila[mid].prioridade);
            return;
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    puts("Item não encontrado (busca binária).");
}

void exibirMenu() {
    printf("=== CÓDIGO DA ILHA - MODO MESTRE ===\n");
    printf("Itens na mochila: %d/%d | Ordenada por nome: %s\n",
           numItens, MAX_ITENS, ordenadaPorNome ? "SIM" : "NÃO");
    puts("1 - Adicionar item");
    puts("2 - Remover item (por nome)");
    puts("3 - Listar itens");
    puts("4 - Ordenar itens (nome/tipo/prioridade)");
    puts("5 - Buscar item por nome (busca binária) -- exige ordenada por nome");
    puts("0 - Sair");
    printf("Escolha: ");
}

int main() {
    int opcao;
    bool sair = false;
    do {
        exibirMenu();
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            puts("Entrada inválida.");
            continue;
        }
        while (getchar() != '\n'); 

        switch (opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                menuDeOrdenacao();
                break;
            case 5:
                buscaBinariaPorNome();
                break;
            case 0:
                sair = true;
                break;
            default:
                puts("Opção inválida.");
        }

        if (!sair) {
            puts("\nPressione ENTER para continuar...");
            getchar();
            limparTela();
        }
    } while (!sair);

    puts("Encerrando. Boa sorte na ilha!");
    return 0;
}
