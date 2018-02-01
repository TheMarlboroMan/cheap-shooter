#include "representacion_manual.h"

using namespace DLibV;

Representacion_manual::Representacion_manual():Representacion_grafica()
{

}

Representacion_manual::Representacion_manual(const Representacion_manual& o):Representacion_grafica(o)
{

}

Representacion_manual::~Representacion_manual()
{
	//La gestión de liberar o no el recurso cae en las clases que extiendan a esta.
	//Esta clase es completamente virtual.
}

Representacion_manual& Representacion_manual::operator=(const Representacion_manual& p_otra)
{
//	Representacion::operator=(p_otra);
	Representacion_grafica::operator=(p_otra);
	this->clave_color=p_otra.clave_color;
	this->clave_color_activa=p_otra.clave_color_activa;
	return *this;
}

void Representacion_manual::establecer_clave_color(int p_r, int p_g, int p_b)
{
	this->clave_color=Gestor_color::color(p_r, p_g, p_b);
	this->ref_recurso()->establecer_transparencia(this->clave_color);
}

void Representacion_manual::establecer_clave_color(Uint32 p_clave)
{
	this->clave_color=p_clave;
	this->ref_recurso()->establecer_transparencia(this->clave_color);
}

void Representacion_manual::establecer_clave_color(SDL_PixelFormat * formato)
{
	this->clave_color=Gestor_color::clave_color_formato(formato);
	this->ref_recurso()->establecer_transparencia(this->clave_color);
}

void Representacion_manual::establecer_clave_color(SDL_Surface * superficie)
{
	this->clave_color=Gestor_color::clave_color_superficie(superficie);
	this->ref_recurso()->establecer_transparencia(this->clave_color);
}

void Representacion_manual::desactivar_clave_color()
{
	this->ref_recurso()->limpiar_transparencia();
}
