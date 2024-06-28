#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <locale.h>

// definições de tamanho usados no codigo
#define TAM_MAX_CNPJ_CPF 20
#define TAM_MAX_NOME 100
#define TAM_MAX_CAPACIDADE 10
#define TAM_MAX_ID 5
#define TAM_MAX_STATUS 15
#define TAM_MAX_IDCONSUMIDOR 20
#define TAM_MAX_DATA 11
#define TAM_MAX_ENERGIA 7
#define TAM_MAX_RA 4

// Defines
#define CONSULTAR_MINIUSINA 1
#define CONSULTAR_CONSUMIDOR 2
#define LISTAR_MINIUSINAS 3
#define LISTAR_MINIUSINAS_OCDGOCE 4 // Lista de As Miniusinas em Operação por Ordem Crescente de Capacidade de Geração de Energia
#define LISTAR_RAS_ODQMU 5          //  Listar as RAs por ordem decrescente de quantidade de miniusinas
#define LISTAR_RAS_ODQC 6           // Listar as RAs por ordem crescente de número de contratos
#define LISTAR_RAS_ODCG 7           //  Listar as RAs por ordem decrescente de capacidade de geração
#define LISTAR_RAS_ODPED 8          // Listar as RAs por ordem decrescente de percentual de energia disponível
#define SAIR 9
// função do menu
void menu()
{

    printf("....... MENU DE OPCOES SINGDF .......\n");
    printf(" \n");
    printf(" \n");
    printf("1) Consultar miniusina cadastrada\n");
    printf("2) Consultar consumidor cadastrado\n");
    printf("3) Listar as miniusinas cadastradas\n");
    printf("4) Listar as miniusinas em operacao por ordem decrescente de capacidade de geracao de energia\n");
    printf("5) Listar as RAs por ordem decrescente de quantidade de miniusinas\n");
    printf("6) Listar as RAs por ordem crescente de numero de contratos\n");
    printf("7) Listar as RAs por ordem decrescente de capacidade de geracao\n");
    printf("8) Listar as RAs por ordem decrescente de percentual de energia disponivel\n");
    printf("9) Sair do programa\n\n");
}

// structs usadas para ler os arquivos usados no decorrer do codigo
struct miniusinas
{
    char cnpj[TAM_MAX_CNPJ_CPF];
    char nome[TAM_MAX_NOME];
    char capacidade[TAM_MAX_CAPACIDADE];
    char id[TAM_MAX_ID];
    char status[TAM_MAX_STATUS];
};

struct contratos
{
    char id_contratos[TAM_MAX_ID];
    char idconsumidores_contratos[TAM_MAX_IDCONSUMIDOR];
    char cnpj_cpf_contratos[TAM_MAX_CNPJ_CPF];
    char data_contratos[TAM_MAX_DATA];
    char energia_contratos[TAM_MAX_ENERGIA];
};

struct consumidores
{
    char cnpj_cpf_consumidor[TAM_MAX_CNPJ_CPF];
    char nome_consumidor[TAM_MAX_NOME];
    char RA_consumidor[TAM_MAX_RA];
};

struct ra
{
    char id_ra[TAM_MAX_ID];
    char nome_ra[TAM_MAX_NOME];
    char miniusinas[5];
    char capacidade_ra[20];
    char quant[20];
    char disponivel[20];
    char gasta[20];
};

// função usada para validar cnpj
bool validarCNPJ(char *cnpj)
{
    // Removendo caracteres não numéricos
    char cnpjNumerico[15];
    int j = 0;
    for (int i = 0; i < strlen(cnpj); i++)
    {
        // Número ou não número
        if (isdigit(cnpj[i]))
        {
            cnpjNumerico[j] = cnpj[i];
            j++;
        }
    }
    cnpjNumerico[j] = '\0';
    // Não contando com o caractere NULL
    if (strlen(cnpjNumerico) != 14)
    {
        return false;
    }
    int digito1 = 0, digito2 = 0;
    int multiplicador1 = 5, multiplicador2 = 6;
    // Cálculo do primeiro dígito verificador
    for (int i = 0; i < 12; i++)
    {
        digito1 += (cnpjNumerico[i] - '0') * multiplicador1;
        // multiplicador1 = multiplicador1 - 1
        multiplicador1--;
        if (multiplicador1 < 2)
        {
            multiplicador1 = 9;
        }
    }
    digito1 = 11 - (digito1 % 11);
    if (digito1 >= 10)
    {
        digito1 = 0;
    }
    // Cálculo do segundo dígito verificador
    for (int i = 0; i < 13; i++)
    {
        digito2 += (cnpjNumerico[i] - '0') * multiplicador2;
        // multiplicador2 = multiplicador2 - 1
        multiplicador2--;
        if (multiplicador2 < 2)
        {
            multiplicador2 = 9;
        }
    }
    digito2 = 11 - (digito2 % 11);
    if (digito2 >= 10)
    {
        digito2 = 0;
    }
    // Verificação dos dígitos verificadores
    if ((cnpjNumerico[12] - '0') == digito1 && (cnpjNumerico[13] - '0') == digito2)
    {
        return true;
    }
    return false;
}

