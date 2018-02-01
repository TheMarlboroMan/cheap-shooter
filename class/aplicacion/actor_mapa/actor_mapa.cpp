#include "actor_mapa.h"

unsigned int Actor_mapa::MODO_EDITOR_ACTIVO=0;

Actor_mapa::Actor_mapa():No_jugador(), Receptor_trigger(), de_nivel_preparado(false), tag(0)
{

}

Actor_mapa::~Actor_mapa()
{

}

/*Este método devolverá false cuando no hay acciones especiales que llevar
a cabo tras la colisión y true cuando las pueda haber...*/

bool Actor_mapa::procesar_colision_proyectil(Proyectil * proyectil)
{
	return false;
}

void Actor_mapa::procesar_colision_jugador()
{

}


/*Intercepta el proceso normal para incluir el tema de los puntos de ruta. Si
hay un punto de ruta adoptará el comportamiento del mismo y se saltará el 
código de movimiento establecido en "No_jugador::procesar_turno" mediante 
"anular_movimiento_natural".
*/

void Actor_mapa::procesar_turno(float p_delta)
{
	if(!this->estructura_ruta.activa)
	{
		establecer_movimiento_natural();
	}
	else
	{
		anular_movimiento_natural();
		Actor::aplicar_vector_externo(this->estructura_ruta.vector_mov, p_delta/No_jugador::divisor_movimiento);		
	}

	No_jugador::procesar_turno(p_delta);
}


/*Implementación de la recepción de trigger. Es básica y común a todos los 
actores y sólo en aquellos en los que merezca la pena lo vamos a sobreescribir.
El que si tendrá para todos es el tema del waypoint. Si los que siguen tienen 
que tener waypoint tendrían que llamar a este método.

Por encima de esta habría otra en "Actor", supuestamente...
*/

bool Actor_mapa::recibir_senal_trigger(const Accion_trigger_datos& p_datos)
{
	unsigned int p_senal=p_datos.acc_tipo();

	switch(p_senal)
	{
	/*El sistema de asignación de una ruta pasa por algunas historias. El 
	tipo "receptor_trigger" puede recibir una nueva dirección mediante el 
	método "reasignar". A su vez, puede almacenar el punto actual
	para que cuando haya llegado podamos preguntar a la lista. En el 
	primer paso, cuando se asigna, preguntaremos cual es el punto actual a 
	los datos de accion_trigger datos y se "reasignará" el destino a partir
	de ese punto. Cada vez que se llegue al destino preguntaremos a los
	datos si hay una nueva posición. El "receptor_trigger" guarda cual
	es el punto actual y la posición hacia la que ir. El resto está 
	almacenado en los datos de Accion_trigger_datos.*/	

		case Accion_trigger::T_RUTA:
		{
			//Lambda utilitaria.
			auto asignar_datos=[this, &p_datos](unsigned int punto)
			{
				Accion_trigger_datos::Datos_posicion er=p_datos.obtener_punto_ruta(punto);

				int destino_x=er.x+p_datos.acc_desp_x();
				int destino_y=er.y+p_datos.acc_desp_y();
				int velocidad=er.vel;
				this->estructura_ruta.reasignar(destino_x, destino_y, this->x, this->y, velocidad, punto);

//				if(er.instantaneo) this->ir_a(destino_x, destino_y);
			};

			//Si no está en marcha la activamos: esta es la "primera entrada".

			if(!this->estructura_ruta.en_marcha)
			{
				if(!p_datos.obtener_total_puntos_ruta())
				{
					return true;
				}
				else
				{
//					this->estructura_ruta.inicializar(p_datos.obtener_punto_inicial());
					asignar_datos(p_datos.obtener_punto_inicial());
					return false;
				}
			}
			//Podemos llegar aquí porque se haya recibido una señal de desactivar.
			else
			{
				//Está en marcha... Si no está activa es que la hemos apagado y por tanto queremos detener la marcha.
				if(!this->estructura_ruta.activa)
				{
					this->estructura_ruta.detener_marcha();				
					return true;
				}
				//En marcha y activa
				else
				{
					//Debe devolver true cuando haya llegado al punto. Se pasan los valores por referencia.
					bool resultado=this->estructura_ruta.en_destino_y_ajustar(this->x, this->y);
		
					if(resultado) 
					{
						//En este momento comprobamos si hay o no un punto siguiente...
						unsigned int actual=this->estructura_ruta.indice_punto;

						Accion_trigger_datos::Datos_posicion er=p_datos.obtener_punto_ruta(actual);
						unsigned int siguiente=er.siguiente;

						if(siguiente)
						{
							asignar_datos(siguiente);
							return false;
						}
						else
						{
							this->estructura_ruta.detener_marcha();				
							this->estructura_ruta.desactivar();
							return true;
						}
					}
				}
			}		
			return false;

		}
		break;

		case Accion_trigger::T_DESACTIVAR_RUTA:
			this->estructura_ruta.desactivar();	
			return true;
		break;

case Accion_trigger::T_MOVIMIENTO_RELATIVO:
		{
			//Lambda utilitaria.
			auto asignar_datos=[this, &p_datos]()
			{
				Accion_trigger_datos::Datos_posicion er=p_datos.acc_datos_posicion();

				//Calcular destino según la propia posición del actor y los valores que

				int destino_x=x+er.x;
				int destino_y=y+er.y;
				int velocidad=er.vel;
				this->estructura_ruta.reasignar(destino_x, destino_y, this->x, this->y, velocidad);
			};

			//Si no está en marcha la activamos: esta es la "primera entrada".

			if(!this->estructura_ruta.en_marcha)
			{
				asignar_datos();
				return false;
			}
			//Podemos llegar aquí porque se haya recibido una señal de desactivar.
			else
			{
				//Está en marcha... Si no está activa es que la hemos apagado y por tanto queremos detener la marcha.
				if(!this->estructura_ruta.activa)
				{
					this->estructura_ruta.detener_marcha();				
					return true;
				}
				//En marcha y activa
				else
				{
					//Debe devolver true cuando haya llegado al punto. Se pasan los valores por referencia.
		
					if(this->estructura_ruta.en_destino_y_ajustar(this->x, this->y))
					{
						this->estructura_ruta.detener_marcha();				
						this->estructura_ruta.desactivar();
						return true;
					}
				}
			}		
			return false;
		}
		break;

		case Accion_trigger::T_DETENER_MOVIMIENTO_RELATIVO:
			this->estructura_ruta.desactivar();
			return true;
		break;

		case Accion_trigger::T_HACER_VISIBLE:
			Representable::hacer_representable();
			return true;
		break;

		case Accion_trigger::T_HACER_INVISIBLE:
			Representable::hacer_no_representable();
			return true;
		break;

		case Accion_trigger::T_INTERCAMBIAR_VISIBILIDAD:
			Representable::intercambiar_representable();
			return true;
		break;		

		default:
			return Actor::recibir_senal_trigger(p_datos);
		break;
	}
}

