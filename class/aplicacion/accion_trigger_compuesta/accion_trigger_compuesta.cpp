#include "accion_trigger_compuesta.h"

Accion_trigger_compuesta::Accion_trigger_compuesta(const Accion_trigger * p_accion):
	accion(p_accion),
	datos(accion->acc_tipo()),
	trigger(NULL),
	estado(0),
	ms_antes(accion->acc_ms_antes()), 
	ms_despues(accion->acc_ms_despues()),
	iniciada(false), 
	invalidada(false)
{
	if(this->ms_antes) this->estado=E_CUENTA_PRE;
	else this->estado=E_ACCION;
}

Accion_trigger_compuesta::~Accion_trigger_compuesta()
{
	this->accion=NULL;
	this->trigger=NULL;
	this->actores.clear();
}

/*Esto hace falta para que el vector de actores no se copie por referencia*/

Accion_trigger_compuesta& Accion_trigger_compuesta::operator=(const Accion_trigger_compuesta& p_otra)
{
	this->accion=p_otra.accion;
	this->datos=p_otra.datos;
	this->trigger=p_otra.trigger;
	this->actores=p_otra.actores;
	this->estado=p_otra.estado;
	this->ms_antes=p_otra.ms_antes;
	this->ms_despues=p_otra.ms_despues;
	this->iniciada=p_otra.iniciada;

	return *this;
}

bool Accion_trigger_compuesta::es_finalizada() const
{
	return this->estado==E_FINALIZADA;
}

void Accion_trigger_compuesta::turno(float p_delta)
{
	if(this->invalidada) return;

	switch(this->estado)
	{
		case E_CUENTA_PRE:

			this->ms_antes-=p_delta * 1000;
		
			if(this->ms_antes <= 0)
			{
				this->ms_antes=0;
				this->estado=E_ACCION;
			}
		break;

		case E_ACCION:

/* Aquí está el tema... Cada actor devolverá verdadero o falso a la hora de 
realizar su acción. Si devuelve verdadero es que todo ha ido bien y ha
terminado. Si devuelve falso es que aún no ha finalizado y que, por tanto,
la acción no puede decirle al trigger que ha terminado.

El trigger recibe dos mensajes: que hemos empezado (una sóla vez) y que hemos
finalizado (otra vez).

Para cada actor vamos a lanzar la acción. Según devuelvan verdadero podremos
eliminarlos del vector. Cuando no quede nada en el vector entonces podremos 
decir que hemos terminado. 
*/

			{
				if(!this->iniciada)
				{
					this->iniciada=true;
					if(this->trigger) this->trigger->recibir_inicio_accion();
				}

				std::vector<Actor_mapa * >::iterator	ini=this->actores.begin(),
									fin=this->actores.end();

				while(ini < fin)
				{
					if((*ini)->recibir_senal_trigger(this->datos))
					{
						ini=this->actores.erase(ini);
						fin=this->actores.end();
					}
					else
					{
						++ini;
					}
				}

				if(!this->actores.size())
				{
					if(this->trigger) this->trigger->recibir_finalizacion_accion();

					if(this->ms_despues) 
					{
						this->estado=E_CUENTA_POST;
					}
					else 
					{
						this->estado=E_FINALIZADA;
					}
				}
			}
		break;

		case E_CUENTA_POST:

			this->ms_despues-=p_delta * 1000;

			if(this->ms_despues <= 0)
			{
				this->ms_despues=0;
				this->estado=E_FINALIZADA;
			}
		break;

		case E_FINALIZADA:
			//Normalmente aquí no llega el código: cuando está finalizada
			//antes se pregunta si ha finalizado y se borra. No hay
			//código de cleanup.
		break;
	}
}

void Accion_trigger_compuesta::forzar_finalizacion()
{
	this->actores.clear();
	if(this->trigger) this->trigger->recibir_finalizacion_accion();
	this->estado=E_FINALIZADA;
}

void Accion_trigger_compuesta::eliminar_actor(Actor_mapa * p_actor)
{
	std::vector<Actor_mapa *>::iterator	ini=this->actores.begin(),
						fin=this->actores.end();

	while(ini < fin)
	{
		if( p_actor== (*ini))
		{
			ini=this->actores.erase(ini);
			fin=this->actores.end();
			break; 	//Asumimos que el actor ya no se presenta más veces.
		}
		++ini;
	}
}