// função usada para validar cpf
bool validarCPF(const char *cpf)
{
    // Remover caracteres não numéricos do CPF
    char cpfNumerico[12];
    int j = 0;
    for (int i = 0; i < strlen(cpf); i++)
    {
        // função que é usada para verificar se o caractere passado é um dígito ou não.
        if (isdigit(cpf[i]))
        {
            cpfNumerico[j++] = cpf[i];
        }
    }
    cpfNumerico[j] = '\0';

    // Verificar se o CPF possui 11 dígitos
    if (strlen(cpfNumerico) != 11)
    {
        return false;
    }

    // Verificar se todos os dígitos são iguais (caso especial)
    bool digitosIguais = true;
    for (int i = 1; i < 11; i++)
    {
        if (cpfNumerico[i] != cpfNumerico[0])
        {
            digitosIguais = false;
            break;
        }
    }
    if (digitosIguais)
    {
        return false;
    }

    // Validar o primeiro dígito verificador
    int soma = 0;
    for (int i = 0; i < 9; i++)
    {
        soma += (cpfNumerico[i] - '0') * (10 - i);
    }
    int digito1 = (soma * 10) % 11;
    if (digito1 == 10)
    {
        digito1 = 0;
    }
    if (digito1 != (cpfNumerico[9] - '0'))
    {
        return false;
    }

    // Validar o segundo dígito verificador
    soma = 0;
    for (int i = 0; i < 10; i++)
    {
        soma += (cpfNumerico[i] - '0') * (11 - i);
    }
    int digito2 = (soma * 10) % 11;
    if (digito2 == 10)
    {
        digito2 = 0;
    }
    if (digito2 != (cpfNumerico[10] - '0'))
    {
        return false;
    }

    return true;
}

void charset()
{
#if defined(_WIN32) || defined(_WIN64)
    system("chcp 65001");
#endif
}

// função para consultar miniusinas
// função 1
void cons_Miniusinas()
{
    FILE *arquivo_miniusina;                // arquivos
    FILE *arquivo_contratos;                // arquivos
    char cnpj_consultado[20];               // variável nova
    struct miniusinas cadastradas;          // variável nova usando a struct como base
    int flag = 0;                           // variável para verificação
    struct contratos contratos_cadastrados; // variável nova usando a struct como base

    printf("Digite um cnpj:");
    scanf("%s", cnpj_consultado);
    printf("\n");

    if (validarCNPJ(cnpj_consultado)) // função validadora
    {
        arquivo_miniusina = fopen("miniusinas.csv", "r");
        if (arquivo_miniusina != NULL) // se o arquivo existir
        {
            // irá apagar a primeira linha do arquivo
            char primeira_linha[1000];
            fscanf(arquivo_miniusina, "%[^\n]\n", primeira_linha);

            while (!feof(arquivo_miniusina))
            {
                fscanf(arquivo_miniusina, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                cadastradas.cnpj,
                cadastradas.nome,
                cadastradas.capacidade,
                cadastradas.id,
                cadastradas.status);

                // comparação de strings retornando 0 ou 1
                if (strcmp(cnpj_consultado, cadastradas.cnpj) == 0)
                {
                    printf("MINIUSINA\n\n");
                    printf("CNPJ: %s\n", cadastradas.cnpj);
                    printf("Nome: %s\n", cadastradas.nome);
                    printf("Capacidade: %s\n", cadastradas.capacidade);
                    printf("ID: %s\n", cadastradas.id);
                    printf("Status: %s\n", cadastradas.status);
                    printf("\n");
                    flag = 1;
                }
            }
            // não foi bem sucedido
            if (flag == 0)
            {
                printf("O cnpj digitado nao consta no arquivo\n");
                printf("\n");
            }
            // foi bem sucedido
            else if (flag == 1)
            {
                arquivo_contratos = fopen("contratos.csv", "r");
                // verifica se o arquivo existe
                if (arquivo_contratos != NULL)
                {
                    // apaga a primeira linha do arquivo
                    char primeiralinhac[100];
                    fscanf(arquivo_contratos, "%[^\n]\n", primeiralinhac);
                    // enquanto o arquivo estiver aberto
                    while (!feof(arquivo_contratos))
                    {
                        fscanf(arquivo_contratos, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                        contratos_cadastrados.id_contratos,
                        contratos_cadastrados.idconsumidores_contratos,
                        contratos_cadastrados.cnpj_cpf_contratos,
                        contratos_cadastrados.data_contratos,
                        contratos_cadastrados.energia_contratos);
                        // comparação de strings
                        if (strcmp(cnpj_consultado, contratos_cadastrados.cnpj_cpf_contratos) == 0)
                        {
                            printf("CONTRATOS\n\n");
                            printf("ID: %s\n", contratos_cadastrados.id_contratos);
                            printf("ID CONSUMIDOR: %s\n", contratos_cadastrados.idconsumidores_contratos);
                            printf("CNPJ: %s\n", contratos_cadastrados.cnpj_cpf_contratos);
                            printf("DATA DE INICIO: %s\n", contratos_cadastrados.data_contratos);
                            printf("ENERGIA: %s\n", contratos_cadastrados.energia_contratos);
                            printf("\n");
                        }
                    }
                }
                // se o arquivo não existir
                else if (arquivo_contratos == NULL)
                {
                    printf("arquivo de contratos nao existe\n");
                }
            }
        }
        // se o arquivo não existir
        else if (arquivo_miniusina == NULL)
        {
            printf("Arquivo nao encontrado\n");
            printf("\n");
        }
    }
    else
    {
        printf("CNPJ invalido\n");
        printf("\n");
    }
    // fecha ambos os arquivos
    fclose(arquivo_miniusina);
    fclose(arquivo_contratos);
}

