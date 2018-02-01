#ifndef REPRESENTACION_COMPUESTA_H
#define REPRESENTACION_COMPUESTA_H

/*Un lienzo sobre el que pueden volcarse otras representaciones. Con la llegada
de las representaciones agrupadas también queda obsoleto, pero se está usando
en alguna parte del código de Cheap Shooter, de modo que lo podemos dejar...*/

#include "../representacion_lienzo.h"
#include "../../../../herramientas/herramientas_sdl/herramientas_sdl.h"

namespace DLibV
{


class Representacion_compuesta:public Representacion_lienzo
{
	protected:
	Uint32 color_actual;

	public:
	
	void volcar_representacion(Representacion&, const SDL_Rect &);
	void volcar_representacion(Representacion&);

	void preparar();
	virtual bool es_estatica() const=0;

	Representacion_compuesta(unsigned int, unsigned int);	//w,h
	Representacion_compuesta(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);	//w,h,r,g,b
	virtual ~Representacion_compuesta();
	Representacion_compuesta& operator=(const Representacion_compuesta&);
};

class Representacion_compuesta_dinamica:public Representacion_compuesta
{
	public:

	Representacion_compuesta_dinamica(unsigned int p_w, unsigned int p_h) :Representacion_compuesta(p_w, p_h) {}
	Representacion_compuesta_dinamica(unsigned int p_w, unsigned int p_h, unsigned int p_r, unsigned int p_g, unsigned int p_b) :Representacion_compuesta(p_w, p_h, p_r, p_g, p_b) {}
	virtual ~Representacion_compuesta_dinamica() {}
	virtual bool es_estatica() const {return false;}
	
	Representacion_compuesta_dinamica& operator=(const Representacion_compuesta_dinamica& o)
	{
		Representacion_compuesta::operator=(o);
		return *this;
	}
};

class Representacion_compuesta_estatica:public Representacion_compuesta
{
	public:

	Representacion_compuesta_estatica(unsigned int p_w, unsigned int p_h) :Representacion_compuesta(p_w, p_h) {}
	Representacion_compuesta_estatica(unsigned int p_w, unsigned int p_h, unsigned int p_r, unsigned int p_g, unsigned int p_b) :Representacion_compuesta(p_w, p_h, p_r, p_g, p_b) {}
	virtual ~Representacion_compuesta_estatica() {}
	virtual bool es_estatica() const {return true;}
	
	Representacion_compuesta_estatica& operator=(const Representacion_compuesta_estatica& o)
	{
		Representacion_compuesta::operator=(o);
		return *this;
	}
};

} //Fin namespace DLibV

#endif
