#ifndef BOSS_02_H
#define BOSS_02_H

#include "boss.h"

class Boss_02:public Boss
{
	////////////////////////////////////////////////////////////////////////
	//Estructuras

	private:

	/*La pata es una de las partes... Hereda de actor, para tener los temas
	de colisión y tal... X e Y indicarían la posición real en el mundo, 
	pero no la controlarían directamente: están vinculados a la posición
	del actor y tienen un desplazamiento a través de offset_x y offset_y.*/
	
	class Pata:public Actor
	{
		////////////////////////////////////////////////////////////////
		//Enumeraciones...

		public:

		//La pata puede estar en tres posiciones o moviéndose entre medio.
		//Cuando se mueve lo hace secuencial: arriba, centro, abajo, 
		//centro, arriba...

		enum posiciones{P_MIN=-1, P_ARRIBA=0, P_CENTRO, P_ABAJO, P_MAX};
	
		////////////////////////////////////////////////////////////////
		//Constantes...

		private:
		static const float T_POSICION;	//Tiempo que estará en cada posición.
		static const float VELOCIDAD;	//Pixeles por segundo..
		static const int DISTANCIA_POSICIONES=32; //Distancia en píxeles entre cada posición.

		////////////////////////////////////////////////////////////////
		//Propiedades..

		private:
		int posicion_actual; //Posición según la enumeración.
		short int direccion;	//1 o -1.
		bool en_movimiento;	
		float tiempo;
		float destino;
		float offset_x;
		float offset_y;

		////////////////////////////////////////////////////////////////
		//Implementando Actor...

		protected:
		void establecer_posicion() {}

		public:
		void procesar_turno(float);	//Estos son "protected" en actor, pero bueno... No voy a ponerme a reparar eso ahora.
		void cuentame() {std::cout<<"SOY PATA BOSS 02"<<std::endl;}

		////////////////////////////////////////////////////////////////
		//Métodos...

		private:
		void ajustar_posicion_destino();

		public:
		void preparar(int, int, DLibV::Mapa_colision_bitmap const *);
		void situar(float, float);
		Pata(unsigned int p, float t);		
	};
	
	/*El cuerpo es otra de las partes... No se mueve pero contiene al punto
	débil principal. La caja de colisión será por bitmap excepto por un
	rectángulo "túnel" que se abrirá a todo lo alto en el centro del actor cuando
	abran las dos cubiertas. Al final de ese túnel estará la caja de colisión
	del punto débil. Tiene también una zona libre de colisión en la mitad
	superior para no chocar con las patas traseras...*/

	class Cuerpo:public Actor
	{
		////////////////////////////////////////////////////////////////
		//Enumeraciones...
		public:
		enum estados {E_CERRADO=0, E_ABIERTO, E_DESTRUIDO};

		////////////////////////////////////////////////////////////////
		//Constantes...

		private:
		static const int X_TUNEL=64;
		static const int Y_TUNEL=0;
		static const int W_TUNEL=128;
		static const int H_TUNEL=256;

		static const int X_ZONA_LIBRE=0;
		static const int Y_ZONA_LIBRE=0;
		static const int W_ZONA_LIBRE=256;
		static const int H_ZONA_LIBRE=106;

		static const int X_DEBIL=106;
		static const int Y_DEBIL=72;
		static const int W_DEBIL=44;
		static const int H_DEBIL=44;

		static const unsigned int T_HERIDO=100;
		static const unsigned int RESISTENCIA=200;

		static const float MIN_TIEMPO_EN_ANGULO;
		static const float PAUSA_DISPAROS;
		static const int ANGULO_MIN=125;
		static const int ANGULO_MAX=245;
		static const int MAX_REP_CANON_CUERPO=4;

		public:

