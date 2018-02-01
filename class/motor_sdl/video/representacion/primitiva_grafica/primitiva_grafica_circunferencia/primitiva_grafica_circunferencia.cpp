#include "primitiva_grafica_circunferencia.h"

using namespace DLibV;

Primitiva_grafica_circunferencia::Primitiva_grafica_circunferencia(int p_x, int p_y, unsigned int p_radio, unsigned int p_ancho, Uint32 p_pixel_color)
	:Primitiva_grafica(p_pixel_color),
	x(p_x), y(p_y), radio(p_radio), ancho(p_ancho), metodo_pixeles(NULL)
{
	this->preparar_posicion();
}

Primitiva_grafica_circunferencia::Primitiva_grafica_circunferencia(const Primitiva_grafica_circunferencia& p)
	:Primitiva_grafica(p),
	x(p.x), y(p.y), radio(p.radio), ancho(p.ancho), metodo_pixeles(p.metodo_pixeles)
{
	
}

Primitiva_grafica_circunferencia& Primitiva_grafica_circunferencia::operator=(const Primitiva_grafica_circunferencia& p)
{
	Primitiva_grafica::operator=(p);

	x=p.x;
	y=p.y;
	radio=p.radio;
	ancho=p.ancho;
	metodo_pixeles=p.metodo_pixeles;

	return *this;
}

Primitiva_grafica_circunferencia::~Primitiva_grafica_circunferencia()
{

}


void Primitiva_grafica_circunferencia::preparar_posicion()
{
	unsigned int w=(this->radio*2)+1;
	establecer_posicion(
		this->x-(int)this->radio,
		this->y-(int)this->radio,
		w,w);

	if(es_recomponer_recorte_con_posicion()) this->recorte_a_posicion();
}

void Primitiva_grafica_circunferencia::circunferencia(int p_x, int p_y, unsigned int p_radio, SDL_Surface * p_superficie, Uint32 p_pixel_color)
{
	SDL_Rect caja_a=DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, p_superficie->w, p_superficie->h);
	circunferencia(p_x, p_y, p_radio, caja_a, caja_a, p_superficie, p_pixel_color);
}

/*
p_vista es el rectángulo hipotético al que se está enfocando.
p_posicion es la caja que actualiza la cámara, es decir, dónde se ve en pantalla.
*/

void Primitiva_grafica_circunferencia::circunferencia(int p_x, int p_y, unsigned int p_radio, SDL_Rect const& p_vista, SDL_Rect const& p_posicion, SDL_Surface * p_superficie, Uint32 p_pixel_color)
{
	/*OJO CON ESTO: p_x y p_y ya lleva una cierta transformación
	por la propia posición de la caja y tal y cual...*/

	//Esta será la caja que usaremos al final para limitar la circunferencia (por si no sale entera por recorte o por cámara...).
	SDL_Rect p_caja=DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0,0,0);
	if(!this->determinar_caja_dibujo_final(p_caja, p_posicion, p_vista)) return;

	int radio=p_radio; //Lo hacemos con signo...

	Uint32 pixel=p_pixel_color;
	double error = (double)-radio;
	double x = (double)radio -0.5;
	double y = (double)0.5;
	double cx = (p_x) - 0.5;
	double cy = (p_y) - 0.5;
	Uint8 alpha=acc_alpha();

	if(SDL_MUSTLOCK(p_superficie)) SDL_LockSurface(p_superficie);

	while(x >= y)
	{
		if(metodo_pixeles) pixel=this->metodo_pixeles((int)(cx + x), (int)(cy + y), this->x, this->y, this->radio, acc_parametro_metodo_pixeles());
		Utilidades_graficas_SDL::SDL_PutPixel(p_superficie, p_caja, (int)(cx + x), (int)(cy + y), pixel, alpha);

		if(metodo_pixeles) pixel=this->metodo_pixeles((int)(cx + y), (int)(cy + x), this->x, this->y, this->radio, acc_parametro_metodo_pixeles());
		Utilidades_graficas_SDL::SDL_PutPixel(p_superficie, p_caja, (int)(cx + y), (int)(cy + x), pixel, alpha);

		if (x!=0)
		{
			if(metodo_pixeles) pixel=this->metodo_pixeles((int)(cx - x), (int)(cy + y), this->x, this->y, this->radio, acc_parametro_metodo_pixeles())	;
			Utilidades_graficas_SDL::SDL_PutPixel(p_superficie, p_caja, (int)(cx - x), (int)(cy + y), pixel, alpha);

			if(metodo_pixeles) pixel=this->metodo_pixeles((int)(cx + y), (int)(cy - x), this->x, this->y, this->radio, acc_parametro_metodo_pixeles());
			Utilidades_graficas_SDL::SDL_PutPixel(p_superficie, p_caja, (int)(cx + y), (int)(cy - x), pixel, alpha);
		}

		if(y!=0)
		{
			if(metodo_pixeles) pixel=this->metodo_pixeles((int)(cx + x), (int)(cy - y), this->x, this->y, this->radio, acc_parametro_metodo_pixeles());
			Utilidades_graficas_SDL::SDL_PutPixel(p_superficie, p_caja, (int)(cx + x), (int)(cy - y), pixel, alpha);

			if(metodo_pixeles) pixel=this->metodo_pixeles((int)(cx - y), (int)(cy + x), this->x, this->y, this->radio, acc_parametro_metodo_pixeles());
			Utilidades_graficas_SDL::SDL_PutPixel(p_superficie, p_caja, (int)(cx - y), (int)(cy + x), pixel, alpha);
		}

		if (x!=0 && y!=0)
		{
			if(metodo_pixeles) pixel=this->metodo_pixeles((int)(cx - x), (int)(cy - y), this->x, this->y, this->radio, acc_parametro_metodo_pixeles());
			Utilidades_graficas_SDL::SDL_PutPixel(p_superficie, p_caja, (int)(cx - x), (int)(cy - y), pixel, alpha);

			if(metodo_pixeles) pixel=this->metodo_pixeles((int)(cx - y), (int)(cy - x), this->x, this->y, this->radio, acc_parametro_metodo_pixeles());
			Utilidades_graficas_SDL::SDL_PutPixel(p_superficie, p_caja, (int)(cx - y), (int)(cy - x), pixel, alpha);
		}

		error+=y;
		++y;
		error+=y;

		if(error >= 0)
		{
			--x;
			error-=x;
			error-=x;
		}
	}

	if(SDL_MUSTLOCK(p_superficie)) SDL_UnlockSurface(p_superficie);
}

