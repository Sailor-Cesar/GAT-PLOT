#include <conio.h>
#include <stdio.h>
#include "PSerial.h"

#define tVet 15										// 15 bytes � o tamanho do vetor de char 'lido'

// declara��o de vari�veis
char string[10] = "";
char lido[tVet] = "";
int i=0;

// prototipa��o de fun��es
void limparLido();

int main(){
		
	limparLido();									// limpa o vetor de char para poder ser utilizado corretamente
	inicializaSerial(0, "COM3", 115200);			// inicializa a porta "serial 0", utilizando a COM3, � 115200bps
	while(serialConectada(0) != 1);					// aguarda a conex�o ser estabelecida para prosseguir
	leSerialAteTimeout(0, lido, tVet, 1000);		// l� at� 15 bytes recebidos pela "serial 0" e armazena no vetor de char 'lido', aguardar� at� 1000ms antes de "desistir" e prosseguir
	printf("%s", lido);								// imprime na tela o conte�do do vetor de char 'lido'
	
	limparLido();									// limpa o vetor de char para poder ser utilizado corretamente
	inicializaSerial(1, "COM4", 9600);				// inicializa a porta "serial 1", utilizando a COM4, � 9600bps
	while(serialConectada(1) != 1);					// aguarda a conex�o ser estabelecida para prosseguir
	leSerialAteTimeout(1, lido, tVet, 1000);		// l� at� 15 bytes recebidos pela "serial 1" e armazena no vetor de char 'lido', aguardar� at� 1000ms antes de "desistir" e prosseguir
	printf("%s", lido);								// imprime na tela o conte�do do vetor de char 'lido'
	
	do{												// fazer:
		
		scanf("%s", &string);						// aguarda o usu�rio digitar um comando, quando digitado armazenar no vetor de char 'string'
		escreveSerial(0, string, 10);				// escreve na "serial 0" at� 10 bytes contidos no vetor de char 'string'
		Sleep(20);									// aguarda 20ms para a informa��o ser recebida no Arduino conectado
		
		limparLido();								// limpa o vetor de char para poder ser utilizado corretamente
		leSerialAteTimeout(0, lido, tVet, 1000);	// l� at� 15 bytes recebidos pela "serial 0" e armazena no vetor de char 'lido', aguardar� at� 1000ms antes de "desistir" e prosseguir
		printf("%s", lido);							// imprime na tela o conte�do do vetor de char 'lido'
		Sleep(20);									// aguarda 20ms para a informa��o ser recebida no Arduino conectado
		
		scanf("%s", &string);						// aguarda o usu�rio digitar um comando, quando digitado armazenar no vetor de char 'string'
		escreveSerial(1, string, 10);				// escreve na "serial 0" at� 10 bytes contidos no vetor de char 'string'
		Sleep(20);									// aguarda 20ms para a informa��o ser recebida no Arduino conectado
		
		limparLido();								// limpa o vetor de char para poder ser utilizado corretamente
		leSerialAteTimeout(1, lido, tVet, 1000);	// l� at� 15 bytes recebidos pela "serial 1" e armazena no vetor de char 'lido', aguardar� at� 1000ms antes de "desistir" e prosseguir
		printf("%s", lido);							// imprime na tela o conte�do do vetor de char 'lido'
		Sleep(20);									// aguarda 20ms para a informa��o ser recebida no Arduino conectado
		
	}while (string[0] != 'X');						// enquanto o primeiro caracter do vetor de char 'string' for diferente de 'X', repetir...
	
	finalizaSerial(0);								// finalizar a porta de comunica��o da "serial 0"
	finalizaSerial(1);								// finalizar a porta de comunica��o da "serial 1"
	return 0;										// return padr�o, finalizando sem erros
	
}

//limpa o vetor de char 'lido', preenchendo-o com '\0'
void limparLido(){
	for (i=0; i<tVet; i++){
		lido[i] = '\0';
	}
}
