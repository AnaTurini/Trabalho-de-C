/* exchange.c: codigo das funcoes*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "exchange.h"

/* MOEDAS  */

/* Le o moedas.txt e devolve quantas moedas leu. Cada linha: SIGLA COTACAO TAXA_COMPRA TAXA_VENDA NOME */
int ler_moedas(Moeda moedas[]) {
    FILE *arquivo;
    int total = 0;

    arquivo = fopen("moedas.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo moedas.txt!\n");
        return 0;
    }

    while (total < MAX_MOEDAS &&
           fscanf(arquivo, "%s %f %f %f %[^\n]", moedas[total].sigla,
                  &moedas[total].cotacao, &moedas[total].taxa_compra,
                  &moedas[total].taxa_venda, moedas[total].nome) == 5) {
        total++;
    }

    fclose(arquivo);
    return total;
}

/* Apaga o moedas.txt e grava tudo de novo */
void gravar_moedas(Moeda moedas[], int total_moedas) {
    FILE *arquivo;
    int i;

    arquivo = fopen("moedas.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo moedas.txt!\n");
        return;
    }

    for (i = 0; i < total_moedas; i++) {
        fprintf(arquivo, "%s %.2f %.2f %.2f %s\n", moedas[i].sigla,
                moedas[i].cotacao, moedas[i].taxa_compra,
                moedas[i].taxa_venda, moedas[i].nome);
    }

    fclose(arquivo);
}

/*  USUARIOS  */
/* Cada linha do usuarios.txt: CPF SENHA REAIS SALDO_DE_CADA_MOEDA NOME*/

int ler_usuario(FILE *arquivo, Usuario *usuario, int total_moedas) {
    int i;

    if (fscanf(arquivo, "%s %s %f", usuario->cpf, usuario->senha,
               &usuario->saldo_reais) != 3) {
        return 0;
    }
    for (i = 0; i < total_moedas; i++) {
        fscanf(arquivo, "%f", &usuario->saldo_moeda[i]);
    }
    fscanf(arquivo, " %[^\n]", usuario->nome);
    return 1;
}

/* Passa UMA struct para uma linha do arquivo */
void escrever_usuario(FILE *arquivo, Usuario *usuario, int total_moedas) {
    int i;

    fprintf(arquivo, "%s %s %.2f", usuario->cpf, usuario->senha,
            usuario->saldo_reais);
    for (i = 0; i < total_moedas; i++) {
        fprintf(arquivo, " %.6f", usuario->saldo_moeda[i]);
    }
    fprintf(arquivo, " %s\n", usuario->nome);
}

/* Le todos os usuarios do arquivo para o vetor e devolve quantos leu */
int ler_usuarios(Usuario usuarios[], int total_moedas) {
    FILE *arquivo;
    int total = 0;

    arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo usuarios.txt!\n");
        return 0;
    }

    while (total < MAX_USUARIOS && ler_usuario(arquivo, &usuarios[total], total_moedas)) {
        total++;
    }

    fclose(arquivo);
    return total;
}

/* Apaga o usuarios.txt e grava o vetor inteiro de novo */
void gravar_usuarios(Usuario usuarios[], int total_usuarios, int total_moedas) {
    FILE *arquivo;
    int i;

    arquivo = fopen("usuarios.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo usuarios.txt!\n");
        return;
    }

    for (i = 0; i < total_usuarios; i++) {
        escrever_usuario(arquivo, &usuarios[i], total_moedas);
    }

    fclose(arquivo);
}

/* Copia a struct do usuario logado para o vetor (achando pelo CPF) e salva no arquivo. Chamar sempre que mudar algum dado do logado. */
void atualizar_usuario(Usuario usuarios[], int total_usuarios,
                       Usuario *logado, int total_moedas) {
    int posicao;

    posicao = procurar_usuario(usuarios, total_usuarios, logado->cpf);
    if (posicao == -1) {
        printf("Erro: usuario nao encontrado!\n");
        return;
    }

    usuarios[posicao] = *logado;   /* copia todos os campos da struct */
    gravar_usuarios(usuarios, total_usuarios, total_moedas);
}

/*  EXTRATO  */

