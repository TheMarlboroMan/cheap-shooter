#include "decoracion_chatarra.h"

Decoracion_chatarra::Decoracion_chatarra(float p_x, float p_y):Decoracion(), duracion(6)
{	
	float v_x=((rand() % 800)-400);
	float v_y=((rand() % 800)-400);

	this->vector_mov=DLibH::Vector_2d(v_x, v_y);

	this->x=p_x;
	this->y=p_y;

	this->establecer_caracteristicas(); 
	this->establecer_representacion();
}

Decoracion_chatarra::~Decoracion_chatarra()
{

}

void Decoracion_chatarra::procesar_turno(float p_delta)
{
	Decoracion::procesar_turno(p_delta);

	this->duracion-=p_delta;

	if(this->duracion < 0) this->duracion=0;
	this->actualizar_representacion();
}

void Decoracion_chatarra::establecer_caracteristicas()
{
	this->w=8;
	this->h=8;
	this->posicion=DLibH::Herramientas_SDL::nuevo_sdl_rect(this->x, this->y, this->w, this->h);
}

void Decoracion_chatarra::establecer_representacion()
{
	unsigned int x_rec=rand()%4;
	unsigned int y_rec=rand()%8;	

	this->establecer_representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_JUEGO));
	Representable::establecer_posicion(this->x,this->y, this->w, this->h);
	this->establecer_recorte(x_rec * 8, 192+(y_rec * 8), 8, 8);
}

void Decoracion_chatarra::actualizar_representacion()
{
	//A la mitad del recorrido (3, como está puesto de forma implícita
	//en el constructor) se empieza a oscurecer.

	if(this->duracion <= 3)
	{
		int valor_alpha=(255 * (this->duracion / 3));
		Representable::establecer_alpha((int)valor_alpha);
	}
}

bool Decoracion_chatarra::es_finalizado()
{
	return this->duracion <= 0;
}

