#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORICO 5

// tipos de prioridade do paciente
#define NORMAL 0
#define EMERGENCIA 1

// contador global de ids
int id_contador = 1;

// -------------------------------------------------------
// estrutura do paciente
// -------------------------------------------------------

typedef struct Paciente {
    int id;
    char nome[80];
    char queixa[120];
    int prioridade; // 0 = normal, 1 = emergencia
} Paciente;

// -------------------------------------------------------
// nos da fila e da pilha
// -------------------------------------------------------

typedef struct NoPaciente {
    Paciente p;
    struct NoPaciente *prox;
} NoPaciente;

typedef struct NoPilha {
    Paciente p;
    struct NoPilha *prox;
} NoPilha;

// -------------------------------------------------------
// estrutura da fila (usada tanto pra normal quanto emergencia)
// -------------------------------------------------------

typedef struct {
    NoPaciente *inicio;
    NoPaciente *fim;
    int tamanho;
} Fila;

// -------------------------------------------------------
// estrutura da pilha
// -------------------------------------------------------

typedef struct {
    NoPilha *topo;
    int tamanho;
} Pilha;

// -------------------------------------------------------
// funcoes da FILA
// -------------------------------------------------------

void iniciar_fila(Fila *f) {
    f->inicio = NULL;
    f->fim = NULL;
    f->tamanho = 0;
}

int fila_vazia(Fila *f) {
    if (f->tamanho == 0)
        return 1;
    return 0;
}

// peek - mostra o primeiro sem remover
Paciente* fila_peek(Fila *f) {
    if (fila_vazia(f)) {
        printf("Fila vazia!\n");
        return NULL;
    }
    return &f->inicio->p;
}

// enqueue - insere no fim da fila
void enqueue(Fila *f, Paciente p) {
    NoPaciente *novo = (NoPaciente*) malloc(sizeof(NoPaciente));
    if (novo == NULL) {
        printf("Erro: sem memoria!\n");
        return;
    }
    novo->p = p;
    novo->prox = NULL;

    if (fila_vazia(f)) {
        f->inicio = novo;
        f->fim = novo;
    } else {
        f->fim->prox = novo;
        f->fim = novo;
    }
    f->tamanho++;
}

// dequeue - remove do inicio da fila
int dequeue(Fila *f, Paciente *dest) {
    if (fila_vazia(f)) {
        return 0;
    }
    NoPaciente *aux = f->inicio;
    *dest = aux->p;
    f->inicio = aux->prox;
    if (f->inicio == NULL)
        f->fim = NULL;
    free(aux);
    f->tamanho--;
    return 1;
}

void imprimir_fila(Fila *f) {
    if (fila_vazia(f)) {
        printf("  (fila vazia)\n");
        return;
    }
    NoPaciente *aux = f->inicio;
    int i = 1;
    while (aux != NULL) {
        printf("  %d) ID: %d | Nome: %s | Queixa: %s\n", i, aux->p.id, aux->p.nome, aux->p.queixa);
        aux = aux->prox;
        i++;
    }
}

// -------------------------------------------------------
// funcoes da PILHA
// -------------------------------------------------------

void iniciar_pilha(Pilha *p) {
    p->topo = NULL;
    p->tamanho = 0;
}

int pilha_vazia(Pilha *p) {
    if (p->tamanho == 0)
        return 1;
    return 0;
}

// peek - mostra o topo sem remover
Paciente* pilha_peek(Pilha *p) {
    if (pilha_vazia(p)) {
        printf("Pilha vazia!\n");
        return NULL;
    }
    return &p->topo->p;
}

// push - insere no topo
// se ja tiver MAX_HISTORICO elementos, remove o mais antigo primeiro
void push(Pilha *p, Paciente pac) {
    // se chegou no limite, remove o mais antigo (fundo da pilha)
    if (p->tamanho == MAX_HISTORICO) {
        // precisa achar o penultimo para desligar o ultimo
        NoPilha *aux = p->topo;
        NoPilha *anterior = NULL;
        while (aux->prox != NULL) {
            anterior = aux;
            aux = aux->prox;
        }
        // aux agora e o fundo (mais antigo)
        if (anterior != NULL)
            anterior->prox = NULL;
        else
            p->topo = NULL;
        free(aux);
        p->tamanho--;
    }

    NoPilha *novo = (NoPilha*) malloc(sizeof(NoPilha));
    if (novo == NULL) {
        printf("Erro: sem memoria!\n");
        return;
    }
    novo->p = pac;
    novo->prox = p->topo;
    p->topo = novo;
    p->tamanho++;
}

