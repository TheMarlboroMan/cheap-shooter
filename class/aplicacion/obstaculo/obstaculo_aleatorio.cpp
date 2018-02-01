#include "obstaculo_aleatorio.h"

Obstaculo_aleatorio::Obstaculo_aleatorio(unsigned int mod_contador, unsigned int p_tipo, bool p_desaparece, bool p_edu)
	:Obstaculo(), 
	tipo_actor_aleatorio(p_tipo), desaparece_colision(p_desaparece), 
	afectado_edu(p_edu)
{
	this->preparar_actor(mod_contador);
}

Obstaculo_aleatorio::~Obstaculo_aleatorio()
{

}

/*Auí se establecen las dimensiones y al final el mapa de colisión...*/

void Obstaculo_aleatorio::establecer_caracteristicas()
{
	int w=0, h=0, desp_rep_x=0, desp_rep_y=0;

	switch(this->tipo_actor_aleatorio)
	{
		case Actor_mapa::T_OBSTACULO_26_26:
			w=26;
			h=26;
			desp_rep_x=-3;
			desp_rep_y=-3;
		break;

		case Actor_mapa::T_OBSTACULO_32_64:
			w=32;
			h=64;
		break;

		case Actor_mapa::T_OBSTACULO_64_26:
			w=64;
			h=32;
		break;

		case Actor_mapa::T_OBSTACULO_128_32:
			w=128;
			h=32;
		break;
	}

	this->w=w;
	this->h=h;
	this->posicion.w=w;
	this->posicion.h=h;
	this->establecer_desp_rep_x(desp_rep_x);
	this->establecer_desp_rep_y(desp_rep_y);
}

/*Aquí se establecen las representaciones por defecto para cada tipo.
Lo que se hace es establecer los valores por defecto y luego llamar al 
método que hace uso de ellos.
*/

void Obstaculo_aleatorio::establecer_representacion()
{
	unsigned int indice_recurso=0;
	SDL_Rect caja_recorte=DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0,0,0);

	caja_recorte.w=this->w;
	caja_recorte.h=this->h;

	switch(this->tipo_actor_aleatorio)
	{
		case Actor_mapa::T_OBSTACULO_26_26:
			caja_recorte.x=64;
			caja_recorte.y=0; 
			caja_recorte.w=32;
			caja_recorte.h=32;
			indice_recurso=Recursos::G_TILE_JUEGO;
		break;

		case Actor_mapa::T_OBSTACULO_32_64:
			caja_recorte.x=96;
			caja_recorte.y=0;
			indice_recurso=Recursos::G_TILE_JUEGO;
		break;

		case Actor_mapa::T_OBSTACULO_64_26:
			caja_recorte.x=0;
			caja_recorte.y=32;
			indice_recurso=Recursos::G_TILE_JUEGO;
		break;

		case Actor_mapa::T_OBSTACULO_128_32:
			caja_recorte.x=0;
			caja_recorte.y=64;
			indice_recurso=Recursos::G_TILE_JUEGO;
		break;
	}

	Representable::mut_indice_recurso(indice_recurso);
	this->crear_representacion(caja_recorte);
}

/*Recibe el recorte porque se ha calculado antes pero no puede aplicarse
hasta que la representación haya sido creada...*/

void Obstaculo_aleatorio::crear_representacion(const SDL_Rect& caja_recorte)
{
	if(this->con_representacion()) return;
	else
	{
		this->establecer_representacion_bitmap_dinamica();	
		Representable::establecer_posicion(this->posicion.x, this->posicion.y);
		Representable::establecer_recorte(caja_recorte);	
		this->actualizar_representacion();
	}
}	

void Obstaculo_aleatorio::actualizar_representacion()
{
	Representable::establecer_posicion(this->posicion);
}
