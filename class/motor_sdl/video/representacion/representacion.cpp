#include "representacion.h"

#include <iostream>

using namespace DLibV;

Representacion::Representacion():
	alpha(SDL_ALPHA_OPAQUE), con_clip_volcado(false), visible(true)
{
	this->reiniciar_posicion();
	this->reiniciar_recorte();
}

Representacion::Representacion(Uint8 p_alpha):
	alpha(p_alpha), visible(true)
{
	this->reiniciar_posicion();
	this->reiniciar_recorte();
}

Representacion::Representacion(const Representacion& p_otra):
	alpha(p_otra.alpha), visible(p_otra.visible)
{
	this->posicion=p_otra.posicion;
	this->recorte=p_otra.recorte;
}

Representacion::~Representacion()
{

}

void Representacion::establecer_posicion(int p_x, int p_y, int p_w, int p_h, int p_flags)
{
	if(p_flags & FRECT_X) this->posicion.x=p_x;
	if(p_flags & FRECT_Y) this->posicion.y=p_y;
	if(p_flags & FRECT_W && p_w != -1) this->posicion.w=p_w;
	if(p_flags & FRECT_H && p_h != -1) this->posicion.h=p_h;
}


void Representacion::reiniciar_posicion()
{
	this->posicion.x=0;
	this->posicion.y=0;
	this->posicion.w=0;
	this->posicion.h=0;
}

void Representacion::desplazar(Sint16 p_x, Sint16 p_y)
{
	this->posicion.x+=p_x;
	this->posicion.y+=p_y;
}

void Representacion::reiniciar_recorte()
{
	this->recorte.x=0;
	this->recorte.y=0;
	this->recorte.w=0;
	this->recorte.h=0;
}

void Representacion::establecer_posicion(const SDL_Rect & p_caja)
{
	posicion=p_caja;
}

void Representacion::establecer_recorte(const SDL_Rect & p_caja)
{
	recorte=p_caja;
}


void Representacion::establecer_recorte(Sint16 p_x, Sint16 p_y, Uint16 p_w, Uint16 p_h, int p_flags)
{
	if(p_flags & FRECT_X) this->recorte.x=p_x;
	if(p_flags & FRECT_Y) this->recorte.y=p_y;
	if(p_flags & FRECT_W) this->recorte.w=p_w;
	if(p_flags & FRECT_H) this->recorte.h=p_h;
}

Representacion& Representacion::operator=(const Representacion& p_otra)
{
	this->posicion=p_otra.posicion;
	this->recorte=p_otra.recorte;
	this->visible=p_otra.visible;

	return *this;
}

//Se pasa el rectángulo de pantalla... Básicamente se comprueba si está dentro. Estática o no.
bool Representacion::volcar(SDL_Surface * p_superficie, const SDL_Rect& p_pant)
{
	if(!en_toma(p_pant)) return false;
	else return this->volcar(p_superficie);
}

//Explícito, con cámara... LLama al de abajo pero es más cómodo.
bool Representacion::volcar(SDL_Surface * p_superficie, const Camara& p_camara)
{
	return this->volcado(p_superficie, p_camara.acc_caja(), p_camara.acc_caja_pos(), p_camara.acc_desp_x(), p_camara.acc_desp_y());
}

//Explícitos, con pantalla...
bool Representacion::volcar(const Pantalla& p_pantalla, const Camara& p_camara)
{
	return this->volcado(p_pantalla.acc_superficie(), p_camara.acc_caja(), p_camara.acc_caja_pos(), p_camara.acc_desp_x(), p_camara.acc_desp_y());
}

bool Representacion::volcar(const Pantalla& p_pantalla)
{
	return this->volcado(p_pantalla.acc_superficie());
}

bool Representacion::volcar(SDL_Surface * p_superficie)
{
	return this->volcado(p_superficie);
}

bool Representacion::volcar(SDL_Surface * p_superficie, const SDL_Rect& p_vista, const SDL_Rect& p_posicion, int desp_x, int desp_y)
{
	return this->volcado(p_superficie, p_vista, p_posicion, desp_x, desp_y);
}

//Se usa para darle un volumen a la posición, que de por si no tiene.
//La posición no tendría interés ninguno de no ser que la cámara la 
//considera para ver si dibujarla o no. Por defecto podemos pensar que 
//es el del recorte.

void Representacion::establecer_dimensiones_posicion_por_recorte()
{
	this->posicion.w=this->recorte.w;
	this->posicion.h=this->recorte.h;
}
