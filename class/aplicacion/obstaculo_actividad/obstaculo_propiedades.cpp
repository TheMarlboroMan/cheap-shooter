#include "obstaculo_propiedades.h"

//Este debería heredar de algún tipo de actor invisible... O algo así.
//Es una putada como un castillo tal y como está todo organizado: habría que
//hacer que la herencia por "representable" estuviera mucho más adelante
//y daría luego problemas con los vectores de actores... Mejor pasar.

Obstaculo_propiedades::Obstaculo_propiedades():
	Obstaculo_actividad(), propiedades_artefactos(0), nueva_velocidad(0), 
 	marca_fin_nivel(false), activo(false), usado(false)
{
	this->preparar_actor(0);
	mut_actua_fuera_de_pantalla(true);
}

Obstaculo_propiedades::~Obstaculo_propiedades()
{

}

void Obstaculo_propiedades::establecer_caracteristicas()
{
	this->w=32;
	this->h=32;
}

/* Realmente no tiene representación... Tan fácil como eso.*/

void Obstaculo_propiedades::establecer_representacion()
{

}

void Obstaculo_propiedades::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p)
{
	this->propiedades_artefactos=p.param1;
	this->nueva_velocidad=p.param2;
	this->velocidad_cambio=p.param3;
	this->marca_fin_nivel=(bool) p.param4;
}

bool Obstaculo_propiedades::recibir_senal_trigger(const Accion_trigger_datos& p_datos)
{
	unsigned int p_senal=p_datos.acc_tipo();

	switch(p_senal)
	{
		case Accion_trigger::T_PROPIEDADES_NIVEL:
			this->activo=true; //Esto permite que el controlador inicie el cambio de color.
			return true;	//Devuelve verdadero porque el cambio es instantáneo...
		break;

		default:
			return true;
		break;
	}
}