// pop - remove do topo
int pop(Pilha *p, Paciente *dest) {
    if (pilha_vazia(p)) {
        return 0;
    }
    NoPilha *aux = p->topo;
    *dest = aux->p;
    p->topo = aux->prox;
    free(aux);
    p->tamanho--;
    return 1;
}

void imprimir_pilha(Pilha *p) {
    if (pilha_vazia(p)) {
        printf("  (historico vazio)\n");
        return;
    }
    NoPilha *aux = p->topo;
    int i = 1;
    printf("  (topo = mais recente)\n");
    while (aux != NULL) {
        printf("  %d) ID: %d | Nome: %s | Queixa: %s\n", i, aux->p.id, aux->p.nome, aux->p.queixa);
        aux = aux->prox;
        i++;
    }
}

// -------------------------------------------------------
// funcoes auxiliares
// -------------------------------------------------------

Paciente criar_paciente(char *nome, char *queixa, int prioridade) {
    Paciente p;
    p.id = id_contador;
    id_contador++;
    p.prioridade = prioridade;
    strcpy(p.nome, nome);
    strcpy(p.queixa, queixa);
    return p;
}

// chama o medico - emergencia tem prioridade sobre fila normal
void atender(Fila *femerg, Fila *fnormal, Pilha *hist) {
    Paciente p;
    int ok = 0;

    if (!fila_vazia(femerg)) {
        ok = dequeue(femerg, &p);
        if (ok)
            printf("\n  [EMERGENCIA] Atendendo: %s | Queixa: %s\n", p.nome, p.queixa);
    } else if (!fila_vazia(fnormal)) {
        ok = dequeue(fnormal, &p);
        if (ok)
            printf("\n  [NORMAL] Atendendo: %s | Queixa: %s\n", p.nome, p.queixa);
    } else {
        printf("\n  Nenhum paciente aguardando.\n");
        return;
    }

    if (ok) {
        push(hist, p);
        printf("  Consulta salva no historico.\n");
    }
}

// -------------------------------------------------------
// cenarios de demonstracao
// -------------------------------------------------------

void cenario1(Fila *fnormal, Fila *femerg, Pilha *hist) {
    printf("\n========================================\n");
    printf("CENARIO 1 - Fila normal e atendimento sequencial\n");
    printf("========================================\n");

    enqueue(fnormal, criar_paciente("Ana Lima", "Dor de cabeca", NORMAL));
    enqueue(fnormal, criar_paciente("Bruno Souza", "Febre", NORMAL));
    enqueue(fnormal, criar_paciente("Carla Mendes", "Tosse seca", NORMAL));

    printf("\nFila normal:\n");
    imprimir_fila(fnormal);

    printf("\nPrimeiro da fila (peek): %s\n", fila_peek(fnormal)->nome);

    printf("\nProcessando 2 atendimentos...\n");
    atender(femerg, fnormal, hist);
    atender(femerg, fnormal, hist);

    printf("\nFila apos atendimentos:\n");
    imprimir_fila(fnormal);

    printf("\nHistorico:\n");
    imprimir_pilha(hist);
}

void cenario2(Fila *fnormal, Fila *femerg, Pilha *hist) {
    printf("\n========================================\n");
    printf("CENARIO 2 - Emergencia tem prioridade\n");
    printf("========================================\n");

    enqueue(fnormal, criar_paciente("Diego Rocha", "Dor lombar", NORMAL));
    enqueue(fnormal, criar_paciente("Eva Torres", "Ansiedade", NORMAL));
    enqueue(femerg, criar_paciente("Fabio Nunes", "Dor no peito", EMERGENCIA));

    printf("\nFila normal:\n");
    imprimir_fila(fnormal);
    printf("\nFila emergencia:\n");
    imprimir_fila(femerg);

    printf("\nProcessando (Fabio deve sair primeiro)...\n");
    atender(femerg, fnormal, hist);
    atender(femerg, fnormal, hist);
    atender(femerg, fnormal, hist);

    printf("\nHistorico atualizado:\n");
    imprimir_pilha(hist);
}

