#include "obstaculo_disparador.h"

Obstaculo_disparador::Obstaculo_disparador(unsigned int mod_contador):Obstaculo_actividad(),
	activo(false), activado_modo_actuacion(false), v_actuacion(0,0), 
	modo_actuacion(M_AUTO), ruido_punteria(0)
{
	this->preparar_actor(mod_contador);
}

Obstaculo_disparador::~Obstaculo_disparador()
{

}

/*Cuando el actor entra en pantalla por primera vez con un modo de actuación
se llama a este método. Evita que, en caso de tener vectores de movimiento
inconvenientes (por ejemplo, hacia arriba) se elimine cuando no está en pantalla
según lo evalúa "No_jugador", puesto que el vector empezaría a 0.*/

void Obstaculo_disparador::activar_modo_actuacion()
{
	activado_modo_actuacion=true;
	vector_mov=v_actuacion;
}

void Obstaculo_disparador::procesar_turno(float p_delta)
{
	//Usamos el normal...
	Obstaculo_actividad::procesar_turno(p_delta);

	p_delta/=divisor_movimiento;

	//Un par de cosas particulares de este...

	if(restar_y_restaurar_valor(this->puntos_turno_actual, this->puntos_turno_total, p_delta))
	{
		activo=true;
	}

	if(modo_actuacion && !activado_modo_actuacion && es_en_pantalla())
	{
		activar_modo_actuacion();
	}
}

bool Obstaculo_disparador::procesar_colision_proyectil(Proyectil * proyectil)
{
	herir();

	this->resistencia-=proyectil->acc_potencia();
	if(this->resistencia < 0) this->resistencia=0;
	return this->resistencia==0;
}

bool Obstaculo_disparador::es_activo() const 
{
	if(this->activo)
	{
		this->activo=false;
		return true;
	}
	else return false;
}

void Obstaculo_disparador::establecer_representacion()
{
	this->mut_indice_recurso(Recursos::G_TILE_JUEGO);
	this->establecer_representacion_bitmap_dinamica();
	Representable::establecer_posicion(this->posicion.x, this->posicion.y);
	this->establecer_recorte(64,224,32,32);
}

void Obstaculo_disparador::establecer_caracteristicas()
{
	this->w=32;
	this->h=32;
	this->resistencia=20;
	this->puntos_turno_total=(((rand() % 1)+1)*2)+1;	//Esto es, al menos un segundo y luego entre 0 y 2 segundos más.
	this->puntos_turno_actual=this->puntos_turno_total;
}

void Obstaculo_disparador::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p)
{
	this->puntos_turno_total=(float)p.param1 / 1000;
	limitar_valor_minimo_tiempo(this->puntos_turno_total);

	this->puntos_turno_actual=this->puntos_turno_total;

	if(p.param2) resistencia=p.param2;
	ruido_punteria=p.param8;

	switch(p.param3)
	{
		case M_VECTORES_MANUAL:
			modo_actuacion=M_VECTORES_MANUAL;
			v_actuacion.x=p.param4;
			v_actuacion.y=p.param5;
			if(p.param6) v_actuacion.x*=-1;
			if(p.param7) v_actuacion.y*=-1;
		break;

		case M_AUTO:
		default:
			modo_actuacion=M_AUTO;
		break;
	}
}

Proyectil * Obstaculo_disparador::disparar(const Actor& p_blanco)
{
	Proyectil * resultado=NULL;

	resultado=Proyectil::generar(*this, p_blanco, Proyectil::D_NORMAL, 0, 0, this->ruido_punteria);
	resultado->establecer_potencia(1);
	resultado->establecer_color(Proyectil::C_VERDE);
	resultado->establecer_velocidad(250.0f);
	resultado->establecer_faccion(Definiciones::F_JUGADOR);

	resultado->establecer_faccion(Definiciones::F_NO_PREPARADO 
					| Definiciones::F_BONUS
					| Definiciones::F_TRIGGER);

	return resultado;
}
