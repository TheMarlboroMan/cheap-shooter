#ifndef ESCUDO_PARTE_H
#define ESCUDO_PARTE_H

#include "../actor/actor.h"
#include "../representable/representable.h"

class Escudo;

class Escudo_parte:public Actor, public Representable
{
	private:

	static const unsigned int X_REPRESENTACION=96;
	static const unsigned int Y_REPRESENTACION=96;
	static const unsigned int W_REPRESENTACION=16;
	static const unsigned int H_REPRESENTACION=16;
	static const unsigned int W=16;
	static const unsigned int H=16;

	static const unsigned int ALPHA_ACTIVO=255;
	static const unsigned int ALPHA_INACTIVO=64;

	protected:

	void establecer_posicion();
	void establecer_representacion();
	void procesar_turno(float);
	void establecer_transparencia(unsigned short int);

	public:

	Escudo_parte();
	virtual ~Escudo_parte();

	void cuentame() {std::cout<<"SOY PARTE ESCUDO"<<std::endl;}

	friend class Escudo;
};

#endif
