//* --- BIBLIOTECAS ---
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

//* --- MACROS ---
#define LINHAS 5
#define COLUNAS 5
#define MAX_PRODUTOS 50
#define ARQUIVO_DB "armazem_db.bin"

//* --- ESTRUTURAS DE DADOS ---
typedef struct
{
    int id;
    char nome[40];
    char categoria[20];
    float preco;
} Produto;

//? A 'SistemaEstoque' agrupa tanto a representação visual (mapa) quanto lógica (catalogo)
typedef struct
{
    int mapa[LINHAS][COLUNAS];      //? Matriz 5x5 para saber ONDE está o produto
    Produto catalogo[MAX_PRODUTOS]; //? Vetor contíguo para busca rápida e ordenação
    int qtdProdutos;                //! Controla o índice do vetor. Se corromper, o sistema quebra.
} SistemaEstoque;

SistemaEstoque sistema; // Variável global (alocada na memória estática)

//* --- UTILITÁRIOS ---

void limparTela()
{
#ifdef _WIN32
    system("cls"); // Windows
#else
    system("clear"); // Linux/Mac
#endif
}

//? Consome qualquer lixo deixado no buffer do teclado (como um \n pendente de um scanf anterior)
//! CRÍTICO: Sem isso, funções como fgets pulam a leitura e causam bugs de UX.
void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

//? Wrapper seguro para fgets que remove a quebra de linha (\n) do final
void lerTexto(char *buffer, int tamanho)
{
    fgets(buffer, tamanho, stdin);
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
    {
        buffer[len - 1] = '\0'; //! Substitui o Enter por terminador nulo
    }
}

void pausa()
{
    printf("\n   >> Pressione [ENTER] para voltar...");
    limparBuffer(); //? Garante que o getchar espere um NOVO enter
    getchar();
}

//* --- DESIGN SYSTEM ---
//? Usa apenas caracteres padrão (-, +, |) para garantir compatibilidade com qualquer terminal/fonte

void desenharLinha(int tamanho)
{
    printf("   +");
    for (int i = 0; i < tamanho; i++)
        printf("-");
    printf("+\n");
}

void mostrarLogo()
{
    printf("\n");
    printf("    .-------------------------------------------------------.\n");
    printf("    |    _____                      _                       |\n");
    printf("    |   / ____|                    | |                      |\n");
    printf("    |  | (___  _ __ ___   __ _ _ __| |_                     |\n");
    printf("    |   \\___ \\| '_ ` _ \\ / _` | '__| __|                    |\n");
    printf("    |   ____) | | | | | | (_| | |  | |_                     |\n");
    printf("    |  |_____/|_| |_| |_|\\__,_|_|   \\__|WAREHOUSE SYSTEM    |\n");
    printf("    '-------------------------------------------------------'\n");
    printf("\n");
}

void desenharTitulo(char *titulo)
{
    limparTela();
    mostrarLogo();
    printf("   ===========================================================\n");
    printf("    MENU :: %s \n", titulo);
    printf("   ===========================================================\n\n");
}

//* --- CORE ---

void inicializarSistema()
{
    FILE *arquivo = fopen(ARQUIVO_DB, "rb");
    if (arquivo)
    {
        //? Tenta ler a struct inteira de uma vez (Dump de memória)
        size_t lidos = fread(&sistema, sizeof(SistemaEstoque), 1, arquivo);
        fclose(arquivo);

        //! Se a leitura falhar (arquivo corrompido/incompatível), zera a memória
        if (lidos != 1)
            memset(&sistema, 0, sizeof(SistemaEstoque));
    }
    else
    {
        memset(&sistema, 0, sizeof(SistemaEstoque)); // Cria banco novo na RAM
    }
}

void salvarDados()
{
    FILE *arquivo = fopen(ARQUIVO_DB, "wb");
    if (arquivo)
    {
        //! Grava o estado atual da RAM direto no Disco (Snapshot)
        fwrite(&sistema, sizeof(SistemaEstoque), 1, arquivo);
        fclose(arquivo);
    }
    else
    {
        printf("\n   [!] ERRO CRITICO: Falha ao salvar no disco!\n");
    }
}

//* --- ALGORITMO DE ORDENAÇÃO ---

//? Função de comparação usada pelo QuickSort (qsort) nativo do C
int compararProdutos(const void *a, const void *b)
{
    return ((Produto *)a)->id - ((Produto *)b)->id;
}

//? Ordena o vetor 'catalogo'. Necessário para a Busca Binária funcionar.
void ordenarCatalogo()
{
    if (sistema.qtdProdutos > 1)
    {
        qsort(sistema.catalogo, sistema.qtdProdutos, sizeof(Produto), compararProdutos);
    }
}

