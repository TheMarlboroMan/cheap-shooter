#include "gestor_color.h"

using namespace DLibV;

Uint32 Gestor_color::color(Uint8 p_r, Uint8 p_g, Uint8 p_b)
{
	if(Gestor_color::comprobar()) return SDL_MapRGB(Gestor_color::formato_pixeles, p_r, p_g, p_b);
	else return 0;
}

Uint32 Gestor_color::color(SDL_Surface * superficie, Uint8 p_r, Uint8 p_g, Uint8 p_b)
{
	return SDL_MapRGB(superficie->format, p_r, p_g, p_b);	
}

Uint32 Gestor_color::clave_color_superficie(SDL_Surface * superficie)
{
	return superficie->format->colorkey;
}

Uint32 Gestor_color::clave_color_formato(SDL_PixelFormat * formato)
{
	return formato->colorkey;
}

void Gestor_color::establecer_formato(SDL_PixelFormat * p_formato)
{
	Gestor_color::formato_pixeles=p_formato;
}

SDL_PixelFormat * Gestor_color::obtener_formato() 
{
	return Gestor_color::formato_pixeles;
}