void cenario3(Fila *fnormal, Fila *femerg, Pilha *hist) {
    printf("\n========================================\n");
    printf("CENARIO 3 - Desfazer ultimo atendimento (pop)\n");
    printf("========================================\n");

    printf("\nHistorico antes do pop:\n");
    imprimir_pilha(hist);

    Paciente p;
    if (pop(hist, &p)) {
        printf("\n  Removido do historico: %s | %s\n", p.nome, p.queixa);
    } else {
        printf("\n  Historico vazio, nada para remover.\n");
    }

    printf("\nHistorico depois do pop:\n");
    imprimir_pilha(hist);

    // testa o limite do historico
    printf("\n--- Testando limite do historico (%d registros max) ---\n", MAX_HISTORICO);
    int i;
    for (i = 0; i < MAX_HISTORICO + 2; i++) {
        char nome[80], queixa[120];
        sprintf(nome, "Paciente %d", i + 1);
        sprintf(queixa, "Queixa %d", i + 1);
        Paciente px = criar_paciente(nome, queixa, NORMAL);
        push(hist, px);
    }
    printf("Historico (so os ultimos %d):\n", MAX_HISTORICO);
    imprimir_pilha(hist);
}

// -------------------------------------------------------
// menu principal
// -------------------------------------------------------

int main() {
    Fila fila_normal, fila_emerg;
    Pilha historico;

    iniciar_fila(&fila_normal);
    iniciar_fila(&fila_emerg);
    iniciar_pilha(&historico);

    int opcao;

    do {
        printf("\n========================================\n");
        printf("   Blua - Central de Teleconsultas\n");
        printf("========================================\n");
        printf("1. Adicionar paciente normal\n");
        printf("2. Adicionar paciente EMERGENCIA\n");
        printf("3. Atender proximo paciente\n");
        printf("4. Ver fila normal\n");
        printf("5. Ver fila de emergencias\n");
        printf("6. Ver historico de atendimentos\n");
        printf("7. Desfazer ultimo atendimento (pop)\n");
        printf("8. Rodar cenarios de demonstracao\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        getchar();

        if (opcao == 1) {
            char nome[80], queixa[120];
            printf("Nome do paciente: ");
            scanf("%[^\n]", nome);
            getchar();
            printf("Queixa: ");
            scanf("%[^\n]", queixa);
            getchar();
            Paciente p = criar_paciente(nome, queixa, NORMAL);
            enqueue(&fila_normal, p);
            printf("Paciente ID %d adicionado.\n", p.id);

        } else if (opcao == 2) {
            char nome[80], queixa[120];
            printf("Nome do paciente: ");
            scanf("%[^\n]", nome);
            getchar();
            printf("Queixa: ");
            scanf("%[^\n]", queixa);
            getchar();
            Paciente p = criar_paciente(nome, queixa, EMERGENCIA);
            enqueue(&fila_emerg, p);
            printf("Emergencia ID %d adicionada.\n", p.id);

        } else if (opcao == 3) {
            atender(&fila_emerg, &fila_normal, &historico);

        } else if (opcao == 4) {
            printf("\nFila normal (%d paciente(s)):\n", fila_normal.tamanho);
            imprimir_fila(&fila_normal);

        } else if (opcao == 5) {
            printf("\nFila de emergencias (%d paciente(s)):\n", fila_emerg.tamanho);
            imprimir_fila(&fila_emerg);

        } else if (opcao == 6) {
            printf("\nHistorico (ultimos %d atendimentos):\n", MAX_HISTORICO);
            imprimir_pilha(&historico);

        } else if (opcao == 7) {
            Paciente p;
            if (pop(&historico, &p))
                printf("Removido: ID %d | %s\n", p.id, p.nome);
            else
                printf("Historico vazio.\n");

        } else if (opcao == 8) {
            cenario1(&fila_normal, &fila_emerg, &historico);
            cenario2(&fila_normal, &fila_emerg, &historico);
            cenario3(&fila_normal, &fila_emerg, &historico);

        } else if (opcao != 0) {
            printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    printf("Encerrando sistema...\n");
    return 0;
}