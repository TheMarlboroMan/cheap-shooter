#include "proceso_superficie_blanco_negro.h"

using namespace DLibV;

Proceso_superficie_blanco_negro::Proceso_superficie_blanco_negro(bool p_trans)
	:Proceso_superficie_base_por_pixel(),
	respetar_transparencia(p_trans)
{

}

bool Proceso_superficie_blanco_negro::procesar_pixel(Uint32 original, Uint32 &resultado, Uint32 trans)
{
	if(original==trans && this->respetar_transparencia)
	{
		return false;
	}
	else
	{
		SDL_GetRGBA(original, this->acc_formato(), &r, &g, &b, &a);
		resultado=(r+g+b)/3;
		return true;
	}
}

Proceso_superficie_blanco_negro::~Proceso_superficie_blanco_negro()
{

}

Proceso_superficie_blanco_negro * Proceso_superficie_blanco_negro::generar(bool p_trans)
{
	Proceso_superficie_blanco_negro * temp=new Proceso_superficie_blanco_negro(p_trans);
	return temp;
}
