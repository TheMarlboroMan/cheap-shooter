#include "representacion_compuesta.h"

using namespace DLibV;

Representacion_compuesta::Representacion_compuesta(unsigned int p_w, unsigned int p_h)
	:Representacion_lienzo(p_w, p_h)
{
	//Eso aquí por si se nos olvida.
	this->establecer_recorte(0,0, p_w, p_h);
	this->establecer_posicion(0,0, p_w, p_h, Representacion_grafica::FRECT_X |
						Representacion_grafica::FRECT_Y |
						Representacion_grafica::FRECT_W |
						Representacion_grafica::FRECT_H);
}

Representacion_compuesta::Representacion_compuesta(unsigned int p_w, unsigned int p_h, unsigned int p_r, unsigned int p_g, unsigned int p_b):Representacion_lienzo(p_w, p_h)
{
	//Eso aquí por si se nos olvida.
	this->establecer_recorte(0,0, p_w, p_h);
	this->establecer_posicion(0,0, p_w, p_h, Representacion_grafica::FRECT_X |
						Representacion_grafica::FRECT_Y |
						Representacion_grafica::FRECT_W |
						Representacion_grafica::FRECT_H);

	this->establecer_clave_color(p_r, p_g, p_b);
}

Representacion_compuesta::~Representacion_compuesta()
{

}

Representacion_compuesta& Representacion_compuesta::operator=(const Representacion_compuesta& p_otra)
{
	Representacion_lienzo::operator=(p_otra);
	return *this;
}

void Representacion_compuesta::volcar_representacion(Representacion &p_otra, const SDL_Rect &p_pos)
{
	SDL_Rect original=p_otra.acc_posicion();
	p_otra.establecer_posicion(p_pos);
	p_otra.volcar(this->ref_recurso()->acc_superficie());
	p_otra.establecer_posicion(original);
}

void Representacion_compuesta::volcar_representacion(Representacion &p_otra)
{
	SDL_Rect pos=DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0, this->ref_recurso()->acc_superficie()->w, this->ref_recurso()->acc_superficie()->h);

	SDL_Rect original=p_otra.acc_posicion();
	p_otra.establecer_posicion(pos);
	p_otra.volcar(this->ref_recurso()->acc_superficie());
	p_otra.establecer_posicion(original);
}

void Representacion_compuesta::preparar()
{
	//Aquí no hay nada que hacer, aunque tenemos que usar el método 
	//para, al menos, marcar la representación como preparada.
	this->marcar_como_preparada();
}
