#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AVL2.h"
#define MAX 100
void marcar();

int main()
{
    struct No *BD = NULL;
    char choice;
    int ID = 0;
    char *email;
    while (1)
    {
        ID = 0;
        printf("Oque voce deseja?\n");
        printf("1 - Agendar Sala\n");
        printf("2 - Adicionar e-mail\n");
        printf("Sua escolha: ");
        scanf("%s", &choice);
        printf("\n");

        if (choice == '1')
        {
            printf("Insira seu ID: "); //Será substituido pelo RFID
            scanf("%d", &ID);
            printf("\n");
            email = return_email(BD, ID);
            if (email == NULL)
            {
                printf("ID inválido\n");
                continue;
            }
            marcar(email);
            continue;
        }
        if (choice == '2')
        {

            printf("Insira seu ID: ");
            scanf("%d", &ID);

            printf("\n");
            printf("Insira seu e-mail: ");
            scanf("%s", email);

            printf("\n");
            BD = inserir(email, ID, BD);
            continue;
        }
        if (choice != '2' && choice != '1')
        {
            printf("Escolha inválida\n");
            continue;
        }
    }
    return (0);
}

void marcar(char *email_)
{
    char command[50];
    char email[50];
    strcpy(email, email_);
    strcpy(command, "node criar.js ");
    strcat(command, email);
    system(command);
}