// função para verificar o consumidor
// Função 2
void cons_Consumidor()
{
    FILE *arquivo_consumidor;
    FILE *arquivo_contratos;
    struct consumidores consumidores_cadastradas;
    struct contratos contratos_cadastrados;
    char cnpj_consultado[20]; // variavel para cnpj escrito
    char cpf_consultado[20];  // variavel para cpf escrito
    int busca;
    int flag = 0;

    printf("Digite a opcao:\n");
    printf("1)CNPJ\n");
    printf("2)CPF\n");
    scanf("%d", &busca);

    arquivo_consumidor = fopen("consumidores.csv", "r");
    arquivo_contratos = fopen("contratos.csv", "r");
    if (busca == 1)
    {

        printf("Digite o cnpj para ser consultado: ");
        scanf("%s", cnpj_consultado);
        printf("\n");
        // função para validar o cnpj
        if (validarCNPJ(cnpj_consultado))
        {
            if (arquivo_consumidor != NULL)
            {
                // apaga a primeira linha
                char primeiralinha[100];
                fscanf(arquivo_consumidor, "%[^\n]\n", primeiralinha);

                while (!feof(arquivo_consumidor))
                {
                    fscanf(arquivo_consumidor, "%[^;];%[^;];%[^\n]\n",
                           consumidores_cadastradas.cnpj_cpf_consumidor,
                           consumidores_cadastradas.nome_consumidor,
                           consumidores_cadastradas.RA_consumidor);
                    // comparação de strings
                    if ((strcmp(consumidores_cadastradas.cnpj_cpf_consumidor, cnpj_consultado) == 0))
                    {
                        printf("CONSUMIDOR\n\n");
                        printf("CNPJ: %s\n", consumidores_cadastradas.cnpj_cpf_consumidor);
                        printf("NOME CONSUMIDOR: %s\n", consumidores_cadastradas.nome_consumidor);
                        printf("RA DO CONSUMIDOR : %s\n", consumidores_cadastradas.RA_consumidor);
                        printf("\n");
                        // verifica que existe
                        flag = 1;
                        break;
                    }
                }
                // verifica que não existe
                if (flag == 0)
                {
                    printf("O CNPJ nao consta no arquivo\n");
                }
                // se existe
                else if (flag == 1)
                {
                    if (arquivo_contratos != NULL)
                    {
                        char primeiralinhac1[100];
                        fscanf(arquivo_contratos, "%[^\n]\n", primeiralinhac1);
                        fflush(stdin);
                        // enquanto o arquivo estiver aberto
                        while (!feof(arquivo_contratos))
                        {
                            fscanf(arquivo_contratos, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                                   contratos_cadastrados.id_contratos,
                                   contratos_cadastrados.idconsumidores_contratos,
                                   contratos_cadastrados.cnpj_cpf_contratos,
                                   contratos_cadastrados.data_contratos,
                                   contratos_cadastrados.energia_contratos);

                            // comparação de strings

                            if (strcmp(cnpj_consultado, contratos_cadastrados.idconsumidores_contratos) == 0)
                            {
                                printf("CONTRATOS\n\n");
                                printf("ID: %s\n", contratos_cadastrados.id_contratos);
                                printf("ID CONSUMIDOR: %s\n", contratos_cadastrados.idconsumidores_contratos);
                                printf("CNPJ: %s\n", contratos_cadastrados.cnpj_cpf_contratos);
                                printf("DATA DE INICIO: %s\n", contratos_cadastrados.data_contratos);
                                printf("ENERGIA: %s\n", contratos_cadastrados.energia_contratos);
                                printf("\n");
                                break;
                            }
                        }
                    }
                }
            }
            else if (arquivo_consumidor == NULL)
            {
                printf("Arquivo nao encontrado\n");
            }
        }
        else
        {
            printf("CNPJ invalido\n");
        }
    }
    else if (busca == 2)
    {
        printf("Digite o cpf para ser consultado: ");
        scanf("%s", cpf_consultado);
        printf("\n");
        // função para validar o cpf
        if (validarCPF(cpf_consultado))
        {

            if (arquivo_consumidor != NULL)
            {
                // apaga a primeira linha
                char primeiralinha[100];
                fscanf(arquivo_consumidor, "%[^\n]\n", primeiralinha);

                while (!feof(arquivo_consumidor))
                {
                    fscanf(arquivo_consumidor, "%[^;];%[^;];%[^\n]\n",
                           consumidores_cadastradas.cnpj_cpf_consumidor,
                           consumidores_cadastradas.nome_consumidor,
                           consumidores_cadastradas.RA_consumidor);
                    // comparação de strings
                    if ((strcmp(consumidores_cadastradas.cnpj_cpf_consumidor, cpf_consultado) == 0))
                    {
                        printf("CONSUMIDOR\n\n");
                        printf("CNPJ: %s\n", consumidores_cadastradas.cnpj_cpf_consumidor);
                        printf("NOME CONSUMIDOR: %s\n", consumidores_cadastradas.nome_consumidor);
                        printf("RA DO CONSUMIDOR : %s\n", consumidores_cadastradas.RA_consumidor);
                        printf("\n");
                        // verificou que existe
                        flag = 1;
                    }
                }
                // verifica que não existe
                if (flag == 0)
                {
                    printf("O CPF não consta no arquivo\n");
                    printf("\n");
                }
                // se existe
                else if (flag == 1)
                {
                    if (arquivo_contratos != NULL)
                    {
                        // enquanto o arquivo estiver aberto
                        while (!feof(arquivo_contratos))
                        {
                            fscanf(arquivo_contratos, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                                   contratos_cadastrados.id_contratos,
                                   contratos_cadastrados.idconsumidores_contratos,
                                   contratos_cadastrados.cnpj_cpf_contratos,
                                   contratos_cadastrados.data_contratos,
                                   contratos_cadastrados.energia_contratos);

                            // comparação de strings
                            if (strcmp(cpf_consultado, contratos_cadastrados.idconsumidores_contratos) == 0)
                            {
                                printf("CONTRATOS\n\n");
                                printf("ID: %s\n", contratos_cadastrados.id_contratos);
                                printf("ID CONSUMIDOR: %s\n", contratos_cadastrados.idconsumidores_contratos);
                                printf("CNPJ: %s\n", contratos_cadastrados.cnpj_cpf_contratos);
                                printf("DATA DE INICIO: %s\n", contratos_cadastrados.data_contratos);
                                printf("ENERGIA: %s\n", contratos_cadastrados.energia_contratos);
                                printf("\n");
                            }
                        }
                    }
                }
            }
            else if (arquivo_consumidor == NULL)
            {
                printf("Arquivo nao encontrado\n");
                printf("\n");
            }
        }
        else
        {
            printf("CPF invalido\n");
            printf("\n");
        }
    }
    fclose(arquivo_consumidor);
}

