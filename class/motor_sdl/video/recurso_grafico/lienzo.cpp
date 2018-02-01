#include "lienzo.h"

using namespace DLibV;

Lienzo::Lienzo():Recurso_grafico()
{
	this->generado=false;
}

Lienzo::Lienzo(const Lienzo& p_otro)
{
	Recurso_grafico::operator=(p_otro);
	this->generado=p_otro.generado;
}

Lienzo::~Lienzo()
{	

}

Lienzo * Lienzo::generar_nuevo(int p_ancho, int p_alto)
{
	Lienzo * resultado=NULL;
	resultado=new Lienzo();
	resultado->generado=true;

	SDL_PixelFormat * formato=Gestor_color::obtener_formato();
	
	resultado->superficie=SDL_CreateRGBSurface(SDL_SWSURFACE, 
		p_ancho, p_alto, 
		formato->BitsPerPixel,
		formato->Rmask, formato->Gmask, formato->Bmask, formato->Amask);

	return resultado;
}

Lienzo& Lienzo::operator=(const Lienzo& p_otro)
{
	Recurso_grafico::operator=(p_otro);
	this->generado=p_otro.generado;
	return *this;
}

