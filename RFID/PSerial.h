/* [ DOCUMENTA��O ] ===============================================================================================================================================
	
	[ Descri��o ] -------------------------------------------------------------------------------------------------------------------------------------------------
	 
	 Este arquivo permite a inicializa��o, leitura, escrita e finaliza��o da porta serial atraves de um programa escrito na linguagem C (.c) no Windows. 
	 Uma capacidade interessante � a possibilidade de se utilizar mais de uma conex�o serial ao mesmo tempo, de forma f�cil, amig�vel e intuitiva.
	 O objetivo do desenvolvimento destas fun��es � tornar mais did�tico e simples a utiliza��o de m�todos de comunica��o entre dispositivos utilizando
	 as portas seriais, tais como placas Arduino, PIC, entre outras.
	 Al�m da comunica��o padr�o, a capacidade de se trabalhar com mais de uma porta serial ao mesmo tempo, � especialmente �til a quem deseja utilizar 
	 m�dulos de bluetooth, por exemplo, ou at� mesmo trabalhar com um �nico software gerenciando a comunica��o entre v�rios dispositivos conectados �s 
	 portas COM simultaneamente.


	[ Como utilizar em seu c�digo ] -------------------------------------------------------------------------------------------------------------------------------
	
	 Basta deixar uma c�pia deste arquivo no diret�rio
	 do ser arquivo fonte e inclu�-lo em seu c�digo, desta forma:
	
	        #include "PSerial.h"


	[ Fun��es dispon�veis ] ---------------------------------------------------------------------------------------------------------------------------------------
	
	 - void inicializaSerial(u_char idSerial, char *nomePorta, u_int baudRate);			// inicializa a comunica��o atrav�s da porta serial passada por par�metro 
	 																					// (Ex.: inicializaSerial(0, "COM3", 9600); - para inicializar a COM3, a 9600bps)
	 
	 - void finalizaSerial(u_char idSerial);											// finaliza a comunica��o atrav�s da porta serial
	 																					// (Ex.: finalizaSerial(0);)
	 
	 - int leSerial(u_char idSerial, char *buffer, u_int qtdChar);						// faz a leitura de bytes recebidos atrav�s da porta serial 
	 																					// (Ex.: leSerial(0, buffer, 10); - para gravar 10 bytes recebidos da COM, no vetor de char "buffer")
	 																					// retorna -1 caso haja falha, e a quantidade de bytes lidos caso haja sucesso
	 
	 - leSerialAteTimeout(u_char idSerial, char *buffer, u_int qtdChar, u_long timeout)	// faz a leitura de bytes recebidos atrav�s da porta serial 
	 																					// (Ex.: leSerialAteTimeout(0, buffer, 10, 1000); - para fazer a leitura de at� 10 bytes recebidos 
																						// da COM especificada e gravar no vetor de char "buffer" e continuar tentando fazer essa leitura 
																						// at� esgotar o tempo especificado em 'timeout' que � em milisegundos)
	 
	 - int escreveSerial(u_char idSerial, char *buffer, u_int qtdChar);					// faz a escrita na porta serial dos bytes contidos no buffer 
	 																					// (Ex.: escreveSerial(0, buffer, 10); - para escrever na serial 10 bytes contidos no vetor "buffer")
	 																					// retorna 0 caso haja falha, e 1 caso haja sucesso
	 
	 - int serialConectada(u_char idSerial);											// simplesmente retorna o status da conex�o (1 = conectado | 0 = desconectado)
	 																					// (ex.: (int teste = serialConectada(0);)
	 
	 
	[ Informa��o importante ] -------------------------------------------------------------------------------------------------------------------------------------
	
	 Quando for tentar endere�ar uma porta COMx cujo "n�mero" for maior que 9 (Ex.: COM10, COM11, ... , COM255) 
	 no Windows � necess�rio especificar a porta dessa forma:
	 
	  "COM10" dever� ser "\\\\.\\COM10"
	  "COM23" dever� ser "\\\\.\\COM23"
	  
	 Obviamente, as portas COMx cujos n�meros forem menores do que 10 (Ex.: COM1, COM2, ... , COM9, etc.) no 
	 Windows tamb�m podem ser especificadas da maneira citada acima, por�m elas tamb�m funcionam se forem 
	 especificadas da forma simples "COM1", "COM2", ... , "COM9".
	 
	 
	[ Notas de autor e vers�o ] -----------------------------------------------------------------------------------------------------------------------------------
	
	 Este c�digo foi traduzido e adaptado da vers�o original em: 'http://playground.arduino.cc/Interfacing/CPPWindows'
	 
	 Vers�o (se fizer modifica��es, adicione abaixo da �ltima linha dispon�vel, seguindo o padr�o de formato)
	 1.0 por Kesede Rodrigues Julio em 2013/05/16
	 2.0 por Yuri Crisostomo Bernardo em 2013/12/28

===================================================================================================================================================================*/

