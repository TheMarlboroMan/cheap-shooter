#include "representacion_pixeles.h"

using namespace DLibV;

Representacion_pixeles::Representacion_pixeles(unsigned int p_w, unsigned int p_h)
	:Representacion_lienzo(p_w, p_h), autolock(true), color_actual(0)
{
	this->recorte_a_medidas_recurso();
}

Representacion_pixeles::Representacion_pixeles(const Representacion_pixeles& o)
	:Representacion_lienzo(o), color_actual(o.color_actual)
{
	
}	

Representacion_pixeles& Representacion_pixeles::operator=(const Representacion_pixeles& p_otra)
{
	Representacion_lienzo::operator=(p_otra);
	this->color_actual=p_otra.color_actual;
	return *this;
}

Representacion_pixeles::~Representacion_pixeles()
{

}

void Representacion_pixeles::preparar()
{
	marcar_como_preparada();
}

void Representacion_pixeles::cambiar_color(int p_r, int p_g, int p_b)
{
	this->color_actual=Gestor_color::color(p_r, p_g, p_b);
}

void Representacion_pixeles::cambiar_color(Uint32 p_c)
{
	this->color_actual=p_c;
}

void Representacion_pixeles::pixel(unsigned int p_x, unsigned int p_y)
{
	SDL_Surface * superficie=this->ref_recurso()->acc_superficie();

	if(autolock)
	{
		if(SDL_MUSTLOCK(superficie)) SDL_LockSurface(superficie);
		Utilidades_graficas_SDL::SDL_PutPixel(superficie, p_x, p_y, this->color_actual);
		if(SDL_MUSTLOCK(superficie)) SDL_UnlockSurface(superficie);
	}
	else
	{
		Utilidades_graficas_SDL::SDL_PutPixel(superficie, p_x, p_y, this->color_actual);
	}

}

void Representacion_pixeles::caja(int p_x, int p_y, int p_w, int p_h)
{
	SDL_Rect p_caja=DLibH::Herramientas_SDL::nuevo_sdl_rect(p_x,p_y,p_w,p_h);
	caja(p_caja);
}

void Representacion_pixeles::caja(const SDL_Rect& p_caja)
{
	SDL_FillRect(this->ref_recurso()->acc_superficie(), &(const_cast<SDL_Rect&> (p_caja)), this->color_actual);
}

void Representacion_pixeles::linea(int p_xa, int p_ya, int p_xb, int p_yb)
{
	SDL_Surface * superficie=this->ref_recurso()->acc_superficie();

	if(autolock)
	{
		if(SDL_MUSTLOCK(superficie)) SDL_LockSurface(superficie);
		Utilidades_graficas_SDL::linea(p_xa, p_ya, p_xb, p_xb, this->ref_recurso()->acc_superficie(), this->color_actual);
		if(SDL_MUSTLOCK(superficie)) SDL_UnlockSurface(superficie);
	}
	else
	{
		Utilidades_graficas_SDL::linea(p_xa, p_ya, p_xb, p_xb, this->ref_recurso()->acc_superficie(), this->color_actual);
	}
}

void Representacion_pixeles::limpiar_transparencia()
{
	this->ref_recurso()->limpiar_transparencia();
}

void Representacion_pixeles::bloquear_superficie()
{
	SDL_Surface * superficie=this->ref_recurso()->acc_superficie();
	if(SDL_MUSTLOCK(superficie)) SDL_LockSurface(superficie);
}

void Representacion_pixeles::desbloquear_superficie()
{
	SDL_Surface * superficie=this->ref_recurso()->acc_superficie();
	if(SDL_MUSTLOCK(superficie)) SDL_UnlockSurface(superficie);
}