bool Primitiva_grafica_circunferencia::volcado(SDL_Surface * p_superficie)
{
	if(!es_visible()) return false;

	unsigned int i=0;
	for(; i< this->ancho; i++)
		this->circunferencia(this->x, this->y, this->radio+i, p_superficie, acc_pixel_color());

	return true;
}

/*
Donde p_vista es camara.caja y p_posicion es camara.posicion.

p_vista es el rectángulo hipotético al que se está enfocando.
p_posicion es la caja que actualiza la cámara, es decir, dónde se ve en pantalla.
Desp_x es el desplazamiento extra x de la cámara.
Desp_y es el desplazamiento extra y de la cámara.
*/

bool Primitiva_grafica_circunferencia::volcado(SDL_Surface * p_superficie, const SDL_Rect& p_vista, const SDL_Rect& p_posicion, int desp_x, int desp_y)
{
	if(!es_visible()) return false;

	int punto_x=this->x+p_posicion.x-p_vista.x;
	int punto_y=this->y+p_posicion.y-p_vista.y;

	if(!this->es_estatica())
	{
		punto_x+=desp_x;
		punto_y+=desp_y;
	}

	unsigned int i=0;
	for(; i< this->ancho; i++)
		this->circunferencia(punto_x, punto_y, this->radio+i, p_vista, p_posicion, p_superficie, acc_pixel_color());

	return true;
}

void Primitiva_grafica_circunferencia::mut_x(int p_param)
{
	this->x=p_param;
	this->preparar_posicion();
}

void Primitiva_grafica_circunferencia::mut_y(int p_param)
{
	this->y=p_param;
	this->preparar_posicion();
}

void Primitiva_grafica_circunferencia::mut_radio(unsigned int p_param)
{
	this->radio=p_param;
	this->preparar_posicion();
}

void Primitiva_grafica_circunferencia::mut_ancho(unsigned int p_param)
{
	this->ancho=p_param;
	this->preparar_posicion();
}

void Primitiva_grafica_circunferencia::mut_circunferencia(int p_x, int p_y, unsigned int p_radio, unsigned int p_ancho)
{
	this->x=p_x;
	this->y=p_y;
	this->radio=p_radio;
	this->ancho=p_ancho;
	this->preparar_posicion();
}

void Primitiva_grafica_circunferencia::ir_a(int p_x, int p_y)
{
	this->x=p_x;
	this->y=p_y;
	this->preparar_posicion();
}
