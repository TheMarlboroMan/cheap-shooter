#ifndef IMAGEN_H
#define IMAGEN_H

#include "recurso_grafico.h"

/*
La imagen es un recurso gráfico que carga un bitmap. Más fácil imposible.
*/

namespace DLibV
{

class Imagen:public Recurso_grafico
{
	public:

	Imagen(SDL_Surface *);
	Imagen(char const *);
	Imagen(std::string const&);
	~Imagen();

	int obtener_tipo(){return Recurso_grafico::T_IMAGEN;}
	bool es_preparado() {return this->superficie!=NULL;}
};

} //Fin namespace DLibV

#endif
