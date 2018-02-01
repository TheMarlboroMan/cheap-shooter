#include "trazador_explosion.h"

Trazador_explosion::Trazador_explosion():
	Obstaculo_actividad(), duracion_original(0.0f), duracion(0.0f),
	contador_humo((float) CONTADOR_HUMO / 1000)
{
	this->preparar_actor(0);
}

Trazador_explosion::~Trazador_explosion()
{
}

void Trazador_explosion::establecer_caracteristicas()
{
	this->w=1;
	this->h=1;
}

/* Realmente no tiene representación... Tan fácil como eso.*/

void Trazador_explosion::establecer_representacion()
{

}

bool Trazador_explosion::puede_soltar_humo() const
{
	if(contador_humo <= 0)
	{
		contador_humo=float(CONTADOR_HUMO) / 1000;
		return true;
	}
	else
	{
		return false;
	}
}

void Trazador_explosion::procesar_turno(float p_delta)
{
	float factor=p_delta / No_jugador::divisor_movimiento;

	No_jugador::procesar_turno(p_delta);
	duracion-=factor;
	contador_humo-=factor;

	if(duracion <=0) marcar_para_borrar();
}

/*A partir de una actor se escoge el ángulo de entre los grados puestos como
parámetros.*/

void Trazador_explosion::configurar(const Actor& p_origen, float p_angulo_ini, float p_angulo_fin)
{
	configurar(p_origen.acc_x_centro(), p_origen.acc_y_centro(), p_angulo_ini, p_angulo_fin);
} 

void Trazador_explosion::configurar(float px, float py, float p_angulo_ini, float p_angulo_fin)
{
	vector_mov=DLibH::Vector_2d::vector_unidad_para_angulo(DLibH::Herramientas::obtener_aleatorio_tramo(p_angulo_ini, p_angulo_fin));
	
	float vel=MIN_VELOCIDAD + (rand()%MAX_EXTRA_ALEATORIO_VELOCIDAD);
	vector_mov*=vel;

	ir_a(px, py);

	duracion_original=(float) ((MIN_DURACION + (rand()%MAX_EXTRA_ALEATORIO_DURACION) ) / 1000.0f);
	duracion=duracion_original;	
} 

/*
void Trazador_explosion::configurar(const Actor& p_origen, float p_angulo_ini, float p_angulo_fin)
{
	vector_mov=DLibH::Vector_2d::vector_unidad_para_angulo(DLibH::Herramientas::obtener_aleatorio_tramo(p_angulo_ini, p_angulo_fin));
	
	float vel=MIN_VELOCIDAD + (rand()%MAX_EXTRA_ALEATORIO_VELOCIDAD);
	vector_mov*=vel;

	ir_a(p_origen.acc_x_centro(), p_origen.acc_y_centro());

	duracion_original=(float) ((MIN_DURACION + (rand()%MAX_EXTRA_ALEATORIO_DURACION) ) / 1000.0f);
	duracion=duracion_original;	
} 
*/

/*El resultado está entre 0 y 3, en función de la relación entre la duración
original y la actual.*/

unsigned short int Trazador_explosion::obtener_parte_humo() const
{
	short int parte=ceil( (duracion * PARTES_HUMO) / duracion_original);
	return abs(PARTES_HUMO - parte);
}
