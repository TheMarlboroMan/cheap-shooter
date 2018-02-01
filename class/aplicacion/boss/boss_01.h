#ifndef BOSS_01_H
#define BOSS_01_H

#include "boss.h"

class Boss_01:public Boss
{
	private:

	////////////////////////////////////////////////////////////////////////
	//Enumeraciones...

	enum estados {
		E_INICIO=0,
		E_CANONES_VERTICALES,
		E_PAUSA_1,	
		E_CANONES_DIAGONALES,
		E_PAUSA_2,
		E_CANON_GIRATORIO,
		E_PAUSA_3,
			E_MAX_ESTADO,
			E_EXPLOSION,
			E_CAYENDO,
			E_TRIGGER,
			E_DESTRUIDO	
	};

	//Para identificar a cuál le hemos dado.
	enum posiciones_debiles{	
		POS_NADA=0,
		POS_DEBIL_IZQ,
		POS_DEBIL_DER,
		POS_DEBIL_CENTRAL		
	};

	////////////////////////////////////////////////////////////////////////
	//Estructuras

	private:

	//Especializando el punto débil...
	struct Punto_debil_boss_01:public Punto_debil
	{
		static const int X_REP_PUNTOS_DEBILES_VULNERABLE=0;
		static const int X_REP_PUNTOS_DEBILES_INVULNERABLE=32;
		static const int X_REP_PUNTOS_DEBILES_HERIDO=64;
		static const int X_REP_PUNTOS_DEBILES_DESTRUIDO=96;		

		float pausa_entre_humos;

		Punto_debil_boss_01(int r, bool v):Punto_debil(r, v), pausa_entre_humos(0.0) {}

		int obtener_x_representacion()
		{
			if(destruido) return X_REP_PUNTOS_DEBILES_DESTRUIDO;
			else if(!vulnerable) return X_REP_PUNTOS_DEBILES_INVULNERABLE;
			else if(tiempo_herido) return X_REP_PUNTOS_DEBILES_HERIDO;
			else return X_REP_PUNTOS_DEBILES_VULNERABLE;
		}

		int obtener_y_representacion() {return Y_REP_PUNTOS_DEBILES;}

		void turno(float p_delta)
		{
			Punto_debil::turno(p_delta);
			if(destruido) pausa_entre_humos+=p_delta;
		}

		bool puede_echar_humo() 
		{
			if(destruido && pausa_entre_humos >= 0.1)
			{
				pausa_entre_humos=0;
				return true;
			}
			else return false;
		}
	};

	////////////////////////////////////////////////////////////////////////
	//Constantes

	static const int W_REP_PUNTOS_DEBILES=32;
	static const int H_REP_PUNTOS_DEBILES=32;
	static const int Y_REP_PUNTOS_DEBILES=256;

	static const float DURACION_MS_INICIO;
	static const float DURACION_MS_CANONES_VERTICALES;
	static const float DURACION_MS_PAUSA_1;
	static const float DURACION_MS_CANONES_DIAGONALES;
	static const float DURACION_MS_PAUSA_2;
	static const float DURACION_MS_CANON_GIRATORIO;
	static const float DURACION_MS_PAUSA_3;

	static const int W_PUNTO_DEBIL=32;
	static const int H_PUNTO_DEBIL=32;

	static const int OFFSET_X_PUNTO_DEBIL_1=32;
	static const int OFFSET_Y_PUNTO_DEBIL_1=64;
	static const int OFFSET_X_PUNTO_DEBIL_2=288;
	static const int OFFSET_Y_PUNTO_DEBIL_2=64;
	static const int OFFSET_X_PUNTO_DEBIL_3=160;
	static const int OFFSET_Y_PUNTO_DEBIL_3=96;

	static const int RESISTENCIA_PUNTOS_DEBILES_LATERALES=150;
	static const int RESISTENCIA_PUNTO_DEBIL_CENTRAL=200;

	static const int ORIGEN_X_CANON_VERTICAL_SUPERIOR_IZQ=32;
	static const int ORIGEN_Y_CANON_VERTICAL_SUPERIOR_IZQ=10;
	static const int ORIGEN_X_CANON_VERTICAL_SUPERIOR_DER=319;
	static const int ORIGEN_Y_CANON_VERTICAL_SUPERIOR_DER=10;
	static const int ORIGEN_X_CANON_VERTICAL_INFERIOR_IZQ=80;
	static const int ORIGEN_Y_CANON_VERTICAL_INFERIOR_IZQ=182;
	static const int ORIGEN_X_CANON_VERTICAL_INFERIOR_DER=272;
	static const int ORIGEN_Y_CANON_VERTICAL_INFERIOR_DER=182;