		static const int OFFSET_X_DISPARO=125;
		static const int OFFSET_Y_DISPARO=175;
		static const float VELOCIDAD_DISPAROS;
		static const float MAX_CUENTA_REP_CANON_FASE_1;
		static const float MAX_CUENTA_REP_CANON_FASE_2;
		static const float MAX_CUENTA_REP_CANON_FASE_3;
		static const short int MAX_FASE_CANON=4;

		////////////////////////////////////////////////////////////////
		//Propiedades...
		private:

		SDL_Rect tunel_colision;
		SDL_Rect caja_debil;
		SDL_Rect zona_libre;

		unsigned int estado;
		float tiempo_herido;
		mutable bool acertado;
		int resistencia;

		float pausa_disparos;
		float tiempo_en_angulo;
		float tiempo_rep_canon;
		mutable short int fase_canon;

		////////////////////////////////////////////////////////////////
		//Implementando Actor...

		protected:
		void establecer_posicion() {}

		public:
		void procesar_turno(float);	//Estos son "protected" en actor, pero bueno... No voy a ponerme a reparar eso ahora.
		void cuentame() {std::cout<<"SOY CUERPO BOSS 02"<<std::endl;}

		////////////////////////////////////////////////////////////////
		//Métodos...

		public:
		void preparar(int, int, DLibV::Mapa_colision_bitmap const *);
		void situar(float, float);
		void abrir() {estado=E_ABIERTO;}
		bool es_cerrado() {return estado==E_CERRADO;}
		bool es_herido() const {return tiempo_herido > 0.0;}
		bool es_acertado() const {return acertado;}
		bool es_destruido() const {return estado==E_DESTRUIDO;}
		bool herir(int);
		void acumular_tiempo_en_angulo(float, Actor const * const);
		bool puede_disparar() const {return tiempo_en_angulo >= MIN_TIEMPO_EN_ANGULO && !pausa_disparos;}
		int obtener_fase_canon();
		virtual bool en_colision_con(Actor const&) const;
		Cuerpo();

	};

	/*El pincho es una parte que está ligada a cada pata. Cada pincho se
	alarga y se contrae en la posición de la pata y dispara cuando está
	extendido. Si se destruye la pata se retrae el pincho. El mapa de 
	colisión con el pincho es un rectángulo (no es exacto, pero bueno).*/

	class Pincho:public Actor
	{
		////////////////////////////////////////////////////////////////
		//Enumeraciones...
		public:
		enum estados {E_INICIO_CUENTA=0, E_RETRAIDO, E_SALIENDO, E_PAUSA_SALIDA, E_DISPARANDO, E_PAUSA_ENTRADA, E_ENTRANDO, E_MAX, E_DESACTIVADO};

		////////////////////////////////////////////////////////////////
		//Constantes...

		private:
		static const int W=17;
		static const unsigned int MIN_H=0;
		static const unsigned int MAX_H=249;

		static const float VELOCIDAD;
		static const float VELOCIDAD_DISPAROS;
		static const float ESPACIADO_DISPAROS;

		static const unsigned int T_RETRAIDO=4;	//Segundos que tarda en hacer las cosas.
		static const unsigned int T_PAUSA_SALIDA=1;
		static const unsigned int T_DISPARANDO=2;
		static const unsigned int T_PAUSA_ENTRADA=1;

		////////////////////////////////////////////////////////////////
		//Propiedades...
		private:

		short int direccion; //Dirección de los disparos.
		short int estado;
		float pausa_disparos;
		float tiempo;
		float aux_h;	//Como la altura en actor es un entero sin signo, usamos este para hacerlo suave.
		bool desactivado;	//No es "desactivado ahora" sino "desactivado cuando termine de retraerse".

		////////////////////////////////////////////////////////////////
		//Implementando Actor...

		protected:
		void establecer_posicion() {}

		public:
		void procesar_turno(float);
		void cuentame() {std::cout<<"SOY PINCHO BOSS 02"<<std::endl;}

		////////////////////////////////////////////////////////////////
		//Métodos...

