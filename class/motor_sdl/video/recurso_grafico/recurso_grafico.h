#ifndef RECURSO_GRAFICO_H
#define RECURSO_GRAFICO_H

#include <SDL/SDL.h>
//#include <string>
#include <fstream>
#include <iostream>
#include "../utilidades_graficas_sdl/utilidades_graficas_sdl.h"
#include "../gestor_color/gestor_color.h"

//Un recurso gráfico es cualquier cosa que tiene una superficie y que puede
//blitearse en una pantalla. No necesariamente un bitmap sino también por
//ejemplo un conjunto de instrucciones poligonales... o algo así.

namespace DLibV
{

class Recurso_grafico
{
	protected:

	SDL_Surface * superficie;
	Uint32 color_transparencia;
	bool transparencia_activa;

	public:

	enum bits{B0=1, B1=2, B2=4, B3=8, B4=16, B5=32, B6=64, B7=128};

	enum T_TIPOS{T_IMAGEN=1, T_LIENZO=2};
	
	Recurso_grafico();
	virtual ~Recurso_grafico();
	Recurso_grafico& operator=(const Recurso_grafico&);
	
	SDL_Surface * acc_superficie() const {return this->superficie;}
	SDL_Surface * acc_superficie() {return this->superficie;}

	void establecer_transparencia(Uint8=0, Uint8=255, Uint8=255, bool=true);
	void establecer_transparencia(Uint32, bool=true);
	void rellenar(Uint8=0, Uint8=255, Uint8=255);
	void rellenar(Uint32 c);
	void limpiar_transparencia();
	void establecer_alpha(Uint8);
	void copiar_desde(Recurso_grafico const&);
	void copiar_desde(Recurso_grafico const&, SDL_Rect);
	void copiar_desde(Recurso_grafico const&, SDL_Rect, SDL_Rect);
	bool es_transparencia_activa() const {return this->transparencia_activa;}
	Uint32 acc_color_transparencia() const {return this->color_transparencia;}

	//Resulta ridículo que esto es lo único que hace abstracta a esta clase :D.
	virtual int obtener_tipo()=0;
	virtual bool es_preparado()=0;
};

} //Fin namespace DLibV
#endif
