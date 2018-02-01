#include "obstaculo_puerta_disparar.h"

Obstaculo_puerta_disparar::Obstaculo_puerta_disparar(unsigned int p_param):
	Obstaculo_puerta(p_param), tiempo_abierta(0.0f), tiempo_abierta_actual(0.0f)
{

}

Obstaculo_puerta_disparar::~Obstaculo_puerta_disparar()
{

}

/*Si el estado es abierto, descenderá el contador de tiempo abierto hasta que
se termine. En ese momento se inicia el estado "cerrando". Cuando se inicie
el estado cerrando restauramos el tiempo de apertura inicial.*/


void Obstaculo_puerta_disparar::implementacion_procesar_turno(float p_delta)
{
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

//Si el estado es "cerrado" inicia el estado "abriendo".

bool Obstaculo_puerta_disparar::procesar_colision_proyectil(Proyectil * proyectil)
{
	if(!proyectil->es_de_faccion(Definiciones::F_JUGADOR)) 
	{
		return false;
	}
	else
	{
		if(this->estado_actual==E_CERRADO)
		{
			this->estado_actual=E_INICIO_ABRIR;
			return true;
		}
		else
		{
			return false;
		}
	}
}

//El parámetro aquí es el tiempo de apertura.

void Obstaculo_puerta_disparar::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p)
{
	this->tiempo_abierta=p.param1 / 1000;
	limitar_valor_minimo_tiempo(this->tiempo_abierta);
	this->tiempo_abierta_actual=this->tiempo_abierta;
}

//Un sonido para el estado "cerrado" y otro para el estado "abierto".

unsigned short int Obstaculo_puerta_disparar::actuar_sonido()
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
