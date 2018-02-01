#ifndef REPRESENTACION_PIXELES_H
#define REPRESENTACION_PIXELES_H

/*Una representación de lienzo con métodos para poder pintar cosas encima.
Queda obsoleta con la llegada de las primitivas gráficas.*/

#include "../representacion_lienzo.h"
#include "../../../utilidades_graficas_sdl/utilidades_graficas_sdl.h"
#include "../../../../herramientas/herramientas_sdl/herramientas_sdl.h"

namespace DLibV
{


class Representacion_pixeles:public Representacion_lienzo
{
	private:
	bool autolock;

	protected:
	Uint32 color_actual;

	public:

	Representacion_pixeles(unsigned int, unsigned int);
	Representacion_pixeles(const Representacion_pixeles&);
	virtual ~Representacion_pixeles();
	Representacion_pixeles& operator=(const Representacion_pixeles&);

	void preparar();
	void cambiar_color(int, int, int);
	void cambiar_color(Uint32);
	void linea(int, int, int, int);
	void caja(int, int, int, int);
	void caja(const SDL_Rect&);
	void pixel(unsigned int, unsigned int);
	void limpiar_transparencia();
	void establecer_autolock(bool v) {autolock=v;}
	bool es_autolock() const {return autolock;}
	void bloquear_superficie();
	void desbloquear_superficie();
	
	virtual bool es_estatica() const=0; 
};

class Representacion_pixeles_dinamica:public Representacion_pixeles
{
	public:

	Representacion_pixeles_dinamica(unsigned int x, unsigned int y) :Representacion_pixeles(x, y) {}
	Representacion_pixeles_dinamica(const Representacion_pixeles_dinamica& o):Representacion_pixeles(o) {}
	Representacion_pixeles_dinamica& operator=(const Representacion_pixeles_dinamica& o)
	{
		Representacion_pixeles::operator=(o);
		return *this;
	}
	virtual ~Representacion_pixeles_dinamica() {}
	bool es_estatica() const {return false;}
};

class Representacion_pixeles_estatica:public Representacion_pixeles
{
	public:

	Representacion_pixeles_estatica(unsigned int x, unsigned int y) :Representacion_pixeles(x, y) {}
	Representacion_pixeles_estatica(const Representacion_pixeles_dinamica& o):Representacion_pixeles(o) {}
	Representacion_pixeles_estatica& operator=(const Representacion_pixeles_dinamica& o)
	{
		Representacion_pixeles::operator=(o);
		return *this;
	}
	~Representacion_pixeles_estatica() {}
	bool es_estatica() const {return true;}
};

} //Fin namespace DLibV

#endif
