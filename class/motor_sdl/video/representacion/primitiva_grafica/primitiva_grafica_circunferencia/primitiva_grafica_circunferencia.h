#ifndef PRIMITIVA_GRAFICA_CIRCUNFERENCIA_H
#define PRIMITIVA_GRAFICA_CIRCUNFERENCIA_H

#include "../primitiva_grafica.h"
#include "../../../utilidades_graficas_sdl/utilidades_graficas_sdl.h"

namespace DLibV
{


class Primitiva_grafica_circunferencia:public Primitiva_grafica
{
	protected:

	int x;
	int y;
	unsigned int radio;
	unsigned int ancho;
	//X, Y, centro x, centro y, radio.
	Uint32 (*metodo_pixeles)(int, int, int, int, unsigned int, void *);
	
	void preparar_posicion();
	void circunferencia(int, int, unsigned int, SDL_Surface *, Uint32=0);
	void circunferencia(int, int, unsigned int, SDL_Rect const&, SDL_Rect const&, SDL_Surface *, Uint32=0);

	bool volcado(SDL_Surface * p_superficie);
	bool volcado(SDL_Surface * p_superficie, const SDL_Rect&, const SDL_Rect&, int=0, int=0);


	public:

	Primitiva_grafica_circunferencia(int, int, unsigned int, unsigned int=1, Uint32=0);
	Primitiva_grafica_circunferencia(const Primitiva_grafica_circunferencia&);
	Primitiva_grafica_circunferencia& operator=(const Primitiva_grafica_circunferencia&);
	virtual ~Primitiva_grafica_circunferencia();
	virtual bool es_estatica() const=0;
	void establecer_metodo_pixeles(Uint32 (*p_param)(int, int, int, int, unsigned int, void *)) {this->metodo_pixeles=p_param;}

	int acc_x() const {return this->x;}
	int acc_y() const {return this->y;}
	unsigned int acc_radio() const {return this->radio;}

	void mut_x(int);
	void mut_y(int);
	void mut_radio(unsigned int);
	void mut_ancho(unsigned int);
	void mut_circunferencia(int, int, unsigned int, unsigned int=1);
	void ir_a(int, int);
};

class Primitiva_grafica_circunferencia_dinamica:public Primitiva_grafica_circunferencia
{
	public:
	Primitiva_grafica_circunferencia_dinamica(int p_x, int p_y, unsigned int p_radio, unsigned int p_ancho, Uint32 p_pixel_color=0):Primitiva_grafica_circunferencia(p_x, p_y, p_radio, p_ancho, p_pixel_color) {}
	Primitiva_grafica_circunferencia_dinamica(const Primitiva_grafica_circunferencia_dinamica& p):Primitiva_grafica_circunferencia(p){}
	Primitiva_grafica_circunferencia_dinamica& operator=(const Primitiva_grafica_circunferencia_dinamica& p)
	{
		Primitiva_grafica_circunferencia::operator=(p);
		return *this;
	}

	virtual ~Primitiva_grafica_circunferencia_dinamica() {}
	bool es_estatica() const {return false;}
};

class Primitiva_grafica_circunferencia_estatica:public Primitiva_grafica_circunferencia
{
	public:
	Primitiva_grafica_circunferencia_estatica(int p_x, int p_y, unsigned int p_radio, unsigned int p_ancho, Uint32 p_pixel_color=0):Primitiva_grafica_circunferencia(p_x, p_y, p_radio, p_ancho, p_pixel_color) {}
	Primitiva_grafica_circunferencia_estatica(const Primitiva_grafica_circunferencia_dinamica& p):Primitiva_grafica_circunferencia(p){}
	Primitiva_grafica_circunferencia_estatica& operator=(const Primitiva_grafica_circunferencia_estatica& p)
	{
		Primitiva_grafica_circunferencia::operator=(p);
		return *this;
	}
	virtual ~Primitiva_grafica_circunferencia_estatica() {}
	bool es_estatica() const {return true;}
};


} //Fin namespace DLibV

#endif
