#ifndef MAPA_COLISION_BITMAP_H
#define MAPA_COLISION_BITMAP_H

#include <SDL/SDL.h>
#include <cstring>	//M-m-memset.
#include <cmath>
#include <string>
#include "../utilidades_graficas_sdl/utilidades_graficas_sdl.h"
#include "../../herramientas/herramientas_sdl/herramientas_sdl.h"

/*
Esto es un contenedor para un array de chars que, bit a bit, vienen a 
representar un mapa de colisiones para un bitmap. El 0 sería "color 
transparente" y el 1, cualquier otra cosa. Lo cargaríamos con un SDL_Surface.
*/

namespace DLibV
{


class Mapa_colision_bitmap
{
	private:

	unsigned char * mapa;
	unsigned int w;	//Copia del ancho de la superficie / rectángulo escogido.
	unsigned int h; //Copia del alto de la superficie / rectángulo escogido...
	static const unsigned short int TAM_UNIDAD=8; 	//Obviedad!!!.
	static const unsigned short int VALOR_VACIO=0; 	//Obviedad!!!.
	void generar_mapa(unsigned short int=VALOR_VACIO);
	void rellenar_mapa(SDL_Surface *, Uint32, SDL_Rect);

	enum bits {B0=1, B1=2, B2=4, B3=8, B4=16, B5=32, B6=64, B7=128};

	public:

	Mapa_colision_bitmap();
	~Mapa_colision_bitmap();
	bool inicializar(SDL_Surface *, Uint32);	//Inicializa TODA la superficie.
	bool inicializar(SDL_Surface *, Uint32, SDL_Rect);	//Inicializa parte de la superficie.
	bool inicializar(SDL_Rect, unsigned short int=VALOR_VACIO);	//Inicializa un mapa para esa caja, vacío....
	bool comprobar(unsigned int, unsigned int) const;	//Comprueba la posición seleccionada.

	unsigned char * acc_mapa() const {return this->mapa;}
	unsigned int acc_w() const {return this->w;}
	unsigned int acc_h() const {return this->h;}
	bool es_preparado() const {return this->mapa!=NULL;}
	static bool comparar_mapas(const Mapa_colision_bitmap &, const Mapa_colision_bitmap &);
	static bool comparar_mapas(const Mapa_colision_bitmap &, const Mapa_colision_bitmap &, const SDL_Rect&, const SDL_Rect&);
	static bool comparar_mapas_posicionados(const Mapa_colision_bitmap &, const Mapa_colision_bitmap &, const SDL_Rect&, const SDL_Rect&);
	static bool comparar_mapas_posicionados(const Mapa_colision_bitmap &, const Mapa_colision_bitmap &, const SDL_Rect&, const SDL_Rect&, const SDL_Rect&);
	static bool comparar_mapa_con_rectangulo(const Mapa_colision_bitmap &, const SDL_Rect&);
	static bool comparar_mapa_con_rectangulo(const Mapa_colision_bitmap &, const SDL_Rect&, const SDL_Rect&);
	static Mapa_colision_bitmap obtener_mapeado(Mapa_colision_bitmap const&, SDL_Rect);
	std::string debug() const;

	void static inline bit_para_cantidad(unsigned char &bit, int cantidad)
	{
		switch(cantidad)
		{
			case 0: bit=B0; break;
			case 1: bit=B1; break;
			case 2: bit=B2; break;
			case 3: bit=B3; break;
			case 4: bit=B4; break;
			case 5: bit=B5; break;
			case 6: bit=B6; break;
			case 7: bit=B7; break;
		}
	}
};

} //Fin namespace DLibV

#endif
