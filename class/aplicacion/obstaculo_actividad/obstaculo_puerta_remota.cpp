#include "obstaculo_puerta_remota.h"

Obstaculo_puerta_remota::Obstaculo_puerta_remota(unsigned int p_param):
	Obstaculo_puerta(p_param), tiempo_abierta(0.0f), tiempo_abierta_actual(0.0f),
	autocierre(false), iniciado_movimiento(false)
{

}

Obstaculo_puerta_remota::~Obstaculo_puerta_remota()
{

}

/*Si el estado es abierto, descenderá el contador de tiempo abierto hasta que
se termine. En ese momento se inicia el estado "cerrando". Cuando se inicie
el estado cerrando restauramos el tiempo de apertura inicial.*/


void Obstaculo_puerta_remota::implementacion_procesar_turno(float p_delta)
{
	if(!this->autocierre) return;

	switch(this->estado_actual)
	{
		case E_ABIERTO:
			if(restar_y_restaurar_valor(this->tiempo_abierta_actual, this->tiempo_abierta, p_delta))
			{
				this->estado_actual=E_INICIO_CERRAR;
			}
		break;	
	}
}

bool Obstaculo_puerta_remota::recibir_senal_trigger(const Accion_trigger_datos& p_datos)
{
	unsigned int p_senal=p_datos.acc_tipo();

	//Ignoramos la señal si ya estamos en marcha.
	if(this->iniciado_movimiento) 
	{
		if(this->estado_actual==E_CERRADO || this->estado_actual==E_ABIERTO)
		{
			this->iniciado_movimiento=false;
			return true;
		}
		else
		{
			return false;
		}
	}
	else 
	{
		switch(p_senal)
		{
			case Accion_trigger::T_ACTIVAR:

				if(this->estado_actual==E_CERRADO) 
				{
					this->estado_actual=E_INICIO_ABRIR;
					this->iniciado_movimiento=true;
				}
			break;

			case Accion_trigger::T_DESACTIVAR:

				if(this->estado_actual==E_ABIERTO) 
				{
					this->estado_actual=E_INICIO_CERRAR;
					this->iniciado_movimiento=true;
				}
			break;

			case Accion_trigger::T_ACTIVAR_DESACTIVAR:

				if(this->estado_actual==E_CERRADO) 
				{
					this->estado_actual=E_INICIO_ABRIR;
					this->iniciado_movimiento=true;
				}
				else if(this->estado_actual==E_ABIERTO) 
				{
					this->estado_actual=E_INICIO_CERRAR;
					this->iniciado_movimiento=true;
				}
			break;
		}

		//Devolvemos false. Sólo es true más arriba, cuando ha finalizado.
		return false;
	}
}
//El parámetro aquí es el tiempo de apertura.

void Obstaculo_puerta_remota::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p)
{
	this->tiempo_abierta=p.param1 / 1000;
	limitar_valor_minimo_tiempo(this->tiempo_abierta);
	this->tiempo_abierta_actual=this->tiempo_abierta;

	this->autocierre=(bool) p.param2;
}

//Un sonido para el estado "cerrado" y otro para el estado "abierto".

unsigned short int Obstaculo_puerta_remota::actuar_sonido()
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
