#include "obstaculo_generador_obstaculos.h"

Obstaculo_generador_obstaculos::Obstaculo_generador_obstaculos():
	Obstaculo_actividad(), tipo_obstaculo(0), toda_dimension(0),
	angulo_inicial(0), angulo_final(0), 
	tiempo_descanso(0.0f), factor_aleatorio(0), multiplicador_vector(1),
	encendido(false)
{
	this->preparar_actor(0);
	mut_actua_fuera_de_pantalla(true);
}

Obstaculo_generador_obstaculos::~Obstaculo_generador_obstaculos()
{

}

void Obstaculo_generador_obstaculos::establecer_caracteristicas()
{
	this->w=32;
	this->h=32;
}

/* Realmente no tiene representación... Tan fácil como eso.*/
void Obstaculo_generador_obstaculos::establecer_representacion()
{

}

void Obstaculo_generador_obstaculos::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p)
{
	this->tipo_obstaculo=p.param1;
	this->toda_dimension=p.param2 ? true : false;
	this->angulo_inicial=p.param3;
	this->angulo_final=p.param4;
	this->tiempo_descanso=(float) p.param5 / 1000;
	this->factor_aleatorio=(float) p.param6 / 1000;
	this->multiplicador_vector=p.param7;
	this->encendido=p.param8 ? true : false;

	limitar_valor_minimo_tiempo(this->tiempo_descanso);

	this->calcular_nuevo_tiempo_descanso();
}

void Obstaculo_generador_obstaculos::calcular_nuevo_tiempo_descanso()
{
	float val=0.0f;
	if(factor_aleatorio) val=rand() % factor_aleatorio;
	this->tiempo_descanso_actual=this->tiempo_descanso + (float) val;
}

bool Obstaculo_generador_obstaculos::recibir_senal_trigger(const Accion_trigger_datos& p_datos)
{
	unsigned int p_senal=p_datos.acc_tipo();

	switch(p_senal)
	{
		case Accion_trigger::T_ACTIVAR:
			this->encendido=true;
			return true;
		break;

		case Accion_trigger::T_DESACTIVAR:
			this->encendido=false;
			return true;
		break;

		case Accion_trigger::T_ACTIVAR_DESACTIVAR:
			this->encendido=!this->encendido;
			return true;
		break;

		default:
			return true;
		break;
	}
}

void Obstaculo_generador_obstaculos::procesar_turno(float p_delta)
{
	if(!this->encendido) return;

	//Usamos el normal...
	Obstaculo_actividad::procesar_turno(p_delta);

	//Y ahora las particularidades... Si está activo restamos del tiempo
	//activo y poco más: el controlador haría el resto. Si no está activo
	//restamos del tiempo de descanso. Intercambiamos al llegar a cero.

	p_delta/=divisor_movimiento;

	if(this->activo)
	{
		//Cuando esté activo generamos un nuevo valor aleatorio y
		//restauramos los tiempos de descanso antes de desactivar.

		this->calcular_nuevo_tiempo_descanso();
		this->activo=false;
	}
	else
	{
		if(restar_y_restaurar_valor(this->tiempo_descanso_actual, this->tiempo_descanso, p_delta))
		{
			this->activo=true;
		}
	}
}

DLibH::Vector_2d Obstaculo_generador_obstaculos::obtener_vector() const
{
	unsigned int angulo=angulo_inicial;
	if(angulo_final!=angulo_inicial) angulo+=(rand() % (angulo_final - angulo_inicial));
	DLibH::Vector_2d resultado=DLibH::Vector_2d::vector_unidad_para_angulo(angulo);
	resultado*=multiplicador_vector;
	return resultado;
}	

void Obstaculo_generador_obstaculos::obtener_posicion(float &x, float &y)
{
	x=this->x;
	y=this->y;	

	if(this->toda_dimension)
	{
		x+=rand() % this->w;
		y+=rand() % this->h;
	}
}