		public:
		void situar(float, float);
		bool puede_disparar() const;
		bool debe_reajustar_representacion() const {return estado==E_SALIENDO || estado==E_ENTRANDO;}
		bool es_colisionable() const {return estado!=E_RETRAIDO;}
		bool es_desactivado() const {return desactivado;}
		void forzar_retraccion() {estado=E_ENTRANDO; desactivado=true;}
		Pincho(short int, float);
		SDL_Rect obtener_rect_representacion();
		DLibH::Vector_2d obtener_vector_disparos() const;
	};

	/*El laser trasero es una suerte de "proyectil...". Realmente es un 
	actor que tiene su caja de colisión a todo lo alto y con lo que 
	se puede chocar. Sus estados son apagado, cargando y disparando aunque
	también puede estar inutilizado si se destruye el punto débil de
	turno.*/

	class Laser_trasero:public Actor
	{
		////////////////////////////////////////////////////////////////
		//Enumeraciones...
		public:
		enum estados {E_INICIO_CUENTA=0, E_APAGADO, E_CARGANDO, E_INICIAR_SONIDO, E_CRECIENDO, E_DISPARANDO, E_DECRECIENDO, E_FINALIZAR_SONIDO, E_DESCARGANDO, E_MAX, E_DESACTIVADO};

		////////////////////////////////////////////////////////////////
		//Constantes...

		private:
		static const int W=20;
		static const unsigned int MAX_H=600;

		static const unsigned int T_APAGADO=4;	//Segundos que tarda en hacer las cosas.
		static const unsigned int T_CARGANDO=2;
		static const unsigned int T_DESCARGANDO=1;
		static const unsigned int T_DISPARANDO=3;

		static const float VELOCIDAD_REP;
		static const float VELOCIDAD;
		static const unsigned int MIN_H_REP=7;
		static const unsigned int MAX_H_REP=32;
		static const int MAX_COLOR_G=128;
		static const int MAX_COLOR_B=255;

		////////////////////////////////////////////////////////////////
		//Propiedades...
		private:

		short int estado;
		float tiempo;
		float aux_h;
		float aux_y;
		float dest_y;
		float aux_rep;		//La representación cambiaría un poco apoyándose en esto.
		DLibA::Canal_audio canal;
//		int canal_audio;

		////////////////////////////////////////////////////////////////
		//Implementando Actor...

		protected:
		void establecer_posicion() {}

		public:
		void procesar_turno(float);
		void cuentame() {std::cout<<"SOY LASER BOSS 02"<<std::endl;}

		////////////////////////////////////////////////////////////////
		//Métodos...

		private:
		
		public:
		bool es_colisionable() const {return estado==E_DISPARANDO || estado==E_CRECIENDO || estado==E_DECRECIENDO;}
		void forzar_destruccion() 
		{
			finalizar_sonido();
			estado=E_DESACTIVADO;
		}

		void situar(float, float);
		Laser_trasero(float);
		~Laser_trasero();
		SDL_Rect obtener_rect_representacion(); //Te da el recorte de la representación con patrón.
		Uint32 obtener_color_canon() const;
		bool debe_solicitar_canal() const;
		void asignar_canal(DLibA::Canal_audio);
		void finalizar_sonido();
	};

	/*Las cubiertas son otra parte: se inician cerradas y bloquean el punto	
	débil del cuerpo. Se abren al acertar en un punto débil que tienen en el
	lateral. Tiene dos cajas de colisión: una cubre todo el flanco central
	y la otra el punto débil. No usa mapa de colisión por bitmap.*/

	class Cubierta:public Actor
	{
		////////////////////////////////////////////////////////////////
		//Enumeraciones...

		public:

		//La pata puede estar en tres posiciones o moviéndose entre medio.
		//Cuando se mueve lo hace secuencial: arriba, centro, abajo, 
		//centro, arriba...

		enum estados{E_CERRADO=0, E_MOVIMIENTO, E_ABIERTO};
	
