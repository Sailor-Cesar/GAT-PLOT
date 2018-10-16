#include <stdio.h>
#include <stdlib.h>
struct No
{
    int info;
    char *email;
    struct No *esquerda;
    struct No *direita;
    struct No *pai;
    int altura;
};
int altura(struct No *no)
{
    if (no == NULL)
    {
        return 0;
    }
    else
    {
        return no->altura;
    }
}
int atualizarAltura(struct No *no)
{
    int altDireita;
    int altEsquerda;
    if (no != NULL)
    {
        altDireita = altura(no->direita);
        altEsquerda = altura(no->esquerda);
        if (altDireita > altEsquerda)
        {
            no->altura = altDireita + 1;
        }
        else
        {
            no->altura = altEsquerda + 1;
        }
    }
    return 0;
}
struct No *criararvore(int info, char *email)
{
    struct No *raiz;
    raiz = (struct No *)malloc(sizeof(struct No));
    raiz->info = info;
    raiz->esquerda = NULL;
    raiz->direita = NULL;
    raiz->altura = 1;
    raiz->email = email;
    return raiz;
}
struct No *inserir(char *email, int info, struct No *arvore)
{
    if (arvore == NULL)
    {
        arvore = criararvore(info, email);
    }
    else if (info < arvore->info)
    {
        arvore->esquerda = inserir(email, info, arvore->esquerda);
        arvore->esquerda->pai = arvore;
    }
    else
    {
        arvore->direita = inserir(email, info, arvore->direita);
        arvore->direita->pai = arvore;
    }
    atualizarAltura(arvore);
    return arvore;
}
struct No *buscar_no(struct No *arvore, int valor)
{
    if (arvore == NULL)
    {
        return NULL;
    }
    else
    {
        if (valor == arvore->info)
        {
            return arvore;
        }
        if (valor < arvore->info)
        {
            return (buscar_no(arvore->esquerda, valor));
        }
        else
        {
            return (buscar_no(arvore->direita, valor));
        }
    }
}
int checkavl(struct No *arvore, int array[100], int n)
{
    int flag = 0;
    int x = 0;
    int y = 0;
    int l = 0;
    for (int i = 0; i < (n); i++)
    {
        if (array[i] != -1)
        {
            x = altura(buscar_no(arvore, array[i])->direita);
            y = altura(buscar_no(arvore, array[i])->esquerda);
            l = x - y;
            if (l > 1 || l < -1)
            {
                flag = 1;
            }
        }
    }
    if (flag == 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
char *return_email(struct No *arvore, int valor)
{
    if (arvore == NULL)
    {
        return NULL;
    }
    else
    {
        if (valor == arvore->info)
        {
            return arvore->email;
        }
        if (valor < arvore->info)
        {
            return (return_email(arvore->esquerda, valor));
        }
        else
        {
            return (return_email(arvore->direita, valor));
        }
    }
}
int inordem(struct No *x){
    if(x!=NULL){
        inordem(x->esquerda);
        printf(" %d",x->info);
        printf(" : %s \n",x->email);
        inordem(x->direita);
    }
}
int onordem(struct No *x){
    if(x!=NULL){

        printf(" %d",x->info);
        printf(" : %s \n",x->email);
        inordem(x->esquerda);
        inordem(x->direita);
    }
}
