#ifndef PRIMITIVA_GRAFICA_CAJA_H
#define PRIMITIVA_GRAFICA_CAJA_H

/*
Simplemente hace un fillrect. Simple, rápido y efectivo.
*/

#include "../primitiva_grafica.h"

namespace DLibV
{
class Primitiva_grafica_caja:public Primitiva_grafica
{
	private:
	
	SDL_Surface * hack_alpha;

	void crear_hack_alpha(SDL_Surface *);
	bool recrear_hack_alpha;

	protected:

	void preparar_posicion();

	public:
	
	void preparar();
	bool es_estatica() const=0;
		
	bool volcado(SDL_Surface *);
	bool volcado(SDL_Surface *, const SDL_Rect&, const SDL_Rect&, int=0, int=0);

	Primitiva_grafica_caja(const SDL_Rect&, Uint32);
	Primitiva_grafica_caja(const Primitiva_grafica_caja&);
	virtual ~Primitiva_grafica_caja();
	Primitiva_grafica_caja& operator=(const Primitiva_grafica_caja&);

	void establecer_posicion(int, int, int=-1, int=-1, int=15);
	void establecer_posicion(const SDL_Rect &);
	void mut_pixel_color(Uint32 param) 
	{
		recrear_hack_alpha=true;
		Primitiva_grafica::mut_pixel_color(param);
	}
};

class Primitiva_grafica_caja_dinamica:public Primitiva_grafica_caja
{
	public:

	bool es_estatica() const {return false;}

	Primitiva_grafica_caja_dinamica(const SDL_Rect& p_pos, Uint32 p_color):Primitiva_grafica_caja(p_pos, p_color){}
	Primitiva_grafica_caja_dinamica(const Primitiva_grafica_caja_dinamica& p_otra):Primitiva_grafica_caja(p_otra) {}
	virtual ~Primitiva_grafica_caja_dinamica() {}
	Primitiva_grafica_caja_dinamica& operator=(const Primitiva_grafica_caja_dinamica& p_otro)
	{
		Primitiva_grafica_caja::operator=(p_otro);
		return *this;
	}
};

class Primitiva_grafica_caja_estatica:public Primitiva_grafica_caja
{
	public:

	bool es_estatica() const {return true;}

	Primitiva_grafica_caja_estatica(const SDL_Rect& p_pos, Uint32 p_color):Primitiva_grafica_caja(p_pos, p_color){}
	Primitiva_grafica_caja_estatica(const Primitiva_grafica_caja_estatica& p_otra):Primitiva_grafica_caja(p_otra) {}
	virtual ~Primitiva_grafica_caja_estatica() {}
	Primitiva_grafica_caja_estatica& operator=(const Primitiva_grafica_caja_estatica& p_otro)
	{
		Primitiva_grafica_caja::operator=(p_otro);
		return *this;
	}
};

}

#endif
