#include "obstaculo_tutorial.h"

Obstaculo_tutorial::Obstaculo_tutorial():
	Obstaculo_actividad(), codigo_mensaje(0), ms_tiempo(0), usado(false)
{
	this->preparar_actor(0);
}

Obstaculo_tutorial::~Obstaculo_tutorial()
{

}

void Obstaculo_tutorial::establecer_caracteristicas()
{
	this->w=32;
	this->h=32;
}

/* Realmente no tiene representación... Tan fácil como eso.*/

void Obstaculo_tutorial::establecer_representacion()
{

}

void Obstaculo_tutorial::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p)
{
	this->codigo_mensaje=p.param1;
	this->ms_tiempo=p.param2;
	this->finaliza_tutoriales=p.param3;
}