// Função 3
// função para listar as miniusinas
void list_miniusinas()
{
    FILE *arquivo_miniusinas;          // arquivo
    struct miniusinas listaminiusinas; // variável nova com base na struct

    arquivo_miniusinas = fopen("miniusinas.csv", "r");
    // verifica se o arquivo existe
    if (arquivo_miniusinas != NULL)
    {
        // apaga primeira linha do arquivo
        char primeiralinha[100];
        fscanf(arquivo_miniusinas, "%[^\n]\n", primeiralinha);
        // enquanto o arquivo estiver aberto
        while (!feof(arquivo_miniusinas))
        {
            fscanf(arquivo_miniusinas, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                   listaminiusinas.cnpj,
                   listaminiusinas.nome,
                   listaminiusinas.capacidade,
                   listaminiusinas.id,
                   listaminiusinas.status);

            printf("CNPJ: %s\n", listaminiusinas.cnpj);
            printf("NOME: %s\n", listaminiusinas.nome);
            printf("CAPACIDADE: %s\n", listaminiusinas.capacidade);
            printf("ID: %s\n", listaminiusinas.id);
            printf("STATUS: %s\n", listaminiusinas.status);
            printf("\n");
        }
    }
    // se não encontrar o arquivo
    else if (arquivo_miniusinas == NULL)
    {
        printf("O arquivo de miniusinas nao existe\n");
        printf("\n");
    }
    // fecha o arquivo
    fclose(arquivo_miniusinas);
}

// Função 4
// função para listar as miniusinas em ordem decrescente
void Listar_MiniUsinas_DEC()
{
    // variavel com base na struct de tamanho 11
    struct miniusinas miniusinas_dec[11];

    // abertura e declaração da variavel de arquivo
    FILE *arquivo_miniusinas = fopen("miniusinas.csv", "r");
    // verifica se existe
    if (arquivo_miniusinas != NULL)
    {
        // apaga a primeira linha
        char primeiralinha[100];
        fscanf(arquivo_miniusinas, "%[^\n]\n", primeiralinha);
        int i = 0; // variavel usada para o for

        while (!feof(arquivo_miniusinas))
        {
            fscanf(arquivo_miniusinas, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                   miniusinas_dec[i].cnpj,
                   miniusinas_dec[i].nome,
                   miniusinas_dec[i].capacidade,
                   miniusinas_dec[i].id,
                   miniusinas_dec[i].status);
            i++;
        }

        fclose(arquivo_miniusinas);
        // iniciação do vetor ordenado
        // percorre de 0 a 10
        for (int i = 0; i < 10; i++)
        {
            // percorre de 0 a 10
            for (int j = i; j < 10; j++)
            { // converte uma string em double     // converte uma string para double
                if (atof(miniusinas_dec[i].capacidade) > atof(miniusinas_dec[j].capacidade))
                { // variavel para verificação
                    struct miniusinas flag = miniusinas_dec[i];
                    miniusinas_dec[i] = miniusinas_dec[j]; // comparação feita entre os valores
                    miniusinas_dec[j] = flag;
                }
            }
        }
        // vê se no processo existe alguma numero menor que o anterior, fazendo com que troquem de lugar
        for (int i = 9; i >= 0; i--)
        {
            printf("CNPJ: %s\n", miniusinas_dec[i].cnpj);
            printf("NOME: %s\n", miniusinas_dec[i].nome);
            printf("CAPACIDADE: %s\n", miniusinas_dec[i].capacidade);
            printf("ID: %s\n", miniusinas_dec[i].id);
            printf("STATUS: %s\n", miniusinas_dec[i].status);
            printf("\n");
        }
    }
    else
    {
        printf("Erro ao abrir o arquivo.\n");
    }

    fclose(arquivo_miniusinas);
}

