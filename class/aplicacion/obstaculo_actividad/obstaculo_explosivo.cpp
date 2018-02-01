#include "obstaculo_explosivo.h"

Obstaculo_explosivo::Obstaculo_explosivo(unsigned int mod_contador):
	Obstaculo_actividad(), activo(false)
{
	this->preparar_actor(mod_contador);	
}

Obstaculo_explosivo::~Obstaculo_explosivo()
{

}

void Obstaculo_explosivo::procesar_turno(float p_delta)
{
	//Usamos el normal...
	if(!activo)
	{
		Obstaculo_actividad::procesar_turno(p_delta);
		p_delta/=divisor_movimiento;
		escoger_recorte_representacion();
		puntos_turno_actual-=p_delta;

		if(puntos_turno_actual <= 0)
		{
			puntos_turno_actual=0;
			activo=true;
		}
	}
}


void Obstaculo_explosivo::escoger_recorte_representacion()
{
	int rx=0, ry=0;

	if(this->puntos_turno_actual > puntos_2_3)
	{
		rx=32;
		ry=224;
	}
	else if(this->puntos_turno_actual > puntos_1_3)
	{
		rx=48;
		ry=224;
	}
	else
	{
		rx=32;
		ry=240;
	}
 
	Representable::establecer_recorte(rx,ry,16,16);
}

bool Obstaculo_explosivo::procesar_colision_proyectil(Proyectil * proyectil)
{
	herir();
	this->resistencia-=proyectil->acc_potencia();
	if(this->resistencia < 0) this->resistencia=0;
	return this->resistencia==0;
}

bool Obstaculo_explosivo::es_activo() const
{
	return activo;
}

void Obstaculo_explosivo::establecer_representacion()
{
	Representable::mut_indice_recurso(Recursos::G_TILE_JUEGO);
	Representable::establecer_representacion_bitmap_dinamica();
	Representable::establecer_posicion(this->posicion.x, this->posicion.y);
	Representable::establecer_recorte(32,224,16,16);
}

void Obstaculo_explosivo::establecer_caracteristicas()
{
	this->w=16;
	this->h=16;
	this->resistencia=10;
	this->puntos_turno_total=(((rand() % 1)+1)*2)+1;	//Esto es, al menos un segundo y luego entre 0 y 2 segundos más.
	this->puntos_turno_actual=this->puntos_turno_total;
	this->puntos_1_3=this->puntos_turno_total / 3;
	this->puntos_2_3=this->puntos_1_3*2;
	this->total_disparos=(rand() % 10)+4;
	this->angulo_inicial=(rand() % 360);
	this->salto_angulo=360 / this->total_disparos; 
}

void Obstaculo_explosivo::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p)
{	
	this->puntos_turno_total=p.param1 / 1000;
	this->total_disparos=p.param2 ? p.param2 : 14;
	this->angulo_inicial=fmod(p.param3, 360);
	this->salto_angulo=360 / this->total_disparos; 

	limitar_valor_minimo_tiempo(this->puntos_turno_total);

	this->puntos_turno_actual=this->puntos_turno_total;
	this->puntos_1_3=this->puntos_turno_total / 4;
	this->puntos_2_3=this->puntos_1_3*2;
}

/*Si quedan disparos generará un nuevo ángulo y lo devolverá.
Si no quedan disparos devolverá -1. Si devuelve un ángulo
añadirá además un pequeño tiempo más al contador. Cuando eso 
ocurra, se marcará para borrar.*/

Proyectil * Obstaculo_explosivo::explotar()
{
	if(!this->total_disparos)	
	{
		return NULL;
	}
	else
	{
		this->total_disparos--;

		float angulo=this->angulo_inicial;
		this->angulo_inicial+=this->salto_angulo;	//Paso de comprobar si pasa de 360.		
		this->puntos_turno_actual+=0.10;		//Una décima de segundo, o algo así???

		DLibH::Vector_2d v=DLibH::Vector_2d::vector_unidad_para_angulo(angulo);
		v.y=-v.y;	//Ojo: los ejes están invertidos.

		Proyectil * resultado=Proyectil::generar(*this, v, Proyectil::D_NORMAL);
		resultado->establecer_potencia(1);
		resultado->establecer_color(Proyectil::C_VIOLETA);
		resultado->establecer_velocidad(300.0f);
		resultado->establecer_faccion(Definiciones::F_JUGADOR);

		resultado->establecer_faccion(Definiciones::F_NO_PREPARADO 
					| Definiciones::F_BONUS
					| Definiciones::F_TRIGGER);


		return resultado;
	}
}

void Obstaculo_explosivo::dividir_cantidad(unsigned int divisor)
{
	this->total_disparos/=divisor;
	this->salto_angulo*=divisor;
}

