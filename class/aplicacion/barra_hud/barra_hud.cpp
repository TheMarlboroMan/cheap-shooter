#include "barra_hud.h"

const Uint32 Barra_hud::COLOR_FONDO=4210752; //Gestor_color::color(64,64,64);
const Uint32 Barra_hud::COLOR_NO_DISPONIBLE=14428200; //Gestor_color::color(220,40,40);
const Uint32 Barra_hud::COLOR_DISPONIBLE=13158464; //Gestor_color::color(200,200,64);
const Uint32 Barra_hud::COLOR_AUXILIAR=5013695; //Gestor_color::color(76, 128, 191);

Barra_hud::Barra_hud():
	Barra(ANCHO, ALTURA), valor_auxiliar(0), color_auxiliar(COLOR_AUXILIAR)
{
	//El valor máximo y altura se usan para darle un tamaño al lienzo que no podremos cambiar.
	establecer_valor_maximo(Barra_hud::ANCHO);
	establecer_color_fondo(COLOR_FONDO);
}

Barra_hud::~Barra_hud()
{

}

void Barra_hud::actualizar()
{
	Barra::actualizar();

	if(valor_auxiliar)
	{	
		dibujar(color_auxiliar, valor_auxiliar);
	}
}
