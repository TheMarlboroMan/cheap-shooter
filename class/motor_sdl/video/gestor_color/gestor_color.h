#ifndef GESTOR_COLOR_H
#define GESTOR_COLOR_H

#include <SDL/SDL.h>

namespace DLibV
{


class Gestor_color
{
	private:

	Gestor_color(){}
	~Gestor_color(){}

	static SDL_PixelFormat * formato_pixeles; //Este es simplemente para usar "color" sin una superficie.
	static bool comprobar() {return Gestor_color::formato_pixeles!=NULL;}

	public:

	static Uint32 color(Uint8, Uint8, Uint8);
	static Uint32 color(SDL_Surface *, Uint8, Uint8, Uint8);
	static void establecer_formato(SDL_PixelFormat *);
	static SDL_PixelFormat * obtener_formato();
//	static Uint32 obtener_color_rgb(int, int, int);
	static Uint32 clave_color_superficie(SDL_Surface *);
	static Uint32 clave_color_formato(SDL_PixelFormat *);

	public:

	struct Color
	{
		Uint8 r, g, b;
		Color(Uint8 pr, Uint8 pg, Uint8 pb):r(pr), g(pg), b(pb){}
		Uint32 obtener() {return Gestor_color::color(r, g, b);}
	};
};

} //Fin namespace DLibV

#endif
