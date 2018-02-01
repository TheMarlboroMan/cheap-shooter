#ifndef Utilidades_graficas_SDL_H
#define Utilidades_graficas_SDL_H

#include <iostream>
#include <fstream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "../../herramientas/log_base/log_base.h"
#include "../../herramientas/herramientas/herramientas.h"
//#include "../../herramientas/debug/debug.h"

namespace DLibV
{

class Utilidades_graficas_SDL
{
	private:
	
	Utilidades_graficas_SDL()
	{
		//Ahahahaaaaaa!!!.
	}

	public:

	enum Inversion_superficie
	{
		HORIZONTAL=1,
		VERTICAL=2
	};

	static void mostrar_ocultar_cursor(bool);

	static SDL_Surface * nueva_superficie_formato(SDL_Surface const *);
	static SDL_Surface * nueva_superficie_formato(SDL_Surface const *, const SDL_Rect&, Uint32=0, Uint32=0);
	static SDL_Surface * copiar_superficie(const SDL_Surface *);
	static SDL_Surface * copiar_superficie(const SDL_Surface *, const SDL_Rect&, Uint32=0, Uint32=0);
	static SDL_Surface * cargar_imagen(const char *);
	static SDL_Surface * cargar_imagen_en_superficie(const char *, int=SDL_HWSURFACE);
	static Uint32 SDL_GetPixel(SDL_Surface *, int, int);
//	static void SDL_PutPixel(SDL_Surface *, int, int, Uint32, short int=0);
//	static void SDL_PutPixel(SDL_Surface *, SDL_Rect const&, int, int, Uint32, short int=0);
	static void SDL_PutPixel(SDL_Surface *, int, int, Uint32, Uint8=SDL_ALPHA_OPAQUE);
	static void SDL_PutPixel(SDL_Surface *, SDL_Rect const&, int, int, Uint32, Uint8=SDL_ALPHA_OPAQUE);
	static inline Uint32 combinar_pixel(SDL_Surface *, Uint32, Uint32, Uint8);
	static void linea(int, int, int, int, SDL_Surface *, Uint32=0, Uint8=SDL_ALPHA_OPAQUE);
	static void linea(int, int, int, int, float, float, SDL_Surface *, Uint32=0, Uint8=SDL_ALPHA_OPAQUE);
	static void linea_horizontal(int, int, int, SDL_Surface *, Uint32=0, Uint8=SDL_ALPHA_OPAQUE);
	static void linea_vertical(int, int, int, SDL_Surface *, Uint32=0, Uint8=SDL_ALPHA_OPAQUE);
	static void circunferencia(int, int, int, SDL_Surface *, Uint32);
	static SDL_Surface * invertir_superficie(const SDL_Surface *, unsigned int);
	static SDL_Surface * invertir_superficie(const SDL_Surface *, const SDL_Rect&, unsigned int);
};

} //Fin namespace DLibV
#endif
