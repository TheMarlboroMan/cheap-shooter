#include "obstaculo_propiedades_fondo.h"

Obstaculo_propiedades_fondo::Obstaculo_propiedades_fondo():
	Obstaculo_actividad(), rojo(0), verde(0), azul(0), velocidad(0), estrellas(0),
	activo(false), finalizado_cambio(false)
{
	this->preparar_actor(0);
	mut_actua_fuera_de_pantalla(true);
}

Obstaculo_propiedades_fondo::~Obstaculo_propiedades_fondo()
{

}

void Obstaculo_propiedades_fondo::establecer_caracteristicas()
{
	this->w=32;
	this->h=32;
}

/* Realmente no tiene representación... Tan fácil como eso.*/

void Obstaculo_propiedades_fondo::establecer_representacion()
{

}

void Obstaculo_propiedades_fondo::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p)
{
	this->rojo=p.param1;
	this->verde=p.param2;
	this->azul=p.param3;
	this->velocidad=p.param4;
	this->estrellas=p.param5;
}

bool Obstaculo_propiedades_fondo::recibir_senal_trigger(const Accion_trigger_datos& p_datos)
{
	unsigned int p_senal=p_datos.acc_tipo();

	switch(p_senal)
	{
		case Accion_trigger::T_PROPIEDADES_FONDO:
			this->activo=true; //Esto permite que el controlador inicie el cambio de color.
			return this->finalizado_cambio;
		break;

		default:
			return true;
		break;
	}
}