void Actor_mapa::preparar_actor(unsigned int mod_contador)
{
	this->establecer_caracteristicas();
	this->iniciar(mod_contador);

	if(this->es_representable())
	{
		this->establecer_representacion();
		Representable::establecer_posicion(this->x, this->y, this->w, this->h);
	}
}

/*Este debe llamarse siempre una vez el método "preparar_actor" se haya 
ejecutado. Se usa simplemente para darle nuevas propiedades de posición y poca
cosa más.*/

void Actor_mapa::preparar_actor_mapa_manual(float p_x, float p_y, float v_x, float v_y)
{
	this->x=p_x;
	this->y=p_y;
	this->vector_mov.x=v_x;
	this->vector_mov.y=v_y;

	this->posicion.x=p_x;
	this->posicion.y=p_y;

	if(this->es_representable())
	{
		Representable::establecer_posicion(this->x, this->y, this->w, this->h);
	}

	this->de_nivel_preparado=true;
}

void Actor_mapa::iniciar(unsigned int mod_contador)
{
	//ESTO NO DEBERIA OCURRIR HASTA QUE NO SE HAYAN ASIGNADO TODOS LOS VALORES DE POSICION!!!!!!!!!!!....

	this->vector_mov.y=0;

	if(this->vel_max() || this->vel_min()) //Por si algún actor tiene los dos a cero: esto puede explotar si no.
	{
		int vel_mod=this->vel_max()+mod_contador;
		this->vector_mov.y=(rand() % vel_mod)+this->vel_min();
	}

	this->vector_mov.x=0;

	this->y=this->h;	//OJO: No -this->h: es unsigned int y terminaremos con un valor ENOOOORME.
	this->y=-this->y;	//Ahora es cuando le damos la vuelta.

	if(this->w < Medidas::U_ANCHO_TERRENO_JUEGO)
	{
		this->x=rand() % (Medidas::U_ANCHO_TERRENO_JUEGO-this->w);
	}
	//Excepción absurda para actores muy grandes.
	else
	{
		this->x=0;
	}

	this->posicion=DLibH::Herramientas_SDL::nuevo_sdl_rect(this->x, this->y, this->w, this->h);
}

//No hace nada... En algunos actores lo usaremos para recalcular la representación.
void Actor_mapa::procesar_cambio_dimensiones()
{

}

/*Este método lo usaremos para actores en nivel preparado al que haya que
pasarles algún parámetro para prepararlos...*/

void Actor_mapa::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p_params)
{
	//Nada....
}


void Actor_mapa::recibir_valores_actor_flexible(const Parametros_actor_flexible& rep)
{
	//Do nothing...
}

void Actor_mapa::recibir_valores_actor_ordenable_profundidad(const Parametros_actor_ordenable_profundidad& p)
{
	//Do nothing...
}

void Actor_mapa::recibir_valores_actor_redimensionable(const Parametros_actor_redimensionable& rep)
{
	this->mut_w(rep.w);
	this->mut_h(rep.h);
}