		////////////////////////////////////////////////////////////////
		//Constantes...

		private:
		static const int W_ACTOR=64;
		static const int H_ACTOR=198;
		static const int DISTANCIA_POSICIONES=32; //Distancia en píxeles entre cada posición.
		static const int W_CAJA_COLISION=24;
		static const int H_CAJA_COLISION=198;
		static const int W_CAJA_DEBIL=24;
		static const int H_CAJA_DEBIL=53;
		static const float VELOCIDAD;	//Pixeles por segundo..
		static const float PAUSA_DISPAROS;
		static const int RESISTENCIA=150;
		static const unsigned int T_HERIDO=100;
		static const int MITAD_ANGULO_CANON_CUBIERTA=45;
		static const int OFFSET_Y_CANONES_CUBIERTA=38;	//Con respecto a la propia cubierta...

		public:
		static const float VELOCIDAD_DISPAROS;
		static const int W_PUNTO_DEBIL=24;
		static const int H_PUNTO_DEBIL=53;
		static const int Y_REP_PUNTO_DEBIL=198;
		static const int X_REP_PUNTO_DEBIL_IZQ=256;
		static const int X_REP_PUNTO_DEBIL_IZQ_ROJO=280;
		static const int X_REP_PUNTO_DEBIL_DER=304;
		static const int X_REP_PUNTO_DEBIL_DER_ROJO=328;

		////////////////////////////////////////////////////////////////
		//Propiedades..

		private:
		short int direccion;	//1 o -1.
		int estado; //Posición según la enumeración.
		float offset_x;
		float offset_y;
		float destino;
		float angulo_min;
		float angulo_max;
		float punto_disparo_x;	//Necesitamos estos valores para saber 
		float punto_disparo_y;	//de dónde sale el disparo y compararlo con el blanco.
		int offset_canon_x;	//Estos los necesitamos porque son distintos 

		int offset_caja_colision_x;
		int offset_caja_colision_y;
		int offset_caja_debil_x;
		int offset_caja_debil_y;

		SDL_Rect caja_colision;
		SDL_Rect caja_debil;

		mutable bool acertado;
		int resistencia;
		float tiempo_herido;
		float pausa_disparos;
		float pausa_entre_humos;

		////////////////////////////////////////////////////////////////
		//Implementando Actor...

		protected:
		void establecer_posicion() {}

		public:
		void procesar_turno(float);	//Estos son "protected" en actor, pero bueno... No voy a ponerme a reparar eso ahora.
		void cuentame() {std::cout<<"SOY CUBIERTA BOSS 02"<<std::endl;}

		////////////////////////////////////////////////////////////////
		//Métodos...

		public:
		virtual bool en_colision_con(Actor const&) const;
		void preparar(int, int, int, int);
		void situar(float, float);
		bool es_cerrada() const {return estado==E_CERRADO;}
		bool es_acertada() const {return acertado;}
		bool herir(int);
		bool es_herida() const {return tiempo_herido > 0.0;}
		bool puede_disparar(const Actor * const) const;
		bool puede_echar_humo() const {return estado!=E_CERRADO && !pausa_entre_humos;}
		float acc_punto_disparo_x() const {return punto_disparo_x;}
		float acc_punto_disparo_y() const {return punto_disparo_y;}
		const SDL_Rect& acc_caja_debil() const {return caja_debil;}
		Cubierta(short int, int);

	};

	//Especialización del punto débil para las patas y el cañón... Podríamos
	//hacerlos como una de las partes, pero también lo podemos dejar así...
	
	struct Punto_debil_boss_02:public Punto_debil
	{
		float pausa_entre_humos;
		Punto_debil_boss_02(int r):Punto_debil(r, true), pausa_entre_humos(0.0) {}

		virtual int obtener_x_representacion()=0;
		virtual int obtener_y_representacion()=0;

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

