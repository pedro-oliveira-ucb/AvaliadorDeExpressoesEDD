# Calculadora de Expressões em C

Este projeto é uma implementação em linguagem C de um avaliador de expressões matemáticas. O programa é capaz de converter expressões entre as notações Infixa (a forma como escrevemos normalmente) e Pós-fixada (Notação Polonesa Reversa), além de calcular o resultado final.

A implementação utiliza a estrutura de dados de **Pilha** como base para todas as operações de conversão e cálculo, demonstrando um caso de uso clássico e poderoso desta estrutura.

## Features

  - **Conversão Bidirecional**: Converte expressões da notação Infixa para Pós-fixada e vice-versa.
  - **Avaliação de Expressões**: Calcula o valor numérico de expressões em ambas as notações.
  - **Operadores Suportados**:
      - **Aritméticos**: `+` (adição), `-` (subtração), `*` (multiplicação), `/` (divisão), `^` (potência), `%` (módulo).
      - **Funções (Unárias)**: `log` (logaritmo base 10), `raiz` (raiz quadrada), `sen` (seno), `cos` (cosseno), `tg` (tangente).
  - **Manuseio de Precedência**: Respeita a ordem matemática das operações, incluindo o uso de parênteses `( )` para agrupamento.
  - **Entrada em Graus**: As funções trigonométricas (`sen`, `cos`, `tg`) aceitam valores de entrada em graus.

## Estrutura do Projeto

O código-fonte é modularizado em três arquivos, seguindo as boas práticas de programação em C:

  - `expressao.h`: O arquivo de cabeçalho. Define a interface pública do módulo, incluindo a `struct Expressao` e os protótipos das funções principais. **Este arquivo não deve ser modificado.**
  - `expressao.c`: O arquivo de implementação. Contém toda a lógica para as conversões e cálculos, incluindo as implementações das pilhas.
  - `main.c`: O arquivo principal. Serve como ponto de entrada do programa e contém exemplos de uso e testes para validar as funcionalidades implementadas em `expressao.c`.

## Pré-requisitos

Para compilar e executar o projeto, você precisará de:

  - Um compilador C, como o **GCC** (GNU Compiler Collection).

## Como Compilar e Executar

1.  Certifique-se de que os três arquivos (`main.c`, `expressao.c`, `expressao.h`) estejam na mesma pasta.

2.  Abra um terminal ou prompt de comando nesse diretório.

3.  Execute o seguinte comando para compilar o projeto:

    ```bash
    gcc expressao.c main.c -o expressao.exe
    ```

      - `gcc`: Chama o compilador.
      - `expressao.c main.c`: Especifica os arquivos-fonte a serem compilados.
      - `-o expressao`: Define o nome do arquivo executável de saída (`expressao.exe` no Windows).

4.  Após a compilação, execute o programa com:

    ```bash
    # No Linux ou macOS
    ./expressao

    # No Windows
    .\expressao.exe
    ```

## Como Funciona

  - **Infixa para Pós-fixada**: A conversão utiliza o **Algoritmo Shunting-yard** de Edsger Dijkstra, que emprega uma pilha para reordenar os operadores com base em sua precedência.
  - **Avaliação Pós-fixada**: A expressão pós-fixada é avaliada usando uma pilha. Os números são empilhados; quando um operador é encontrado, os operandos necessários são desempilhados, a operação é realizada e o resultado é empilhado de volta.
  - **Pós-fixada para Infixa**: Este processo também usa uma pilha para construir a expressão infixa, adicionando parênteses para garantir que a ordem das operações seja preservada.

-----