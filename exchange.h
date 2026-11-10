/*tipos e lista das funcoes*/

#ifndef EXCHANGE_H
#define EXCHANGE_H

#include <stdio.h>

#define MAX_USUARIOS 100
#define MAX_MOEDAS 10
#define ADM_CPF "00000000000"
#define ADM_SENHA "999999"

/* Dados de criptomoeda */
typedef struct {
	char sigla[6];
	char nome[30];
	float cotacao;
	float taxa_compra;
	float taxa_venda;
} Moeda;

/* Dados de investidor */
typedef struct {
	char nome[50];
	char cpf[12];
	char senha[7];
	float saldo_reais;
	float saldo_moeda[MAX_MOEDAS];
} Usuario;

/* Moedas: arquivo e vetor */
int ler_moedas(Moeda moedas[]);
void gravar_moedas(Moeda moedas[], int total_moedas);

/* Usuarios: arquivo e struct */
int ler_usuario(FILE *arquivo, Usuario *usuario, int total_moedas);
void escrever_usuario(FILE *arquivo, Usuario *usuario, int total_moedas);
int ler_usuarios(Usuario usuarios[], int total_moedas);
void gravar_usuarios(Usuario usuarios[], int total_usuarios, int total_moedas);
void atualizar_usuario(Usuario usuarios[], int total_usuarios,
                       Usuario *logado, int total_moedas);

/* Extrato */
void anotar_no_extrato(Usuario *usuario, char sinal, float valor, char sigla[],
                       float cotacao, float taxa, Moeda moedas[], int total_moedas);

/* Ajudantes */
int cpf_valido(char cpf[]);
int senha_valida(char senha[]);
int procurar_usuario(Usuario usuarios[], int total_usuarios, char cpf[]);
int pedir_senha(Usuario *usuario);
void mostrar_cpf(char cpf[]);
void mostrar_saldos(Usuario *usuario, Moeda moedas[], int total_moedas);

/* Opcoes do menu */
int fazer_login(Usuario usuarios[], int total_usuarios, Usuario *logado);
void ver_saldo(Usuario *logado, Moeda moedas[], int total_moedas);
void ver_extrato(Usuario *logado);
void depositar(Usuario *logado, Usuario usuarios[], int total_usuarios,
               Moeda moedas[], int total_moedas);
void sacar(Usuario *logado, Usuario usuarios[], int total_usuarios,
           Moeda moedas[], int total_moedas);
void alterar_dados(Usuario *logado, Usuario usuarios[], int total_usuarios,
                   int total_moedas);
void atualizar_cotacao(Moeda moedas[], int total_moedas);
void comprar(Usuario *logado, Usuario usuarios[], int total_usuarios,
             Moeda moedas[], int total_moedas);
void vender(Usuario *logado, Usuario usuarios[], int total_usuarios,
            Moeda moedas[], int total_moedas);
void menu_adm(Usuario usuarios[], int *total_usuarios,
              Moeda moedas[], int *total_moedas);
void adm_cadastrar_moeda(Usuario usuarios[], int total_usuarios,
                         Moeda moedas[], int *total_moedas);
void adm_excluir_moeda(Usuario usuarios[], int total_usuarios,
                       Moeda moedas[], int *total_moedas);
void adm_ver_saldo(Usuario usuarios[], int total_usuarios,
                   Moeda moedas[], int total_moedas);
void adm_ver_extrato(Usuario usuarios[], int total_usuarios);
void adm_cadastrar_investidor(Usuario usuarios[], int *total_usuarios,
                  	int total_moedas);
void adm_excluir_investidor(Usuario usuarios[], int *total_usuarios,
                            Moeda moedas[], int total_moedas);                    
#endif