// Função 5
// função para listar as RA por ordem decrescente de em relação as miniusinas
void listaRAmini()
{

    FILE *arquivo_ra;
    struct ra ra_cadastradas[33];

    arquivo_ra = fopen("ra.csv", "r");

    if (arquivo_ra != NULL)
    {
        // Lê a primeira linha do arquivo e armazena em primeiralinha
        char primeiralinha[100];
        fscanf(arquivo_ra, "%[^\n]\n", primeiralinha);

        // Loop para ler as linhas restantes do arquivo até o final
        int i = 0;
        while (!feof(arquivo_ra))
        {
            fscanf(arquivo_ra, "%[^;];%[^\n]\n", // Lê os campos separados por ponto e vírgula e armazena nos membros da struct ra_decrescente
                   ra_cadastradas[i].id_ra,
                   ra_cadastradas[i].nome_ra);
            strcpy(ra_cadastradas[i].miniusinas, "0"); // Define a quantidade de miniusinas como "0" para cada RA lida

            i++; // Incrementa o índice i para ler a próxima linha do arquivo
        }
    }
    else if(arquivo_ra == NULL)
    {
        printf("Arquivo não encontrado\n");
    }

    FILE *arquivo_miniusinas;
    struct miniusinas mini_cadastrada[10];

    arquivo_miniusinas = fopen("miniusinas.csv", "r");

    if (arquivo_miniusinas != NULL)
    {
        char primeiralinha[100];
        fscanf(arquivo_miniusinas, "%[^\n]\n", primeiralinha);
        int i = 0;
        while (!feof(arquivo_miniusinas))
        {
            fscanf(arquivo_miniusinas, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                   mini_cadastrada[i].cnpj,
                   mini_cadastrada[i].nome,
                   mini_cadastrada[i].capacidade,
                   mini_cadastrada[i].id,
                   mini_cadastrada[i].status);

            for (int j = 0; j < 33; j++) // Loop para percorrer cada RA
            {                            // Verifica se o ID_RA da miniusina corresponde ao ID_RA da RA
                if (strcmp(mini_cadastrada[i].id, ra_cadastradas[j].id_ra) == 0)
                {
                    int numero = atoi(ra_cadastradas[j].miniusinas);
                    numero++;
                    sprintf(ra_cadastradas[j].miniusinas, "%d", numero);
                    break;
                }
            }
            i++;
        }
    }
    // Ordenação das RAs por quantidade de miniusinas em ordem decrescente
    for (int i = 0; i < 32; i++)
    {
        for (int j = i + 1; j < 33; j++)
        {
            // quantidade_i e quantidade_j são obtidos convertendo as quantidades de miniusinas para inteiros usando atoi.
            int quant_i = atoi(ra_cadastradas[i].miniusinas);
            int quant_j = atoi(ra_cadastradas[j].miniusinas);

            // Compara as quantidades de miniusinas entre duas RAs
            if (quant_i < quant_j)
            {
                // Realiza a troca das posições das RAs na estrutura ra_decrescente
                struct ra flag = ra_cadastradas[i];
                ra_cadastradas[i] = ra_cadastradas[j];
                ra_cadastradas[j] = flag;
            }
        }
    }

    printf("RAs por ordem decrescente de quantidade de miniusinas:\n\n");
    for (int i = 0; i < 33; i++)
    {
        printf("%d. %s - %s miniusinas\n", i + 1, ra_cadastradas[i].nome_ra, ra_cadastradas[i].miniusinas);
        printf("\n");
    }

    fclose(arquivo_ra);
    fclose(arquivo_miniusinas);
}

