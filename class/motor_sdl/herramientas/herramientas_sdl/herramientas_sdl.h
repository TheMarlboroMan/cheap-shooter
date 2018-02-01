#ifndef HERRAMIENTAS_SDL_H
#define HERRAMIENTAS_SDL_H

#include <SDL/SDL.h>
#include <iostream>
#include <cmath>

namespace DLibH
{

class Herramientas_SDL
{
	private:
	
	Herramientas_SDL()
	{
		//Ahahahaaaaaa!!!.
	}

	public:

	static void error();
	static bool iniciar_SDL(Uint32=(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK));
        static void apagar_SDL();
	static bool arrancar_sistema_SDL(Uint32);
	static bool es_sistema_arrancado_SDL(Uint32);
	static void detener_sistema_SDL(Uint32);

	static SDL_Rect copiar_sdl_rect(SDL_Rect const&, Sint16=0,  Sint16=0, Uint16=0, Uint16=0);
	static SDL_Rect copiar_sdl_rect(SDL_Rect const*, Sint16=0,  Sint16=0, Uint16=0, Uint16=0);
	static SDL_Rect * copiar_sdl_rect_dinamico(SDL_Rect const&, Sint16=0, Sint16=0, Uint16=0, Uint16=0);
	static SDL_Rect * copiar_sdl_rect_dinamico(SDL_Rect const*, Sint16=0, Sint16=0, Uint16=0, Uint16=0);
	static SDL_Rect nuevo_sdl_rect(Sint16=0, Sint16=0, Uint16=0, Uint16=0);
        static bool rectangulos_superpuestos(SDL_Rect const&, SDL_Rect const&, bool=false);
        static bool rectangulos_superpuestos(SDL_Rect const&, SDL_Rect const&, SDL_Rect &, bool=false);
	static bool segmentos_superpuestos(int, int, int, int, bool=false);
	static void posicion_dimension_segmentos(int, int, int, int, Sint16 &, Uint16 &);
	static void posicion_dimension_segmentos_solucion_parcial(int, int, int, int, Sint16 &, Uint16 &);
	static void posicion_dimension_segmentos_solucion_completa(int, int, int, int, Sint16 &, Uint16 &);
        static bool punto_en_caja(SDL_Rect const&, int, int);
        static bool caja_en_caja(SDL_Rect const&, SDL_Rect const&);
	static void copiar_caja_de_a(SDL_Rect const&, SDL_Rect &);
	static bool son_cajas_iguales(SDL_Rect const&, SDL_Rect const&);
	static void describe_caja(SDL_Rect const&);
//	static void posicion_decimal_caja(SDL_Rect&, float, float);
//	static float round(float);
};

}
#endif
