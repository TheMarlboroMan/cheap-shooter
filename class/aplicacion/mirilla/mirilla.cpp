#include "mirilla.h"

Mirilla::Mirilla()
{
	this->inicializar();
	this->establecer_representacion();
	this->w=16;
	this->h=16;
	this->posicion=DLibH::Herramientas_SDL::nuevo_sdl_rect((int)this->x, (int)this->y, this->w, this->h);
	this->establecer_posicion();
	this->activa_gamepad=false;
	this->input_gamepad_x=0;
	this->input_gamepad_y=0;
}

Mirilla::~Mirilla()
{

}

void Mirilla::establecer_posicion()
{
	int x=this->acc_int_x();
	int y=this->acc_int_y();

	Actor::establecer_posicion(x, y);
	Representable::establecer_posicion(x,y);
}

void Mirilla::inicializar()
{
	this->x=400;
	this->y=300;
}

void Mirilla::ir_a(float p_x, float p_y)
{
	this->x=p_x;
	this->y=p_y;

	this->establecer_posicion();
}

void Mirilla::procesar_turno(float p_delta)
{
	this->establecer_posicion();
}

void Mirilla::guardar_valores_gamepad(float p_ix, float p_iy)
{
	this->input_gamepad_x=p_ix;
	this->input_gamepad_y=p_iy;
}

/*El gamepad funciona en dos pasos: primero se guarda el input del mando y 
luego, una vez que el jugador se ha movido, se combina ese input con la posición
del jugador para colocar la mirilla. Of course, esto es sólo en modo gamepad.*/ 

void Mirilla::procesar_valores_gamepad(float p_centro_x, float p_centro_y)
{
	float punto_x=p_centro_x-(this->w/2)+(64*this->input_gamepad_x);
	float punto_y=p_centro_y-(this->h/2)+(64*this->input_gamepad_y);
	this->ir_a(punto_x, punto_y);
}

void Mirilla::establecer_representacion()
{	
	this->mut_indice_recurso(Recursos::G_TILE_JUGADOR);
	this->establecer_representacion_bitmap_dinamica();
	Representable::establecer_posicion((int)this->x, (int)this->y);
	this->establecer_recorte(160, 32, 16, 16);
}

void Mirilla::establecer_estado_disparo(bool p_valor)
{
	if(p_valor) this->establecer_recorte(176, 32, 16, 16);
	else this->establecer_recorte(160, 32, 16, 16);
}

void Mirilla::mostrar_como_escudo()
{
	this->establecer_recorte(176, 48, 16, 16);
}
