#include "obstaculo_propiedades_movimiento_relativo.h"

Obstaculo_propiedades_movimiento_relativo::Obstaculo_propiedades_movimiento_relativo():
	Obstaculo_actividad(), desplazamiento_x(0.0f), desplazamiento_y(0.0f), velocidad(0.0f)
{
	this->preparar_actor(0);
}

Obstaculo_propiedades_movimiento_relativo::~Obstaculo_propiedades_movimiento_relativo()
{

}

void Obstaculo_propiedades_movimiento_relativo::establecer_caracteristicas()
{
	this->w=32;
	this->h=32;
}

/* Realmente no tiene representación... Tan fácil como eso.*/

void Obstaculo_propiedades_movimiento_relativo::establecer_representacion()
{

}

void Obstaculo_propiedades_movimiento_relativo::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p)
{
	this->desplazamiento_x=p.param1;
	this->desplazamiento_y=p.param2;
	if(p.param3) this->desplazamiento_x=-this->desplazamiento_x;
	if(p.param4) this->desplazamiento_y=-this->desplazamiento_y;
	this->velocidad=p.param5;
}
