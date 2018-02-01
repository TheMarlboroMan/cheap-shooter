#ifndef DECORACION_H
#define DECORACION_H

/*
La decoración es un actor no jugador que no tiene efecto en el juego y es, 
simplemente, decorativo. Explosiones o sistemas de partículas serían 
ejemplos de esto.

Las decoraciones deben implementar un método para indicar si pueden ser
eliminadas porque su utilidad ha pasado.
*/

#include "../no_jugador/no_jugador.h"

class Decoracion:public No_jugador
{
	protected:

	virtual void establecer_representacion()=0;
	virtual void establecer_caracteristicas()=0;
	
	virtual bool puede_rebotar() const {return false;}

	public:

	enum TIPOS_DECORACION{
		D_CHATARRA, 
		D_CHISPA, 
		D_EXPLOSION, 
		D_PUNTUACION, 
		D_EDU, 
		D_HUMO,
		D_FANTASMA};

	Decoracion();
	virtual ~Decoracion();
	virtual bool es_afectado_por_edu() const {return false;}

	virtual bool es_finalizado()=0;
	virtual unsigned short int tipo_decoracion()=0;

	virtual void procesar_turno(float=0);
	
};

#endif

