#ifndef PROCESO_SUPERFICIE_FUNDIDO_H
#define PROCESO_SUPERFICIE_FUNDIDO_H

#include "proceso_superficie.h"
#include "../utilidades_graficas_sdl/utilidades_graficas_sdl.h"
#include "../gestor_color/gestor_color.h"
#include "../../herramientas/linea/linea.h"

/*Dibuja una superficie con RGB, y va modificando el alpha de la misma a lo
largo de una línea de tiempo.*/

namespace DLibV
{

class Proceso_superficie_fundido:public Proceso_superficie
{
	private:
	
	Uint8 r,g,b;
	Uint8 alpha_inicial, alpha_final, alpha_actual;
	float duracion;
	float tics;
	DLibH::Linea linea;

	SDL_Surface * superficie;

	Proceso_superficie_fundido(SDL_Surface const *, Uint8, Uint8, Uint8, Uint8, Uint8, float);
	void inline calcular_alpha(float);

	protected:
	void proceso(SDL_Surface * , float=0);

	public:


	~Proceso_superficie_fundido();
	static Proceso_superficie_fundido * generar(SDL_Surface const *, Uint8, Uint8, Uint8, Uint8, Uint8, float);
};

} //Fin namespace DLibV

#endif
