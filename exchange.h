/* arquivo .h serve pra mostrar as funcoes que existem e os tipos delas, e o codigo ta la no exchange.c */

/* tipos e lista das funcoes */
#include <stdio.h>
#ifndef EXCHANGE_H /* se o exchange nao foi definido */
#define EXCHANGE_H
#define MAX_USUARIOS 100
#define MAX_MOEDAS 10
#define ADM_CPF "00000000000"
#define ADM_SENHA "999999"

/* dados de criptomoeda */
typedef struct { /* o struct vai juntar varios dados em um pacote so*/
	char sigla[6]; 
	char nome[30];
	float cotacao;
	float taxa_compra;
	float taxa_venda;
} Moeda; /*  <------ nome do struct, que e moeda, pra nao ter que escrever struct toda hora */

/* dados de investidor */
typedef struct { /* o struct vai juntar varios dados em um pacote so*/
	char nome[50];
	char cpf[12];
	char senha[7];
	float saldo_reais;
	float saldo_moeda[MAX_MOEDAS]; /* se o btc e moedas[0], o saldo de btc do usuario e saldo_moeda[0] por isso qnd excluir uma moeda a gente puxa os saldos de todos os usuarios */
} Usuario; /*  <------ nome do struct, que e usuario, pra nao ter que escrever struct toda hora */
 
/* moedas: arquivo e vetor */
int ler_moedas(Moeda moedas[]); /* devolve quantas moedas leu */
void gravar_moedas(Moeda moedas[], int total_moedas); /* grava o vetor no moedas .txt */

/* usuarios: arquivo e struct */
int ler_usuario(FILE *arquivo, Usuario *usuario, int total_moedas); /* le uma linha para uma struct */
void escrever_usuario(FILE *arquivo, Usuario *usuario, int total_moedas); /* escreve uma struct em uma linha */
int ler_usuarios(Usuario usuarios[], int total_moedas); /* le o arquivo todo, devolve quantos */
void gravar_usuarios(Usuario usuarios[], int total_usuarios, int total_moedas); /* gravar o usuario e saldo */
void atualizar_usuario(Usuario usuarios[], int total_usuarios, Usuario *logado, int total_moedas); /* atualizacao pro usuario caso compre venda etc */

/* extrato */
void anotar_no_extrato(Usuario *usuario, char sinal, float valor, char sigla[], float cotacao, float taxa, Moeda moedas[], int total_moedas); /* extrato do usuario que foi pedido */

/* apoiadores, checar se o cpf ta certo etc */
int cpf_valido(char cpf[]); /* 1 se tem 11 numeros, senao 0 */
int senha_valida(char senha[]); /* 1 se tem 6 numeros, senao 0 */
int procurar_usuario(Usuario usuarios[], int total_usuarios, char cpf[]); /* posicao no vetor ou -1 */
int pedir_senha(Usuario *usuario); /* pede e confere a senha */
void mostrar_cpf(char cpf[]);
void mostrar_saldos(Usuario *usuario, Moeda moedas[], int total_moedas);

/* opcoes do menu pr usuario (deixar separado pra ficar legivel pra explicar pra prof */
int fazer_login(Usuario usuarios[], int total_usuarios, Usuario *logado); /* 0 = falhou, 1 = investidor, 2 = adm */
 
void ver_saldo(Usuario *logado, Moeda moedas[], int total_moedas);

void ver_extrato(Usuario *logado);

void depositar(Usuario *logado, Usuario usuarios[], int total_usuarios, Moeda moedas[], int total_moedas);

void sacar(Usuario *logado, Usuario usuarios[], int total_usuarios, Moeda moedas[], int total_moedas);

void alterar_dados(Usuario *logado, Usuario usuarios[], int total_usuarios, int total_moedas);

void atualizar_cotacao(Moeda moedas[], int total_moedas);

void comprar(Usuario *logado, Usuario usuarios[], int total_usuarios, Moeda moedas[], int total_moedas);

void vender(Usuario *logado, Usuario usuarios[], int total_usuarios, Moeda moedas[], int total_moedas);

/* aq ja e as funcoes do admin */

void menu_adm(Usuario usuarios[], int *total_usuarios, Moeda moedas[], int *total_moedas);

void adm_cadastrar_moeda(Usuario usuarios[], int total_usuarios, Moeda moedas[], int *total_moedas);

void adm_excluir_moeda(Usuario usuarios[], int total_usuarios, Moeda moedas[], int *total_moedas);

void adm_ver_saldo(Usuario usuarios[], int total_usuarios, Moeda moedas[], int total_moedas);

void adm_ver_extrato(Usuario usuarios[], int total_usuarios);

void adm_cadastrar_investidor(Usuario usuarios[], int *total_usuarios, int total_moedas);

void adm_excluir_investidor(Usuario usuarios[], int *total_usuarios, Moeda moedas[], int total_moedas);
                   
#endif /* fim da protecao contra inclusao dupla */