	struct Punto_debil_boss_02_pata:public Punto_debil_boss_02
	{
		static const int X_REP_HERIDO=370;
		static const int X_REP_NORMAL=352;
		static const int Y_REP=198;
		Punto_debil_boss_02_pata(int r):Punto_debil_boss_02(r) {}
		int obtener_x_representacion() {if(tiempo_herido) return X_REP_HERIDO; else return X_REP_NORMAL;}
		int obtener_y_representacion() {return Y_REP;}
	};

	struct Punto_debil_boss_02_canon:public Punto_debil_boss_02
	{
		static const int X_REP=464;
		static const int Y_REP_NORMAL=192;
		static const int Y_REP_HERIDO=224;
		Punto_debil_boss_02_canon(int r):Punto_debil_boss_02(r) {}
		int obtener_x_representacion() {return X_REP;}
		int obtener_y_representacion() {if(tiempo_herido) return Y_REP_HERIDO; else return Y_REP_NORMAL;}
	};

	////////////////////////////////////////////////////////////////////////
	//Enumeraciones...

	private:

	/*En primer lugar está oculto. Cuando el scroll se detenga se hará 
	visible, se trasladará hacia abajo y se moverá hacia arriba hasta su 
	posición final. En esta	posición final se iniciará el ciclo normal.*/

	enum estados {
		E_OCULTO=0,
		E_VISIBLE, 		//Las representaciones se hacen visibles.
		E_APARECIENDO,		//Viene desde abajo hasta su posición inicial.
		E_LOOP,			//Loop de batalla.
		E_EXPLOSION,		//Explosión decorativa.
		E_CAYENDO,		//Case hacia abajo.
		E_TRIGGER,		//Activación del trigger.
		E_DESTRUIDO
	};

	//Para identificar a cuál le hemos dado.
	enum posiciones_impacto{	
		POS_NADA=0,
		POS_PATA_EXTERIOR_IZQ,
		POS_PATA_INTERIOR_IZQ,
		POS_PATA_EXTERIOR_DER,
		POS_PATA_INTERIOR_DER,
		POS_CUBIERTA_IZQ,
		POS_CUBIERTA_DER,
		POS_CUERPO,
		POS_PATA_EXTERIOR_IZQ_PUNTO_DEBIL,
		POS_PATA_INTERIOR_IZQ_PUNTO_DEBIL,
		POS_PATA_EXTERIOR_DER_PUNTO_DEBIL,
		POS_PATA_INTERIOR_DER_PUNTO_DEBIL,
		POS_CANON_TRASERO_IZQ,
		POS_CANON_TRASERO_DER,
		POS_PINCHO_PATA_EXTERIOR_IZQ,
		POS_PINCHO_PATA_INTERIOR_IZQ,
		POS_PINCHO_PATA_EXTERIOR_DER,
		POS_PINCHO_PATA_INTERIOR_DER,
		POS_LASER_TRASERO_IZQ,
		POS_LASER_TRASERO_DER
	};

	public:
	
	enum senales_sonido {SND_NADA=0, SND_CANON=1, SND_EXPLOSION=2, SND_GATLING=4};

	////////////////////////////////////////////////////////////////////////
	//Constantes

	static const int X_CUERPO=224;
	static const int Y_CUERPO=7;
	static const int W_CUERPO=256;
	static const int H_CUERPO=256;

	static const int W_CUBIERTAS=64;
	static const int H_CUBIERTAS=198;
	static const int X_CUBIERTA_IZQ=288; //Posición de las cubiertas cerradas...
	static const int X_CUBIERTA_DER=352;
	static const int Y_CUBIERTAS=0;

	static const int X_COLISION_CUBIERTA_IZQ=40;
	static const int Y_COLISION_CUBIERTA_IZQ=0;
	static const int X_DEBIL_CUBIERTA_IZQ=16;
	static const int Y_DEBIL_CUBIERTA_IZQ=52;

