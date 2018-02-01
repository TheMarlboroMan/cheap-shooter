#ifndef ESTRUCTURA_INSTRUCCIONES_H
#define ESTRUCTURA_INSTRUCCIONES_H

#include <vector>
#include <string>
#include "../../motor_sdl/def_video.h"
#include "../../motor_sdl/herramientas/herramientas/herramientas.h"
#include "../../motor_sdl/herramientas/herramientas_sdl/herramientas_sdl.h"
#include "../../motor_sdl/herramientas/vector_2d/vector_2d.h"

/*Esto no es más que una representación compuesta con algunos métodos propios
para moverse de un lado a otro...*/

class Estructura_instrucciones
{
	private:

	DLibV::Representacion_compuesta_dinamica rep;

	public:

	Estructura_instrucciones()
		:rep(Medidas::ANCHO_VISTA, Medidas::ALTO_VISTA)
	{
		this->rep.ref_recurso()->establecer_transparencia(0,0,0);
	}

	Estructura_instrucciones(const Estructura_instrucciones& p_otra):
		rep(Medidas::ANCHO_VISTA, Medidas::ALTO_VISTA)
	{
		rep=p_otra.rep;
		this->rep.ref_recurso()->establecer_transparencia(0,0,0);
	}

	~Estructura_instrucciones()
	{
	}
	
	//Necesitamos el operador para cuando metamos esto en un vector. De
	//lo contrario la representación dinámica explotará.
	Estructura_instrucciones& operator=(const Estructura_instrucciones& p_otra)
	{
		rep=p_otra.rep;
		return *this;
	}

	void componer(DLibV::Representacion & p_rep, const SDL_Rect & p_rect)
	{
		rep.volcar_representacion(p_rep, p_rect);
	} 
 
	DLibV::Representacion_compuesta_dinamica& acc_representacion() {return rep;} 
};

#endif
