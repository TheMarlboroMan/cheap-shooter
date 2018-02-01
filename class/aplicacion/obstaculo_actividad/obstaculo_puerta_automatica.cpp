#include "obstaculo_puerta_automatica.h"

Obstaculo_puerta_automatica::Obstaculo_puerta_automatica(unsigned int p_param):
	Obstaculo_puerta(p_param), tiempo_abierta(0.0f), tiempo_cerrada(0.0f),
	tiempo_abierta_actual(0.0f), tiempo_cerrada_actual(0.0f)
{

}

Obstaculo_puerta_automatica::~Obstaculo_puerta_automatica()
{

}

/*Si el estado es abierto, descenderá el contador de tiempo abierto hasta que
se termine. En ese momento se inicia el estado "cerrando". Cuando se inicie
el estado cerrando restauramos el tiempo de apertura inicial. Añadimos también
el estado "abierto", que hace justo lo contrario.*/


void Obstaculo_puerta_automatica::implementacion_procesar_turno(float p_delta)
{
	switch(this->estado_actual)
	{
		case E_ABIERTO:
			if(restar_y_restaurar_valor(this->tiempo_abierta_actual, this->tiempo_abierta, p_delta))
			{
				this->estado_actual=E_INICIO_CERRAR;
			}
		break;

		case E_CERRADO:
			if(restar_y_restaurar_valor(this->tiempo_cerrada_actual, this->tiempo_cerrada, p_delta))
			{
				this->estado_actual=E_INICIO_ABRIR;
			}
		break;
	}
}

//El parámetro aquí es el tiempo de apertura y cierre.

void Obstaculo_puerta_automatica::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p)
{
	this->tiempo_abierta=p.param1 / 1000;
	limitar_valor_minimo_tiempo(this->tiempo_abierta);
	this->tiempo_abierta_actual=this->tiempo_abierta;

	this->tiempo_cerrada=p.param2 / 1000;
	limitar_valor_minimo_tiempo(this->tiempo_cerrada);
	this->tiempo_cerrada_actual=this->tiempo_cerrada;

	mut_actua_fuera_de_pantalla(p.param3);
}

//Un sonido para el estado "cerrado" y otro para el estado "abierto".

unsigned short int Obstaculo_puerta_automatica::actuar_sonido()
{
	switch(this->estado_actual)
	{
		case E_INICIO_ABRIR:
		case E_FIN_ABRIR:
		case E_INICIO_CERRAR:
		case E_FIN_CERRAR:
			return Recursos::S_DISPARO;
		break;
		default:
			return 0;
		break;
	}
}
