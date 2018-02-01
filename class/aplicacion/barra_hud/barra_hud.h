#ifndef BARRA_HUD_H
#define BARRA_HUD_H

#include "../barra/barra.h"

class Barra_hud:public Barra
{
	private:

	unsigned int valor_auxiliar;
	Uint32 color_auxiliar;

	const static unsigned int ALTURA=2; 

	public:

	const static unsigned int ANCHO=60; 
	const static Uint32 COLOR_FONDO;
	const static Uint32 COLOR_DISPONIBLE;
	const static Uint32 COLOR_NO_DISPONIBLE;
	const static Uint32 COLOR_AUXILIAR;

	Barra_hud();
	~Barra_hud();

	void establecer_valor_auxiliar(unsigned int p_valor) {this->valor_auxiliar=p_valor;}
	void establecer_color_auxiliar(Uint32 p_param){this->color_auxiliar=p_param;}
	virtual void actualizar();
};


#endif
