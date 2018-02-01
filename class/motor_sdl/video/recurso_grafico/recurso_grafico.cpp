#include "recurso_grafico.h"

using namespace DLibV;

Recurso_grafico::Recurso_grafico():color_transparencia(0), transparencia_activa(false)
{

}

Recurso_grafico::~Recurso_grafico()
{
	if(this->superficie)
	{
		SDL_FreeSurface(this->superficie);
		this->superficie=NULL;
	}

}

Recurso_grafico& Recurso_grafico::operator=(const Recurso_grafico& p_otro)
{	
	this->color_transparencia=p_otro.color_transparencia;
	this->transparencia_activa=p_otro.transparencia_activa;

	this->superficie=Utilidades_graficas_SDL::copiar_superficie(p_otro.superficie);

	return *this;
}

void Recurso_grafico::establecer_transparencia(Uint8 r, Uint8 g, Uint8 b, bool p_rle)
{
	if(this->superficie) this->establecer_transparencia(SDL_MapRGB(this->superficie->format, r, g, b), p_rle);
	else 
	{
		DLibH::Log_motor::L()<<DLibH::Log_base_n(1)<<DLibH::Log_base_t()<<"Recurso_grafico::establecer_transparencia(Uint8, Uint8, Uint8, bool) : No hay superficie para recurso"<<std::endl;
	}
}

void Recurso_grafico::establecer_transparencia(Uint32 c, bool p_rle)
{
	if(this->superficie) 
	{
		unsigned int flags=p_rle ? SDL_SRCCOLORKEY : SDL_SRCCOLORKEY | SDL_RLEACCEL;

		SDL_SetColorKey(this->superficie, flags, c);
		this->transparencia_activa=true;
		this->color_transparencia=c;
	}
	else 
	{
		DLibH::Log_motor::L()<<DLibH::Log_base_n(1)<<DLibH::Log_base_t()<<"Recurso_grafico::establecer_transparencia(Uint32, bool) : No hay superficie para recurso"<<std::endl;
	}
}

void Recurso_grafico::rellenar(Uint8 r, Uint8 g, Uint8 b)
{
	if(this->superficie) this->rellenar(Gestor_color::color(r,g,b));
	else
	{
		DLibH::Log_motor::L()<<DLibH::Log_base_n(1)<<DLibH::Log_base_t()<<"Recurso_grafico::rellenar(Uint8, Uint8, Uint8) : No hay superficie para recurso"<<std::endl;
	}
}

void Recurso_grafico::rellenar(Uint32 c)
{
	if(this->superficie) 
	{
		SDL_FillRect(this->superficie, NULL, c);
	}
	else 
	{
		DLibH::Log_motor::L()<<DLibH::Log_base_n(1)<<DLibH::Log_base_t()<<"Recurso_grafico::rellenar(Uint32) : No hay superficie para recurso"<<std::endl;
	}
}

void Recurso_grafico::limpiar_transparencia()
{
	if(this->superficie) 
	{
		SDL_SetColorKey(this->superficie, 0, 0);
		this->transparencia_activa=false;
		this->color_transparencia=false;
	}
	else 
	{
		DLibH::Log_motor::L()<<DLibH::Log_base_n(1)<<DLibH::Log_base_t()<<"Recurso_grafico::limpiar_transparencia() : No hay superficie para recurso"<<std::endl;
	}
}

void Recurso_grafico::establecer_alpha(Uint8 p_valor)
{
	SDL_SetAlpha(this->superficie, SDL_SRCALPHA, p_valor);
}

void Recurso_grafico::copiar_desde(Recurso_grafico const& p_recurso)
{
	SDL_BlitSurface(p_recurso.acc_superficie(), NULL, this->superficie, NULL);
}

void Recurso_grafico::copiar_desde(Recurso_grafico const& p_recurso, SDL_Rect p_rect_origen)
{
	SDL_BlitSurface(p_recurso.acc_superficie(), &p_rect_origen, this->superficie, NULL);
}

void Recurso_grafico::copiar_desde(Recurso_grafico const& p_recurso, SDL_Rect p_rect_origen, SDL_Rect p_rect_destino)
{
	SDL_BlitSurface(p_recurso.acc_superficie(), &p_rect_origen, this->superficie, &p_rect_destino);
}
