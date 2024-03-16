
#include <iostream>
#include <chrono>
#if defined(_WIN32) && !defined(__WINPTHREADS_VERSION)

#else
#include <thread>
#endif
#include<windows.h>
#include "Temperatura.h"
#include "conio2.h" // deve ser ajustado o link para conio caso 32bits ou conio_64 caso 64bits.
using namespace std;

#if defined(_WIN32) && !defined(__WINPTHREADS_VERSION)
typedef struct VariaveisThread {
	Temperatura* t;
}PARAMETROS, *PPARAMETROS;
#endif

void exibirTemperatura(Temperatura* t) {
	gotoxy(70, 1);
	cout << "         ";
	gotoxy(70, 1);
	cout << t->lerTemp(); // L� a temperatura
}

#if defined(_WIN32) && !defined(__WINPTHREADS_VERSION)
// Fun��o a ser executada caso seja Win32Threads o parametro tem um tipo espec�fico definido no Windows.h e depois � feito um cast para uma struct, definida apos os includes
DWORD WINAPI exibirTemperaturaThread(LPVOID lpParam) {
	PPARAMETROS parametros;						//Struct para receber os par�metros da threads
	parametros = (PPARAMETROS)lpParam;			// Troca for�ada do tipo de dados de ponteiro void Windows para o tipo de dados utilziad
	while(true){
		exibirTemperatura(parametros->t);
		Sleep(1000);
	}
	return 0;
}
#else
void exibirTemperaturaThread(Temperatura* t) { 	//Parametro pado diretamente
	//Thread padr�o Posix
	while(true){
		exibirTemperatura(t);
		Sleep(1000);
	}
}
#endif


int main() {
	Temperatura t; //Cria um objeto t da classe Temperatura
	double temp;
	t.inicializa(); // Inicializa a simula��o
	clrscr();
#if defined(_WIN32) && !defined(__WINPTHREADS_VERSION)
	// C�digo executado caso seja Win32 Threads
	DWORD resp;									// O retorno da fun��o deve possuir uma vari�vel obrigat�ria
	PPARAMETROS dados;							// Struct com os par�metros passados para a thread
	dados = (PPARAMETROS) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(PARAMETROS));	// Aloca��o de mem�ria para a Thread utilizando estrutura do Windows.h
	dados->t = &t;								// Passarem de par�metro para a struct que ser� enviada a thread
	CreateThread(NULL,0,exibirTemperaturaThread,dados,0,&resp);			// Chamada de thread conforme descrito em https://docs.microsoft.com/pt-br/windows/win32/procthread/creating-threads
#else
	// C�digo executado caso seja Threads Posix
	thread TexibirTemperatura(exibirTemperaturaThread, &t);
#endif
	t.enviarComando(RES_ON); // Aciona o resistor
	int op = 1;
	while (op != 0) {
		gotoxy(1, 1);
		// Limpar �rea do menu
		cout << "Menu" << endl;
		cout << "Digite 2 para ligar o ventilador" << endl;
		cout << "Digite 3 para desligar o ventilador" << endl;
		cout << "Digite 4 para ligar o resistor" << endl;
		cout << "Digite 5 para desligar o resistor" << endl;
		cout << "Digite 8 para controlar temperatura entre 26 e 27 graus" << endl;
		cout << "Digite 0 para sair" << endl;
		cin >> op;
		if ((op>1)and(op<6))
			t.enviarComando(op);
		if (op==1)
			exibirTemperatura(&t);
		if (op==8){
			for (int i = 0; i < 40; ++i) {
				temp = t.lerTemp();
				cout << temp << endl;
				if (temp>27){
					t.enviarComando(VENT_ON);
					t.enviarComando(RES_OFF);
				}
				if (temp<26){
					t.enviarComando(VENT_OFF);
					t.enviarComando(RES_ON);
				}
				Sleep(1000);
			}
		}

	}

	return EXIT_SUCCESS;
}
