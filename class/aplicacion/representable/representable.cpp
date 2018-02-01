#include "representable.h"

Representable::Representable()
	:desp_rep_x(0), desp_rep_y(0), indice_recurso(0), 
	indice_patron(0), representable(true), representacion(NULL)
{

}	

Representable::~Representable()
{
	if(this->representacion)
	{
		delete this->representacion;
	}
}

void Representable::establecer_representacion_bitmap_dinamica()
{
	if(!this->representacion)
	{
		this->representacion=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(indice_recurso));
	}
}

void Representable::establecer_representacion_bitmap_dinamica(DLibV::Recurso_grafico * recurso)
{
	if(!this->representacion)
	{
		this->representacion=new DLibV::Representacion_bitmap_dinamica(recurso);
	}
}

void Representable::establecer_representacion_patron_dinamica(const SDL_Rect& p_caja, const SDL_Rect& p_patron, Uint32 p_color, unsigned int p_indice_patron)
{
	if(!this->representacion)
	{
		this->indice_patron=p_indice_patron;

		this->representacion=new DLibV::Representacion_patron_dinamica
		(
		p_caja, 
		p_patron, 
		p_color,
		DLibV::Gestor_recursos_graficos::obtener(indice_patron));
	}
}

void Representable::establecer_representacion_agrupada_dinamica(bool poseer)
{
	if(!this->representacion)
	{
		this->representacion=new DLibV::Representacion_agrupada_dinamica(poseer);
	}
}

void Representable::establecer_representacion(DLibV::Representacion * p_rep)
{
	if(!this->representacion)
	{
		this->representacion=p_rep;
	}
}

void Representable::mut_indice_patron(unsigned int p_indice)
{
	if(this->representacion)
	{
		this->indice_patron=p_indice;
		
		DLibV::Representacion_patron_dinamica * pr=static_cast<DLibV::Representacion_patron_dinamica *>(this->representacion);

		unsigned int indice=DLibV::Gestor_recursos_graficos::comprobar(p_indice) ? p_indice : RECURSO_POR_DEFECTO;
		pr->actualizar_patron(DLibV::Gestor_recursos_graficos::obtener(indice));
		pr=NULL;
	}
}

//COmo siempre, ojo, se castea y no se comprueba...

void Representable::insertar_en_representacion_agrupada(DLibV::Representacion * p_rep)
{
	if(this->representacion)
	{
		DLibV::Representacion_agrupada_dinamica * pr=static_cast<DLibV::Representacion_agrupada_dinamica *>(this->representacion);
		pr->insertar_representacion(p_rep);
		pr=NULL;
	}
}

/*Ojo con esto, se hace el cast sin comprobar nada*/

inline void Representable::establecer_caja_patron(const SDL_Rect& p_caja)
{
	if(this->representacion)
	{
		DLibV::Representacion_patron_dinamica * pr=static_cast<DLibV::Representacion_patron_dinamica *>(this->representacion);
		pr->actualizar_caja_patron(p_caja);
		pr=NULL;
	}
}

void Representable::establecer_caja_patron(int px, int py, int pw, int ph)
{
	SDL_Rect caja=DLibH::Herramientas_SDL::nuevo_sdl_rect(px, py, pw, ph);
	this->establecer_caja_patron(caja);
}

void Representable::establecer_alineamiento(int px, int py)
{
	if(this->representacion)
	{
		DLibV::Representacion_patron_dinamica * pr=static_cast<DLibV::Representacion_patron_dinamica *>(this->representacion);
		pr->actualizar_alineamiento(px, py);
		pr=NULL;
	}
}


//De nuevo, ojo, se hace la conversión y poco más.
SDL_Rect Representable::acc_caja_patron() const 
{
	if(this->representacion)
	{
		DLibV::Representacion_patron_dinamica * pr=static_cast<DLibV::Representacion_patron_dinamica *>(this->representacion);
		return pr->acc_caja_patron();
	}
	else
	{
		return DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0,0,0);
	}
}

int Representable::acc_alineamiento_x() const
{
	if(this->representacion)
	{
		DLibV::Representacion_patron_dinamica * pr=static_cast<DLibV::Representacion_patron_dinamica *>(this->representacion);
		return pr->acc_alineamiento_x();
	}
	else
	{
		return 0;
	}
}

int Representable::acc_alineamiento_y() const
{
	if(this->representacion)
	{
		DLibV::Representacion_patron_dinamica * pr=static_cast<DLibV::Representacion_patron_dinamica *>(this->representacion);
		return pr->acc_alineamiento_y();
	}
	else
	{
		return 0;
	}
}

void Representable::establecer_recorte(const SDL_Rect& p_caja)
{
	if(this->representacion)
	{
		this->representacion->establecer_recorte(p_caja);
	}
}

void Representable::establecer_recorte(int px, int py, int pw, int ph)
{
	if(this->representacion)
	{
		this->representacion->establecer_recorte(px, py, pw, ph);
	}
}

void Representable::establecer_posicion(int px, int py )
{
	if(this->representacion)
	{
		this->representacion->establecer_posicion(px+desp_rep_x, py+desp_rep_y);
	}
}

void Representable::establecer_posicion(int px, int py, int pw, int ph)
{
	if(this->representacion)
	{
		this->representacion->establecer_posicion(px+desp_rep_x, py+desp_rep_y, pw, ph);
	}
}

void Representable::establecer_posicion(const SDL_Rect& p_caja)
{
	if(this->representacion)
	{
		this->representacion->establecer_posicion(p_caja.x+desp_rep_x, p_caja.y+desp_rep_y, p_caja.w, p_caja.h);
	}
}
