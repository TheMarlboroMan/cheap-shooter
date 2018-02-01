#ifndef BOSS_H
#define BOSS_H

#include "../obstaculo_actividad/obstaculo_actividad.h"
#include "../trigger/trigger.h"
#include "../proyectil/proyectil.h"
#include "../visitante/visitante.h"

class Boss: public Obstaculo_actividad, public Trigger
{
	////////////////////////////////////////////////////////////////////////
	//Estructuras internas...

	public:

		/*Esto está aquí para no exponer la decoración por completo a
		esta otra clase. Se intercambiarán estas estructuras con el 
		controlador y ya está.*/
	
	struct Estructura_decoracion_boss
	{
		enum tipos{
			ED_CHATARRA, //Generará una explosión de chatarra.
			ED_EXPLOSION, //Generará una sóla explosión.
			ED_HUMO	//Generará un solo bloque de humo.
		};

		float x;
		float y;
		unsigned int tipo;
		float aux;
		DLibH::Vector_2d vector;
		float aux_desp_x;	//Para generar chatarra con la explosión, básicamente.
		float aux_desp_y;

		Estructura_decoracion_boss(float px, float py, unsigned int t, float a=0, float vx=0, float vy=0, float pdx=0, float pdy=0):
			x(px), y(py), tipo(t), aux(a), vector(vx, vy), aux_desp_x(pdx), aux_desp_y(pdy) {}
	};

	protected:

	struct Punto_debil
	{
		static const unsigned int T_HERIDO=100;

		int resistencia;
		float tiempo_herido;
		bool vulnerable;
		bool destruido;
		SDL_Rect caja;

		Punto_debil(int r, bool v): resistencia(r), tiempo_herido(0.0), vulnerable(v), destruido(false) {}

		bool alcanzar(int pot)
		{	
			tiempo_herido=(float)T_HERIDO / 1000;
			resistencia-=pot;
			if(resistencia <= 0) 
			{
				resistencia=0;
				destruido=true;
			}

			return destruido;
		}

		virtual void turno(float p_delta)
		{
			if(tiempo_herido)
			{
				tiempo_herido-=p_delta / No_jugador::divisor_movimiento;
				if(tiempo_herido < 0.0f) tiempo_herido=0.0f;
			}
		}

		virtual int obtener_x_representacion()=0;
		virtual int obtener_y_representacion()=0;
	};

	////////////////////////////////////////////////////////////////////////
	//Propiedades...

	private:

	std::vector<Proyectil *> disparos_efectuados; //Los proyectiles generados se meterán aquí, y el controlador los recuperará.
	std::vector<Estructura_decoracion_boss> decoraciones_generadas; //Idem, pero con explosiones y cosas así.
	mutable bool activado;	//Se cambiaría normalmente al preguntar si está activo, que es const. Ergo, mutable.

	////////////////////////////////////////////////////////////////////////
	//Métodos para implementar:

	protected:

	virtual void comprobar_destruccion()=0;
	virtual void controlar_estado(float)=0;
	virtual void controlar_representacion()=0;
	virtual unsigned short int obtener_posicion_impacto(Actor const&) const=0;
	virtual void evaluar_y_disparar()=0;	//Es protegido porque se llamaría desde el turno y no siempre generaría disparos.
	virtual void evaluar_y_generar_decoraciones()=0;	//Idem.

	public:

	virtual bool realiza_actividad() const=0;
	virtual bool es_activo() const=0;
	virtual bool es_preparado_para_lanzar_trigger() const=0;
	virtual void finalizar_estado_trigger()=0;	

	////////////////////////////////////////////////////////////////////////
	//Métodos Implementados.

	protected:

	void activar() const {activado=true;}	//estos const son una falacia :D.
	void desactivar() const {activado=false;}
	bool es_activado() const {return activado;}
	float vel_max() {return 0;}
	float vel_min() {return 0;}

	void insertar_proyectil(Proyectil * p) {disparos_efectuados.push_back(p);}
	void insertar_decoracion(Estructura_decoracion_boss e) {decoraciones_generadas.push_back(e);}
	unsigned int cantidad_disparos() const {return disparos_efectuados.size();}
	unsigned int cantidad_decoraciones() const {return decoraciones_generadas.size();}

	public:

	unsigned int tipo_a_colision_proyectil() const {return T_DESTRUYE_PROYECTIL;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_NO_PREPARADO;}
	virtual bool es_cuenta_destruible() const {return false;} //No cuentan como destruibles... 
	virtual void recibir_inicio_accion();
	virtual void recibir_finalizacion_accion();
	std::vector<Proyectil *> & obtener_disparos() {return disparos_efectuados;}
	std::vector<Estructura_decoracion_boss> & obtener_decoraciones() {return decoraciones_generadas;}
	bool puede_disparar() const {return disparos_efectuados.size() > 0;}	//Equivale a "ha generado disparos".
	bool puede_generar_decoraciones() const {return decoraciones_generadas.size() > 0;}
	void vaciar_proyectiles(){disparos_efectuados.clear();}
	void vaciar_decoraciones() {decoraciones_generadas.clear();}

	Boss();
	~Boss();
};

#endif
