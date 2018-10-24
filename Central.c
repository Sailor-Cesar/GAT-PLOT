#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore1.h"
#include "serial.h"
#define MAX 100
void marcar();
void excluir();
void clear();

int main()
{
    struct No *BD = NULL;
    int choice;
    char *ID = malloc(sizeof(char) * 40);
    char Last_ID[8];
    char *email = malloc(sizeof(char) * 120);
    int x = 0;

    while (1)
    {
        /* ---------------------- MENU DE OPÇÔES ---------------------------------------------- */
        // clear();
        printf("\n");
        printf("\n//--//--//--//--//--//--//--//--//--//--//\n");
        printf("    Oque voce deseja?");
        printf("\n //--//--//--//--//--//--//--//--//--//--//\n");
        printf("\n1 - Criar/Cancelar agendamento\n");
        printf("\n-----------------------------------\n");
        printf("\n2 - Cadastramento\n");
        printf("\n-----------------------------------\n");
        printf("\n3 - Mostrar cadastros\n");
        printf("\n//--//--//--//--//--//--//--//--//--//--//\n");
        printf("\n\nSua escolha: ");
/* -------------------------------------------------------------------------------------------- */
        scanf("%d", &choice); //SUA ESCOLHA
        printf("\n");
        switch (choice)
        {
        case 1:
/* ----------------------------- OPÇÃO 1 ------------------------------------------------------ */
            clear();
            printf("\n//--//--//--//--//--//--//--//--//--//--//\n");
            printf("        Você Selecionou : Agendamento");
            printf("\n//--//--//--//--//--//--//--//--//--//--//\n\n");
            printf("Passe o cartão:\n "); //RFID
            do{
            serial(ID);
            }while(return_email(BD, ID)==NULL);                 //Lê duas vezes por um problema no RFID que não envia completamente o ID

            if (ID == Last_ID) //Se o ID que quer agendar foi o ultimo a agendar ele cancela o ultimo agendamento
            {
                printf("Cancelando ...");
                excluir(); //CANCELA
                strcpy(Last_ID,"");
                clear();
                printf("Cancelado!");
                continue;
            }
            strcpy(Last_ID,ID);  //DEFINE O ULTIMO ID A SER USADO
            email = return_email(BD, ID); //RETORNA E-MAIL DA ARVORE
            marcar(email); //MARCA
            clear();
            printf("\n//--//--//--//--//--//--//--//--//--//--//\n");
            printf("                Marcado!");
            printf("\n//--//--//--//--//--//--//--//--//--//--//\n\n");
            continue;

/* ------------------------ OPÇÃO 2----------------------------------------------------------- */
        case 2:

            clear();
            printf("\n//--//--//--//--//--//--//--//--//--//--//\n");
            printf("      Você Selecionou : Cadastramento!");
            printf("\n//--//--//--//--//--//--//--//--//--//--//\n\n");
            printf("Passe o cartão:"); //LER RFID
            while(x!= 1){
                serial(ID);
                printf(" %s",ID);
                printf("  O ID está correto?");
                scanf("%d",&x);
            }
            serial(ID);
            serial(ID);
            serial(ID);
            printf("%s", ID);
            printf("\n");

            printf("Insira seu e-mail: "); //LER E-MAIL
            scanf("%s", email);
            clear();

            printf("Cadastrando...\n");
            BD = inserir(email, ID, BD); //INSERE NA ARVORE
            x = 0;
            clear();
            printf("\n//--//--//--//--//--//--//--//--//--//--//\n");
            printf("                Cadastrado!");
            printf("\n//--//--//--//--//--//--//--//--//--//--//\n\n");
            continue;

/* ------------------------ OPÇÃO 3 --------------------------------------------------------- */
        case 3:
            clear();
            printf("\n//--//--//--//--//--//--//--//--//--//--//\n");
            printf("    Você Selecionou : Verificar Cadastro");
            printf("\n//--//--//--//--//--//--//--//--//--//--//\n\n");
            printf("\n");
            inordem(BD); //MOSTRA A ARVORE
            printf("\n");

/* ------------------------ DEFAULT --------------------------------------------------------- */
        // default:
        //     clear();
        //     printf("Escolha inválida\n"); //CASO NÂO ESCOLHA NENHUMA DAS OPÇÕES DADAS
        //     continue;
        }
    }
    free(BD); //LIBERA O ESPAÇO NA MEMORIA
    return (0);
}
/* ----------------------- FUNÇÃO PARA MARCAR NO CALENDAR ----------------------------------- */
void marcar(char *email_)
{
    char command[50];
    char email[50];
    strcpy(email, email_);             //CRIA A STRING E-MAIL
    strcpy(command, "node criar.js "); //CRIA A STRING COM O COMANDO PADRÃO
    strcat(command, email);            //COLOCA O E-MAIL COMO ARGUMENTO
    system(command);                   //RODA O PROGRAMA NO BASH
}
/* ----------------------- FUNÇÃO PARA EXCLUIR NO CALENDAR ----------------------------------- */
void excluir()
{
    char command[50];
    strcpy(command, "node criar.js 1"); //CRIA A STRING COMANDO COM ARGUMENTO 1
    system(command);                    //RODA O PROGRAMA NO BASH
}
void clear(){
    char command[50];
    strcpy(command, "clear");
    system(command);
 }