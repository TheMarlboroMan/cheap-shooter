#ifndef ACTOR_MAPA_H
#define ACTOR_MAPA_H

#include "../no_jugador/no_jugador.h"
#include "../proyectil/proyectil.h"

/*Actores que aparecen espontáneamente en el mapa o se generan al romper 
otros.
Es también "receptor_trigger".
*/

class Actor_mapa:public No_jugador, public Receptor_trigger
{
	private:

	static unsigned int MODO_EDITOR_ACTIVO;
	bool de_nivel_preparado;

	protected:

	virtual bool puede_rebotar() const {return true;}
	void preparar_actor(unsigned int);		//The one and only cosa que hay que llamar en el constructor de los actores.
	void iniciar(unsigned int);
	virtual float vel_max()=0;
	virtual float vel_min()=0;
	int resistencia;
	unsigned int tag;
		
	public:

	static bool EN_EDITOR() {return MODO_EDITOR_ACTIVO;}
	static void activar_en_editor() {MODO_EDITOR_ACTIVO=1;}
	static void desactivar_en_editor() {MODO_EDITOR_ACTIVO=0;}

	/*Este enum define las diferentes profundidades a las que un actor puede
	ser dibujado... Están organizados con esos valores para poder usarse 
	como flags en el editor. Nada más. Es cutre, pero no tengo ganas de
	escribir un puente sólo para eso.*/

	enum PROFUNDIDAD_REPRESENTACION{
P_FONDO=50,	//Hasta 50 son "fondo"... Podemos ordenar hasta 50 niveles.
P_NORMAL=79,	//De 51 a 79 son niveles propios del motor para mostrar actores.
P_FRENTE=80,	//De 80 en adelante son "frente". Podemos ordenar hasta... muuuchos niveles.
P_MAX_PROFUNDIDAD=130
	};

	/*Estos enum definen el comportamiento de los actores contra varias cosas
	OJO OJO OJO: Ojo con cambiar nada aquí!. Si cambiamos algún valor existente
	por otro nos cargaremos todos los niveles prehechos!!!.*/


	enum TIPOS_ACTOR_MAPA {
//Bonus

T_BONUS_DISPARO=10,
T_BONUS_TIEMPO=11,
T_BONUS_VIDA=12,
T_BONUS_PIMIENTO=13,
T_BONUS_NORMAL=14,
T_BONUS_COMPONENTE_ESCUDO=15,
T_BONUS_CARGA_ESCUDO=16,

//Obstáculos para fases aleatorias...

T_OBSTACULO_26_26=20,
T_OBSTACULO_32_64=21,
T_OBSTACULO_64_26=22,
T_OBSTACULO_128_32=23,
T_OBSTACULO_800_32=24,
T_OBSTACULO_BONUS=30,
T_OBSTACULO_DESTRUIBLE=31,
T_OBSTACULO_DESTRUIBLE_SECRETO=32,

//Obstáculos de actividad...

T_OBSTACULO_ACTIVIDAD_GRAVEDAD=40,
T_OBSTACULO_ACTIVIDAD_DISPARADOR=41,
T_OBSTACULO_ACTIVIDAD_EXPLOSIVO=42,
T_OBSTACULO_ACTIVIDAD_CANON_PROGRAMABLE=43,
T_OBSTACULO_ACTIVIDAD_PUERTA_DISPARAR=44,
T_OBSTACULO_ACTIVIDAD_PUERTA_AUTOMATICA=45,
T_OBSTACULO_ACTIVIDAD_CANON_GIRATORIO=46,
T_OBSTACULO_ACTIVIDAD_PUERTA_REMOTA=47,
T_OBSTACULO_ACTIVIDAD_CANON_APUNTADO=48,
T_OBSTACULO_ACTIVIDAD_BOSS_01=49,
T_OBSTACULO_ACTIVIDAD_BOSS_02=50,


//Especiales...
T_OBSTACULO_PROPIEDADES=80,
T_OBSTACULO_TRIGGER_CONTACTO=81,
T_OBSTACULO_TRIGGER_INTERRUPTOR=82,
T_PUNTO_RUTA=83,
T_OBSTACULO_PROPIEDADES_FONDO=84,
T_OBSTACULO_VECTOR=85,
T_OBSTACULO_ACTIVIDAD_GENERADOR_DECORACION=86,
T_OBSTACULO_ACTIVIDAD_GENERADOR_OBSTACULOS=87,
T_OBSTACULO_TRIGGER_AUTO=88,
T_OBSTACULO_TUTORIAL=89,
T_OBSTACULO_COLOCADOR_JUGADOR=90,
T_TRAZADOR_FANTASMA=91,
T_OBSTACULO_PROPIEDADES_MOVIMIENTO_RELATIVO=92,
T_TRAZADOR_EXPLOSION=93,

//Obstáculos flexibles -> niveles prehechos...

T_OBSTACULO_FLEX=100,
T_OBSTACULO_FLEX_DIAGONAL_32_1=101,
T_OBSTACULO_FLEX_DIAGONAL_32_2=102,
T_OBSTACULO_FLEX_DIAGONAL_32_3=103,
T_OBSTACULO_FLEX_DIAGONAL_32_4=104,
T_OBSTACULO_FLEX_DIAGONAL_64_1=105,
T_OBSTACULO_FLEX_DIAGONAL_64_2=106,
T_OBSTACULO_FLEX_DIAGONAL_64_3=107,
T_OBSTACULO_FLEX_DIAGONAL_64_4=108,
T_OBSTACULO_FLEX_DIAGONAL_32_64_1=109,
T_OBSTACULO_FLEX_DIAGONAL_32_64_2=110,
T_OBSTACULO_FLEX_DIAGONAL_32_64_3=111,
T_OBSTACULO_FLEX_DIAGONAL_32_64_4=112,
T_OBSTACULO_FLEX_DIAGONAL_64_32_1=113,
T_OBSTACULO_FLEX_DIAGONAL_64_32_2=114,
T_OBSTACULO_FLEX_DIAGONAL_64_32_3=115,
T_OBSTACULO_FLEX_DIAGONAL_64_32_4=116,
T_DECORACION_NIVEL_FONDO=117,
T_DECORACION_NIVEL_FRENTE=118
};