// Função 6
// função para listar as RAs por ordem crescente de número de contratos
void ListarRACONT()
{

    FILE *arquivo_ra;
    FILE *arquivo_contratos;
    FILE *arquivo_miniusinas;
    struct miniusinas mini[11]; // variável baseaada na struct
    struct ra ras_cadasttradas[33]; // variável baseaada na struct
    struct contratos cont[8]; // variável baseaada na struct
    char linha[256];
    // abri o arquivo
    arquivo_ra = fopen("ra.csv", "r");
    // vê se existe
    if (arquivo_ra != NULL)
    {
        char primeiralinha[100]; // Lê a primeira linha do arquivo e armazena em primeiralinha
        fscanf(arquivo_ra, "%[^\n]\n", primeiralinha);
        int i = 0; // Loop para ler as linhas restantes do arquivo até o final
        while (!feof(arquivo_ra))
        {
            fscanf(arquivo_ra, "%[^;];%[^\n]\n",
                   ras_cadasttradas[i].id_ra,
                   ras_cadasttradas[i].nome_ra);
            strcpy(ras_cadasttradas[i].capacidade_ra, "0");

            i++;
        }
        fclose(arquivo_ra);
    }

    arquivo_contratos = fopen("contratos.csv", "r");

    if (arquivo_contratos == NULL)
    {
        printf("Erro ao abrir o arquivo de contratos\n");
    }
    else
    {
        int i = 0; // Loop para ler as linhas restantes do arquivo até o final

        while (fgets(linha, sizeof(linha), arquivo_contratos))
        {
            sscanf(linha, "%[^;];%[^;];%[^;];%[^;];%[^\n]",
                   cont[i].id_contratos,
                   cont[i].idconsumidores_contratos,
                   cont[i].cnpj_cpf_contratos,
                   cont[i].data_contratos,
                   cont[i].energia_contratos);

            i++;
            // incrementa o indice i para ler a próxima linha
        }

        fclose(arquivo_contratos);

        arquivo_miniusinas = fopen("miniusinas.csv", "r");

        if (arquivo_miniusinas != NULL)
        {
            char primeiralinha[100]; // Lê a primeira linha do arquivo e armazena em primeiralinha
            fscanf(arquivo_miniusinas, "%[^\n]\n", primeiralinha);

            int i = 0;// Loop para ler as linhas restantes do arquivo até o final
            while (!feof(arquivo_miniusinas))
            {
                fscanf(arquivo_miniusinas, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                       mini[i].cnpj,
                       mini[i].nome,
                       mini[i].capacidade,
                       mini[i].id,
                       mini[i].status);

                i++;
            }
            fclose(arquivo_miniusinas);
        }

        struct ra ras_ord[33];
        for (int i = 0; i < 33; i++)
        {
            ras_ord[i] = ras_cadasttradas[i];
        }

        int contcontratos[33];
        for (int i = 0; i < 33; i++)
        {
            contcontratos[i] = 0;
        }

        for (int i = 0; i < 33; i++)
        {
            for (int j = 0; j < 11; j++)
            {
                // verifica se o ID da RA bate com o RA de miniusinas
                if (strcmp(ras_cadasttradas[i].id_ra, mini[j].id) == 0)
                {
                    for (int k = 0; k < 8; k++)
                    {
                        // verifica se o CNPJ de miniusinas bate com o CNPJ de contratos 
                        if (strcmp(mini[j].cnpj, cont[k].cnpj_cpf_contratos) == 0)
                        {
                            contcontratos[i]++;
                        }
                    }
                }
            }
        }

        // Ordenação em ordem crescente
        for (int i = 0; i < 32; i++)
        {
            for (int j = i + 1; j < 33; j++)
            {
                if (contcontratos[i] > contcontratos[j])
                {
                    struct ra aux = ras_ord[i];
                    ras_ord[i] = ras_ord[j];
                    ras_ord[j] = aux;
                    int aux2 = contcontratos[i];
                    contcontratos[i] = contcontratos[j];
                    contcontratos[j] = aux2;
                }
            }
        }

        // Imprimir resultados
        for (int i = 0; i < 33; i++)
        {
            printf("%d. RA: %s - %s - Contratos: %d\n", i + 1, ras_ord[i].id_ra, ras_ord[i].nome_ra, contcontratos[i]);
            printf("\n");
        }
    }
}

