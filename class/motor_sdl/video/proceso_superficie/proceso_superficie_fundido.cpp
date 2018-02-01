#include "proceso_superficie_fundido.h"

using namespace DLibV;

Proceso_superficie_fundido::Proceso_superficie_fundido(
		SDL_Surface const * p_origen, Uint8 p_r, Uint8 p_g, Uint8 p_b, 
		Uint8 p_alpha_ini, Uint8 p_alpha_fin, float p_duracion)
		:
		r(p_r), g(p_g), b(p_b),  	
		alpha_inicial(p_alpha_ini), alpha_final(p_alpha_fin), 
		alpha_actual(p_alpha_ini), 	
		duracion(p_duracion), tics(0.0),
		linea(DLibH::Linea::crear_desde_puntos(0, p_alpha_ini, p_duracion, p_alpha_fin))	//No me puedo creer que esto funcione!.
{
	this->superficie=Utilidades_graficas_SDL::nueva_superficie_formato(p_origen); //, SDL_SRCALPHA);
	SDL_FillRect(this->superficie, NULL, Gestor_color::color(this->r, this->g, this->b));
}
	
/*El proceso es siempre lineal, de modo que podemos usar una ecuación lineal
para esto. La línea se construye en un eje cartesiano donde X es tiempo e 
Y es cantidad de alpha.*/

void Proceso_superficie_fundido::calcular_alpha(float p_delta)
{
	this->tics+=p_delta;
	
	if(this->tics > this->duracion)
	{
		this->finalizar();
	}
	else
	{	
		this->alpha_actual=this->linea.obtener_y_para_x(this->tics);
	}
}

void Proceso_superficie_fundido::proceso(SDL_Surface * p_superficie, float p_delta)
{
	this->calcular_alpha(p_delta);

//std::cout<<"APLICANDO ALPHA "<<(unsigned short int)this->alpha_actual;

	SDL_SetAlpha(this->superficie, SDL_SRCALPHA, this->alpha_actual);
	SDL_BlitSurface(this->superficie, NULL, p_superficie, NULL);
}

Proceso_superficie_fundido::~Proceso_superficie_fundido()
{
	if(this->superficie) SDL_FreeSurface(this->superficie);
}

Proceso_superficie_fundido * Proceso_superficie_fundido::generar(SDL_Surface const * p_origen, Uint8 p_r, Uint8 p_g, Uint8 p_b, Uint8 p_desde, Uint8 p_hasta, float p_duracion)
{
	Proceso_superficie_fundido * temp=new Proceso_superficie_fundido(p_origen, p_r, p_g, p_b, p_desde, p_hasta, p_duracion);
	return temp;
}