//* --- ALGORITMO DE BUSCA ---
//? Busca Binária
//? Divide o espaço de busca pela metade a cada chamada.
int buscaBinariaRecursiva(int id, int inicio, int fim)
{
    if (inicio > fim)
        return -1; //! Caso Base: Não encontrou

    int meio = (inicio + fim) / 2;

    if (sistema.catalogo[meio].id == id)
        return meio; //! Caso Base: Encontrou

    // Chamadas Recursivas
    if (sistema.catalogo[meio].id < id)
        return buscaBinariaRecursiva(id, meio + 1, fim); // Direita

    return buscaBinariaRecursiva(id, inicio, meio - 1); // Esquerda
}

//* --- CRUD (Create, Read, Update and Delete) ---

void visualizarMapa()
{
    desenharTitulo("VISAO GERAL DO ARMAZEM");

    printf("         C0      C1      C2      C3      C4   \n");
    printf("       ______________________________________ \n");

    for (int i = 0; i < LINHAS; i++)
    {
        printf("   L%d | ", i);
        for (int j = 0; j < COLUNAS; j++)
        {
            if (sistema.mapa[i][j] == 0)
                printf(" [ . ] "); // Visual limpo para vazio
            else
                printf(" [%03d] ", sistema.mapa[i][j]); // Mostra o ID
        }
        printf(" |\n");
    }
    printf("      |_____________________________________| \n");

    //? Barra de Progresso Visual (Regra de Três simples)
    printf("\n   CAPACIDADE DO ESTOQUE:\n   [");
    int slotsOcupados = sistema.qtdProdutos;
    int slotsTotais = MAX_PRODUTOS;
    int barras = (slotsOcupados * 20) / slotsTotais; // Normaliza para 20 caracteres

    for (int i = 0; i < 20; i++)
    {
        if (i < barras)
            printf("#");
        else
            printf("-");
    }
    printf("] %d/%d Itens\n", slotsOcupados, slotsTotais);
}

void listarProdutosTabela()
{
    desenharTitulo("RELATORIO DE PRECOS");

    if (sistema.qtdProdutos == 0)
    {
        printf("   [!] O sistema esta vazio. Adicione produtos primeiro.\n");
        return;
    }

    printf("   +-----+----------------------+----------------+------------+\n");
    printf("   | ID  | PRODUTO              | CATEGORIA      | PRECO      |\n");
    printf("   +-----+----------------------+----------------+------------+\n");

    for (int i = 0; i < sistema.qtdProdutos; i++)
    {
        Produto p = sistema.catalogo[i];
        printf("   | %03d | %-20s | %-14s | R$ %7.2f |\n",
               p.id, p.nome, p.categoria, p.preco);
    }
    printf("   +-----+----------------------+----------------+------------+\n");
}

void adicionarProduto()
{
    visualizarMapa();
    printf("\n   --- NOVO CADASTRO ---\n");

    if (sistema.qtdProdutos >= MAX_PRODUTOS)
    {
        printf("   [!] ERRO: Armazem lotado.\n");
        return;
    }

    int l, c, id;
    printf("   > Informe a LINHA (0-4): ");
    scanf("%d", &l);
    printf("   > Informe a COLUNA (0-4): ");
    scanf("%d", &c);

    //! Validação de Limites da Matriz (Evita SegFault)
    if (l < 0 || l >= LINHAS || c < 0 || c >= COLUNAS)
    {
        printf("   [!] ERRO: Coordenada invalida.\n");
        return;
    }
    //! Validação de Regra de Negócio (Colisão Espacial)
    if (sistema.mapa[l][c] != 0)
    {
        printf("   [!] ERRO: Posicao ja ocupada pelo ID %d.\n", sistema.mapa[l][c]);
        return;
    }

    printf("   > Informe o ID do Produto: ");
    scanf("%d", &id);

    //! Validação de Unicidade (Busca Binária)
    if (buscaBinariaRecursiva(id, 0, sistema.qtdProdutos - 1) != -1)
    {
        printf("   [!] ERRO: ID ja cadastrado.\n");
        return;
    }

    //? Ponteiro aponta para a próxima vaga livre no vetor 'catalogo'
    Produto *novo = &sistema.catalogo[sistema.qtdProdutos];
    novo->id = id;

    printf("   > Nome do Produto: ");
    limparBuffer(); //? Limpa o \n do scanf do ID antes de ler string
    lerTexto(novo->nome, 40);

    printf("   > Categoria: ");
    lerTexto(novo->categoria, 20);

    printf("   > Preco (R$): ");
    scanf("%f", &novo->preco);

    //? Commit das alterações nas duas estruturas
    sistema.mapa[l][c] = id;
    sistema.qtdProdutos++;

    ordenarCatalogo(); //? Mantém o vetor ordenado para buscas futuras
    salvarDados();
    printf("\n   [OK] PRODUTO CADASTRADO COM SUCESSO!\n");
}

