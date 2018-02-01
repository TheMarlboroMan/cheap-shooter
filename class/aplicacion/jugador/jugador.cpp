#include "jugador.h"

const float Jugador::GASTO_TIEMPO_POR_SEGUNDO=100.0f;
const float Jugador::CANTIDAD_MAX_TIEMPO=100.0f;
const float Jugador::MAX_ABSOLUTO_TIEMPO=200.0f;
const float Jugador::RECUPERACION_TIEMPO_SEGUNDO=25.0f;
const float Jugador::COOLOFF_TIEMPO=0.5f;
const float Jugador::FACTOR_RECUPERACION_VELOCIDAD_TIEMPO=5.f;
const float Jugador::FACTOR_ENLENTECIMIENTO_TIEMPO=5.f;
const float Jugador::CANTIDAD_CARGA_POR_BONUS_TIEMPO=10.f;

const float Jugador::MAX_POTENCIA_ACUMULABLE=100.f;
const float Jugador::CARGA_CANON_POR_SEGUNDO=100.f;
const float Jugador::CANTIDAD_MAX_CANON=100.0f;
const float Jugador::MAX_ABSOLUTO_CANON=200.0f;
const float Jugador::RECUPERACION_CANON_SEGUNDO=25.0f;
//const float Jugador::COOLOFF_CANON=0.4f;
const float Jugador::COOLOFF_CANON=0.1f;
const float Jugador::CANTIDAD_CARGA_POR_BONUS_CANON=25.0f;

const float Jugador::CANTIDAD_MAX_EDU=100.0f;
const float Jugador::RECUPERACION_EDU_SEGUNDO=10.0f;
const float Jugador::COOLOFF_EDU=1.0f;

const float Jugador::VECTOR_X_CAMBIO_ANIM_1=120.0f;
const float Jugador::VECTOR_X_CAMBIO_ANIM_2=80;
const float Jugador::VECTOR_X_CAMBIO_ANIM_N1=-80.0f;
const float Jugador::VECTOR_X_CAMBIO_ANIM_N2=-120.0f;

Jugador::Jugador()
	:Actor_jugable(), 
	datos_tiempo(CANTIDAD_MAX_TIEMPO, RECUPERACION_TIEMPO_SEGUNDO, COOLOFF_TIEMPO, false, false),
	datos_canon(CANTIDAD_MAX_CANON, RECUPERACION_CANON_SEGUNDO, COOLOFF_CANON),
	datos_edu(CANTIDAD_MAX_EDU, RECUPERACION_EDU_SEGUNDO, COOLOFF_EDU, true)
{
	this->inicializar();
	this->restaurar_valores_originales();

	this->w=32;
	this->h=32;

	this->establecer_representacion();
	this->posicion=DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, this->w, this->h);
	this->establecer_posicion();

	this->mapa_colision=Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_JUGADOR);
}

Jugador::~Jugador()
{

}

/*Establece los valores básicos... Al principio los teníamos en una lista de inicialización
pero se va de largo*/

void Jugador::inicializar()
{
	this->x=400;
	this->y=500;

	this->vidas=VIDAS_INICIO;
	this->invulnerable=0;
	this->max_invulnerable=180;	//Esto es lo mismo decir que el max es 3 y el factor de gasto es 0.1, o algo así.
	this->factor_gasto_invulnerabilidad=60.f;
	invulnerabilidad_infinita=false;
	this->divisor_tiempo=1;
	this->intenta_parar_tiempo=false;
	estado_divisor_tiempo=D_NADA;
	this->artefacto_actual=Definiciones::A_CANON;
	cargando_canon=false;
	carga_canon_acumulada=0.0f;
	potencia_siguiente_disparo=0;
	canal_sonido_tiempo=-1;
}

void Jugador::restaurar_valores_originales()
{
	datos_tiempo.restaurar(CANTIDAD_MAX_TIEMPO, RECUPERACION_TIEMPO_SEGUNDO, COOLOFF_TIEMPO);
	datos_edu.restaurar(CANTIDAD_MAX_EDU, RECUPERACION_EDU_SEGUNDO, COOLOFF_EDU);
	datos_canon.restaurar(CANTIDAD_MAX_CANON, RECUPERACION_CANON_SEGUNDO, COOLOFF_CANON);
}

void Jugador::establecer_representacion()
{
	this->mut_indice_recurso(Recursos::G_TILE_JUGADOR);
	this->establecer_representacion_bitmap_dinamica();
	Representable::establecer_posicion(400,500);
	this->actualizar_representacion();
}

