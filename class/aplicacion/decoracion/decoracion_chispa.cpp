#include "decoracion_chispa.h"

Decoracion_chispa::Decoracion_chispa(float p_x, float p_y, short int p_gama):Decoracion()
{	
	float v_x=((rand() % 800)-400);
	float v_y=((rand() % 800)-400);
	gama=p_gama;

	this->vector_mov=DLibH::Vector_2d(v_x, v_y);

	this->duracion=(( (rand()%100)+1) / 100)+0.2;


	this->x=p_x;
	this->y=p_y;

	this->establecer_caracteristicas(); 
	this->establecer_representacion();
}

Decoracion_chispa::~Decoracion_chispa()
{

}

void Decoracion_chispa::establecer_vector(const DLibH::Vector_2d& v)
{
	this->vector_mov=v;
}

void Decoracion_chispa::procesar_turno(float p_delta)
{
	Decoracion::procesar_turno(p_delta);

	this->duracion-=p_delta / No_jugador::divisor_movimiento;
	if(this->duracion < 0) this->duracion=0;
}

void Decoracion_chispa::establecer_caracteristicas()
{
	this->w=1;
	this->h=1;
	this->posicion=DLibH::Herramientas_SDL::nuevo_sdl_rect(this->x, this->y, this->w, this->h);
}

void Decoracion_chispa::establecer_representacion()
{
	Representable::mut_indice_recurso(Recursos::G_TILE_JUEGO);
	Representable::establecer_representacion_bitmap_dinamica();

	unsigned int x_rec=rand()%8;
	unsigned int y_rec=0;

	switch(gama)
	{
		case TODAS: y_rec=rand()%MAX_GAMA; break;
		default: y_rec=gama; break;
	}

	Representable::establecer_recorte(32+x_rec,192+y_rec,this->w,this->h);
	Representable::establecer_posicion(this->x,this->y, this->w, this->h);	
}

bool Decoracion_chispa::es_finalizado()
{
	return this->duracion==0;
}
