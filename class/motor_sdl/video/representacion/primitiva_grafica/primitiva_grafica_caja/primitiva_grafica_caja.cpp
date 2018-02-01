#include "primitiva_grafica_caja.h"

using namespace DLibV;

Primitiva_grafica_caja::Primitiva_grafica_caja(const SDL_Rect& p_pos, Uint32 p_color)
	:Primitiva_grafica(p_color), hack_alpha(NULL), recrear_hack_alpha(false)
{
	establecer_posicion(p_pos);
	this->preparar_posicion();
}

Primitiva_grafica_caja::Primitiva_grafica_caja(const Primitiva_grafica_caja& p_otra)
	:Primitiva_grafica(p_otra), hack_alpha(p_otra.hack_alpha)
{

}

Primitiva_grafica_caja& Primitiva_grafica_caja::operator=(const Primitiva_grafica_caja& p_otro)
{
	Primitiva_grafica::operator=(p_otro);
	//No se copia el hack de alpha.

	return *this;
}

Primitiva_grafica_caja::~Primitiva_grafica_caja()
{
	if(hack_alpha) 
	{
		SDL_FreeSurface(hack_alpha);
	}
}

void Primitiva_grafica_caja::crear_hack_alpha(SDL_Surface * p_superficie)
{
	if(hack_alpha)  SDL_FreeSurface(hack_alpha);

	hack_alpha=DLibV::Utilidades_graficas_SDL::nueva_superficie_formato(p_superficie, acc_posicion(), 0, this->acc_pixel_color());
	SDL_SetColorKey(hack_alpha, 0, 0);
	recrear_hack_alpha=false;
}

bool Primitiva_grafica_caja::volcado(SDL_Surface * p_superficie)
{
	if(!es_visible()) return false;

	Uint8 alpha=this->acc_alpha();
	SDL_Rect pos=acc_posicion();

	if(alpha!=SDL_ALPHA_OPAQUE)
	{
		if(!hack_alpha || recrear_hack_alpha) crear_hack_alpha(p_superficie);
		SDL_SetAlpha(hack_alpha, SDL_SRCALPHA | SDL_RLEACCEL, alpha);
		return SDL_BlitSurface(hack_alpha, NULL, p_superficie, &pos) >= 0;
	}
	else
	{
		SDL_FillRect(p_superficie, &pos, this->acc_pixel_color());
		return true;
	}


}

bool Primitiva_grafica_caja::volcado(SDL_Surface * p_superficie, const SDL_Rect& p_vista, const SDL_Rect& p_posicion, int desp_x, int desp_y)
{
	if(!es_visible()) return false;

	SDL_Rect caja;
	SDL_Rect pos=acc_posicion();
	caja.x=pos.x+p_posicion.x-p_vista.x;
	caja.y=pos.y+p_posicion.y-p_vista.y;
	caja.w=pos.w;
	caja.h=pos.h;

	Uint8 alpha=this->acc_alpha();

	if(alpha!=SDL_ALPHA_OPAQUE)
	{
		if(!hack_alpha || recrear_hack_alpha) crear_hack_alpha(p_superficie);
		SDL_SetAlpha(hack_alpha, SDL_SRCALPHA | SDL_RLEACCEL, alpha);
		return SDL_BlitSurface(hack_alpha, NULL, p_superficie, &caja) >= 0;
	}
	else
	{
		SDL_FillRect(p_superficie, &caja, this->acc_pixel_color());
		return true;
	}
}

void Primitiva_grafica_caja::establecer_posicion(const SDL_Rect & p_caja)
{
	Primitiva_grafica::establecer_posicion(p_caja);
	Uint8 alpha=this->acc_alpha();	
	if(alpha!=SDL_ALPHA_OPAQUE) recrear_hack_alpha=true;
}


void Primitiva_grafica_caja::establecer_posicion(int p_x, int p_y, int p_w, int p_h, int p_flags)
{
	Primitiva_grafica::establecer_posicion(p_x, p_y, p_w, p_h, p_flags);
	Uint8 alpha=this->acc_alpha();	
	if(alpha!=SDL_ALPHA_OPAQUE) recrear_hack_alpha=true;
}

void Primitiva_grafica_caja::preparar_posicion()
{

}
