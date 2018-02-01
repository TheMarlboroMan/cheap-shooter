#include "actor.h"

SDL_Rect Actor::caja_terreno_juego=DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0, Medidas::ANCHO_TERRENO_JUEGO, Medidas::ALTO_TERRENO_JUEGO);

Actor::Actor():vector_mov(0,0)
{
	this->mapa_colision=NULL;
	this->superficie=0;
}	

Actor::~Actor()
{

}

/*Esto es una aberración pero ocurre que el actor tiene una propiedad estática
que no es más que una copia del rectángulo de la cámara de modo que puede
decir si está o no en pantalla...*/

bool Actor::es_en_pantalla() const
{
	return DLibH::Herramientas_SDL::rectangulos_superpuestos(this->posicion, Actor::caja_terreno_juego);
}

/*
void Actor::establecer_posicion()
{
	this->posicion.x=DLibH::Herramientas::round(this->x);
	this->posicion.y=DLibH::Herramientas::round(this->y);
}
*/

void Actor::establecer_posicion(int px, int py)
{
	this->posicion.x=px;
	this->posicion.y=py;
}

void Actor::posicion_desde_coordenadas_propias()
{
	this->posicion.x=DLibH::Herramientas::round(this->x);
	this->posicion.y=DLibH::Herramientas::round(this->y);
}

void Actor::detener_vector()
{
	this->vector_mov.x=0;
	this->vector_mov.y=0;
}

void Actor::aplicar_vector(float p_delta)
{
	this->x+=this->vector_mov.x*p_delta;
	this->y+=this->vector_mov.y*p_delta;
	this->establecer_posicion();
}

void Actor::aplicar_vector_externo(const DLibH::Vector_2d& v, float p_delta)
{
	this->x+=v.x*p_delta;
	this->y+=v.y*p_delta;
	this->establecer_posicion();
}

void Actor::controlar_posiciones()
{

}

void Actor::ir_a(float x, float y)
{	
	this->x=x;
	this->y=y;
	this->establecer_posicion();
}

void Actor::establecer_vector_desde_actor(const Actor& p_otro, float p_divisor)
{
	this->vector_mov.x=p_otro.vector_mov.x / p_divisor;
	this->vector_mov.y=p_otro.vector_mov.y / p_divisor;
}

void Actor::establecer_posicion_desde_actor(const Actor& p_otro, float p_x, float p_y)
{
	this->x=p_otro.x+p_x;
	this->y=p_otro.y+p_y;
	this->posicion.x=p_otro.posicion.x+p_x;
	this->posicion.y=p_otro.posicion.y+p_y;
}

//De timo... En realidad NO usa los floats para esto, sino la posición.

bool Actor::en_colision_con(Actor const& otro) const
{

//	return DLibH::Herramientas_SDL::rectangulos_superpuestos(this->posicion, otro.posicion);

	SDL_Rect caja=DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0,0,0);

	if(!DLibH::Herramientas_SDL::rectangulos_superpuestos(this->posicion, otro.posicion, caja)) 
	{
		return false;
	}
	else 
	{
		const DLibV::Mapa_colision_bitmap * mapa_uno=this->obtener_mapa_colision();
		const DLibV::Mapa_colision_bitmap * mapa_otro=otro.obtener_mapa_colision();

		if(mapa_uno && mapa_otro)
		{
			return DLibV::Mapa_colision_bitmap::comparar_mapas_posicionados(*mapa_uno, *mapa_otro, this->posicion, otro.posicion, caja);
		}
		else
		{
			if(!mapa_uno && !mapa_otro)
			{
				return true;	//Ya sabemos que han chocado... Por eso está aquí.
			}
			else
			{
				if(mapa_uno)
				{
					return DLibV::Mapa_colision_bitmap::comparar_mapa_con_rectangulo(*mapa_uno, this->posicion, otro.posicion);
				}
				else
				{
					return DLibV::Mapa_colision_bitmap::comparar_mapa_con_rectangulo(*mapa_otro, otro.posicion, this->posicion);
				}
			}
		}
	}
}

bool Actor::en_colision_con(const SDL_Rect &p_caja) const
{
	if(!DLibH::Herramientas_SDL::rectangulos_superpuestos(this->posicion, p_caja)) 
	{
		return false;
	}
	else 
	{
		const DLibV::Mapa_colision_bitmap * mapa_uno=this->obtener_mapa_colision();

		if(!mapa_uno)
		{
			return true;
		}
		else
		{	
			return DLibV::Mapa_colision_bitmap::comparar_mapa_con_rectangulo(*mapa_uno, this->posicion, p_caja);
		}
	}
}

DLibH::Vector_2d Actor::obtener_vector_unidad_hasta_actor(const Actor& p_a)
{
	DLibH::Vector_2d resultado=DLibH::Vector_2d::obtener_para_puntos(p_a.x, p_a.y, this->x, this->y);
	return resultado;
}

DLibH::Vector_2d Actor::obtener_vector_unidad_hasta_posicion(float p_x, float p_y)
{
	DLibH::Vector_2d resultado=DLibH::Vector_2d::obtener_para_puntos(p_x, p_y, this->x, this->y);
	return resultado;
}
/*
DLibH::Vector_2d Actor::obtener_vector_unidad_desde_hasta(Actor * p_a, Actor * p_b)
{
	DLibH::Vector_2d resultado=DLibH::Vector_2d::obtener_para_puntos(p_b->x, p_b->y, p_a->x, p_a->y);
	return resultado;
}
*/

DLibH::Vector_2d Actor::obtener_vector_unidad_desde_hasta(const Actor& p_a, const Actor& p_b)
{
	DLibH::Vector_2d resultado=DLibH::Vector_2d::obtener_para_puntos(p_b.x, p_b.y, p_a.x, p_a.y);
	return resultado;
}

void Actor::sumar_vector(DLibH::Vector_2d p_v, float p_multiplicador)
{
//	p_v=p_v*p_multiplicador;
	p_v*=p_multiplicador;
	this->vector_mov=this->vector_mov+p_v;
}

void Actor::restar_vector(DLibH::Vector_2d p_v, float p_multiplicador)
{
//	p_v=p_v*p_multiplicador;
	p_v*=p_multiplicador;
	this->vector_mov=this->vector_mov-p_v;
}

void Actor::establecer_vector(DLibH::Vector_2d p_v, float p_multiplicador)
{
//	p_v=p_v*p_multiplicador;
	p_v*=p_multiplicador;
	this->vector_mov=p_v;
}

void Actor::recargar_posicion()
{
	this->posicion.x=x;
	this->posicion.y=y;
	this->posicion.w=w;
	this->posicion.h=h;
}

bool Actor::recibir_senal_trigger(const Accion_trigger_datos& p_datos)
{
	unsigned int p_senal=p_datos.acc_tipo();

	switch(p_senal)
	{
		case Accion_trigger::T_IR_A:
		{
			float x=0, y=0;

			x=p_datos.acc_datos_posicion().x;
			y=p_datos.acc_datos_posicion().y;

			this->ir_a(x, y);
			return true;
		}

		case Accion_trigger::T_CAMBIAR_VECTOR:
		{
			float x=0, y=0;

			x=p_datos.acc_datos_posicion().x;
			y=p_datos.acc_datos_posicion().y;
			this->vector_mov.x=x / 100;
			this->vector_mov.y=y / 100;
			return true;
		}
		break;

		default:
			return true;
		break;
	}
}
