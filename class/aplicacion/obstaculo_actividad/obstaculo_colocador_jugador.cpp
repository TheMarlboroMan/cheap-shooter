#include "obstaculo_colocador_jugador.h"

Obstaculo_colocador_jugador::Obstaculo_colocador_jugador():
	Obstaculo_actividad(), usado(false)
{
	this->preparar_actor(0);
	mut_actua_fuera_de_pantalla(true);
}

Obstaculo_colocador_jugador::~Obstaculo_colocador_jugador()
{

}

void Obstaculo_colocador_jugador::establecer_caracteristicas()
{
	this->w=32;
	this->h=64;
}

/* Realmente no tiene representación... Tan fácil como eso.*/

void Obstaculo_colocador_jugador::establecer_representacion()
{

}
