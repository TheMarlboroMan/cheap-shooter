#include "barra.h"

Barra::Barra(unsigned int pw, unsigned int ph):
	ancho(pw), altura(ph), 
	valor_maximo(0), valor_actual(0), orientacion(O_HORIZONTAL), 
	modelo_relleno(M_IZQUIERDA_A_DERECHA), color_barra(0), color_fondo(0)
{
	//El valor máximo y altura se usan para darle un tamaño al lienzo que no podremos cambiar.
	this->rep=new DLibV::Representacion_pixeles_estatica(ancho, altura);
	this->rep->establecer_clave_color(0,0,0);
	this->rep->ref_recurso()->establecer_alpha(127);
}

Barra::~Barra()
{
	if(this->rep) delete this->rep;
}

void Barra::actualizar()
{
	this->rep->rellenar(0,0,0);

	//Calcular la longitud según la relación ancho_maximo y valor_máximo...
	// ANCHO_MAXIMO -> valor_maximo
	// w ->		   valor_actual 
	//
	// w= (ANCHO_MAXIMO * valor_actual) / valor_maximo

	rellenar(color_fondo);
	dibujar(color_barra);
}

void Barra::dibujar(Uint32 color)
{
	dibujar(color, valor_actual);
}

void Barra::dibujar(Uint32 color, int cantidad)
{
	int dim=orientacion==O_HORIZONTAL ? ancho : altura;
	int w=(dim * cantidad) / valor_maximo;
	int h=altura;
	int x=0;
	int y=0;

	if(modelo_relleno==M_DERECHA_A_IZQUIERDA)
	{
		if(orientacion==O_HORIZONTAL) x=ancho-w;
		else 
		{
			std::swap(w, h);
			y=altura-h;
		}
	}
	else if(orientacion==O_VERTICAL) 
	{
		std::swap(w, h);
	}
	
	SDL_Rect caja=DLibH::Herramientas_SDL::nuevo_sdl_rect(x, y, w, h);

	this->rep->cambiar_color(color);
	this->rep->caja(caja);
}

void Barra::rellenar(Uint32 color)
{
	SDL_Rect caja=DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, ancho, altura);
	this->rep->cambiar_color(color);
	this->rep->caja(caja);

}
