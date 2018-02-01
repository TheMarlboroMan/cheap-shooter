#include "obstaculo_trigger_auto.h"

Obstaculo_trigger_auto::Obstaculo_trigger_auto()
	:Obstaculo_actividad(), Trigger(), encendido(true)
{
	this->preparar_actor(0);
	mut_actua_fuera_de_pantalla(false);
}

Obstaculo_trigger_auto::~Obstaculo_trigger_auto()
{

}

void Obstaculo_trigger_auto::establecer_representacion()
{

}

void Obstaculo_trigger_auto::establecer_caracteristicas()
{
	this->w=32;
	this->h=32;
}

bool Obstaculo_trigger_auto::es_activo() const
{
	return this->puede_ser_activado() && this->es_en_pantalla();
}

void Obstaculo_trigger_auto::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p)
{
	Trigger::mut_tag_accion(p.param1);
	Trigger::mut_repetible(p.param2 ? true : false);
	this->encendido=p.param3 ? true : false;
}

bool Obstaculo_trigger_auto::recibir_senal_trigger(const Accion_trigger_datos& p_datos)
{
	unsigned int p_senal=p_datos.acc_tipo();

	switch(p_senal)
	{
		case Accion_trigger::T_ACTIVAR:
			this->encendido=true;
		break;

		case Accion_trigger::T_DESACTIVAR:
			this->encendido=false;
		break;

		case Accion_trigger::T_ACTIVAR_DESACTIVAR:
			this->encendido=!this->encendido;
		break;
	}

	return true;
}
