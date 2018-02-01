#include "no_jugador.h"

float No_jugador::divisor_movimiento=1;
bool No_jugador::permitir_vector_cero=false;

No_jugador::No_jugador():Actor(), 
	marcado_para_borrar(false), movimiento_natural(true),
	rebotes_restantes(0), tiempo_herido(0.0f)
{

}

/*
//TODO: Revisar... supuestamente toda la cadena para arriba debe llamar al
destructor del actor de modo que quitaríamos el destructor particular de cada
uno*/
No_jugador::~No_jugador()
{

}

void No_jugador::establecer_posicion()
{
	int x=this->acc_int_x();
	int y=this->acc_int_y();

	Actor::establecer_posicion(x, y);
	Representable::establecer_posicion(x,y);
}

void No_jugador::procesar_tiempo_herido(float p_delta)
{
	if(tiempo_herido)
	{
		tiempo_herido-=p_delta / No_jugador::divisor_movimiento;

		if(tiempo_herido>0.0f)
		{
			procesar_tiempo_herido_herido();
		}
		else
		{
			tiempo_herido=0.0f;
			procesar_tiempo_herido_ok();
		}
	}
}

/*Ojo: si esto viene de un Actor_mapa que tiene punto de ruta no vamos a entrar
por aquí: directamente irá a aplicar el vector a la clase padre...*/

void No_jugador::procesar_turno(float p_delta)
{
	if(tiempo_herido) procesar_tiempo_herido(p_delta);	//Ojo, esto aparece en actor_mapa cuando NO llega por aquí!.

	if(movimiento_natural)
	{
		this->aplicar_vector(p_delta/No_jugador::divisor_movimiento);

		if(this->puede_rebotar())
		{ 
			if(this->x <=0 || this->x+this->posicion.w >= Medidas::ANCHO_TERRENO_JUEGO)
			{
				this->rebotes_restantes--;

				if(this->rebotes_restantes>=0)
				{
					this->vector_mov.x=-this->vector_mov.x;
				}
			}
		}

		//Para evitar que se quede parado. Lo de la propiedad estática, leer
		//el método mut_permitir_vector_cero.
		if(this->vector_mov.y==0 && !No_jugador::permitir_vector_cero)
		{
			this->vector_mov.y++;
		}
	}
}

/*Indica si el No_jugador ya está fuera de la zona de juego y puede ser 
eliminado, por ejemplo. En un principio bastaba con comprobar si estaba dentro
de la pantalla. Luego empezamos a comprobar que está fuera de la pantalla porque
su vector le empuja. 

Con la llegada de los niveles prehechos en el editor hay que comprobar también
(en un principio) si, aún sin vector, se han salido por la parte de abajo 
(el scroll será de arriba a abajo).*/

bool No_jugador::es_fuera_de_juego() const
{
	bool resultado=false;

	//El -16 nos da un márgen: los obstáculos vienen desde fuera de la pantalla y 
	//si no lo hacemos de este modo nunca aparecerán.

	//Inferior y superior se refiere a la representación en pantalla: 
	//para Y superior sería 0 e inferior 600.

	int lim_x_der=Actor::acc_terreno_juego_x()+(w)+Medidas::ANCHO_TERRENO_JUEGO;
	int lim_x_izq=Actor::acc_terreno_juego_x()-16;
	int lim_y_inf=Actor::acc_terreno_juego_y()+(h)+Medidas::ALTO_TERRENO_JUEGO;
	int lim_y_sup=Actor::acc_terreno_juego_y()-16;

	if(this->vector_mov.y || this->vector_mov.x)
	{
		//Si hay vector se comprueba que no sale de la pantalla por sus propios medios.

		 resultado=(this->y > lim_y_inf && this->vector_mov.y > 0) || 
			(this->y+this->h < lim_y_sup && this->vector_mov.y < 0) ||
		  	(this->x+this->w > lim_x_der && this->vector_mov.x > 0) || 
			(this->x+this->w < lim_x_izq && this->vector_mov.x < 0);
	}
	else
	{
		//Si no hay vector de ningún tipo sólo comprobamos que no se salga "por abajo".
		resultado=this->y > lim_y_inf;
	}

	return resultado;
}

/*Hasta bastante avanzado el desarrollo era un requisito que el vector de 
movimiento y nunca pudiera ser cero para ningún "no_jugador". Esto podría 
permitir que los actores se quedaran para siempre en la pantalla y nunca se
terminarían los niveles. 

Con la llegada de los niveles hechos en el editor esto queda obsoleto porque
los obstáculos ya no caen siempre hacia el jugador, sino que el jugador se
mueve hacia los mismos, que están inmóviles.

Aunque lo ideal aquí era meter un parámetro más para permitir o no el vector 
cero al ejecutar cada turno de No_jugador, otras clases implementan este
mismo método y habría que cambiarlas todas. La solución es esta: no estamos
intentando ganar un oscar, sino terminar el juego :).
*/



void No_jugador::mut_permitir_vector_cero(bool p_valor)
{
	No_jugador::permitir_vector_cero=p_valor;
}

void No_jugador::establecer_divisor_movimiento(float p_v) 
{
	No_jugador::divisor_movimiento=p_v;
}
