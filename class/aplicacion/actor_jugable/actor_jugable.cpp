#include "actor_jugable.h"

void Actor_jugable::procesar_retraccion_vectores(float p_delta)
{
	/*
	La retracción se produce cuando no se hace input de movimiento
	o cuando se lleva la contraria al vector de turno... O cuando 
	directamente le indicamos que no lo haga por medio del miembro
	"retraer_vectores"
	*/

	if(!this->retraer_vectores)
	{
		return;
	}
	else
	{
		float F=this->factor_retraccion_vectores()*p_delta;

		bool vector_x_en_contra=this->input_x!=0 && ( (this->input_x < 0 && this->vector_mov.x > 0) || (this->input_x > 0 && this->vector_mov.x < 0));
		bool vector_y_en_contra=this->input_y!=0 && ( (this->input_y < 0 && this->vector_mov.y > 0) || (this->input_y > 0 && this->vector_mov.y < 0));

		/*Hacemos las comprobaciones por separado: si retraemos hacia la
		izquierda no queremos terminar andando hacia la izquierda*/

		if(this->vector_mov.x && (!this->input_x || vector_x_en_contra))
		{
			if(this->vector_mov.x > 0) 
			{
				this->vector_mov.x -= F;
				if(this->vector_mov.x < 0) this->vector_mov.x=0;
			}
			else 
			{
				this->vector_mov.x += F;
				if(this->vector_mov.x > 0) this->vector_mov.x=0;
			}
		}

		if(this->vector_mov.y && (!this->input_y || vector_y_en_contra))
		{
			if(this->vector_mov.y > 0) 
			{
				this->vector_mov.y -= F;
				if(this->vector_mov.y < 0) this->vector_mov.y=0;
			}
			else 
			{
				this->vector_mov.y += F;
				if(this->vector_mov.y > 0) this->vector_mov.y=0;
			}
		}
	}		
}

/*La explicación al "terreno de juego" es la misma que se da a 
"permite_vector_cero" en la clase "Actor": para no meter un parámetro
y tener que remodelar toda la estructura metemos ese parche y no nos dan un 
óscar.
*/

void Actor_jugable::controlar_posiciones()
{
	if(!this->debe_controlar_posiciones)
	{
		return;
	}	
	else
	{
		if(this->x < 0) 
		{
			this->x=0;
			this->vector_mov.x=0;
		}
		else if(this->x+(float)this->posicion.w > Medidas::ANCHO_TERRENO_JUEGO) 
		{
			this->x=Medidas::ANCHO_TERRENO_JUEGO-(float)posicion.w;
			this->vector_mov.x=0;
		}

		Sint16 offset_y=-Actor::acc_terreno_juego_y();

		if(this->y+offset_y < 0) 
		{
			this->y=-offset_y;
			this->vector_mov.y=0;
		}
		else if(this->y+(float)this->posicion.h+offset_y > Medidas::ALTO_TERRENO_JUEGO) 
		{
			this->y=Medidas::ALTO_TERRENO_JUEGO-(float)posicion.h-offset_y;
			this->vector_mov.y=0;
		}
	}
}


Actor_jugable::Actor_jugable()
	:Actor(), 
	input_x(0), input_y(0), estado_aceleracion(EA_NADA), retraer_vectores(true), debe_controlar_posiciones(true)
{

}

Actor_jugable::~Actor_jugable()
{

}

void Actor_jugable::recibir_input(int p_x, int p_y, float p_delta)
{
	this->input_x=p_x;
	this->input_y=p_y;

	if(!input_y)
	{
		estado_aceleracion=EA_NADA;
	}	
	else
	{
		if(input_y > 0) estado_aceleracion=EA_FRENANDO;
		else estado_aceleracion=EA_ACELERANDO;
	}

	float FM=this->factor_multiplicador_input();
	float MV=this->maximo_vector_desplazamiento();

	float temp=this->vector_mov.x+(FM*this->input_x*p_delta);
	if(temp >= -MV && temp <= MV) this->vector_mov.x=temp;

	temp=this->vector_mov.y+(FM*this->input_y*p_delta);
	if(temp >= -MV && temp <= MV) this->vector_mov.y=temp;
}

void Actor_jugable::establecer_posicion()
{
	int x=this->acc_int_x();
	int y=this->acc_int_y();

	Actor::establecer_posicion(x, y);
	Representable::establecer_posicion(x,y);
}