/* Escreve uma linha no fim do extrato do usuario (extrato_CPF.txt).*/
void anotar_no_extrato(Usuario *usuario, char sinal, float valor, char sigla[],
                       float cotacao, float taxa, Moeda moedas[], int total_moedas) {
    FILE *arquivo;
    char nome_arquivo[30];
    time_t agora;
    struct tm *data;
    int i;

    sprintf(nome_arquivo, "extrato_%s.txt", usuario->cpf);

    arquivo = fopen(nome_arquivo, "a");   /* "a" escreve no final sem apagar */
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s!\n", nome_arquivo);
        return;
    }

    /* pega a data e a hora do computador */
    agora = time(NULL);
    data = localtime(&agora);

    fprintf(arquivo, "%02d-%02d-%04d %02d:%02d ",
            data->tm_mday, data->tm_mon + 1, data->tm_year + 1900,
            data->tm_hour, data->tm_min);
    fprintf(arquivo, "%c %.2f %-4s CT: %.2f TX: %.2f REAL: %.2f",
            sinal, valor, sigla, cotacao, taxa, usuario->saldo_reais);
    for (i = 0; i < total_moedas; i++) {
        fprintf(arquivo, " %s: %.4f", moedas[i].sigla, usuario->saldo_moeda[i]);
    }
    fprintf(arquivo, "\n");

    fclose(arquivo);
}

/*  AJUDANTES  */

/* Devolve 1 se o CPF tem 11 numeros, senao 0 */
int cpf_valido(char cpf[]) {
    int i;

    if (strlen(cpf) != 11) {
        return 0;
    }
    for (i = 0; i < 11; i++) {
        if (!isdigit(cpf[i])) {
            return 0;
        }
    }
    return 1;
}

/* Devolve 1 se a senha tem 6 numeros, senao 0 */
int senha_valida(char senha[]) {
    int i;

    if (strlen(senha) != 6) {
        return 0;
    }
    for (i = 0; i < 6; i++) {
        if (!isdigit(senha[i])) {
            return 0;
        }
    }
    return 1;
}

/* Devolve a posicao do usuario no vetor, ou -1 se o CPF nao existe */
int procurar_usuario(Usuario usuarios[], int total_usuarios, char cpf[]) {
    int i;

    for (i = 0; i < total_usuarios; i++) {
        if (strcmp(usuarios[i].cpf, cpf) == 0) {
            return i;
        }
    }
    return -1;
}


int pedir_senha(Usuario *usuario) {
    char senha[20];

    printf("Digite sua senha: ");
    scanf("%19s", senha);

    if (strcmp(usuario->senha, senha) == 0) {
        return 1;
    }
    printf("Senha incorreta!\n");
    return 0;
}

/* Mostra o CPF com pontos e traco: 123.456.789-00 */
void mostrar_cpf(char cpf[]) {
    int i;

    for (i = 0; i < 11; i++) {
        if (i == 3 || i == 6) {
            printf(".");
        }
        if (i == 9) {
            printf("-");
        }
        printf("%c", cpf[i]);
    }
    printf("\n");
}

/* Mostra nome, CPF e todos os saldos */
void mostrar_saldos(Usuario *usuario, Moeda moedas[], int total_moedas) {
    int i;

    printf("\nNome: %s\n", usuario->nome);
    printf("CPF: ");
    mostrar_cpf(usuario->cpf);
    printf("Reais: %.2f\n", usuario->saldo_reais);
    for (i = 0; i < total_moedas; i++) {
        printf("%s: %.4f\n", moedas[i].nome, usuario->saldo_moeda[i]);
    }
}

/*  OPCOES DO MENU = */

int fazer_login(Usuario usuarios[], int total_usuarios, Usuario *logado) {
    char cpf[20];
    char senha[20];
    int posicao;

    printf("\n===== LOGIN =====\n");
    printf("CPF (so numeros): ");
    scanf("%19s", cpf);
    printf("Senha (6 numeros): ");
    scanf("%19s", senha);

    if (!cpf_valido(cpf)) {
        printf("CPF invalido! Digite 11 numeros, sem pontos e traco.\n");
        return 0;
    }
    if (!senha_valida(senha)) {
        printf("Senha invalida! Digite 6 numeros.\n");
        return 0;
    }

    posicao = procurar_usuario(usuarios, total_usuarios, cpf);
    if (posicao == -1) {
        printf("CPF ou senha incorretos!\n");
        return 0;
    }
    if (strcmp(usuarios[posicao].senha, senha) != 0) {
        printf("CPF ou senha incorretos!\n");
        return 0;
    }

    *logado = usuarios[posicao];   /* copia os dados para a struct do logado */
    printf("Bem-vindo(a), %s!\n", logado->nome);
    return 1;
}

