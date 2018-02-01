#include "proceso_superficie_base_por_pixel.h"

using namespace DLibV;

Proceso_superficie_base_por_pixel::Proceso_superficie_base_por_pixel():
	color_transparente(0)
{

}

Proceso_superficie_base_por_pixel::~Proceso_superficie_base_por_pixel()
{

}

void Proceso_superficie_base_por_pixel::proceso(SDL_Surface * p_superficie, float p_delta)
{
	int x=0, y=0;
	this->formato=p_superficie->format;
	Uint32 color_resultante;
	Uint32 px_origen;	

	if(SDL_MUSTLOCK(p_superficie)) SDL_LockSurface(p_superficie);

	for(; y < p_superficie->h; y++)
	{
		for(x=0; x < p_superficie->w; x++)
		{
			px_origen=Utilidades_graficas_SDL::SDL_GetPixel(p_superficie, x, y);
			if(procesar_pixel(px_origen, color_resultante, color_transparente))
			{
				Utilidades_graficas_SDL::SDL_PutPixel(p_superficie, x, y, color_resultante);
			}
		}
	}

	if(SDL_MUSTLOCK(p_superficie)) SDL_UnlockSurface(p_superficie);
}

void Proceso_superficie_base_por_pixel::proceso_parcial(SDL_Surface * p_superficie, SDL_Rect parte, float p_delta)
{
	if(parte.x==0 && parte.y==0 && parte.h==0 && parte.w==0)
	{
		parte.w=p_superficie->w;
		parte.h=p_superficie->h;
	}

	int x=parte.x, y=parte.y;
	int lim_x=parte.w+x;
	int lim_y=parte.h+y;

	this->formato=p_superficie->format;
	Uint32 color_transparente=this->formato->colorkey;
	Uint32 color_resultante;
	Uint32 px_origen;

	if(SDL_MUSTLOCK(p_superficie)) SDL_LockSurface(p_superficie);

	for(; y < p_superficie->h && y < lim_y; y++)
	{
		for(x=0; x < p_superficie->w && x < lim_x; x++)
		{
			px_origen=Utilidades_graficas_SDL::SDL_GetPixel(p_superficie, x, y);
			if(procesar_pixel(px_origen, color_resultante, color_transparente))
			{
				Utilidades_graficas_SDL::SDL_PutPixel(p_superficie, x, y, color_resultante);
			}
		}
	}

	if(SDL_MUSTLOCK(p_superficie)) SDL_UnlockSurface(p_superficie);
}
