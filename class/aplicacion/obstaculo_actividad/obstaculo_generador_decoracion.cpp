#include "obstaculo_generador_decoracion.h"

Obstaculo_generador_decoracion::Obstaculo_generador_decoracion():
	Obstaculo_actividad(), activo(0), explosion(0), chispas(0), chatarra(0),
	humo_explosion(0)
{
	
	this->preparar_actor(0);
}

Obstaculo_generador_decoracion::~Obstaculo_generador_decoracion()
{

}

void Obstaculo_generador_decoracion::establecer_caracteristicas()
{
	this->w=32;
	this->h=32;
}

/* Realmente no tiene representación... Tan fácil como eso.*/

void Obstaculo_generador_decoracion::establecer_representacion()
{

}

void Obstaculo_generador_decoracion::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p)
{
	this->explosion=p.param1;
	this->chispas=p.param2;
	this->chatarra=p.param3;
	this->humo_explosion=p.param4;
}

bool Obstaculo_generador_decoracion::recibir_senal_trigger(const Accion_trigger_datos& p_datos)
{
	unsigned int p_senal=p_datos.acc_tipo();

	switch(p_senal)
	{
		case Accion_trigger::T_GENERAR_DECORACION:
			this->activo=true; //Esto permite que el controlador inicie su actividad.
			return true;	//Devuelve verdadero porque el cambio es instantáneo...
		break;

		default:
			return true;
		break;
	}
}
