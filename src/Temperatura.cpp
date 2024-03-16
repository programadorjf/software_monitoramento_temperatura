#include "Temperatura.h"

void loopTemp(inicializarFunc i ){
	while (true) {
		i();
		Sleep(1000);
	}
}


Temperatura::Temperatura() {
#ifdef __x86_64
	hSimulador = LoadLibrary("simuladorTermico64.dll");
#elif __i386__
	hSimulador = LoadLibrary("simuladorTermico32.dll");
#endif
	if (!hSimulador) {
		std::cerr << "Erro ao carregar DLL do simulador termico" << std::endl;
	} else {
		if (!(leitura = (lerTempFunc) GetProcAddress(hSimulador, "atualizar")))
			std::cerr << "Erro ao carregar a função atualizar" << std::endl;
		ativo = true;
	}
}

void Temperatura::inicializa() {
	inicializarFunc inicializar;
	if (!(inicializar = (inicializarFunc) GetProcAddress(hSimulador, "inicializar")))
		std::cerr << "Erro ao carregar a função inicializar" << std::endl;
	inicializar();
	//std::thread t(&Temperatura::loopTemp, this);
}

double Temperatura::lerTemp() {
	if (!(leitura = (lerTempFunc) GetProcAddress(hSimulador, "atualizar")))
		std::cerr << "Erro ao carregar a função atualizar" << std::endl;
	return leitura();
}

int Temperatura::enviarComando(int c) {
	enviarComandoFunc comando;
	if (!(comando = (enviarComandoFunc) GetProcAddress(hSimulador, "comando")))
		std::cerr << "Erro ao carregar a função comando" << std::endl;
	return comando(c);
}

void Temperatura::loopTemp() {
	while (true) {
		leitura();
		Sleep(1000);
	}
}

Temperatura::~Temperatura() {
	FreeLibrary(hSimulador);
	std::cout << "Desconectado de simulador termico" << std::endl;
}
