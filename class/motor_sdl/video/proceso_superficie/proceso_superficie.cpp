#include "proceso_superficie.h"

using namespace DLibV;

Proceso_superficie::Proceso_superficie():
	activo(true), finalizado(false)
{

}

Proceso_superficie::~Proceso_superficie()
{

}

void Proceso_superficie::actuar(SDL_Surface * p_superficie, float p_delta)
{
	if(this->activo)
	{
		this->proceso(p_superficie, p_delta);
	}
}

