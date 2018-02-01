#include "bonus_normal.h"

//Tiene una animación en seis fases. Durante 2.5 segundos está en un frame y 
//durante 0.4 en cuatro.
const float Bonus_normal::BRILLO=3.0f;

Bonus_normal::Bonus_normal(unsigned int mod_contador)
	:Bonus(), brillo(BRILLO)
{
	this->preparar_actor(mod_contador);
}

Bonus_normal::~Bonus_normal()
{

}

void Bonus_normal::procesar_turno(float p_delta)
{
	Bonus::procesar_turno(p_delta);

	brillo-=p_delta;
	if(brillo <=0) brillo=BRILLO;
	int i=0;

	if(brillo > 0.4f) i=0;
	else if(brillo > 0.3f) i=1;
	else if(brillo > 0.2f) i=2;
	else if(brillo > 0.1f) i=3;
	else i=4;
	
	this->establecer_recorte(i*w,288,23,23);
}

void Bonus_normal::establecer_representacion()
{
	this->mut_indice_recurso(Recursos::G_TILE_JUEGO);
	this->establecer_representacion_bitmap_dinamica();
	Representable::establecer_posicion(this->posicion.x, this->posicion.y);
	this->establecer_recorte(0,288,23,23);
}

void Bonus_normal::establecer_caracteristicas()
{
	this->w=23;
	this->h=23;
}
/*
bool Bonus_normal::en_colision_con(Actor const& otro)
{
	bool resultado=Utilidades_graficas_SDL::rectangulos_superpuestos(this->posicion, otro.acc_posicion());

	if(resultado)
	{
		std::cout<<"HOLA: SOy la colision del bonus"<<std::endl;
	}

	return resultado;
}*/
