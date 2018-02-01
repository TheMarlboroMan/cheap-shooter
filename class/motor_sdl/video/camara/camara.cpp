#include "camara.h"

using namespace DLibV;

Camara::Camara(Sint16 p_x, Sint16 p_y, Uint16 p_w, Uint16 p_h, unsigned int p_px, unsigned int p_py)
{
	this->caja.x=p_x;
	this->caja.y=p_y;
	this->caja.w=p_w;
	this->caja.h=p_h;
	this->con_clip=true;
	this->desp_x=0;
	this->desp_y=0;
	this->pos_x=p_px;
	this->pos_y=p_py;

	this->limitada=false;
	this->limite_min_x=0;
	this->limite_min_y=0;
	this->limite_max_x=0;
	this->limite_max_y=0;

	this->siguiendo_a=NULL;
	this->seguimiento_offset_x=0;
	this->seguimiento_offset_y=0;

	this->sincronizar_cajas();
}

Camara::~Camara()
{
	this->siguiendo_a=NULL;
}

void Camara::sincronizar_cajas()
{
	this->caja_ficticia.x=this->caja.x-this->desp_x;
	this->caja_ficticia.y=this->caja.y-this->desp_y;
	this->caja_ficticia.w=this->caja.w;
	this->caja_ficticia.h=this->caja.h;
}

/*Esta es la que nos da un rectángulo que refleja la posición de la cámara
en la pantalla. Ojo: no representa al lugar a donde apunta, sino simplemente a 
las coordenadas que ocupa la cámara en la pantalla.*/

SDL_Rect Camara::acc_caja_pos() const
{
	SDL_Rect resultado;
	resultado.x=this->pos_x;
	resultado.y=this->pos_y;
	resultado.w=this->caja.w;
	resultado.h=this->caja.h;
	return resultado;
}

/*Mueve la posición a la que apunta la cámara en la pantalla. Se usan las
coordenadas provistas.*/

void Camara::enfocar_a(Sint16 p_x, Sint16 p_y)
{
	if(this->limitada)
	{
		if(p_x >= this->limite_min_x && p_x <= this->limite_max_x) this->caja.x=p_x;
		else if(p_x < this->limite_min_x) this->caja.x=this->limite_min_x;
		else if(p_x > this->limite_max_x) this->caja.x=this->limite_max_x;

		if(p_y >= this->limite_min_y && p_y <= this->limite_max_y) this->caja.y=p_y;
		else if(p_y < this->limite_min_y) this->caja.y=this->limite_min_y;
		else if(p_y > this->limite_max_y) this->caja.y=this->limite_max_y;
	}
	else
	{
		this->caja.x=p_x;
		this->caja.y=p_y;
	}

	this->sincronizar_cajas();
}

/*Mueve la posición a la que apunta la cámara sumando las cantidades provistas
en los parámetros.*/

void Camara::movimiento_relativo(Sint16 p_x, Sint16 p_y)
{
	this->enfocar_a(this->caja.x+p_x, this->caja.y+p_y);
}

/*Establece los límites a los que la cámara puede apuntar: es decir, los límites
máximos para la cámara dentro del "mundo".*/

void Camara::establecer_limites(Sint16 p_min_limite_x, Sint16 p_min_limite_y, Sint16 p_max_limite_x, Sint16 p_max_limite_y)
{
	this->limitada=true;
	this->limite_max_x=p_max_limite_x-this->caja.w+this->desp_x;
	this->limite_max_y=p_max_limite_y-this->caja.h+this->desp_y;
	this->limite_min_x=p_min_limite_x;
	this->limite_min_y=p_min_limite_y;
}

void Camara::limpiar_limite()
{
	this->limitada=false;
	this->limite_max_x=0;
	this->limite_max_y=0;
	this->limite_min_x=0;
	this->limite_min_y=0;
}

/*
void Camara::establecer_seguimiento(Representacion_grafica * const p_rep, int p_offset_x, int p_offset_y)
{
	this->siguiendo_a=p_rep;
	this->seguimiento_offset_x=p_offset_x;
	this->seguimiento_offset_y=p_offset_y;
}

void Camara::establecer_seguimiento(Representacion_grafica const& p_rep, int p_offset_x, int p_offset_y)
{
	this->siguiendo_a=const_cast<Representacion_grafica *> (&p_rep);
	this->seguimiento_offset_x=p_offset_x;
	this->seguimiento_offset_y=p_offset_y;
}
*/

void Camara::establecer_seguimiento(const SDL_Rect& p_rep, int p_offset_x, int p_offset_y)
{
	this->siguiendo_a=&p_rep;
	this->seguimiento_offset_x=p_offset_x;
	this->seguimiento_offset_y=p_offset_y;
}

void Camara::calcular_seguimiento()
{
	if(this->siguiendo_a)
	{
//		SDL_Rect pos_rep=this->siguiendo_a->acc_posicion();

		int x=this->siguiendo_a->x - (this->caja.w / 2) + this->seguimiento_offset_x;
		int y=this->siguiendo_a->y - (this->caja.h / 2) + this->seguimiento_offset_y;

		this->enfocar_a(x,y);
	}
}

bool Camara::caja_en_toma(const SDL_Rect& p_rep) const
{	
	//Ojo, esto tiene en cuenta el desplazamiento x e y de esta cámara usando la caja ficticia....
	return DLibH::Herramientas_SDL::rectangulos_superpuestos(this->caja_ficticia, p_rep, true);
}
