#include "representacion_grafica.h"

using namespace DLibV;

Representacion_grafica::Representacion_grafica()
	:Representacion(), recurso(NULL), preparada(false)
{
	this->reiniciar_posicion();
	this->reiniciar_recorte();
}

Representacion_grafica::Representacion_grafica(const Representacion_grafica& o)
	:Representacion(o) ,recurso(o.recurso), preparada(o.preparada)
{

}

Representacion_grafica& Representacion_grafica::operator=(const Representacion_grafica& o)
{
	Representacion::operator=(o);
	recurso=o.recurso;
	preparada=o.preparada;

	return *this;
}

/*
Representacion_grafica& Representacion_grafica::operator=(const Representacion_grafica& p_otra)
{
	Representacion::operator=(p_otra);
	this->recurso=p_otra.recurso;
	this->preparada=p_otra.preparada;

	return *this;
}
*/

Representacion_grafica::~Representacion_grafica()
{
	//OJO: No se borra el recurso gráfico porque se asume que lo hemos
	//obtenido de un gestor de recursos. Esta clase base NO gestiona los
	//recursos asignados. Una superior, en cambio, si podría.
}

void Representacion_grafica::recorte_a_medidas_recurso()
{
	establecer_recorte(0,0, this->recurso->acc_superficie()->w, this->recurso->acc_superficie()->h);
}

bool Representacion_grafica::volcado(SDL_Surface * p_superficie)
{
	//Si no está en la pantalla no perdemos tiempo.
	if(!this->es_visible())
	{
		return false;
	}
	else
	{
		if(!this->es_preparada()) this->preparar();
		
		//Definimos aquí estas variables: puede que al "preparar" hayan los valores.
		SDL_Rect rec=acc_recorte();
		SDL_Rect pos=acc_posicion();

		Uint8 alpha_original=this->recurso->acc_superficie()->format->alpha;
		if(this->acc_alpha()) this->recurso->establecer_alpha(this->acc_alpha());

		SDL_Rect temp=DLibH::Herramientas_SDL::copiar_sdl_rect(pos, 0, 0);

		bool resultado=false;

		if(SDL_BlitSurface(this->ref_superficie(), &rec, p_superficie, &temp) >= 0)
		{
			resultado=true;
		}

		if(this->acc_alpha()) this->recurso->establecer_alpha(alpha_original);

		return resultado;
	}
}

bool Representacion_grafica::volcado(SDL_Surface * p_superficie, const SDL_Rect& p_vista, const SDL_Rect& p_posicion, int desp_x, int desp_y)
{
	if(!this->es_visible())
	{
		return false;
	}
	else
	{	
		//Las representaciones estáticas están SIEMPRE en las mismas
		//posiciones para la cámara que la vea. Simplemente veremos
		//si está dentro de la caja de la cámara en 0,0.

		SDL_Rect pos=acc_posicion();
		bool en_toma=true;
		
		if(this->es_estatica())
		{
			SDL_Rect caja_cam=DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, p_vista.w, p_vista.h);
			en_toma=DLibH::Herramientas_SDL::rectangulos_superpuestos(caja_cam, pos, true);
		}
		else
		{
			en_toma=DLibH::Herramientas_SDL::rectangulos_superpuestos(p_vista, pos, true);
		}

		if(!en_toma)
		{
			return false;
		}
		else
		{
			Sint16 t_x, t_y;

			if(!this->es_preparada()) this->preparar();

			SDL_Rect rec=acc_recorte();
			pos=acc_posicion();	//Again, por si al preparar ha cambiado.

			if(this->es_estatica())
			{
				t_x=p_posicion.x+desp_x;
				t_y=p_posicion.y+desp_y;
			}
			else
			{
				t_x=p_posicion.x-p_vista.x+desp_x;
				t_y=p_posicion.y-p_vista.y+desp_y;
			}

//			SDL_Rect temp=DLibH::Herramientas_SDL::copiar_sdl_rect(pos, t_x, t_y);
			pos.x+=t_x;
			pos.y+=t_y;

			bool resultado=false;

			Uint8 alpha_original=this->recurso->acc_superficie()->format->alpha;
			if(this->acc_alpha()) this->recurso->establecer_alpha(this->acc_alpha());

			if(SDL_BlitSurface(this->ref_superficie(), &rec, p_superficie, &pos) >= 0)
			{
				resultado=true;
			}

			if(this->acc_alpha()) this->recurso->establecer_alpha(alpha_original);

			return resultado;
		}
	}
}

void Representacion_grafica::rellenar(Uint8 p_r, Uint8 p_g, Uint8 p_b)
{
	this->recurso->rellenar(p_r, p_g, p_b);
}

void Representacion_grafica::rellenar(Uint32 c)
{
	this->recurso->rellenar(c);
}

void Representacion_grafica::liberar_recurso()
{
	if(this->recurso)
	{
		delete this->recurso;
		this->recurso=NULL;
	}
}