// #include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#define ARDUINO_WAIT_TIME 2000		// arduino mega pode demorar at� 10 segundos para reiniciar
#define QTD_PORTAS 2				// altere aqui a quantidade de portas seriais que deseja trabalhar simultaneamente

typedef struct Pserial{				// criando uma estrutura para armazenas as informa��es relevantes da conex�o serial
  HANDLE hSerial; 					// handler de comunica��o Serial
  int conectada;  					// flag da situa��o da conex�o
  COMSTAT status; 					// cont�m v�rias informa��es sobre a conex�o
  DWORD erros;    					// mant�m a rastreabilidade do �ltimo erro
}Pserial;							// nome da estrutura

Pserial serial[QTD_PORTAS];			// instanciando um vetor de estrururas Pserial que ser�o utilizadas nas fun��es

// inicializa a comunica��o serial com a porta de comunica��o recebida por par�metro
void inicializaSerial(u_char idSerial, char *nomePorta, u_int baudRate){
    
	serial[idSerial].conectada = 0;											// a conex�o ainda n�o est� estabelecida

    // tenta conectar-se � porta atrav�s de um CreateFile
    serial[idSerial].hSerial = CreateFile(nomePorta, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if(serial[idSerial].hSerial == INVALID_HANDLE_VALUE){					// verifica se algum erro ocorreu
        if(GetLastError() == ERROR_FILE_NOT_FOUND){							// se ocorreu, imprime erro
            printf("ERRO: Handle was not attached. Razao: %s nao disponivel.\n", nomePorta);			// este erro � impresso quando a porta n�o est� dispon�vel (porta n�o existe, por exemplo)
        }else{
            printf("ERRO!!!");												// imprime um erro "gen�rico"
        }
    }else{																	// se estiver conectado corretamente
        DCB dcbSerialParams = {0};											// tenta configurar os par�metros de cominuca��o da porta
        if (!GetCommState(serial[idSerial].hSerial, &dcbSerialParams)){		// tenta contactar a porta de comunica��o verificando se � poss�vel prosseguir (obtendo os par�metros de configura��o atuais)
            printf("Falha na obtencao dos parametros da serial!");			// se n�o for poss�vel, mostra um erro
        }else{																// sendo poss�vel, define os par�metros da comunica��o serial com a placa
            dcbSerialParams.BaudRate = baudRate; 							// baud rate           = de acordo com o par�metro recebido em 'baudRate'
            dcbSerialParams.ByteSize = 8;									// bits de paridade    = 8 bits
            dcbSerialParams.StopBits = ONESTOPBIT;							// bits de parada      = 1
            dcbSerialParams.Parity   = NOPARITY;							// paridade            = sem paridade

	        if(!SetCommState(serial[idSerial].hSerial, &dcbSerialParams)){	// aplica os novos par�metros � porta e verifica se foram aplicados corretamente
	           printf("ALERTA: Nao poderia atribuir parametros para a Porta Serial");					// caso os par�metros n�o tenham sido aplicados corretamente, imprime erro
	        }else{															// caso senham sido aplicados corretamente
	            serial[idSerial].conectada = 1;								// define o estado da nossa porta de comunica��o como "conectada"
	            Sleep(ARDUINO_WAIT_TIME);									// aguarda 2 segundos (ARDUINO_WAIT_TIME) aguardando a placa arduino reiniciar 
																			// NOTA: funciona bem com Arduino UNO, mas a placa Arduino Mega pode demorar mais do que isso!
	        }
        }
    }
}

// fecha a conex�o NOTA: por alguma raz�o, seu programa pode n�o conseguir conectar novamente antes de sua aplica��o rodar essa fun��o mais uma vez
void finalizaSerial(u_char idSerial){
    if(serial[idSerial].conectada){											// verifica o estado da conex�o antes de tentar desconectar
        serial[idSerial].conectada = 0;										// define que a porta n�o est� mais conectada
        CloseHandle(serial[idSerial].hSerial);								// finaliza o handler da comunica��o serial
    }
}

// l� os dados atrav�s do handler de comunica��o serial e armazena no buffer passado por par�metro.
// se qtdChar for maior do que o total de bytes dispon�vel, ser� retornado somente o que estava dispon�vel para ser lido.
// NOTA: ao utilizar esta fun��o, certificar-se de aguardar tempo suficiente para o buffer estar carregado com os valores desejados
// a fun��o retornar� -1 quando nada puder ser lido
int leSerial(u_char idSerial, char *buffer, u_int qtdChar){
	
    DWORD bytesRead;														// quantidade de bytes que efetivamente estavam prontos para serem lidos
    u_int toRead;															// quantidade de bytes que foram solicitados a serem lidos
    ClearCommError(serial[idSerial].hSerial, &serial[idSerial].erros, &serial[idSerial].status);	// a fun��o 'ClearCommError' foi utilizada para pegar todas as informa��es de estado da porta serial

    if(serial[idSerial].status.cbInQue > 0){								// verifica se h� algo dispon�vel pronto para ser lido, caso haja faz o que segue
        // aqui uma verifica��o para saber se a quantidade de dados dispon�vel para ser lida � maior do que a quantidade informada no par�metro 'qtdChar'
		if(serial[idSerial].status.cbInQue > qtdChar){						// se houver mais bytes dispon�ves do que solicitados
            toRead = qtdChar;												// a quantidade de bytes que devem ser lidos ser� limitada � quantidade que foi solicitada
        }else{																// se n�o (caso haja uma quantidade igual ou menor de bytes dispon�veis do que solicitados)
            toRead = serial[idSerial].status.cbInQue;						// a quantidade de bytes que devem ser lidos, ser� exatamente a quantidade de bytes dispon�veis
        }
        if(ReadFile(serial[idSerial].hSerial, buffer, toRead, &bytesRead, NULL) && bytesRead != 0){	// tenta ler a quantidade requerida de bytes e retornar o n�mero de bytes lidos corretamente
            return bytesRead;												// retorna o n�mero de bytes lidos corretamente
        }
    }
    return -1;																// caso n�o haja nada a ser lido ou tenha ocorrido algum erro, retorna -1
}

// l� os dados atrav�s do handler de comunica��o serial e armazena no buffer passado por par�metro.
// se qtdChar for maior do que o total de bytes dispon�vel, ser� retornado somente o que estava dispon�vel para ser lido.
// NOTA: esta fun��o tem como argumento a espera da resposta do dispositivo conectado (preenchendo o buffer da serial) at� o tempo definido passado por 'timeout' em milisegundos
// a fun��o retornar� -1 quando nada puder ser lido
int leSerialAteTimeout(u_char idSerial, char *buffer, u_int qtdChar, u_long timeout){
	
    DWORD bytesRead;													// quantidade de bytes que efetivamente estavam prontos para serem lidos
    u_int toRead;														// quantidade de bytes que foram solicitados a serem lidos
    ClearCommError(serial[idSerial].hSerial, &serial[idSerial].erros, &serial[idSerial].status);		// a fun��o 'ClearCommError' foi utilizada para pegar todas as informa��es de estado da porta serial
    long tentativas = 0;												// contador das tentativas de leitura durante o per�odo de tentativas
	int interval = 100;													// tempo de intervalo entre tentavivas de leitura (cada tentativa dever� ocorrer a cada intervalo, e a soma dos 
																		// intervalos n�o dever� ultrapassar o tempo total estipulado no timeout) isto � necess�rio para sincronizar o tempo
																		// total do envio dos bytes com o in�cio de uma tentativa de leitura (para que n�o haja um in�cio de leitura 
																		// precipitado e uma consequente quebra na leitura dos bytes que ainda n�o foram enviados pelo dispositivo conectado)
																		// NOTA: taxas de transfer�ncias mais altas e mensagens mais curtas aceitam tempos de intervalos menores
	
	while ((timeout / interval) > tentativas){							// enquanto houver tentativas restantes (a raz�o entre a quantidade de tempo, dividido pelo intervalo resulta na quantidade de tentativas dispon�veis)
	    ClearCommError(serial[idSerial].hSerial, &serial[idSerial].erros, &serial[idSerial].status);	// a fun��o 'ClearCommError' foi utilizada para pegar todas as informa��es de estado da porta serial
		if(serial[idSerial].status.cbInQue > 0){						// verifica se h� algo dispon�vel pronto para ser lido, caso haja faz o que segue
	        // aqui uma verifica��o para saber se a quantidade de dados dispon�vel para ser lida � maior do que a quantidade informada no par�metro 'qtdChar'
			if(serial[idSerial].status.cbInQue > qtdChar){				// se houver mais bytes dispon�ves do que solicitados
	            toRead = qtdChar;										// a quantidade de bytes que devem ser lidos ser� limitada � quantidade que foi solicitada
	        }else{														// se n�o (caso haja uma quantidade igual ou menor de bytes dispon�veis do que solicitados)
	            toRead = serial[idSerial].status.cbInQue;				// a quantidade de bytes que devem ser lidos, ser� exatamente a quantidade de bytes dispon�veis
	        }
	        if(ReadFile(serial[idSerial].hSerial, buffer, toRead, &bytesRead, NULL) && bytesRead != 0){	// tenta ler a quantidade requerida de bytes e retornar o n�mero de bytes lidos corretamente
				return bytesRead;										// retorna o n�mero de bytes lidos corretamente
	        }
	    }
	    tentativas++;													// incremento no contador de tentativas
	    Sleep(interval);												// tempo de espera at� a pr�xima tentativa (necess�rio para fins de sincronismo - cuidado ao mexer neste tempo)
	}
    return -1;															// caso n�o haja nada a ser lido ou tenha ocorrido algum erro, retorna -1
}

// escreve dados de um buffer atrav�s da conex�o serial. retorna 1 quando houver sucesso e 0 quando houver falha
int escreveSerial(u_char idSerial, char *buffer, u_int qtdChar){
	
    DWORD bytesSend;																		// quantidade de bytes enviados corretamente, apenas para completar a fun��o (pode ter mais utilidade em futuras revis�es)

    if(!WriteFile(serial[idSerial].hSerial, (void *)buffer, qtdChar, &bytesSend, 0)){		// tenta escrever do buffer na porta serial, e caso resulte em algum erro
        ClearCommError(serial[idSerial].hSerial, &serial[idSerial].erros, &serial[idSerial].status);	// utiliza a fun��o 'ClearCommError' para pegar as informa��es de estado da porta serial
        return 0;																			// e retorna 0 (falha)
    }else{																					// caso tudo ocorra bem
    	return 1;																			// retorna 1 (sucesso)
    }
}

// verifica se a porta serial est� conectada
int serialConectada(u_char idSerial){
    return serial[idSerial].conectada;									// simplesmente retorna o status da conex�o
}
