/*
 * main.c: programa principal (menu)
 * Para compilar: gcc main.c exchange.c -o exchange
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "exchange.h"

int main(void) {
    Usuario usuarios[MAX_USUARIOS];
    Moeda moedas[MAX_MOEDAS];
    Usuario logado;      
    int total_usuarios;
    int total_moedas;
    int entrou = 0;
    int opcao;

    /* le as moedas primeiro, porque o usuarios.txt tem um saldo para cada moeda */
    total_moedas = ler_moedas(moedas);
    srand(time(NULL));   /* faz o rand() dar numeros diferentes a cada execucao */
    total_usuarios = ler_usuarios(usuarios, total_moedas);

    printf("===== EXCHANGE DE CRIPTOMOEDAS =====\n");

    /* so sai daqui quando o login der certo */
    while (!entrou) {
        entrou = fazer_login(usuarios, total_usuarios, &logado);
    }
    
        /* se for o administrador, abre o menu dele e termina o programa */
    if (entrou == 2) {
        menu_adm(usuarios, &total_usuarios, moedas, total_moedas);
        return 0;
    }

    do {
        printf("\n========== MENU ==========\n");
        printf("1. Consultar saldo\n");
        printf("2. Consultar extrato\n");
        printf("3. Depositar\n");
        printf("4. Sacar\n");
        printf("5. Comprar criptomoedas\n");
        printf("6. Vender criptomoedas\n");
        printf("7. Atualizar cotacao\n");
        printf("8. Sair\n");
        printf("9. Alterar meus dados\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                ver_saldo(&logado, moedas, total_moedas);
                break;
            case 2:
                ver_extrato(&logado);
                break;
            case 3:
                depositar(&logado, usuarios, total_usuarios, moedas, total_moedas);
                break;
            case 4:
                sacar(&logado, usuarios, total_usuarios, moedas, total_moedas);
                break;
            case 5:
                /* parte da dupla: comprar */
                comprar(&logado, usuarios, total_usuarios, moedas, total_moedas);
                break;
            case 6:
                /* parte da dupla: vender */
                vender(&logado, usuarios, total_usuarios, moedas, total_moedas);
                break;
            case 7:
                /* parte da dupla: atualizar cotacao */
                atualizar_cotacao(moedas, total_moedas);
                break;
            case 8:
                atualizar_usuario(usuarios, total_usuarios, &logado, total_moedas);
                gravar_moedas(moedas, total_moedas);
                printf("Dados salvos. Ate logo, %s!\n", logado.nome);
                break;
            case 9:
                alterar_dados(&logado, usuarios, total_usuarios, total_moedas);
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 8);

    return 0;
}
