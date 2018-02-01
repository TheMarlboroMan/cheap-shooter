#include "decoracion_edu.h"

float const Decoracion_edu::FACTOR_CRECIMIENTO=200.0;

/*Al construirla le damos un radio inicial de 6. Este
radio se irá expandiendo según pasen los turnos.*/

Decoracion_edu::Decoracion_edu(float p_x, float p_y, float p_potencia, unsigned short int p_factor)
	:Decoracion(), duracion(p_potencia / 100), duracion_original(duracion), 
	factor(FACTOR_CRECIMIENTO / p_factor), radio_actual(10)
{
	this->x=p_x;
	this->y=p_y;

	//Esta decoración se queda quieta donde se originó.
	this->vector_mov=DLibH::Vector_2d(0, 0);

	this->establecer_caracteristicas();
	this->establecer_representacion();  
}

Decoracion_edu::~Decoracion_edu()
{

}

void Decoracion_edu::actualizar_representacion()
{
	//Con el radio actual hacemos crecer el círculo y cambiamos el color...

	this->color_actual=Decoracion_edu::calcular_color(duracion, duracion_original);

	this->representacion_propia->mut_radio((int)this->radio_actual);
	this->representacion_propia->mut_pixel_color(this->color_actual);
}

/*La decoración es azul. El azul dependerá de la duración que le quede. Si
está a duración máxima es super azul. Si no, tirará más a negro.
*/

Uint32 Decoracion_edu::calcular_color(float duracion, float duracion_original)
{
	int azul=(255 * duracion) / duracion_original;
	if(azul < 20) azul=20;
	return DLibV::Gestor_color::color(32, 32, azul);
}

void Decoracion_edu::establecer_representacion()
{
	//Crear la representación, de puertas para dentro.
	this->representacion_propia=new DLibV::Primitiva_grafica_circunferencia_dinamica(this->x, this->y, (int)this->radio_actual, 1, this->color_actual);
	this->representacion_propia->establecer_alpha(192);

	//Asignación al puntero de la clase "Actor", de puertas para afuera.
	Representable::establecer_representacion(this->representacion_propia);
}

void Decoracion_edu::establecer_caracteristicas()
{
	//Por el tipo de representación que usa creo que podemos ignorar el ancho y alto
	this->w=0;
	this->h=0;
	this->posicion=DLibH::Herramientas_SDL::nuevo_sdl_rect(this->x, this->y, this->w, this->h);
	this->color_actual=Decoracion_edu::calcular_color(duracion, duracion_original);
}

/*A cada turno le quitamos algo de duración y actualizamos los parámetros para
que la representación se vaya expandiendo y cambiando de color.*/

void Decoracion_edu::procesar_turno(float p_delta)
{
	duracion-=p_delta / No_jugador::divisor_movimiento;
	if(duracion < 0) duracion=0;
	radio_actual+=(p_delta*factor);
	actualizar_representacion();
}

bool Decoracion_edu::es_finalizado()
{
	return this->duracion<=0;
}

