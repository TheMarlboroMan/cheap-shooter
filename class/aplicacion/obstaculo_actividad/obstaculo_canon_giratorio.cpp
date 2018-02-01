#include "obstaculo_canon_giratorio.h"

const float Obstaculo_canon_giratorio::INTERLUDIO_ENTRE_DISPAROS=0.10f;

Obstaculo_canon_giratorio::Obstaculo_canon_giratorio(unsigned int mod_contador)
	:Obstaculo_actividad(),
	tiempo_descanso(0.0f), tiempo_activo(0.0f), angulo(0.0f), 
	grados_por_segundo(0.0f), cantidad_canones(1),
	grados_entre_canones(0.0f), tiempo_descanso_actual(0.0f), 
	tiempo_activo_actual(0.0f), interludio_entre_disparos(0.0f), activo(false),
	encendido(true)
{
	this->preparar_actor(mod_contador);
}

Obstaculo_canon_giratorio::~Obstaculo_canon_giratorio()
{

}

void Obstaculo_canon_giratorio::procesar_turno(float p_delta)
{
	if(!this->encendido) return;

	//Usamos el normal...
	Obstaculo_actividad::procesar_turno(p_delta);

	//Y ahora las particularidades... Si está activo restamos del tiempo
	//activo y poco más: el controlador haría el resto. Si no está activo
	//restamos del tiempo de descanso. Intercambiamos al llegar a cero.

	p_delta/=divisor_movimiento;

	this->angulo=fmod(this->angulo + (this->grados_por_segundo / 100), 360);

	if(this->activo)
	{
		this->interludio_entre_disparos-=p_delta;

		if(restar_y_restaurar_valor(this->tiempo_activo_actual, this->tiempo_activo, p_delta))
		{
			this->activo=false;
		}
	}
	else
	{
		if(restar_y_restaurar_valor(this->tiempo_descanso_actual, this->tiempo_descanso, p_delta))
		{
			this->activo=true;
		}
	}
}

bool Obstaculo_canon_giratorio::es_activo() const {return this->activo;}

void Obstaculo_canon_giratorio::establecer_representacion()
{
	this->mut_indice_recurso(Recursos::G_TILE_JUEGO);
	this->establecer_representacion_bitmap_dinamica();
	Representable::establecer_posicion(this->posicion.x, this->posicion.y);
	this->establecer_recorte(48,192,32,32);
}

void Obstaculo_canon_giratorio::establecer_caracteristicas()
{
	this->w=32;
	this->h=32;
	this->resistencia=100;
}

void Obstaculo_canon_giratorio::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p)
{
	this->tiempo_activo=(float) p.param1 / 1000;
	this->tiempo_descanso=(float) p.param2 / 1000;
	this->angulo=fmod((float) p.param3, 360);
	this->grados_por_segundo=(float) p.param4 / 100;
	this->cantidad_canones=p.param5 ? p.param5 : 1;
	this->grados_entre_canones=p.param6;
	mut_actua_fuera_de_pantalla(p.param7);
	this->encendido=(bool) p.param8;

	limitar_valor_minimo_tiempo(this->tiempo_activo);
	limitar_valor_minimo_tiempo(this->tiempo_descanso);

	this->tiempo_activo_actual=tiempo_activo;
	this->tiempo_descanso_actual=tiempo_descanso;
	this->interludio_entre_disparos=INTERLUDIO_ENTRE_DISPAROS;
}

Proyectil * Obstaculo_canon_giratorio::disparar(unsigned int canon_actual)
{
	Proyectil * resultado=NULL;

	if(this->interludio_entre_disparos <= 0)
	{
		float canon_angulo=fmod( this->angulo+(this->grados_entre_canones*canon_actual), 360);

		DLibH::Vector_2d v=DLibH::Vector_2d::vector_unidad_para_angulo(canon_angulo);
		v.y=-v.y;	//Ojo: los ejes están invertidos.

		resultado=Proyectil::generar(*this, v, Proyectil::D_NORMAL);
		resultado->establecer_potencia(1);
		resultado->establecer_color(Proyectil::C_AZUL);
		resultado->establecer_velocidad(400.0f);

		resultado->establecer_faccion(Definiciones::F_NO_PREPARADO 
					| Definiciones::F_BONUS
					| Definiciones::F_TRIGGER);

		if(canon_actual==this->cantidad_canones-1)
		{
			this->interludio_entre_disparos=INTERLUDIO_ENTRE_DISPAROS+(-this->interludio_entre_disparos);
		}
	}
	
	return resultado;
}

bool Obstaculo_canon_giratorio::recibir_senal_trigger(const Accion_trigger_datos& p_datos)
{
	unsigned int p_senal=p_datos.acc_tipo();

	switch(p_senal)
	{
		case Accion_trigger::T_ACTIVAR:
			this->encendido=true;
		break;

		case Accion_trigger::T_DESACTIVAR:
			this->encendido=false;
		break;

		case Accion_trigger::T_ACTIVAR_DESACTIVAR:
			this->encendido=!this->encendido;
		break;
	}

	return true;
}
