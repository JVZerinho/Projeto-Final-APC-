# Projeto-Final-APC

Nome: João Victor Pereira Santos
<br>
Matricula: 252039062
<br>

# 📦 Warehouse System (Sistema de Estoque em C)

> Um sistema de gerenciamento de armazém via linha de comando (CLI), focado em eficiência algorítmica e persistência de dados.

![Language](https://img.shields.io/badge/Language-C-blue.svg)
![Status](https://img.shields.io/badge/Status-Completed-success.svg)

## 📖 Sobre o Projeto

Este projeto implementa um sistema de controle de estoque que combina _gestão lógica_ (catálogo de produtos) com _gestão espacial_ (mapa do armazém 5x5).

Desenvolvido em _C puro_, o sistema foi projetado para demonstrar conceitos fundamentais de Ciência da Computação, como manipulação de memória, algoritmos de busca e ordenação, e persistência de dados em arquivos binários.

## 🚀 Funcionalidades

- _Visualização Espacial:_ Mapa 5x5 interativo mostrando a ocupação das prateleiras.
- _CRUD Completo:_ Adicionar, Ler, Atualizar (via remoção/adição) e Deletar produtos.
- _Persistência de Dados:_ Salvamento automático em arquivo binário (armazem_db.bin), mantendo os dados entre execuções.
- _Busca Otimizada:_ Consulta de produtos por ID.
- _Relatórios:_ Listagem tabular formatada com preços e categorias.

## 🛠 Destaques Técnicos

Este não é apenas um CRUD simples. O código implementa estruturas e algoritmos importantes:

1.  _Busca Binária Recursiva:_
    - Para garantir performance nas consultas, o catálogo é mantido ordenado. A busca não varre o vetor inteiro, ela divide o problema ao meio recursivamente.
2.  _Ordenação (QuickSort):_
    - Utilização do qsort nativo para manter o vetor de produtos ordenado pelo ID, pré-requisito para a busca binária.
3.  _Algoritmo "Shift Left":_
    - Na remoção de um item, o sistema realiza um deslocamento de memória para cobrir o "buraco" deixado no vetor, mantendo a contiguidade dos dados.
4.  _Matriz vs Vetor:_
    - Uso híbrido de estruturas: Uma _Matriz_ para representar a posição física e um _Vetor de Structs_ para os dados lógicos.

## 💻 Como Executar

### Pré-requisitos

- Compilador C (GCC recomendado).
- Terminal (Windows CMD, PowerShell ou Bash no Linux).

### Compilação

Abra o terminal na pasta do projeto e execute:

```bash
# Compilar
gcc GerenciadorDeEstoque.c -o armazem.exe

# Executar (Windows)
.\armazem.exe

# Executar (Linux)
./armazem
```
