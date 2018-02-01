#ifndef TRAZADOR_EXPLOSION_H
#define TRAZADOR_EXPLOSION_H

/*No es nada visible, simplemente es un actor que tiene una velocidad y vector 
y a cada turno genera alguna decoración. Lo usaremos para poner varios en el
centro de una explosión y añadir humo.*/

#include "obstaculo_actividad.h"
#include "../visitante/visitante.h"

class Trazador_explosion:public Obstaculo_actividad
{
	protected:

	static const int MIN_DURACION=200;
	static const int MAX_EXTRA_ALEATORIO_DURACION=600;

	static const int MIN_VELOCIDAD=50;
	static const int MAX_EXTRA_ALEATORIO_VELOCIDAD=450;

	static const int CONTADOR_HUMO=20;

	static const int PARTES_HUMO=4; //Define los tramos de humo que hay. 
					//En función de la duración que quede 
					//estaremos en uno de los cuatro tramos
					//que dicen el sprite de humo que se usa.

	float duracion_original;
	float duracion;
	mutable float contador_humo;

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	float vel_max() {return 0;}
	float vel_min() {return 0;}

	Trazador_explosion();
	virtual ~Trazador_explosion();

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_TRAZADOR_EXPLOSION;}
	bool es_afectado_por_edu() const {return false;}
	bool desaparece_tras_colision() const {return false;}
	bool es_parametrizable() const {return false;}
	void parametrizar_actor_en_nivel_preparado(const Parametros_actor& a){}	//Ya es la tercera vez que tenemos que escribir este método cuando no se necesita...
	void cuentame() {std::cout<<"SOY TRAZADOR DE EXPLOSION"<<std::endl;}
	unsigned int tipo_a_colision_jugador() const {return T_NADA;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_JUGADOR;}
	unsigned int tipo_a_cuenta() const {return T_NADA;}
	void procesar_turno(float);

	bool es_representable() const {return false;}
	bool es_activo() const {return true;}
	unsigned short int actuar_sonido() {return 0;}
	unsigned short int obtener_parte_humo() const;
	bool puede_soltar_humo() const;
	void configurar(const Actor&, float, float);
	void configurar(float, float, float, float);
	void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
