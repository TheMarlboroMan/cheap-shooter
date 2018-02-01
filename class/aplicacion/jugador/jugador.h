#ifndef JUGADOR_H
#define JUGADOR_H

#include "../actor_jugable/actor_jugable.h"
#include "../definiciones/definiciones.h"
#include "../../motor_sdl/herramientas/punto_2d/punto_2d.h"

/*Esta clase se usa para controlar el funcionamiento interno 
de los artefactos del jugador. El escudo va por otro lado, pero tampoco me
preocupa ahora mismo...*/

class Medidor_artefacto
{
	private:

	//Estos son "patrones".
	float max_cantidad;	//Por defecto, cantidad máxima que se puede tener de la habilidad.
	float factor_recuperacion_cantidad;	//Cantidad que se recupera en un segundo.
	float cantidad_cooloff_tras_uso;	//Tras usarlo se activa un cooloff de X ms.

	//Estos son configuraciones..
	bool recarga_con_cooloff;
	bool cooloff_automatico;

	//Y estos son valores reales.

	float cantidad;
	float cooloff;

	//Estos son auxiliares.
	
	bool recien_finalizada_carga_completa;
	bool recien_finalizado_cooloff;
	bool recien_usado;

	public:

	Medidor_artefacto(float p_mc, float p_frc, float p_cctu, bool p_rc=false, bool p_ca=true)
		:max_cantidad(p_mc), factor_recuperacion_cantidad(p_frc), 
		cantidad_cooloff_tras_uso(p_cctu), recarga_con_cooloff(p_rc),
		cooloff_automatico(p_ca), cantidad(max_cantidad),
		cooloff(0.0f), recien_finalizada_carga_completa(false),
		recien_finalizado_cooloff(false), recien_usado(false)
	{}

	bool es_recien_finalizada_carga_completa() const {return recien_finalizada_carga_completa;}
	bool es_recien_finalizado_cooloff() const {return recien_finalizado_cooloff;}
	bool es_recien_usado() const {return recien_usado;}
	bool es_usable() const {return !cooloff && cantidad;}
	float acc_cantidad() const {return cantidad;}
	float acc_max_cantidad() const {return max_cantidad;}

	void restaurar(float p_mc, float p_frc, float p_cctu)
	{
		max_cantidad=p_mc;
		factor_recuperacion_cantidad=p_frc;
		cantidad_cooloff_tras_uso=p_cctu;
		cantidad=max_cantidad;
		cooloff=0;
		recien_finalizada_carga_completa=false;
		recien_finalizado_cooloff=false;
		recien_usado=false;
	}

	void aumentar_max_cantidad(float p_cantidad, bool p_recarga_completa=true)
	{
		max_cantidad+=p_cantidad;

		if(p_recarga_completa)
		{
			cantidad=max_cantidad;
			cooloff=0;
		}
	}

	void turno(float p_delta)
	{
		recien_finalizada_carga_completa=false;
		recien_finalizado_cooloff=false;
		recien_usado=false;

		//Restar cooloff.
		if(cooloff)
		{
			cooloff-=p_delta;
			if(cooloff <= 0) 
			{
				cooloff=0;
				recien_finalizado_cooloff=true;
			}
		}
			
		if(cantidad < max_cantidad)
		{
			if(!cooloff || recarga_con_cooloff)
			{
				cantidad+=factor_recuperacion_cantidad * p_delta;
				if(cantidad >= max_cantidad) 
				{
					cantidad=max_cantidad;
					recien_finalizada_carga_completa=false;
				}
			}
		}
	}

	float usar(float p_gasto)
	{
		float resultado=0;
		
		if(!cooloff)
		{
			recien_usado=true;

			if(p_gasto > cantidad)
			{
				resultado=cantidad;
				cantidad=0;
			}
			else
			{
				cantidad-=p_gasto;
				resultado=p_gasto;			
			}

			if(resultado && cooloff_automatico) activar_cooloff();
		}
		
		return resultado;
	}

