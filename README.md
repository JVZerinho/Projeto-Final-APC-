# Projeto-Final-APC
Nome: João Victor Pereira Santos
<br>
Matricula: 252039062
<br>
# 📦 Warehouse System (Sistema de Estoque em C)

> Um sistema de gerenciamento de armazém via linha de comando (CLI), focado em eficiência algorítmica e persistência de dados.

![Language](https://img.shields.io/badge/Language-C-blue.svg)
![Status](https://img.shields.io/badge/Status-Completed-success.svg)
![License](https://img.shields.io/badge/License-MIT-lightgrey.svg)

## 📖 Sobre o Projeto

Este projeto implementa um sistema de controle de estoque que combina *gestão lógica* (catálogo de produtos) com *gestão espacial* (mapa do armazém 5x5). 

Desenvolvido em *C puro*, o sistema foi projetado para demonstrar conceitos fundamentais de Ciência da Computação, como manipulação de memória, algoritmos de busca e ordenação, e persistência de dados em arquivos binários.

## 🚀 Funcionalidades

* *Visualização Espacial:* Mapa 5x5 interativo mostrando a ocupação das prateleiras.
* *CRUD Completo:* Adicionar, Ler, Atualizar (via remoção/adição) e Deletar produtos.
* *Persistência de Dados:* Salvamento automático em arquivo binário (armazem_db.bin), mantendo os dados entre execuções.
* *Busca Otimizada:* Consulta de produtos por ID.
* *Relatórios:* Listagem tabular formatada com preços e categorias.

## 🛠 Destaques Técnicos

Este não é apenas um CRUD simples. O código implementa estruturas e algoritmos importantes:

1.  *Busca Binária Recursiva:*
    * Para garantir performance $O(\log n)$ nas consultas, o catálogo é mantido ordenado. A busca não varre o vetor inteiro, ela divide o problema ao meio recursivamente.
2.  *Ordenação (QuickSort):*
    * Utilização do qsort nativo para manter o vetor de produtos ordenado pelo ID, pré-requisito para a busca binária.
3.  *Algoritmo "Shift Left":*
    * Na remoção de um item, o sistema realiza um deslocamento de memória para cobrir o "buraco" deixado no vetor, mantendo a contiguidade dos dados.
4.  *Matriz vs Vetor:*
    * Uso híbrido de estruturas: Uma *Matriz* para representar a posição física e um *Vetor de Structs* para os dados lógicos.

## 💻 Como Executar

### Pré-requisitos
* Compilador C (GCC recomendado).
* Terminal (Windows CMD, PowerShell ou Bash no Linux).

### Compilação

Abra o terminal na pasta do projeto e execute:

```bash
# Compilar
gcc armazem.c -o armazem

# Executar (Windows)
.\armazem.exe

# Executar (Linux)
./armazem
