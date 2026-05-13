# Blua — Sistema de Gerenciamento de Fila de Atendimento

> **Sprint:** Estruturas de Dados Aplicadas à Saúde Digital  
> **Disciplina:** Estruturas de Dados  
> **Linguagem:** C (padrão C99)

---

## Descrição

A **Care Plus** está expandindo a plataforma **Blua** para oferecer cuidado remoto proativo. Este projeto implementa o núcleo do sistema de gerenciamento de filas de teleconsultas, capaz de:

- **Enfileirar pacientes** que solicitam teleconsulta (Fila FIFO)
- **Priorizar emergências** em uma fila separada atendida antes da fila normal
- **Processar atendimentos** simulando a chamada sequencial por um médico disponível
- **Registrar histórico** das últimas N consultas finalizadas em uma Pilha (LIFO), permitindo auditoria e desfazer

Todas as estruturas são construídas **do zero com listas encadeadas** — sem uso de bibliotecas prontas de fila/pilha.

---

## Estruturas de Dados Implementadas

| Estrutura | Tipo | Uso |
|-----------|------|-----|
| `Fila` | FIFO (lista encadeada simples) | Pacientes comuns esperando teleconsulta |
| `FilaPrioridade` | FIFO com prioridade de processamento | Emergências médicas |
| `Pilha` | LIFO (lista encadeada) com capacidade máxima | Histórico das últimas N consultas |

### Operações disponíveis

```
Fila / FilaPrioridade:
  fila_enqueue  / fprio_enqueue  → insere no final
  fila_dequeue  / fprio_dequeue  → remove do início
  fila_peek     / fprio_peek     → consulta sem remover
  fila_is_empty / fprio_is_empty → verifica se vazia

Pilha:
  pilha_push     → insere no topo (descarta o mais antigo se cheio)
  pilha_pop      → remove do topo
  pilha_peek     → consulta sem remover
  pilha_is_empty → verifica se vazia
```

---

## Como Compilar e Executar

### Pré-requisitos

- Compilador C: `gcc` (Linux/macOS) ou `MinGW` / `MSVC` (Windows)

### Compilação e Execução

**Linux / macOS:**
```bash
gcc -std=c99 -Wall -Wextra -o blua main.c
./blua
```

**Windows (MinGW):**
```cmd
gcc -std=c99 -Wall -Wextra -o blua.exe main.c
blua.exe
```

**Online (OnlineGDB) — sem instalação:**
1. Acesse https://www.onlinegdb.com/online_c_compiler
2. Apague o código de exemplo que aparece no editor
3. Cole todo o conteúdo do arquivo `main.c`
4. Certifique-se de que a linguagem selecionada é **C** (canto superior direito)
5. Clique em **Run**

> **Dica:** No OnlineGDB o menu interativo funciona normalmente. Para demonstrar os 3 cenários de uma vez, escolha a opção **8** assim que o programa iniciar.

---

## Vídeo de Demonstração

[![Demonstração Blua](https://img.youtube.com/vi/kpLI2lhdKm4/0.jpg)](https://www.youtube.com/watch?v=kpLI2lhdKm4)

🔗 https://www.youtube.com/watch?v=kpLI2lhdKm4

---

## Menu Interativo

Ao executar, o sistema apresenta o menu:

```
╔══════════════════════════════════════════╗
║   Blua — Central de Teleconsultas        ║
╠══════════════════════════════════════════╣
║ 1. Adicionar paciente normal             ║
║ 2. Adicionar paciente EMERGÊNCIA         ║
║ 3. Processar próximo atendimento         ║
║ 4. Ver fila normal                       ║
║ 5. Ver fila de emergências               ║
║ 6. Ver histórico de atendimentos         ║
║ 7. Desfazer último registro (pop)        ║
║ 8. Executar cenários de demonstração     ║
║ 0. Sair                                  ║
╚══════════════════════════════════════════╝
```

---

## Cenários de Demonstração (opção 8)

### Cenário 1 — Fluxo básico
Enfileira 3 pacientes normais e processa 2 atendimentos sequencialmente, demonstrando o comportamento FIFO e o registro no histórico.

### Cenário 2 — Emergências com prioridade
Adiciona 2 pacientes normais e 1 emergência. Ao processar, a emergência é atendida **antes** dos pacientes normais, independentemente da ordem de chegada.

### Cenário 3 — Auditoria e capacidade da pilha
Demonstra o `pop` para desfazer o último registro (reatribuição/cancelamento), e preenche o histórico além da capacidade máxima (`HISTORICO_MAX = 5`) para mostrar o descarte automático dos registros mais antigos.

---

## Estrutura do Projeto

```
main.c   ← código-fonte único com menu interativo
README.md            ← este arquivo
relatorio_tecnico.pdf← análise das estruturas e complexidade
```

---

## Complexidade das Operações (resumo)

| Operação | Fila | Pilha |
|----------|------|-------|
| enqueue / push | O(1) | O(1) |
| dequeue / pop  | O(1) | O(1) |
| peek           | O(1) | O(1) |
| is_empty       | O(1) | O(1) |
| Imprimir tudo  | O(n) | O(n) |

---

## Constantes Configuráveis

No início do arquivo `main.c`:

```c
#define MAX_NOME      80   // tamanho máximo do nome do paciente
#define MAX_QUEIXA   120   // tamanho máximo da queixa
#define HISTORICO_MAX  5   // capacidade da pilha de histórico
```

---

## Integrantes

Projeto desenvolvido para o Sprint de Estruturas de Dados — Care Plus / Blua.

| Nome | RM |
|------|----|
| Isabela Marques de Oliveira | 567230 |
| Isabelle Ramos De Filippis | 566783 |
| João Vitor Anunciação Oliveira | 567539 |
| Paulo Ribeiro Marinho | 567459 |
| Samy Tamires de Sousa Cruz | 566674 |
