#include "primitiva_grafica_linea.h"

using namespace DLibV;

Primitiva_grafica_linea
	::Primitiva_grafica_linea(int px1, int py1, int px2, int py2, Uint32 p_pixel_color)
	:Primitiva_grafica(p_pixel_color), 
	x1(px1),y1(py1),x2(px2),y2(py2), metodo_pixeles(NULL)
	
{
	this->preparar_posicion();
}

Primitiva_grafica_linea::Primitiva_grafica_linea(const Primitiva_grafica_linea& o)
	:Primitiva_grafica(o),
	x1(o.x1), y1(o.y1), x2(o.x2), y2(o.y2), metodo_pixeles(o.metodo_pixeles)
{

}

Primitiva_grafica_linea& Primitiva_grafica_linea::operator=(const Primitiva_grafica_linea& o)
{
	Primitiva_grafica::operator=(o);
	x1=o.x1;
	y1=o.y1;
	x2=o.x2;
	y2=o.y2;
	metodo_pixeles=o.metodo_pixeles;

	return *this;
}

Primitiva_grafica_linea::~Primitiva_grafica_linea()
{
	
}

/*Se explica sólo: generamos la caja que encajonaría la línea*/

void Primitiva_grafica_linea::preparar_posicion()
{
	SDL_Rect pos=DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0,0,0);

	if(x1 <= x2)
	{
		pos.x=x1;
		pos.w=x2-x1;
	}
	else //if(x1 > x2)
	{
		pos.x=x2;
		pos.w=x1-x2;
	}

	if(y1 <= y2)
	{
		pos.y=y1;
		pos.h=y2-y1;
	}
	else //if(y1 > y2)
	{
		pos.y=y2;
		pos.h=y1-y2;
	}

	establecer_posicion(pos);
	if(es_recomponer_recorte_con_posicion()) this->recorte_a_posicion();
}

/*Estos son para modificar las dimensiones de la línea. Hay varias, desde 
alterar un sólo parámetro hasta modificarlos todos. Todos ellos, al finalizar,
vuelven a calcular la caja.*/

void Primitiva_grafica_linea::mut_x1(int param)
{
	this->x1=param;
	this->preparar_posicion();
}

void Primitiva_grafica_linea::mut_y1(int param)
{
	this->y1=param;
	this->preparar_posicion();
}

void Primitiva_grafica_linea::mut_x2(int param)
{
	this->x2=param;
	this->preparar_posicion();
}

void Primitiva_grafica_linea::mut_y2(int param)
{
	this->y2=param;
	this->preparar_posicion();
}

void Primitiva_grafica_linea::mut_punto_1(int p_x, int p_y)
{
	this->x1=p_x;
	this->y1=p_y;
	this->preparar_posicion();
}

void Primitiva_grafica_linea::mut_punto_2(int p_x, int p_y)
{
	this->x2=p_x;
	this->y2=p_y;
	this->preparar_posicion();
}

void Primitiva_grafica_linea::mut_puntos(int p_x1, int p_y1, int p_x2, int p_y2)
{
	this->x1=p_x1;
	this->y1=p_y1;
	this->x2=p_x2;
	this->y2=p_y2;
	this->preparar_posicion();
}

//Es un caso especial, como se puede ver.

void Primitiva_grafica_linea::ir_a(int p_x, int p_y)
{
	this->x1+=p_x;
	this->x2+=p_x;
	this->y1+=p_y;
	this->y2+=p_y;
	this->preparar_posicion();
}

/*Esta es la implementación de la manipulación de la superficie.*/

bool Primitiva_grafica_linea::volcado(SDL_Surface * p_superficie)
{
	if(!es_visible()) return false;
	this->linea(x1, y1, x2, y2, p_superficie, acc_pixel_color());
	return true;
}

bool Primitiva_grafica_linea::volcado(SDL_Surface * p_superficie, const SDL_Rect& p_vista, const SDL_Rect& p_posicion, int desp_x, int desp_y)
{
	if(!es_visible()) return false;
	int xe1=x1+p_posicion.x-p_vista.x;
	int ye1=y1+p_posicion.y-p_vista.y;
	int xe2=x2+p_posicion.x-p_vista.x;
	int ye2=y2+p_posicion.y-p_vista.y;

	if(!this->es_estatica())
	{
		xe1+=desp_x;
		xe2+=desp_x;
		ye1+=desp_y;
		ye2+=desp_y;
	}

	this->linea(xe1, ye1, xe2, ye2, p_vista, p_posicion, p_superficie, acc_pixel_color());
	
	return true;
}

/*Este es el método de entrada principal para  dibujar una línea. Le pasaremos
los puntos, la superficie y el color y se encargará de determinar si necesitamos
calcular pendientes o no para llamar a los métodos de turno. Recibe los puntos
como parámetros por si han sido alterados para , por ejemplo, entrar en una
cámara.*/