void Jugador::actualizar_representacion()
{
	int y=0, x=0; 
	Uint8 alpha=SDL_ALPHA_OPAQUE;

	if(this->invulnerable) 
	{
		y=32; alpha=192;
	}

	if(vector_mov.x < VECTOR_X_CAMBIO_ANIM_N2) x=128;
	else if(vector_mov.x < VECTOR_X_CAMBIO_ANIM_N1) x=96;
	else if(vector_mov.x > VECTOR_X_CAMBIO_ANIM_1) x=64;
	else if(vector_mov.x > VECTOR_X_CAMBIO_ANIM_2) x=32;

	Representable::establecer_recorte(x, y, 32, 32);
	Representable::establecer_alpha(alpha);
}

void Jugador::hacer_invulnerable() 
{
	this->invulnerable=this->max_invulnerable;
	this->actualizar_representacion();
}	

/* Recuperar el propio divisor del tiempo cuando no está activado hasta que tenga
el valor de 1. Idem con la energía... */

void Jugador::procesar_recuperacion_tiempo(float p_delta)
{
	if(!this->intenta_parar_tiempo)
	{
		datos_tiempo.turno(p_delta);
	}
}

void Jugador::procesar_recuperacion_canon(float p_delta)
{
	datos_canon.turno(p_delta);
}

bool Jugador::usar_canon()
{
	cargando_canon=false;
	potencia_siguiente_disparo=carga_canon_acumulada;	//Metemos este valor en caché, para cuando nos venga bien que el controlador lo solicite.
	datos_canon.usar(carga_canon_acumulada);
	carga_canon_acumulada=0.0f;
	return true;
}

void Jugador::iniciar_carga_canon()
{
	cargando_canon=true;
	carga_canon_acumulada=0.0f;	
}

void Jugador::forzar_fin_carga_canon()
{
	cargando_canon=false;
	carga_canon_acumulada=0.0f;
}

void Jugador::procesar_recuperacion_edu(float p_delta)
{
	datos_edu.turno(p_delta);
}

/*Esto "reacelera el tiempo"...*/

void Jugador::procesar_recuperacion_divisor_tiempo(float p_delta)
{
	float original=divisor_tiempo;
	if(original < 1) original=1; //Ojo con esto, para no dar falsos positivos cuando se recupere!.

	if(divisor_tiempo > 1) divisor_tiempo-=FACTOR_RECUPERACION_VELOCIDAD_TIEMPO*p_delta;
	else divisor_tiempo=1;

	evaluar_estado_divisor_tiempo(original);
}

/*Procesar el divisor de tiempo si está activo... Si no es posible, vuelve a acelerar el tiempo.*/

bool Jugador::procesar_divisor_tiempo(float p_delta)
{
	bool resultado=false;

	if(datos_tiempo.es_usable())
	{
		resultado=datos_tiempo.usar(GASTO_TIEMPO_POR_SEGUNDO*p_delta);
		if(resultado) 
		{
			float original=divisor_tiempo;
			divisor_tiempo+=FACTOR_ENLENTECIMIENTO_TIEMPO*p_delta;
			evaluar_estado_divisor_tiempo(original);
		}
	}

	return resultado;
}

/*Esto evalua el estado del divisor de tiempo para poder generar los sonidos
de acelerar y decelerar...*/

void Jugador::evaluar_estado_divisor_tiempo(float t_original)
{
	if(t_original==divisor_tiempo) estado_divisor_tiempo=D_NADA;
	else if(t_original > divisor_tiempo)
	{
		//Si estamos en "inicio de acelerar" el siguiente es "acelerando normal".
		//Si estamos en "acelerando normal" se queda tal cual.
		//Cualquier otra cosa te pone en inicio de acelerar

		if(estado_divisor_tiempo==D_INI_ACELERAR || estado_divisor_tiempo==D_ACELERAR) estado_divisor_tiempo=D_ACELERAR;
		else estado_divisor_tiempo=D_INI_ACELERAR;
	}
	else	
	{
		if(estado_divisor_tiempo==D_INI_ENLENTECER || estado_divisor_tiempo==D_ENLENTECER) estado_divisor_tiempo=D_ENLENTECER;
		else estado_divisor_tiempo=D_INI_ENLENTECER;
	}
}

void Jugador::procesar_invulnerabilidad(float p_delta)
{
	if(this->invulnerable > 0 && !invulnerabilidad_infinita) 
	{
		this->invulnerable-=this->factor_gasto_invulnerabilidad*p_delta;

		if(this->invulnerable<=0)
		{
			this->invulnerable=0;
			this->actualizar_representacion();
		}
	}
}

void Jugador::reiniciar_senal_sonido()
{
	this->senales_sonido=0;
}

void Jugador::quitar_vida(bool p_otorgar) 
{
	this->vidas--;
	if(p_otorgar) this->hacer_invulnerable();
	this->restaurar_valores_originales();
}

