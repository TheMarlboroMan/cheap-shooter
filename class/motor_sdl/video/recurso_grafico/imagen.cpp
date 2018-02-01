#include "imagen.h"

using namespace DLibV;

//Toma propiedad del puntero a partir de este momento..

Imagen::Imagen(SDL_Surface * p_superficie):Recurso_grafico()
{
	this->superficie=p_superficie;
}

Imagen::Imagen(char const * p_ruta):Recurso_grafico()
{
	this->superficie=Utilidades_graficas_SDL::cargar_imagen_en_superficie(p_ruta);
}

Imagen::Imagen(std::string const & p_ruta):Recurso_grafico()
{
	this->superficie=Utilidades_graficas_SDL::cargar_imagen_en_superficie(p_ruta.c_str());
}

Imagen::~Imagen()
{

}