	void activar_cooloff()
	{
		cooloff=cantidad_cooloff_tras_uso;
	}
};


class Jugador:public Actor_jugable
{
	private:

	static const int VIDAS_INICIO=3;

	static const float VECTOR_X_CAMBIO_ANIM_1;
	static const float VECTOR_X_CAMBIO_ANIM_2;
	static const float VECTOR_X_CAMBIO_ANIM_N1;
	static const float VECTOR_X_CAMBIO_ANIM_N2;

	static const float GASTO_TIEMPO_POR_SEGUNDO;
	static const float CANTIDAD_MAX_TIEMPO;
	static const float MAX_ABSOLUTO_TIEMPO;
	static const float RECUPERACION_TIEMPO_SEGUNDO;
	static const float CANTIDAD_CARGA_POR_BONUS_TIEMPO;
	static const float COOLOFF_TIEMPO;
	static const float FACTOR_RECUPERACION_VELOCIDAD_TIEMPO;
	static const float FACTOR_ENLENTECIMIENTO_TIEMPO;

	static const float CARGA_CANON_POR_SEGUNDO;
	static const float CANTIDAD_MAX_CANON;
	static const float MAX_ABSOLUTO_CANON;
	static const float RECUPERACION_CANON_SEGUNDO;
	static const float COOLOFF_CANON;
	static const float CANTIDAD_CARGA_POR_BONUS_CANON;
	static const float MAX_POTENCIA_ACUMULABLE;
	static const short int POTENCIA_BASE_CANON=0; //10;

	static const float CANTIDAD_MAX_EDU;
	static const float RECUPERACION_EDU_SEGUNDO;
	static const float COOLOFF_EDU;

	/*Todas estas están relacionadas con la habilidad de parar el tiempo*/
	
	Medidor_artefacto datos_tiempo;
	bool intenta_parar_tiempo;			//Bandera de control que nos dice si intenta parar el tiempo y es posible.
	bool parando_tiempo;				//Se usa para saber si lo está parando o no.
	float divisor_tiempo;				//Divisor del tiempo aplicado actualmente.
	
	/*Estas son para la habilidad de disparo*/
	Medidor_artefacto datos_canon;
	bool cargando_canon;
	float carga_canon_acumulada;
	unsigned int potencia_siguiente_disparo; 	//Se acumula y guarda aquí porque el acumulado volverá a ser cero.

	/*Estas son para el emisor de densidad Unificada*/
	Medidor_artefacto datos_edu;
	bool intenta_edu;

	/*Vidas e invulnerabilidad*/
	unsigned int vidas;				//Vidas actuales.
	float invulnerable;				//Invulnerabilidad actual.
	unsigned int max_invulnerable;			//Tiempo máximo de invulnerabilidad.
	float factor_gasto_invulnerabilidad;		//Cantidad de "invulnerable" que se gasta en un segundo.
	bool invulnerabilidad_infinita;			//Esto es para debugear, más que nada.

	/*Otros*/
	unsigned short int artefacto_actual;		//Artefacto actual en uso.
	unsigned short int senales_sonido;		//8 bits para sonidos diferentes que puede producir.

	//Métodos privados varios.

	void hacer_invulnerable();
	void procesar_invulnerabilidad(float);
	void restaurar_valores_originales();
	void inicializar();
	void establecer_representacion();
	void actualizar_representacion();

	//Métodos privados para artefactos.
	void procesar_recuperacion_edu(float);	
	void procesar_recuperacion_canon(float);
	bool usar_canon();
	void procesar_recuperacion_tiempo(float);
	void procesar_recuperacion_divisor_tiempo(float);
	bool procesar_divisor_tiempo(float);
	void evaluar_estado_divisor_tiempo(float);

	//Todos estos son los factores que se recuperarán por segundo.
	float factor_retraccion_vectores(){return 600.f;}	//Estos son métodos por la clase padre... Podrían ser igualmente normales y corrientes.
	float maximo_vector_desplazamiento() {return 400.f;}
	float factor_multiplicador_input() {return 400.f;}