	static const int ORIGEN_X_CANON_DIAGONAL_SUPERIOR_IZQ=80;
	static const int ORIGEN_Y_CANON_DIAGONAL_SUPERIOR_IZQ=24;
	static const int ORIGEN_X_CANON_DIAGONAL_SUPERIOR_DER=272;
	static const int ORIGEN_Y_CANON_DIAGONAL_SUPERIOR_DER=24;
	static const int ORIGEN_X_CANON_DIAGONAL_INFERIOR_IZQ=150;
	static const int ORIGEN_Y_CANON_DIAGONAL_INFERIOR_IZQ=243;
	static const int ORIGEN_X_CANON_DIAGONAL_INFERIOR_DER=201;
	static const int ORIGEN_Y_CANON_DIAGONAL_INFERIOR_DER=243;

	static const int ORIGEN_X_CANON_CIRCULAR=175;
	static const int ORIGEN_Y_CANON_CIRCULAR=144;

	static const int VECTOR_Y_CANON_VERTICAL=1;
	static const int VECTOR_X_CANON_VERTICAL=0;
	static const int VECTOR_X_CANON_DIAGONAL=1;
	static const int VECTOR_Y_CANON_DIAGONAL=1;

	static const int CANONES_VERTICALES=4;
	static const int CANONES_DIAGONALES=4;
	static const int CANONES_CIRCULARES=2;

	//Acumulador de tiempo a alcanzar según cada cañón....
	static const float MS_PAUSA_CANONES_VERTICALES;
	static const float MS_PAUSA_CANONES_DIAGONALES;
	static const float MS_PAUSA_CANON_CIRCULAR;

	static const int VELOCIDAD_X=100;
	static const int VELOCIDAD_X_DESTRUIDO=100;
	static const int VELOCIDAD_Y_DESTRUIDO=100;

	///////////////////////////////////////////////////////////////////////
	//Propiedades...
	private:

	//Partes gráficas...
	DLibV::Representacion_bitmap_dinamica * rep_general;
	DLibV::Representacion_bitmap_dinamica * rep_punto_debil_izq;
	DLibV::Representacion_bitmap_dinamica * rep_punto_debil_der;
	DLibV::Representacion_bitmap_dinamica * rep_punto_debil_central;

	unsigned short int estado;
	float tiempo_ms_estado;
	unsigned int angulo_disparo_canon_circular;
	unsigned int salto_angulo_disparo_canon_circular;
	float pausa_entre_disparos;
	int direccion;

	Punto_debil_boss_01 punto_debil_izq;
	Punto_debil_boss_01 punto_debil_der;
	Punto_debil_boss_01 punto_debil_central;

	///////////////////////////////////////////////////////////////////////
	//Implementaciones del fondo de la jerarquía...

	protected: 

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	void procesar_turno(float=0);
	bool procesar_colision_proyectil(Proyectil *);
	unsigned short int actuar_sonido() {return Recursos::S_DISPARO;}
	short int puntuacion_por_destruir() {return 1500;}
	void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);
	void cuentame() {std::cout<<"SOY BOSS_01 DISPARADOR"<<std::endl;}
	short unsigned int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_ACTIVIDAD_BOSS_01;}
	void aceptar_visitante(Visitante& v) {v.visitar(*this);}
	unsigned int acc_columnas_humo_explosion() const {return 0;}
	bool en_colision_con(Actor const&) const;
	virtual bool es_afectado_por_edu() const {return false;}
	virtual bool es_fuera_de_juego() const {return false;} //Nunca estará en fuera de juego y por tanto no se borrará por salir de la pantalla.
	
	////////////////////////////////////////////////////////////////////////
	//Métodos propios.

	private:

	void controlar_direccion();
	void generar_proyectil(int, int, float, float);
	void generar_proyectil(int, int, DLibH::Vector_2d);
	void controlar_posicion_puntos_debiles();

	public:

	bool es_preparado_para_hacer_explosion() {return estado==E_EXPLOSION;}
	void finalizar_estado_explosion() {estado=E_CAYENDO;}

	Boss_01();
	virtual ~Boss_01();


	////////////////////////////////////////////////////////////////////////
	//Implementando Boss...

	protected:

	virtual void comprobar_destruccion();
	virtual void controlar_estado(float);
	virtual void controlar_representacion();
	virtual unsigned short int obtener_posicion_impacto(Actor const&) const;
	virtual void evaluar_y_disparar();
	virtual void evaluar_y_generar_decoraciones();

	public:

	virtual bool es_preparado_para_lanzar_trigger() const {return estado==E_TRIGGER;}	
	virtual void finalizar_estado_trigger() {estado=E_DESTRUIDO;}

	public:

	virtual bool realiza_actividad() const;
	virtual bool es_activo() const;
	bool procesa_turno() const {return true;} //Siempre procesa...
};

#endif