	enum TIPOS_COLISION_PROYECTIL {
T_DESTRUYE_PROYECTIL=1,
T_DESTRUYE_PROYECTIL_ACTOR,
T_DESTRUYE_PROYECTIL_RESTA_RESISTENCIA_ACTOR,
T_DESTRUYE_PROYECTIL_RESTA_RESISTENCIA_ACTOR_DA_ITEM,
T_DESTRUYE_PROYECTIL_RESTA_RESISTENCIA_ACTOR_GENERA_PROYECTILES,	//Sólo para las minas.
T_DESTRUYE_PROYECTIL_RESTA_RESISTENCIA_ACTOR_ES_SECRETO,		//Sólo para proyectil secreto.
T_DESTRUYE_PROYECTIL_LANZA_ACCION,
T_DESTRUYE_PROYECTIL_NO_RESTA_COMBO
};

	enum TIPOS_COLISION_JUGADOR {
T_DESTRUYE_JUGADOR=1,
T_OTORGA_BONUS_JUGADOR,
T_AUMENTA_CONTADOR_NIVEL,
T_DISPARA_ACCION
};

	enum TIPO_CUENTA {
T_CUENTA_OBSTACULO=1,
T_CUENTA_BONUS,
T_CUENTA_GEMA
};

	enum NADA {T_NADA=0};

	Actor_mapa();
	virtual ~Actor_mapa();
	virtual unsigned short int tipo_actor_mapa()=0;

	int acc_resistencia() const {return this->resistencia;}

	//Las acciones por defecto para procesar colisiones son "nada". 
	virtual bool procesar_colision_proyectil(Proyectil *);
	virtual void procesar_colision_jugador();
	virtual bool desaparece_tras_colision() const {return true;}
	virtual bool realiza_actividad() const {return false;}	//Indica si debe ser introducido dentro del vector de actores que realizan actividades.
	virtual bool procesa_turno() const {return true;}	//Indica si debe procesar su turno en este tic.
	virtual bool es_parametrizable() const {return false;}
	virtual bool es_flexible() const {return false;}
	virtual bool es_redimensionable() const {return false;}
	virtual bool es_ordenable_profundidad() const {return false;}
	virtual bool es_colisionable() const {return true;}
	virtual unsigned int tipo_a_colision_proyectil() const {return T_NADA;}	//Determina lo que ocurrirá cuando choque con un proyectil.
	virtual unsigned int tipo_a_colision_jugador() const {return T_DESTRUYE_JUGADOR;} //Determina lo que ocurrirá cuando choque con el jugador.
	virtual unsigned int tipo_a_cuenta() const {return T_CUENTA_OBSTACULO;} //Determina cómo cuenta para niveles aleatorios. Básicamente casi todo cuenta como obstáculo, menos bonus y gemas.
	virtual unsigned int tipo_a_faccion() const {return Definiciones::F_PREPARADO;}	//Determina la facción para el choque con proyectiles, puesto que todos los proyectiles van en el mismo vector.
	virtual short int puntuacion_por_destruir() {return 0;}
	static unsigned short int tipo_a_faccion(unsigned int);
	virtual void preparar_actor_mapa_manual(float, float, float, float);
	virtual void procesar_turno(float=0);
	virtual unsigned short int acc_profundidad_representacion() const {return P_NORMAL;}
	void mut_tag(unsigned int p_param) {this->tag=p_param;}
	unsigned int acc_tag() const {return this->tag;}
	bool es_de_nivel_preparado() const {return this->de_nivel_preparado;}

	//Estas están aquí para ser la base de todo, básicamente por las 
	//decoraciones de nivel.

	virtual void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);
	virtual void recibir_valores_actor_flexible(const Parametros_actor_flexible&);
	virtual void recibir_valores_actor_redimensionable(const Parametros_actor_redimensionable&);
	virtual void recibir_valores_actor_ordenable_profundidad(const Parametros_actor_ordenable_profundidad&);
	virtual bool recibir_senal_trigger(const Accion_trigger_datos&);
	virtual void procesar_cambio_dimensiones();
	virtual unsigned int acc_columnas_humo_explosion() const {return 0;}
	virtual bool es_con_chatarra_explosion() const {return true;}	
	virtual bool es_cuenta_recogible() const {return false;}
	virtual bool es_cuenta_destruible() const {return false;}

	friend class Ordenador_actor_mapa_por_profundidad;
};

struct Ordenador_actor_mapa_por_profundidad
{
	bool operator()(Actor_mapa * i, Actor_mapa * j) const
	{
		if(i->acc_profundidad_representacion() == j->acc_profundidad_representacion())
		{
			if(i->y==j->y) return i->x < j->x;
			else return i->y < j->y;
		}
		else return i->acc_profundidad_representacion() < j->acc_profundidad_representacion();
	}
};

#endif

