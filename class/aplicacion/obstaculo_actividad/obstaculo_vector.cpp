#include "obstaculo_vector.h"

Obstaculo_vector::Obstaculo_vector():
	Obstaculo_actividad(), nuevo_vector_x(0.0f), nuevo_vector_y(0.0f)
{
	this->preparar_actor(0);
}

Obstaculo_vector::~Obstaculo_vector()
{

}

void Obstaculo_vector::establecer_caracteristicas()
{
	this->w=32;
	this->h=32;
}

/* Realmente no tiene representación... Tan fácil como eso.*/

void Obstaculo_vector::establecer_representacion()
{

}

void Obstaculo_vector::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p)
{
	this->nuevo_vector_x=p.param1;
	this->nuevo_vector_y=p.param2;

	if(p.param3) this->nuevo_vector_x=-this->nuevo_vector_x;
	if(p.param4) this->nuevo_vector_y=-this->nuevo_vector_y;
}