	static const int X_COLISION_CUBIERTA_DER=0;
	static const int Y_COLISION_CUBIERTA_DER=0;
	static const int X_DEBIL_CUBIERTA_DER=24;
	static const int Y_DEBIL_CUBIERTA_DER=52;

	static const int W_PATAS_INTERIORES=120;
	static const int H_PATAS_INTERIORES=96;
	static const int X_PATAS_INTERIORES_IZQ=128;
	static const int X_PATAS_INTERIORES_DER=457;
	static const int Y_PATAS_INTERIORES=96;		//Se corresponde con la posición superior.

	static const int W_PATAS_EXTERIORES=248;
	static const int H_PATAS_EXTERIORES=128;
	static const int X_PATAS_EXTERIORES_IZQ=0;
	static const int X_PATAS_EXTERIORES_DER=456; //424;
	static const int Y_PATAS_EXTERIORES=140;	//Se corresponde con la posición superior.
	
	static const int DESPLAZAMIENTO_Y_APARICION=800;
	static const int VELOCIDAD_APARICION=-100;
	static const int VELOCIDAD_DESTRUCCION_Y=50;
	static const int VELOCIDAD_DESTRUCCION_X=150;

	static const int X_PUNTO_DEBIL=109;
	static const int Y_PUNTO_DEBIL=75;
	static const int W_PUNTO_DEBIL=38;
	static const int H_PUNTO_DEBIL=42;

	static const int Y_REP_PUNTO_DEBIL_CUERPO=192;
	static const int X_REP_PUNTO_DEBIL_CUERPO=388;
	static const int X_REP_PUNTO_DEBIL_CUERPO_ROJO=426;

	static const int W_PUNTO_DEBIL_CANON=26;
	static const int H_PUNTO_DEBIL_CANON=32;
	static const int X_PUNTO_DEBIL_CANON_IZQ=3;	//Offsets con respecto al cuerpo.
	static const int Y_PUNTO_DEBIL_CANON_IZQ=22;
	static const int X_PUNTO_DEBIL_CANON_DER=227;
	static const int Y_PUNTO_DEBIL_CANON_DER=22;

	static const int W_PUNTO_DEBIL_PATA=18;
	static const int H_PUNTO_DEBIL_PATA=57;
	static const int X_PUNTO_DEBIL_PATA_INTERIOR_IZQ=11; //Offsets con respecto a cada pata.
	static const int Y_PUNTO_DEBIL_PATA_INTERIOR_IZQ=37;
	static const int X_PUNTO_DEBIL_PATA_INTERIOR_DER=91;
	static const int Y_PUNTO_DEBIL_PATA_INTERIOR_DER=37;
	static const int X_PUNTO_DEBIL_PATA_EXTERIOR_IZQ=11;
	static const int Y_PUNTO_DEBIL_PATA_EXTERIOR_IZQ=70;
	static const int X_PUNTO_DEBIL_PATA_EXTERIOR_DER=219;
	static const int Y_PUNTO_DEBIL_PATA_EXTERIOR_DER=70;

	static const int X_PINCHO_PATA_INTERIOR_IZQ=12;	//Offsets con respecto a la pata en si.
	static const int Y_PINCHO_PATA_INTERIOR_IZQ=67;
	static const int X_PINCHO_PATA_EXTERIOR_IZQ=12;
	static const int Y_PINCHO_PATA_EXTERIOR_IZQ=102;
	static const int X_PINCHO_PATA_INTERIOR_DER=92;
	static const int Y_PINCHO_PATA_INTERIOR_DER=71;
	static const int X_PINCHO_PATA_EXTERIOR_DER=220;
	static const int Y_PINCHO_PATA_EXTERIOR_DER=100;

	static const int OFFSET_LASER_X=-1;
	static const int W_REP_LASER=24;

	static const int X_LASER_TRASERO_IZQ=229;
	static const int X_LASER_TRASERO_DER=453 ;
	static const int Y_LASER_TRASERO=58;