void ver_saldo(Usuario *logado, Moeda moedas[], int total_moedas) {
    printf("\n===== SALDO =====\n");
    if (pedir_senha(logado)) {
        mostrar_saldos(logado, moedas, total_moedas);
    }
}

/* Mostra o arquivo extrato_CPF.txt linha por linha */
void ver_extrato(Usuario *logado) {
    FILE *arquivo;
    char nome_arquivo[30];
    char linha[300];

    printf("\n===== EXTRATO =====\n");
    if (!pedir_senha(logado)) {
        return;
    }

    printf("\nNome: %s\n", logado->nome);
    printf("CPF: ");
    mostrar_cpf(logado->cpf);

    sprintf(nome_arquivo, "extrato_%s.txt", logado->cpf);
    arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Voce ainda nao fez nenhuma operacao.\n");
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        printf("%s", linha);
    }

    fclose(arquivo);
}

void depositar(Usuario *logado, Usuario usuarios[], int total_usuarios,
               Moeda moedas[], int total_moedas) {
    float valor;

    printf("\n===== DEPOSITO =====\n");
    printf("Quanto deseja depositar? R$ ");
    scanf("%f", &valor);

    if (valor <= 0) {
        printf("Valor invalido! Digite um valor maior que zero.\n");
        return;
    }

    logado->saldo_reais = logado->saldo_reais + valor;
    atualizar_usuario(usuarios, total_usuarios, logado, total_moedas);
    anotar_no_extrato(logado, '+', valor, "REAL", 0, 0, moedas, total_moedas);

    printf("Deposito feito!\n");
    mostrar_saldos(logado, moedas, total_moedas);
}

void sacar(Usuario *logado, Usuario usuarios[], int total_usuarios,
           Moeda moedas[], int total_moedas) {
    float valor;

    printf("\n===== SAQUE =====\n");
    printf("Saldo em reais: R$ %.2f\n", logado->saldo_reais);
    printf("Quanto deseja sacar? R$ ");
    scanf("%f", &valor);

    if (valor <= 0) {
        printf("Valor invalido! Digite um valor maior que zero.\n");
        return;
    }
    if (valor > logado->saldo_reais) {
        printf("Saldo insuficiente!\n");
        return;
    }

    logado->saldo_reais = logado->saldo_reais - valor;
    atualizar_usuario(usuarios, total_usuarios, logado, total_moedas);
    anotar_no_extrato(logado, '-', valor, "REAL", 0, 0, moedas, total_moedas);

    printf("Saque feito!\n");
    mostrar_saldos(logado, moedas, total_moedas);
}

/* Deixa o usuario trocar o proprio nome ou a senha. O CPF nao muda, porque ele identifica o usuario e o arquivo de extrato. */
void alterar_dados(Usuario *logado, Usuario usuarios[], int total_usuarios,
                   int total_moedas) {
    int opcao;
    char nova_senha[20];
    char confirmacao[20];

    printf("\n===== ALTERAR MEUS DADOS =====\n");
    if (!pedir_senha(logado)) {
        return;
    }

    printf("1. Alterar nome\n");
    printf("2. Alterar senha\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            printf("Novo nome: ");
            scanf(" %49[^\n]", logado->nome);
            break;
        case 2:
            printf("Nova senha (6 numeros): ");
            scanf("%19s", nova_senha);
            if (!senha_valida(nova_senha)) {
                printf("Senha invalida! Digite 6 numeros.\n");
                return;
            }
            printf("Repita a nova senha: ");
            scanf("%19s", confirmacao);
            if (strcmp(nova_senha, confirmacao) != 0) {
                printf("As senhas nao sao iguais!\n");
                return;
            }
            strcpy(logado->senha, nova_senha);
            break;
        default:
            printf("Opcao invalida!\n");
            return;
    }

    atualizar_usuario(usuarios, total_usuarios, logado, total_moedas);
    printf("Dados atualizados!\n");
}