	enum DIRECCION_DIVISOR_TIEMPO{
		D_NADA=0, //Sin diferencia entre divisor anterior y actual,
		D_INI_ENLENTECER, //Primera bajada
		D_ENLENTECER, //Bajada constante
		D_INI_ACELERAR, //Primera subida
		D_ACELERAR	//Subida constante.
	};

	unsigned int estado_divisor_tiempo;
	int canal_sonido_tiempo;

	public:

	enum SENALES_SONIDO{S_DISPARO=1, S_RECARGA=2, S_INICIO_PARAR_TIEMPO=4, S_FIN_PARAR_TIEMPO=8};

	Jugador();
	virtual ~Jugador();

	//Métodos para el cañón...
	float acc_carga_maxima_canon() const {return datos_canon.acc_max_cantidad();}
	float acc_carga_canon() const {return datos_canon.acc_cantidad();}
	float acc_carga_canon_acumulada() const {return carga_canon_acumulada;}
	bool es_canon_usable() const {return datos_canon.es_usable();}
	void iniciar_carga_canon();
	void forzar_fin_carga_canon();
	bool aumentar_disparo();	
	unsigned int acc_potencia_disparo() const {return POTENCIA_BASE_CANON+potencia_siguiente_disparo;}

	//Métodos para el tiempo...
	float acc_divisor_tiempo() const {return this->divisor_tiempo;}
	float acc_carga_maxima_parar_tiempo() const {return datos_tiempo.acc_max_cantidad();}
	float acc_carga_parar_tiempo() const {return datos_tiempo.acc_cantidad();}
	bool es_parar_tiempo_usable() const {return datos_tiempo.es_usable();}	
	bool es_parando_tiempo() const {return this->parando_tiempo;}

	//Métodos para EDU.
	unsigned int acc_max_cantidad_edu() const 	{return datos_edu.acc_max_cantidad();}
	float acc_cantidad_edu() const 			{return datos_edu.acc_cantidad();}
	float es_usable_edu() const 			{return datos_edu.es_usable();}
	float usar_edu();

	//Métodos de la interfaz pública...
	unsigned int acc_vidas() const {return this->vidas;}
	unsigned int es_invulnerable() const {return this->invulnerable;}	//Nótese que pone "es" pero devuelve int.
	bool es_jugable() const {return this->vidas;}
	unsigned short int acc_senales_sonido() const {return this->senales_sonido;}
	unsigned short int acc_artefacto_actual() const {return this->artefacto_actual;}


//	void ciclar_artefacto(int, unsigned char);
//	bool es_artefacto_permitido(unsigned char);

	void establecer_artefacto_actual(unsigned short int a) {artefacto_actual=a;}
	void reiniciar_senal_sonido();
	void quitar_vida(bool=true);
	void recibir_senal_tiempo(unsigned short int);
	bool recibir_senal_artefacto_down();
	bool recibir_senal_artefacto_up();
	bool recibir_senal_artefacto_pulsado();
	void procesar_turno(float=0);
	bool aumentar_tiempo();
	void aumentar_vida();
	void intercambiar_invulnerabilidad_infinita() //Prácticamente esto es para debug.
	{
		invulnerabilidad_infinita=!invulnerabilidad_infinita;
		if(invulnerabilidad_infinita) hacer_invulnerable();
	}

	void cuentame() {std::cout<<"SOY JUGADOR"<<std::endl;}
	DLibH::Punto_2d<int> obtener_punto_propulsor(unsigned int);
	void dime_donde(){std::cout<<this->x<<" "<<this->y<<" / "<<this->vector_mov.x<<" "<<this->vector_mov.y<<std::endl;}
	bool puede_generar_chispas_propulsor() const;

	void establecer_canal_sonido_tiempo(int p_c) {canal_sonido_tiempo=p_c;}
	int obtener_canal_sonido_tiempo() const {return canal_sonido_tiempo;}
};

#endif