void Primitiva_grafica_linea::linea(int xa, int ya, int xb, int yb, SDL_Surface * superficie, Uint32 pixel)
{
	SDL_Rect caja=DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, superficie->w, superficie->h);
	linea(xa, ya, xb, yb, caja, caja, superficie, pixel);
}

void Primitiva_grafica_linea::linea(int xa, int ya, int xb, int yb,  SDL_Rect const& p_vista, SDL_Rect const& p_posicion, SDL_Surface * superficie, Uint32 pixel)
{
	/*OJO CON ESTO: xa, ya, xb y yb ya llevan una cierta transformación
	por la propia posición de la caja y tal y cual...*/

	//Esta será la caja que usaremos al final para limitar la circunferencia (por si no sale entera por recorte o por cámara...).
	SDL_Rect p_caja=DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0,0,0);

	if(!this->determinar_caja_dibujo_final(p_caja, p_posicion, p_vista)) return;

	float m, b;

	//Si no es vertical...
	if(xa!=xb)
	{       
		//La pendiente: m=(yb-ya)/(xb-xa)...            
		m=((float)yb-(float)ya)/((float)xb-(float)xa);  

		//La constante b... b=y-(mx)
		b=ya-(m*xa);

		//Horizontal, sin pendiente...
		if(m==0)
		{
			Primitiva_grafica_linea::linea_horizontal(xa, ya, xb, p_caja, superficie, pixel);
		}
		//Pendiente...
		else
		{
			Primitiva_grafica_linea::linea(xa, ya, xb, yb, m, b, p_caja, superficie, pixel);
		}
	}
	else
	{
		Primitiva_grafica_linea::linea_vertical(xa, ya, yb, p_caja, superficie, pixel);
	}
}

void Primitiva_grafica_linea::linea(int xa, int ya, int xb, int yb, float m, float b, SDL_Rect const& p_caja, SDL_Surface * superficie, Uint32 p_pixel)
{
	int xini, xfin, y, old_y;
	Uint32 pixel=p_pixel;

	//Esto es terrible, parece determinar la dirección de la línea de esta manera.
	xini=xb > xa ? xa : xb;
	xfin=xb > xa ? xb : xa;

	old_y=(m*xini)+b;       //Para la primera iteración igualamos...
	Uint8 alpha=acc_alpha();

	if(SDL_MUSTLOCK(superficie)) SDL_LockSurface(superficie);

	for(;xini<=xfin; xini++)
	{
		//y=mx+b...
		y=(m*xini)+b;

		//Calculamos a raíz de X. Para líneas con una 
		//pendiente muy pronunciada tendremos que rellenar
		//más píxeles en y...

		if(abs(y-old_y) > 1)    
		{
			for(; old_y !=y; old_y < y ? old_y++ : old_y--)
			{
				if(this->metodo_pixeles) pixel=this->metodo_pixeles(xini, old_y, xa, ya, xb, yb, acc_parametro_metodo_pixeles());				
				Utilidades_graficas_SDL::SDL_PutPixel(superficie, p_caja, xini, old_y, pixel, alpha);
			}                               
		}                               

		if(this->metodo_pixeles) pixel=this->metodo_pixeles(xini, y, xa, ya, xb, yb, acc_parametro_metodo_pixeles());
		Utilidades_graficas_SDL::SDL_PutPixel(superficie, p_caja, xini, y, pixel, alpha);
		old_y=y;
	}

	if(SDL_MUSTLOCK(superficie)) SDL_UnlockSurface(superficie);
}

void Primitiva_grafica_linea::linea_horizontal(int xa, int y, int xb, SDL_Rect const& p_caja, SDL_Surface * superficie, Uint32 pixel)
{
	int xini=xa;
	int xfin=xb;
	Uint8 alpha=acc_alpha();

	if(SDL_MUSTLOCK(superficie)) SDL_LockSurface(superficie);

	for(;xa!=xb; xa < xb ? xa++ : xa--)
	{
		if(this->metodo_pixeles) pixel=this->metodo_pixeles(xa, y, xini, y, xfin, y, acc_parametro_metodo_pixeles());
		Utilidades_graficas_SDL::SDL_PutPixel(superficie, p_caja, xa, y, pixel, alpha);
	}

	if(SDL_MUSTLOCK(superficie)) SDL_UnlockSurface(superficie);
}

void Primitiva_grafica_linea::linea_vertical(int x, int ya, int yb, SDL_Rect const& p_caja, SDL_Surface * superficie, Uint32 pixel)
{
	if(SDL_MUSTLOCK(superficie)) SDL_LockSurface(superficie);

	int yini=ya;
	int yfin=yb;
	Uint8 alpha=acc_alpha();
	for(; ya!=yb; ya<yb ? ya++ : ya--)
	{
		if(this->metodo_pixeles) pixel=this->metodo_pixeles(x, ya, x, yini, x, yfin, acc_parametro_metodo_pixeles());
		Utilidades_graficas_SDL::SDL_PutPixel(superficie, p_caja, x, ya, pixel, alpha);
	}

	if(SDL_MUSTLOCK(superficie)) SDL_UnlockSurface(superficie);
}