void removerProduto()
{
    listarProdutosTabela();
    if (sistema.qtdProdutos == 0)
    {
        pausa();
        return;
    }

    int id;
    printf("\n   --- REMOCAO DE ITEM ---\n");
    printf("   > Digite o ID para excluir: ");
    scanf("%d", &id);

    int index = buscaBinariaRecursiva(id, 0, sistema.qtdProdutos - 1);
    if (index == -1)
    {
        printf("   [!] ID nao encontrado.\n");
        return;
    }

    Produto p = sistema.catalogo[index];
    printf("   [?] Remover '%s'? (1=SIM, 0=NAO): ", p.nome);
    int op;
    scanf("%d", &op);

    if (op == 1)
    {
        // 1. Remove da Matriz (Espacial)
        for (int i = 0; i < LINHAS; i++)
        {
            for (int j = 0; j < COLUNAS; j++)
            {
                if (sistema.mapa[i][j] == id)
                    sistema.mapa[i][j] = 0;
            }
        }

        // 2. Remove do Vetor (Lógico) - Algoritmo Shift Left
        //? Move todos os elementos à frente uma casa para trás para tapar o buraco
        for (int i = index; i < sistema.qtdProdutos - 1; i++)
        {
            sistema.catalogo[i] = sistema.catalogo[i + 1];
        }

        sistema.qtdProdutos--; //? Reduz o tamanho lógico do vetor
        salvarDados();
        printf("   [OK] ITEM REMOVIDO.\n");
    }
    else
    {
        printf("   [!] Operacao cancelada.\n");
    }
}

void consultarProduto()
{
    desenharTitulo("CONSULTA INTELIGENTE");
    int id;
    printf("   > Digite o ID para buscar: ");
    scanf("%d", &id);

    //? Busca recursiva
    int index = buscaBinariaRecursiva(id, 0, sistema.qtdProdutos - 1);

    if (index != -1)
    {
        Produto p = sistema.catalogo[index];
        printf("\n   +--- RESULTADO -----------------------------+\n");
        printf("   | PRODUTO:   %-30s |\n", p.nome);
        printf("   | CATEGORIA: %-30s |\n", p.categoria);
        printf("   | PRECO:     R$ %-27.2f |\n", p.preco);
        printf("   +-------------------------------------------+\n");
    }
    else
    {
        printf("\n   [!] Produto nao encontrado no sistema.\n");
    }
}

//* --- MENU PRINCIPAL ---

int main()
{
    setlocale(LC_ALL, "C"); //? Força encoding C padrão para evitar bugs de ASCII Art no Windows
    inicializarSistema();
    int opcao = 0;

    do
    {
        limparTela();
        mostrarLogo();

        printf("   +-------------------------------------------------------+\n");
        printf("   |                   PAINEL DE CONTROLE                  |\n");
        printf("   +-------------------------------------------------------+\n");
        printf("   |  [ 1 ] - VISUALIZAR ESTOQUE                           |\n");
        printf("   |  [ 2 ] - TABELA DE PRECOS                             |\n");
        printf("   |  [ 3 ] - ADICIONAR PRODUTO (ENTRADA)                  |\n");
        printf("   |  [ 4 ] - REMOVER PRODUTO (SAIDA)                      |\n");
        printf("   |  [ 5 ] - CONSULTAR POR ID                             |\n");
        printf("   |  [ 6 ] - SAIR E SALVAR DADOS                          |\n");
        printf("   +-------------------------------------------------------+\n");
        printf("\n   >>> ESCOLHA UMA OPCAO: ");

        //! Validação de Input Numérico (Evita loop infinito se digitar letras)
        if (scanf("%d", &opcao) != 1)
        {
            limparBuffer();
            opcao = 0;
        }

        switch (opcao)
        {
        case 1:
            visualizarMapa();
            pausa();
            break;
        case 2:
            listarProdutosTabela();
            pausa();
            break;
        case 3:
            adicionarProduto();
            pausa();
            break;
        case 4:
            removerProduto();
            pausa();
            break;
        case 5:
            consultarProduto();
            pausa();
            break;
        case 6:
            salvarDados();
            printf("\n   [INFO] Sistema encerrado.\n");
            break;
        default:
            printf("\n   [!] Opcao Invalida.\n");
            pausa();
        }

    } while (opcao != 6);

    return 0;
}