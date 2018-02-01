#ifndef PRIMITIVA_GRAFICA_LINEA_H
#define PRIMITIVA_GRAFICA_LINEA_H

#include "../primitiva_grafica.h"
#include "../../../utilidades_graficas_sdl/utilidades_graficas_sdl.h"
#include "../../../../herramientas/linea/linea.h"

namespace DLibV
{

class Primitiva_grafica_linea:public Primitiva_grafica
{
	//TODO: Mirar el algoritmo de Bresenham. O algo así.

	protected:
	
	int x1;
	int y1;
	int x2;
	int y2;

	//Estoo... Una referencia a una función que se 
	//puede usar para calcular el color del pixel
	//en función de los parámetros pasados (x, y, inix,iniy, finx,finy, 
	//y un puntero a... algo). Es más que suficiente para cosas que no
	//sean demasiado sofisticadas.
	//Un ejemplo de como aplicar todo esto está en test_03.cpp.
	Uint32 (*metodo_pixeles)(int, int, int, int, int, int, void *);

	void preparar_posicion();
	void linea(int, int, int, int, SDL_Surface *, Uint32=0);
	void linea(int, int, int, int, SDL_Rect const&, SDL_Rect const&, SDL_Surface *, Uint32=0);
	void linea(int, int, int, int, float, float, SDL_Rect const&,  SDL_Surface *, Uint32=0);
	void linea_horizontal(int, int, int, SDL_Rect const&, SDL_Surface *, Uint32=0);
	void linea_vertical(int, int, int, SDL_Rect const&, SDL_Surface *, Uint32=0);

	bool volcado(SDL_Surface *);
	bool volcado(SDL_Surface *, const SDL_Rect&, const SDL_Rect&, int=0, int=0);

	public:

	Primitiva_grafica_linea(int, int, int, int, Uint32=0);
	Primitiva_grafica_linea(const Primitiva_grafica_linea&);
	Primitiva_grafica_linea& operator=(const Primitiva_grafica_linea&);
	virtual ~Primitiva_grafica_linea();

	virtual bool es_estatica() const=0;
	void establecer_metodo_pixeles(Uint32 (*p_param)(int, int, int, int, int, int, void *)) {this->metodo_pixeles=p_param;}

	int acc_x1() const {return this->x1;}
	int acc_y1() const {return this->y1;}
	int acc_x2() const {return this->x2;}
	int acc_y2() const {return this->y2;}

	void mut_x1(int);
	void mut_y1(int);
	void mut_x2(int);
	void mut_y2(int);
	void mut_punto_1(int, int);
	void mut_punto_2(int, int);
	void mut_puntos(int, int, int, int);
	void ir_a(int, int);	
};

class Primitiva_grafica_linea_dinamica:public Primitiva_grafica_linea
{
	public:
	Primitiva_grafica_linea_dinamica(int px1, int py1, int px2, int py2, Uint32 p_pixel_color):Primitiva_grafica_linea(px1, py1, px2, py2, p_pixel_color) {}
	Primitiva_grafica_linea_dinamica(const Primitiva_grafica_linea_dinamica& o):Primitiva_grafica_linea(o) {}
	Primitiva_grafica_linea_dinamica& operator=(const Primitiva_grafica_linea_dinamica& o)
	{
		Primitiva_grafica_linea::operator=(o);
		return *this;
	}
	~Primitiva_grafica_linea_dinamica() {}
	bool es_estatica() const {return false;}
};

class Primitiva_grafica_linea_estatica:public Primitiva_grafica_linea
{
	public:
	Primitiva_grafica_linea_estatica(int px1, int py1, int px2, int py2, Uint32 p_pixel_color):Primitiva_grafica_linea(px1, py1, px2, py2, p_pixel_color) {}
	Primitiva_grafica_linea_estatica(const Primitiva_grafica_linea_estatica& o):Primitiva_grafica_linea(o) {}
	Primitiva_grafica_linea_estatica& operator=(const Primitiva_grafica_linea_estatica& o)
	{
		Primitiva_grafica_linea::operator=(o);
		return *this;
	}
	~Primitiva_grafica_linea_estatica() {}
	bool es_estatica() const {return true;}
};


} //Fin namespace DLibV

#endif
