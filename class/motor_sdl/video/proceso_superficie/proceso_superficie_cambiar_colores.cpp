#include "proceso_superficie_cambiar_colores.h"

using namespace DLibV;

Proceso_superficie_cambiar_colores::Proceso_superficie_cambiar_colores()
{

}

bool Proceso_superficie_cambiar_colores::procesar_pixel(Uint32 original, Uint32 &resultado, Uint32 trans)
{
	if(original!=this->color_original)
	{
		return false;
	}	
	else
	{
		resultado=this->nuevo_color;
		return true;
	}
}

Proceso_superficie_cambiar_colores::~Proceso_superficie_cambiar_colores()
{

}

Proceso_superficie_cambiar_colores * Proceso_superficie_cambiar_colores::generar(Uint32 p_original, Uint32 p_nuevo)
{
	Proceso_superficie_cambiar_colores * temp=new Proceso_superficie_cambiar_colores();
	
	temp->color_original=p_original;
	temp->nuevo_color=p_nuevo;
	return temp;
}
