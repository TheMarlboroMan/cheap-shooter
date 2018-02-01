#include "representacion_patron.h"

using namespace DLibV;

Representacion_patron::Representacion_patron(const SDL_Rect& p_caja_pos,const SDL_Rect& p_caja_patron, Uint32 p_color, const Recurso_grafico* p_rec)
	:Primitiva_grafica(p_color), alineamiento_x(0), alineamiento_y(0)
{
	this->patron=const_cast<Recurso_grafico *>(p_rec);

	establecer_posicion(p_caja_pos);
	caja_patron=p_caja_patron;
	this->preparar_posicion();
}

Representacion_patron::Representacion_patron(const Representacion_patron& o):
	Primitiva_grafica(o),	
	caja_patron(o.caja_patron),
	patron(o.patron),
	alineamiento_x(o.alineamiento_x),
	alineamiento_y(o.alineamiento_y)
{

}

Representacion_patron& Representacion_patron::operator=(const Representacion_patron& p_otro)
{
	Primitiva_grafica::operator=(p_otro);

	this->patron=p_otro.patron;
	this->caja_patron=p_otro.caja_patron;

	return *this;
}

Representacion_patron::~Representacion_patron()
{
	this->patron=NULL;
}


bool Representacion_patron::volcado(SDL_Surface * p_superficie)
{
	if(!es_visible()) return false;

	SDL_Rect pos=acc_posicion();

	int pos_x=pos.x-this->alineamiento_x;
	int pos_y=pos.y-this->alineamiento_y;

	int fin_x=0;
	int fin_y=pos_y+pos.h;

	SDL_Rect destino=DLibH::Herramientas_SDL::nuevo_sdl_rect(pos_x+this->alineamiento_x, pos_y+this->alineamiento_y, pos.w, pos.h);
	SDL_Rect blit_original;
	SDL_GetClipRect(p_superficie, &blit_original);
	SDL_SetClipRect(p_superficie, &destino);

	Uint8 alpha_original=this->patron->acc_superficie()->format->alpha;
	if(this->acc_alpha()) this->patron->establecer_alpha(this->acc_alpha());

	while(pos_y < fin_y)
	{
		fin_x=pos_x+pos.w+this->alineamiento_x;

		while(pos_x < fin_x)
		{
			destino.x=pos_x;
			destino.y=pos_y;
			destino.w=pos.w;
			destino.h=pos.h;

			SDL_BlitSurface(this->patron->acc_superficie(), &caja_patron, p_superficie, &destino);
			pos_x+=this->caja_patron.w;
		}

		pos_y+=this->caja_patron.h;
		pos_x=pos.x-this->alineamiento_x;
	}

	if(this->acc_alpha()) this->patron->establecer_alpha(alpha_original);

	SDL_SetClipRect(p_superficie, &blit_original);

	return true;
}

bool Representacion_patron::volcado(SDL_Surface * p_superficie, const SDL_Rect& p_vista, const SDL_Rect& p_posicion, int desp_x, int desp_y)
{
	if(!es_visible()) return false;

	SDL_Rect pos=acc_posicion();

	int pos_x=pos.x+p_posicion.x-p_vista.x-this->alineamiento_x;
	int pos_y=pos.y+p_posicion.y-p_vista.y-this->alineamiento_y;

	int salto_x=this->caja_patron.w ? this->caja_patron.w : 1;
	int salto_y=this->caja_patron.h ? this->caja_patron.h : 1;
	
	int fin_x=0;
	int fin_y=pos_y+pos.h+this->alineamiento_y;

	if(!this->es_estatica())
	{
		pos_x+=desp_x;
		pos_y+=desp_y;
	}

	SDL_Rect destino=DLibH::Herramientas_SDL::nuevo_sdl_rect(pos_x+this->alineamiento_x, pos_y+this->alineamiento_y, pos.w, pos.h);
	SDL_Rect blit_original;
	SDL_GetClipRect(p_superficie, &blit_original);
	SDL_SetClipRect(p_superficie, &destino);

	while(pos_y < fin_y)
	{
		fin_x=pos_x+pos.w+this->alineamiento_x;

		while(pos_x < fin_x)
		{
			destino.x=pos_x;
			destino.y=pos_y;
			destino.w=pos.w;
			destino.h=pos.h;

			SDL_BlitSurface(this->patron->acc_superficie(), &caja_patron, p_superficie, &destino);
			pos_x+=salto_x;
		}

		pos_y+=salto_y;
		pos_x=pos.x+p_posicion.x-p_vista.x-this->alineamiento_x;
	}

	SDL_SetClipRect(p_superficie, &blit_original);
	
	return true;
}

void Representacion_patron::actualizar_patron(const Recurso_grafico * p_rec)
{
	this->patron=const_cast<Recurso_grafico *>(p_rec);
}

void Representacion_patron::actualizar_caja_patron(const SDL_Rect& p_otra)
{
	this->caja_patron.x=p_otra.x;
	this->caja_patron.y=p_otra.y;
	this->caja_patron.w=p_otra.w;
	this->caja_patron.h=p_otra.h;
	this->preparar_posicion();
}

void Representacion_patron::actualizar_alineamiento(int p_x, int p_y)
{
	this->alineamiento_x=p_x;
	this->alineamiento_y=p_y;
}

void Representacion_patron::preparar_posicion()
{
	
}