// Funçao 7
// função para listar as RAs por ordem decrescente de capacidade de geração
void ListaRACAP()
{
    FILE *arquivo_ra;
    struct ra ra_cadastradas[33]; // variável baseaada na struct
    // abri o arquivo
    arquivo_ra = fopen("ra.csv", "r");
    // vê se existe
    if (arquivo_ra != NULL)
    {
        // Lê a primeira linha do arquivo e armazena em primeiralinha
        char primeiralinha[100];
        fscanf(arquivo_ra, "%[^\n]\n", primeiralinha);

        // Loop para ler as linhas restantes do arquivo até o final
        int i = 0;
        while (!feof(arquivo_ra))
        {
            fscanf(arquivo_ra, "%[^;];%[^\n]\n", // Lê os campos separados por ponto e vírgula e armazena nos membros da struct ra_decrescente
                   ra_cadastradas[i].id_ra,
                   ra_cadastradas[i].nome_ra);
            strcpy(ra_cadastradas[i].capacidade_ra, "0"); // Define a quantidade de miniusinas como "0" para cada RA lida

            i++; // Incrementa o índice i para ler a próxima linha do arquivo
        }
    }
    else if (arquivo_ra == NULL)
    {
        printf("Arquivo não encontrado\n");
    }
    

    FILE *arquivo_miniusinas;
    struct miniusinas mini_cadastrada[10]; // variável baseada na struct
    // abri o arquivo
    arquivo_miniusinas = fopen("miniusinas.csv", "r");
    // vê se o mesmo existe
    if (arquivo_miniusinas != NULL)
    {
        // Lê a primeira linha do arquivo e armazena em primeiralinha
        char primeiralinha[100];
        fscanf(arquivo_miniusinas, "%[^\n]\n", primeiralinha);
        int i = 0;
        while (!feof(arquivo_miniusinas))
        {
            fscanf(arquivo_miniusinas, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                   mini_cadastrada[i].cnpj,
                   mini_cadastrada[i].nome,
                   mini_cadastrada[i].capacidade,
                   mini_cadastrada[i].id,
                   mini_cadastrada[i].status);

            for (int j = 0; j < 33; j++) // Loop para percorrer cada RA
            {                            // Verifica se o ID_RA da miniusina corresponde ao ID_RA da RA
                if (strcmp(mini_cadastrada[i].id, ra_cadastradas[j].id_ra) == 0)
                {
                    double numero = atof(ra_cadastradas[j].capacidade_ra);
                    numero += atof(mini_cadastrada[i].capacidade);
                    sprintf(ra_cadastradas[j].capacidade_ra, "%.2lf", numero);
                    break;
                }
            }
            i++;
        }
    }
    else if(arquivo_miniusinas == NULL)
    {
        printf("Arquivo não encontrado\n");
    }
    // Ordenação das RAs por quantidade de miniusinas em ordem decrescente
    for (int i = 0; i < 32; i++)
    {
        for (int j = i + 1; j < 33; j++)
        {
            // quantidade_i e quantidade_j são obtidos convertendo as quantidades de miniusinas para double usando atoi.
            float quant_i = atof(ra_cadastradas[i].capacidade_ra);
            float quant_j = atof(ra_cadastradas[j].capacidade_ra);

            // Compara as quantidades de miniusinas entre duas RAs
            if (quant_i < quant_j)
            {
                // Realiza a troca das posições das RAs na estrutura ra_decrescente
                struct ra flag = ra_cadastradas[i];
                ra_cadastradas[i] = ra_cadastradas[j];
                ra_cadastradas[j] = flag;
            }
        }
    }

    printf("RAs por ordem decrescente de quantidade de miniusinas:\n\n");
    for (int i = 0; i < 33; i++)
    {
        printf("%d %s - %.2f kWh de capacidade\n", i + 1, ra_cadastradas[i].nome_ra, atof(ra_cadastradas[i].capacidade_ra));
        printf("\n");
    }
    // fechamento dos arquivos
    fclose(arquivo_ra);
    fclose(arquivo_miniusinas);
}

