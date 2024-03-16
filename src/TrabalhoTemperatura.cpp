
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
	cout << t->lerTemp(); // Lê a temperatura
}

#if defined(_WIN32) && !defined(__WINPTHREADS_VERSION)
// Função a ser executada caso seja Win32Threads o parametro tem um tipo específico definido no Windows.h e depois é feito um cast para uma struct, definida apos os includes
DWORD WINAPI exibirTemperaturaThread(LPVOID lpParam) {
	PPARAMETROS parametros;						//Struct para receber os parâmetros da threads
	parametros = (PPARAMETROS)lpParam;			// Troca forçada do tipo de dados de ponteiro void Windows para o tipo de dados utilziad
	while(true){
		exibirTemperatura(parametros->t);
		Sleep(1000);
	}
	return 0;
}
#else
void exibirTemperaturaThread(Temperatura* t) { 	//Parametro pado diretamente
	//Thread padrão Posix
	while(true){
		exibirTemperatura(t);
		Sleep(1000);
	}
}
#endif


int main() {
	Temperatura t; //Cria um objeto t da classe Temperatura
	double temp;
	t.inicializa(); // Inicializa a simulação
	clrscr();
#if defined(_WIN32) && !defined(__WINPTHREADS_VERSION)
	// Código executado caso seja Win32 Threads
	DWORD resp;									// O retorno da função deve possuir uma variável obrigatória
	PPARAMETROS dados;							// Struct com os parâmetros passados para a thread
	dados = (PPARAMETROS) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(PARAMETROS));	// Alocação de memória para a Thread utilizando estrutura do Windows.h
	dados->t = &t;								// Passarem de parâmetro para a struct que será enviada a thread
	CreateThread(NULL,0,exibirTemperaturaThread,dados,0,&resp);			// Chamada de thread conforme descrito em https://docs.microsoft.com/pt-br/windows/win32/procthread/creating-threads
#else
	// Código executado caso seja Threads Posix
	thread TexibirTemperatura(exibirTemperaturaThread, &t);
#endif
	t.enviarComando(RES_ON); // Aciona o resistor
	int op = 1;
	while (op != 0) {
		gotoxy(1, 1);
		// Limpar área do menu
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