void Jugador::recibir_senal_tiempo(unsigned short int senal)
{		
	switch(senal)
	{
		case 1:	//Tecla down...
			if(this->datos_tiempo.es_usable()) 
			{
				this->intenta_parar_tiempo=true;
			}
			else 
			{
				intenta_parar_tiempo=false;
			}
		break;

		case 2: //Tecla levantada...
			datos_tiempo.activar_cooloff();
			this->intenta_parar_tiempo=false;			
		break;

		default: break;
	}
}

bool Jugador::recibir_senal_artefacto_down()
{
	switch(this->artefacto_actual)
	{
		case Definiciones::A_CANON:
			return datos_canon.es_usable();
		break;

		case Definiciones::A_TELETRANSPORTADOR:
			return true;
		break;

		case Definiciones::A_EMISOR_DENSIDAD_UNIFICADA:
			return datos_edu.es_usable();
		break;

		case Definiciones::A_ESCUDO:
			return true;
		break;

		default:
			return false;
		break;
	}
}

/*Básicamente se usa sólo para "he dejado de teletransportarme".*/

bool Jugador::recibir_senal_artefacto_up()
{
	switch(this->artefacto_actual)
	{
		case Definiciones::A_TELETRANSPORTADOR:
			return true;
		break;

		case Definiciones::A_ESCUDO:
			return true;
		break;

		case Definiciones::A_CANON:
			return cargando_canon && usar_canon();
		break;

		default:
			return false;
		break;
	}
}

bool Jugador::recibir_senal_artefacto_pulsado()
{
	//Puro código legacy.
	return false;
}

void Jugador::procesar_turno(float p_delta)
{
	this->procesar_invulnerabilidad(p_delta);
	this->aplicar_vector(p_delta);	
	this->controlar_posiciones();

	if(this->intenta_parar_tiempo)	//Si tiene pulsada la tecla se intenta procesar aunque igual no tiene éxito.
	{
		if(!this->procesar_divisor_tiempo(p_delta))
		{
			this->procesar_recuperacion_divisor_tiempo(p_delta);
			this->parando_tiempo=false;
		}
		else
		{
			this->parando_tiempo=true;
		}
	}
	else
	{
		this->parando_tiempo=false;
		this->procesar_recuperacion_tiempo(p_delta);
		this->procesar_recuperacion_divisor_tiempo(p_delta);
	}

	//Control del sonido...

	switch(estado_divisor_tiempo)
	{
		case D_INI_ENLENTECER: senales_sonido+=S_INICIO_PARAR_TIEMPO; break;
		case D_INI_ACELERAR: senales_sonido+=S_FIN_PARAR_TIEMPO; break;
	}

	//if(datos_canon.es_recien_finalizado_cooloff()) this->senales_sonido+=S_RECARGA;
	if(datos_canon.es_recien_usado()) this->senales_sonido+=S_DISPARO;

	if(this->cargando_canon)
	{
		carga_canon_acumulada+=CARGA_CANON_POR_SEGUNDO*p_delta;
		float cantidad_canon=datos_canon.acc_cantidad();
		float cantidad_max = MAX_POTENCIA_ACUMULABLE > cantidad_canon ? cantidad_canon : MAX_POTENCIA_ACUMULABLE;

		if(carga_canon_acumulada > cantidad_max) carga_canon_acumulada=cantidad_max;
	}

	this->procesar_recuperacion_edu(p_delta);		
	this->procesar_recuperacion_canon(p_delta);
	this->procesar_retraccion_vectores(p_delta);	//La retracción sólo se producirá en algunos casos. Ver clase base.

	this->actualizar_representacion();
}

bool Jugador::aumentar_tiempo()
{
	if(datos_tiempo.acc_cantidad() < MAX_ABSOLUTO_TIEMPO)
	{
		datos_tiempo.aumentar_max_cantidad(CANTIDAD_CARGA_POR_BONUS_TIEMPO);
		return true;
	}
	else return false;
}

bool Jugador::aumentar_disparo()
{
	if(datos_canon.acc_max_cantidad() < MAX_ABSOLUTO_CANON)
	{
		datos_canon.aumentar_max_cantidad(CANTIDAD_CARGA_POR_BONUS_CANON);
		return true;
	}
	else return false;
}

void Jugador::aumentar_vida()
{
	this->vidas++;
}

float Jugador::usar_edu()
{
	float resultado=datos_edu.usar(datos_edu.acc_cantidad());
	return resultado;
}

DLibH::Punto_2d<int> Jugador::obtener_punto_propulsor(unsigned int p_param)
{
	DLibH::Punto_2d<int> resultado=DLibH::Punto_2d<int>(x, y+32);
	switch(p_param)
	{
		case 0:	resultado.x+=6; break;
		default: resultado.x+=26; break;
	}
	
	return resultado;
}

bool Jugador::puede_generar_chispas_propulsor() const
{
	return estado_aceleracion==EA_ACELERANDO;
}
