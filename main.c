/* bibliotecas e linkando o exchange.h pro compilador saber que as funcoes existem */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "exchange.h"

int main(void) {
    Usuario usuarios[MAX_USUARIOS];
    Moeda moedas[MAX_MOEDAS];
    Usuario logado; /* uma struct, dados de quem esta logado */      
    int total_usuarios; /* quantos usuarios estao em uso no vetor */
    int total_moedas; /* quantas moedas estao em uso no vetor */
    int entrou = 0; /* 0 = nao logou; 1 = investidor; 2 = adm */
    int opcao; /* opcao digitada no menu */
 
    /* le as moedas primeiro, porque o usuarios.txt tem um saldo para cada moeda */
    total_moedas = ler_moedas(moedas);
    srand(time(NULL));   /* faz o rand() dar numeros diferentes a cada execucao  pra mudanca da cotacao */
    total_usuarios = ler_usuarios(usuarios, total_moedas); 
    
    printf("===== EXCHANGE DE CRIPTOMOEDAS =====\n");

    /* so sai daqui quando o login der certo !entrou: com entrou = 0, !0 e verdadeiro e repete; com 1 ou 2 sai. &logado: passa o endereco para a funcao preencher os dados de quem entrou */
    while (!entrou) {
        entrou = fazer_login(usuarios, total_usuarios, &logado);
    }
    
    /* se for o administrador, abre o menu dele e termina o programa */
    if (entrou == 2) {
        menu_adm(usuarios, &total_usuarios, moedas, &total_moedas);
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
                ver_saldo(&logado, moedas, total_moedas); /* &logado = endereco da struct, moedas = vetor (sem &) total_moedas = so leitura (copia basta) */
                break;
            case 2:
                ver_extrato(&logado);
                break;
            case 3:
                depositar(&logado, usuarios, total_usuarios, moedas, total_moedas); /* recebe &logado porque altera o saldo, e o vetor usuarios para salvar a mudanca no arquivo */
                break;
            case 4:
                sacar(&logado, usuarios, total_usuarios, moedas, total_moedas);
                break;
            case 5:
                comprar(&logado, usuarios, total_usuarios, moedas, total_moedas); /* atualizar cotacao: so mexe no vetor de moedas */
                break;
            case 6:
                vender(&logado, usuarios, total_usuarios, moedas, total_moedas);
                break;
            case 7:
                atualizar_cotacao(moedas, total_moedas); /* atualizar cotacao: so mexe no vetor de moedas */
                break;
            case 8: /* ao sair ele copia o logado de volta para o vetor e salva os arquivos, logado.nome usa ponto porque aqui logado e a struct em si (dentro das funcoes, que recebem ponteiro) */
                atualizar_usuario(usuarios, total_usuarios, &logado, total_moedas); 
                gravar_moedas(moedas, total_moedas);
                printf("Dados salvos. Ate logo, %s!\n", logado.nome);
                break;
            case 9:
                alterar_dados(&logado, usuarios, total_usuarios, total_moedas);
                break;
            default: /* qualquer numero que nao seja de 1 a 9 */
                printf("Opcao invalida!\n");
        }
    } while (opcao != 8);
    return 0;
}
