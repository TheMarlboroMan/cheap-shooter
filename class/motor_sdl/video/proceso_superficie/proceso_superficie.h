#ifndef PROCESO_SUPERFICIE_H
#define PROCESO_SUPERFICIE_H

#include <SDL/SDL.h>

/*
Los procesos de pantalla se aplican a la pantalla. A cada paso se llama a 
"actuar" con la superficie de la pantalla una vez que la pantalla ha sido 
dibujada con todo. Cada uno haría algo con esa superficie.

Los procesos pueden estar "activos" o "inactivos". Sólo los activos se procesan.

Los procesos pueden ser finitos o infinitos. Los procesos finitos se desactivan
al finalizar. Los infinitos siempre están activos.

Y esa es la teoría.
*/

namespace DLibV
{

class Proceso_superficie
{
	private:

	bool activo;
	bool finalizado;	

	protected:

	Proceso_superficie();
	void finalizar() {this->finalizado=true;}
	virtual void proceso(SDL_Surface *, float)=0;

	public:

	virtual ~Proceso_superficie();
	bool es_finalizado() const {return this->finalizado;}

	bool es_activo() const {return this->activo;}
	void mut_activo(bool p_valor) {this->activo=p_valor;}

	void actuar(SDL_Surface *, float=0);
};

} //Fin namespace DLibV

#endif
