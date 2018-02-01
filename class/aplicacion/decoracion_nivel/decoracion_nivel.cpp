#include "decoracion_nivel.h"

Decoracion_nivel::Decoracion_nivel(unsigned int p_prof)
	:Obstaculo(), profundidad(p_prof)
{
	this->preparar_actor(0);
}

Decoracion_nivel::~Decoracion_nivel()
{

}

void Decoracion_nivel::establecer_caracteristicas()
{
	this->w=W_DEFECTO;
	this->h=H_DEFECTO;
	this->posicion.w=W_DEFECTO;
	this->posicion.h=H_DEFECTO;
}

void Decoracion_nivel::establecer_representacion()
{
	SDL_Rect caja_recorte=DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0,this->w,this->h);
	SDL_Rect patron=DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, 32, 32);
	this->establecer_representacion_patron_dinamica(this->posicion, patron, DLibV::Gestor_color::color(0, 255, 255), Recursos::G_TILE_JUEGO_IND);
		
	Representable::establecer_recorte(caja_recorte);	
	this->actualizar_representacion();
}	

void Decoracion_nivel::actualizar_representacion()
{
	Representable::establecer_posicion(this->posicion);
}

void Decoracion_nivel::refrescar_propiedades()
{
	this->actualizar_representacion();
	this->recargar_posicion();	
}

void Decoracion_nivel::procesar_cambio_dimensiones()
{
	this->actualizar_representacion();
	this->recargar_posicion();	
}

void Decoracion_nivel::recibir_valores_actor_flexible(const Parametros_actor_flexible& rep)
{
	Representable::establecer_caja_patron(rep.rec_x, rep.rec_y, rep.rec_w, rep.rec_h);
	Representable::mut_indice_patron(rep.indice_recurso);
	Representable::establecer_alineamiento(rep.alx, rep.aly);
	this->refrescar_propiedades();
}

void Decoracion_nivel::recibir_valores_actor_redimensionable(const Parametros_actor_redimensionable& red)
{
	Actor_mapa::recibir_valores_actor_redimensionable(red);
	this->refrescar_propiedades();
}

void Decoracion_nivel::recibir_valores_actor_ordenable_profundidad(const Parametros_actor_ordenable_profundidad& pro)
{
	unsigned int min=limite_min_profundidad();
	unsigned int max=limite_max_profundidad();
	unsigned int p=pro.p;

	if(p > max ) p=max;
	else if(p < min) p=min;

	profundidad=p;
}