	static const int CANONES_POR_PINCHO=5;
	static const int DISTANCIA_ENTRE_CANONES_PINCHO=32;
	static const int OFFSET_Y_PRIMER_CANON_PINCHO=48;

	static const int RESISTENCIA_CANON=200;
	static const int RESISTENCIA_PATA=130;

	static const int OFFSET_X_CANON_CUBIERTA_IZQ=27;
	static const int OFFSET_X_CANON_CUBIERTA_DER=36;

	static const int W_REP_CANON_CUERPO=14;
	static const int H_REP_CANON_CUERPO=16;
	static const int X_REP_CANON_CUERPO=388;
	static const int Y_REP_CANON_CUERPO=234;
	static const int OFFSET_X_REP_CANON=121;
	static const int OFFSET_Y_REP_CANON=170;


	static const int X_CAJA_AVISO_CANON_IZQ=4; //Con respecto al cuerpo.
	static const int X_CAJA_AVISO_CANON_DER=228;	
	static const int Y_CAJA_AVISO_CANON=48;
	static const int W_CAJA_AVISO_CANON=24;
	static const int H_CAJA_AVISO_CANON=5;

	///////////////////////////////////////////////////////////////////////
	//Propiedades...

	private:

	//Partes lógicas...
	int estado;
	float posicion_y_final;
	bool batalla_iniciada;
	unsigned short int senal_sonido;

	Actor * blanco; //Una referencia a aquello a lo que está disparando.

	Pata pata_exterior_izq;
	Pata pata_interior_izq;
	Pata pata_exterior_der;
	Pata pata_interior_der;
	Cuerpo cuerpo;
	Cubierta cubierta_izq;
	Cubierta cubierta_der;
	Punto_debil_boss_02_canon punto_debil_canon_izq;
	Punto_debil_boss_02_canon punto_debil_canon_der;
	Punto_debil_boss_02_pata punto_debil_pata_exterior_izq;
	Punto_debil_boss_02_pata punto_debil_pata_interior_izq;
	Punto_debil_boss_02_pata punto_debil_pata_exterior_der;
	Punto_debil_boss_02_pata punto_debil_pata_interior_der;
	Pincho pincho_pata_exterior_izq;
	Pincho pincho_pata_interior_izq;
	Pincho pincho_pata_exterior_der;
	Pincho pincho_pata_interior_der;
	Laser_trasero laser_trasero_izq;
	Laser_trasero laser_trasero_der;

	SDL_Rect caja_canon_izq;
	SDL_Rect caja_canon_der;
	SDL_Rect caja_punto_debil_pata_interior_izq;
	SDL_Rect caja_punto_debil_pata_interior_der;
	SDL_Rect caja_punto_debil_pata_exterior_izq;
	SDL_Rect caja_punto_debil_pata_exterior_der;

