#ifndef TEMPERATURA_H_
#define TEMPERATURA_H_
#include<windows.h>
#include <iostream>
#include <thread>
#include <chrono>

typedef void (__cdecl *inicializarFunc)();
typedef double (__cdecl *lerTempFunc)();
typedef int (__cdecl *enviarComandoFunc)(int c);

class Temperatura {
	HMODULE hSimulador;
	lerTempFunc leitura;
	bool ativo = false;
	void loopTemp();
public:
	int enviarComando(int c);
	void inicializa();
	double lerTemp();
	Temperatura();
	virtual ~Temperatura();
};


#define LER_TEMP 	1
#define VENT_ON 	2
#define VENT_OFF 	3
#define RES_ON 		4
#define RES_OFF 	5
#define LAMP_ON 	6
#define LAMP_OFF 	7

#endif /* TEMPERATURA_H_ */
