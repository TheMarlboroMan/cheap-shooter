#include "obstaculo_gravedad.h"

Obstaculo_gravedad::Obstaculo_gravedad(unsigned int mod_contador):Obstaculo_actividad()
{
	this->preparar_actor(mod_contador);
}

Obstaculo_gravedad::~Obstaculo_gravedad()
{

}

//Donde delta es la cantidad de ms pasados desde la última vez...

void Obstaculo_gravedad::procesar_turno(float p_delta)
{
	//Usamos el normal...
	Obstaculo_actividad::procesar_turno(p_delta);
	p_delta/=No_jugador::divisor_movimiento;


	//Un par de cosas particulares de este...

	if(this->tiempo_turno_actual > 0)
	{
		this->tiempo_turno_actual-=p_delta;

		if(this->tiempo_turno_actual > this->tiempo_turno_mitad) Representable::establecer_recorte(0, 128, 32, 32);
		else if(this->tiempo_turno_actual > 0) Representable::establecer_recorte(32, 128, 32, 32);
		else Representable::establecer_recorte(64, 128, 32, 32);
	}
}

unsigned short int Obstaculo_gravedad::actuar_sonido()
{
	unsigned short int resultado=0;

	if(!this->ha_emitido_sonido)
	{
		this->ha_emitido_sonido=true;
		resultado=Recursos::S_I_PHASER_02;
	}

	return resultado;
}

bool Obstaculo_gravedad::procesar_colision_proyectil(Proyectil * proyectil)
{
	herir();
	this->resistencia-=proyectil->acc_potencia();
	if(this->resistencia < 0) this->resistencia=0;
	return this->resistencia==0;
}

bool Obstaculo_gravedad::es_activo() const 
{
	return 
		this->tiempo_turno_actual <= this->tiempo_turno_mitad 
		&& this->tiempo_turno_actual > 0;
}

void Obstaculo_gravedad::establecer_representacion()
{	
	this->mut_indice_recurso(Recursos::G_TILE_JUEGO);
	this->establecer_representacion_bitmap_dinamica();
	Representable::establecer_posicion(this->posicion.x, this->posicion.y);
	this->establecer_recorte(0,128,32,32);
}

void Obstaculo_gravedad::establecer_caracteristicas()
{
	this->w=27;
	this->h=29;
	this->resistencia=20;
	this->establecer_desp_rep_x(-3);
	this->establecer_desp_rep_x(-1);

	this->tiempo_turno_total=(((rand() % 1)+1)*3)+1;	//Esto es, al menos un segundo y luego entre 0 y 3 segundos más.
	this->tiempo_turno_actual=this->tiempo_turno_total;
	this->tiempo_turno_mitad=this->tiempo_turno_total / 2;	//Sólo lo calculamos una vez...
	this->ha_emitido_sonido=false;
}

//Viene especificado en milisegundos...

void Obstaculo_gravedad::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p)
{
	this->tiempo_turno_total=p.param1 / 1000;

	limitar_valor_minimo_tiempo(this->tiempo_turno_total);

	this->tiempo_turno_actual=this->tiempo_turno_total;
	this->tiempo_turno_mitad=this->tiempo_turno_total / 2;
}
