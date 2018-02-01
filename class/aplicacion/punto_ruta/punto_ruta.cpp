#include "punto_ruta.h"

Punto_ruta::Punto_ruta()
	:Obstaculo(), agrupacion(0), identificador(0), siguiente(0),
	velocidad(0.0f), instantaneo(false)
{
	this->preparar_actor(0);
}

Punto_ruta::~Punto_ruta()
{

}

void Punto_ruta::establecer_caracteristicas()
{
	this->w=8;
	this->h=8;
}

/* Realmente no tiene representación... Tan fácil como eso.*/

void Punto_ruta::establecer_representacion()
{
	//this->representacion=NULL;
}

void Punto_ruta::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p)
{
	this->agrupacion=p.param1;
	this->identificador=p.param2;
	this->siguiente=p.param3;
	this->velocidad=p.param4;
	this->instantaneo=(bool) p.param5;
}
