/******************************************************************************
Aluno(a): Enrico Martins Mantoan Zoratto
Matrícula: 222006688
Declaro que eu sou o(a) autor(a) deste trabalho e que o código não foi copiado
de outra pessoa nem repassado para alguém. Estou ciente de que poderei sofrer
penalizações na avaliação em caso de detecção de plágio.
*******************************************************************************/

// Decalaração de bibliotecas 
#include <stdio.h>  
#include "222006688.h"
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <locale.h>

//Função principal
    int main()
    {
        charset();
        setlocale(LC_CTYPE, ".utf8");

        int opc;
        // Impressão do Menu e leitura de escolha do usuario
        do
        {
            menu();
        
                while (scanf("%d", &opc) != 1) 
                { //loop para verificar se não foi digitado um caracter invalido, se for valido continua para o loop seguinte
                // Limpa o buffer de entrada
                int c;
                while ((c = getchar()) != '\n' && c != EOF);

                printf("Comando inexistente! Digite novamente:\n");
                }
        
    

            // condicional que irá executar o comando do usuario
            switch (opc)
            {
            case CONSULTAR_MINIUSINA:
                cons_Miniusinas(); // função 1
                break;
            case CONSULTAR_CONSUMIDOR:
                cons_Consumidor(); // função 2
                break;
            case LISTAR_MINIUSINAS:
                list_miniusinas(); // função 3
                break;
            case LISTAR_MINIUSINAS_OCDGOCE:
                Listar_MiniUsinas_DEC(); // função 4
                break;
            case LISTAR_RAS_ODQMU:
                listaRAmini(); // função 5
                break;
            case LISTAR_RAS_ODQC:
                ListarRACONT(); // função 6
                break;
            case LISTAR_RAS_ODCG:
                ListaRACAP(); // função 7
                break;
            case LISTAR_RAS_ODPED:
                ListaRAPERCENTO(); // função 8
                break;
            case SAIR:
                printf("Programa Finalizado!\n");
                break;
            default:
                printf("Opção Inexistente!\n");
                break;
            }
        } 
    while (opc != SAIR);
}
