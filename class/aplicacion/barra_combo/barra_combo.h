#ifndef BARRA_COMBO_H
#define BARRA_COMBO_H

#include "../barra/barra.h"

class Barra_combo:public Barra
{
	private:

	const static unsigned int POS_X=730;
	const static unsigned int POS_Y=12;
	const static unsigned int ALTURA=40; 

	public:

	const static unsigned int ANCHO=4; 
	const static Uint32 COLOR_FONDO;
	const static Uint32 COLOR_BARRA;

	Barra_combo();
	~Barra_combo();
};

#endif
