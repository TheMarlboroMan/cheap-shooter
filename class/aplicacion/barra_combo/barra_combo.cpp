#include "barra_combo.h"

const Uint32 Barra_combo::COLOR_FONDO=4210752; //Gestor_color::color(64,64,64);
const Uint32 Barra_combo::COLOR_BARRA=16777215; //Gestor_color::color(255,255,255);

Barra_combo::Barra_combo():
	Barra(ANCHO, ALTURA)
{
	establecer_orientacion(O_VERTICAL);
	establecer_modelo_relleno(M_DERECHA_A_IZQUIERDA);
	establecer_color_fondo(COLOR_FONDO);
	establecer_color_barra(COLOR_BARRA);
	establecer_posicion(POS_X, POS_Y);
}

Barra_combo::~Barra_combo()
{

}
