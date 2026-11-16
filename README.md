# 🦸 The Boys — Simulação de Eventos Futuros

Projeto acadêmico desenvolvido em **C** para a disciplina **Programação I**, da **Universidade Federal do Paraná (UFPR)**, no semestre **2024/2**.

O programa simula um mundo com heróis, bases e missões ao longo do tempo. Em vez de atualizar todos os elementos continuamente, utiliza uma **Lista de Eventos Futuros (LEF)**: os acontecimentos são agendados e processados em ordem temporal.

## 🎯 Objetivo

Aplicar **Tipos Abstratos de Dados (TADs)** e estruturas de dados para construir uma simulação orientada a eventos, com gerenciamento de memória e organização modular em C.

Na simulação, os heróis têm habilidades, experiência, paciência e velocidade. Eles se deslocam entre bases, podem aguardar em filas e participam de missões. Cada missão exige determinadas habilidades, e o sistema avalia as condições necessárias para sua realização.

## ⚙️ Funcionamento

O mundo é inicializado com:

- **50 heróis**, com atributos e habilidades sorteados;
- **10 bases**, com posições, capacidades e filas de espera;
- **5.256 missões**, distribuídas pelo mundo;
- **10 habilidades** possíveis;
- **30 unidades de Composto V**;
- **525.600 unidades de tempo** para a simulação.

A LEF organiza eventos como chegada a uma base, espera, entrada, saída, viagem, desistência, morte, tentativa de missão e encerramento. Ao final, o programa apresenta estatísticas da simulação.

## 🧱 Estrutura do projeto

| Arquivos | Responsabilidade |
|---|---|
| `theboys.c` | Inicialização do mundo e função `main` |
| `entidades.c/.h` | Estruturas e gerenciamento das entidades |
| `eventos.c/.h` | Regras e processamento dos diferentes eventos |
| `lef.c/.h` | Inicialização, processamento e destruição da Lista de Eventos Futuros |
| `fila.c/.h` | TAD de fila |
| `fprio.c/.h` | TAD de fila de prioridade |
| `conjunto.h` e `conjunto.o` | Interface e módulo objeto do TAD conjunto |
| `makefile` | Compilação, execução, limpeza e verificação com Valgrind |

### Conceitos praticados

- **Tipos Abstratos de Dados:** conjuntos, filas e filas de prioridade;
- **Simulação por eventos discretos:** agendamento e execução de acontecimentos;
- **Ponteiros e alocação dinâmica:** criação e liberação de entidades;
- **Modularização:** separação de interfaces (`.h`) e implementações (`.c`);
- **Estruturas de dados:** organização de heróis, bases, missões e eventos.

## 🚀 Como compilar e executar

**Requisitos:** GCC e Make em ambiente compatível com o projeto.

Clone o repositório:

```bash
git clone https://github.com/PedroStahel/TheBoysUFPR.git
cd TheBoysUFPR
```

Compile:

```bash
make
```

Execute:

```bash
make run
```

Ou, após a compilação:

```bash
./theboys
```

O `makefile` utiliza **C99** e as opções `-Wall -Wextra -Werror -g`.

### Verificação de memória

Com o Valgrind instalado, execute:

```bash
make valgrind
```

Para remover os arquivos gerados na compilação:

```bash
make clean
```

> **Observação:** o repositório contém `conjunto.o`, utilizado na vinculação do executável. Por ser um arquivo objeto pré-compilado, sua compatibilidade pode depender do sistema e da arquitetura utilizados.

## 👨‍💻 Autoria

**Pedro Stahel de Azevedo** — GRR20252084  
Ciência da Computação — UFPR

---

*Projeto acadêmico da disciplina de Programação I — UFPR.*