	//Partes gráficas... Tiene muchas.
	DLibV::Representacion_bitmap_dinamica * rep_cuerpo;
	DLibV::Representacion_bitmap_dinamica * rep_canon_cuerpo;
	DLibV::Representacion_bitmap_dinamica * rep_cuerpo_punto_debil;
	DLibV::Representacion_bitmap_dinamica * rep_cubierta_izq;
	DLibV::Representacion_bitmap_dinamica * rep_cubierta_izq_punto_debil;
	DLibV::Representacion_bitmap_dinamica * rep_cubierta_der;
	DLibV::Representacion_bitmap_dinamica * rep_cubierta_der_punto_debil;
	DLibV::Representacion_bitmap_dinamica * rep_pata_interior_izq;
	DLibV::Representacion_bitmap_dinamica * rep_pata_interior_der;
	DLibV::Representacion_bitmap_dinamica * rep_pata_exterior_izq;
	DLibV::Representacion_bitmap_dinamica * rep_pata_exterior_der;
	DLibV::Representacion_bitmap_dinamica * rep_pata_interior_izq_punto_debil;
	DLibV::Representacion_bitmap_dinamica * rep_pata_interior_der_punto_debil;
	DLibV::Representacion_bitmap_dinamica * rep_pata_exterior_izq_punto_debil;
	DLibV::Representacion_bitmap_dinamica * rep_pata_exterior_der_punto_debil;
	DLibV::Representacion_bitmap_dinamica * rep_canon_izq_punto_debil;
	DLibV::Representacion_bitmap_dinamica * rep_canon_der_punto_debil;
	DLibV::Representacion_bitmap_dinamica * rep_pincho_pata_interior_izq;
	DLibV::Representacion_bitmap_dinamica * rep_pincho_pata_exterior_izq;
	DLibV::Representacion_bitmap_dinamica * rep_pincho_pata_interior_der;
	DLibV::Representacion_bitmap_dinamica * rep_pincho_pata_exterior_der;
	DLibV::Representacion_patron_dinamica * rep_laser_trasero_izq;
	DLibV::Representacion_patron_dinamica * rep_laser_trasero_der;
	DLibV::Primitiva_grafica_caja_dinamica * rep_aviso_laser_trasero_izq;
	DLibV::Primitiva_grafica_caja_dinamica * rep_aviso_laser_trasero_der;


	///////////////////////////////////////////////////////////////////////
	//Implementaciones del fondo de la jerarquía...

	protected: 

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	void procesar_turno(float=0);
	bool procesar_colision_proyectil(Proyectil *);
	unsigned short int actuar_sonido() {return SND_NADA;} //se pedirán a mano en su turno porque puede hacer más de un sonido.
	short int puntuacion_por_destruir() {return 1500;}
	void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);
	void cuentame() {std::cout<<"SOY BOSS_02"<<std::endl;}
	short unsigned int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_ACTIVIDAD_BOSS_02;}
	void aceptar_visitante(Visitante& v) {v.visitar(*this);}
	unsigned int acc_columnas_humo_explosion() const {return 0;}
	bool en_colision_con(Actor const&) const;
	virtual bool es_afectado_por_edu() const {return false;}
	virtual bool es_fuera_de_juego() const {return false;} //Nunca estará en fuera de juego y por tanto no se borrará por salir de la pantalla.
		
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

	////////////////////////////////////////////////////////////////////////
	//Reimplementaciones...

	public:

	virtual unsigned short int acc_profundidad_representacion() const;
	virtual bool realiza_actividad() const;
	virtual bool es_activo() const;
	bool procesa_turno() const {return true;} //Siempre procesa...
	void iniciar_batalla() 
	{
		batalla_iniciada=true;
		estado=E_VISIBLE;
	}

	////////////////////////////////////////////////////////////////////////
	//Métodos propios.

	private:

	//....	
	void generar_disparos(const Pincho&);
	void generar_disparos(const Cubierta&, const Actor * const);
	void generar_disparos(const Cuerpo&, const Actor * const);
	void controlar_posicion_partes();
	void generar_proyectil(int, int, DLibH::Vector_2d, unsigned int, float=1);
	void generar_explosion_parte(const SDL_Rect&);
	void generar_humo_desde_caja(const SDL_Rect&);

	public:

	Boss_02();
	virtual ~Boss_02();
	void preparar_actor_mapa_manual(float, float, float, float);

	bool es_batalla_iniciada() {return batalla_iniciada;}
	bool es_destruido() const {return estado==E_DESTRUIDO;}
	void iniciar_caida() {estado=E_CAYENDO;}
	void establecer_blanco(Actor * p_b) {blanco=p_b;}

	bool debe_solicitar_canal() const {return laser_trasero_izq.debe_solicitar_canal() || laser_trasero_der.debe_solicitar_canal();}
	void obtener_canal(DLibA::Canal_audio);
	unsigned short int obtener_senales_sonido() const {return senal_sonido;}
};

#endif