// Função 8
// função para listar as RAs por ordem decrescente de percentual de energia disponível
void ListaRAPERCENTO()
{   
    //declaradas variaveis para abrir o arquivo de ra
    FILE *arquivo_ra;
    struct ra ra_decres[34];
    arquivo_ra = fopen("ra.csv","r");
    if (arquivo_ra!=NULL)
    {
        //arquivo de ra aberto
        //primeira linha descartada
        char primeiralinha[1000];
        fscanf(arquivo_ra,"%[^\n]\n",primeiralinha);
        //int que sera responsavel por pecorrer pela struct
        int i=0;
        while (!feof(arquivo_ra))
        {
            //arquivo esta sendo lido e as variaveis preechidas
           fscanf(arquivo_ra,"%[^;];%[^\n]\n",
                    ra_decres[i].id_ra,
                    ra_decres[i].nome_ra);
            //as outras variaveis q nao consta no arquivo inicializa com 0
            //variaveis que serão importantes no decorrer do codigo 
            strcpy(ra_decres[i].quant,"0");
            strcpy(ra_decres[i].gasta,"0");
            strcpy(ra_decres[i].disponivel,"0");
            i++;
        }
        //declaração variaveis que serão usadas para abrir o arquivo miniusinas
        FILE *arquivo_miniusina;
        struct miniusinas miniusinas_descres[11];
        arquivo_miniusina=fopen("miniusinas.csv","r");
        if (arquivo_miniusina!=NULL)
        {
            //arquivo miniusinas aberto
            //primeira linha descartada
            char primeiralinha2[100];
            fscanf(arquivo_miniusina,"%[^\n]\n",primeiralinha2);
            //int que sera responsavel por pecorrer pela struct
            int k=0;
            while (!feof(arquivo_miniusina))
            {
                //arquivo miniusinas esta sendo lido e as variveis preechidas
                fscanf(arquivo_miniusina,"%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                                miniusinas_descres[k].cnpj,
                                miniusinas_descres[k].nome,
                                miniusinas_descres[k].capacidade,
                                miniusinas_descres[k].id,
                                miniusinas_descres[k].status);
                //for que sera responsavel por somar a capacidade total de uma ra 
                for (int i=0; i<33; i++)
                {
                    //a variavel i ira pecorrer pela struct de ra 
                    if (strcmp(ra_decres[i].id_ra,miniusinas_descres[k].id)==0)
                    {
                        //caso o id da ra bater com a da miniusina a capacidade ira somar 
                        //primeiro o contador atribui o valor da variavel quantidade
                        int soma=atoi(ra_decres[i].quant);
                        //depois o contador soma com a capacidade da miniusina em questão
                        soma+=atoi(miniusinas_descres[k].capacidade);
                        //depois a variavel quantidade é atualizada com o novo valor 
                        sprintf(ra_decres[i].quant,"%d",soma);
                    }            
                }
                k++;
            }
            //declaradas variveis para abrir o arquivo contratos
            FILE *arquivo_contratos;
            struct contratos contratos_decres[8];
            arquivo_contratos=fopen("contratos.csv","r");
            if (arquivo_contratos!=NULL)
            {
                //arquivo de contratos aberto
                //primeira linha descartada
                char primeiralinha3[1000];
                fscanf(arquivo_contratos,"%[^\n]\n",primeiralinha3);
                //int que sera responsavel por pecorrer pela struct
                int j=0;
                while (!feof(arquivo_contratos))
                {
                    //arquivo de contratos esta sendo lido e as variaveis preechidas
                    fscanf(arquivo_contratos,"%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                                        contratos_decres[j].id_contratos,
                                        contratos_decres[j].idconsumidores_contratos,
                                        contratos_decres[j].cnpj_cpf_contratos,
                                        contratos_decres[j].data_contratos,
                                        contratos_decres[j].energia_contratos);
                    //for que sera responsavel por somar a quantidade de energia contratada por ra 
                    for (int i=0; i<33; i++)
                    {
                        //a variavel i ira pecorrer pela struct de ra 
                        for (int k=0; k<11; k++)
                        {
                            //a variavel k ira pecorrer pela struct de miniusinas
                            if (strcmp(ra_decres[i].id_ra,miniusinas_descres[k].id)==0)
                            {
                                //caso o id da ra foi encontrado no arquivo de miniusinas
                                if (strcmp(miniusinas_descres[k].cnpj,contratos_decres[j].cnpj_cpf_contratos)==0)
                                {
                                    //foi encontrados os contratos associados aos cnpj das miniusinas associadas a ra 
                                    //gasto ira receber o valor da varivel gasta
                                    int consumido=atoi(ra_decres[i].gasta);
                                    //gasto ira somar a quantidade de energia contratada
                                    consumido +=atoi(contratos_decres[j].energia_contratos);
                                    //a variavel gasta atualizada
                                    sprintf(ra_decres[i].gasta,"%d",consumido);
                                }                                
                            }                    
                        }                  
                    }
                    j++;          
                }
                //for que ira calcular a porcentagem 
                for (int i=0; i<33; i++)
                {
                    if (!strcmp(ra_decres[i].quant,"0"))
                    {
                        //as variaveis que nao possui capacidade serão desconsideradas
                        continue;
                    }
                    //a porcentagem sera calculada 
                    float porcento = atoi(ra_decres[i].gasta);
                    porcento=(porcento*100.0)/atoi(ra_decres[i].quant);
                    sprintf(ra_decres[i].disponivel,"%.2f",100-porcento);       
                }
                //for para ordenar em ordem decrescente 
                for (int i=0; i<32; i++)
                {
                    if (!strcmp(ra_decres[i].quant,"0"))
                    {
                        //as ra q nao possui capacidade serão desconsideradas 
                        continue;
                    } 
                    for (int j=i+1; j<33; j++)
                    {
                        if (!strcmp(ra_decres[j].quant,"0"))
                        {
                            //as ra q nao possui capacidade serão desconsideradas 
                            continue;
                        }   
                        int comparar1 =atoi(ra_decres[i].disponivel);
                        int comparar2=atoi(ra_decres[j].disponivel);
                        if (comparar1 < comparar2)
                        {
                            //as posições serão trocadas em ordem de decrescente
                            struct ra aux=ra_decres[i];
                            ra_decres[i]=ra_decres[j];
                            ra_decres[j]=aux;
                        }                    
                    }            
                }
                //for para printar o resultado
                for (int i=0; i<33; i++)
                {
                    if (!strcmp(ra_decres[i].quant,"0"))
                    {
                        continue;
                    }
                        printf("ID da RA: %s\n",ra_decres[i].id_ra);
                        printf("Nome da RA: %s\n",ra_decres[i].nome_ra);
                        printf("Percentual de energia disponivel: %s %%\n",ra_decres[i].disponivel);
                        printf("\n");             
                }     
            }
            else if (arquivo_contratos==NULL)
            {
                printf("Arquivo de contratos nao encontrado\n");
                printf("\n");
            }      
        }
        else if (arquivo_miniusina==NULL)
        {
            printf("Arquivo de miniusinas nao encontrado\n");
            printf("\n");
        }     
    }
    else if (arquivo_ra==NULL)
    {
        printf("Arquivo de ra nao encontrado\n");
        printf("\n");
    }    
}