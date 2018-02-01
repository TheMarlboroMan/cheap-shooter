#ifndef ESCUDO_H
#define ESCUDO_H

#include <vector>
#include "../../motor_sdl/herramientas/herramientas/herramientas.h"
#include "../actor/actor.h"
#include "../proyectil/proyectil.h"
#include "../escudo_parte/escudo_parte.h"

/*El escudo está siempre en la misma posición que el jugador. A partir del 
punto al que apunte la mirilla despliega a una distancia "DISTANCIA" una
cantidad "CANTIDAD_PARTES" de objetos que despliegan los proyectiles. El 
"ANGULO_MITAD" se usa para determinar a cuántos grados de "angulo" por cada
lado se despliegan como máximo.

Los estados sirven para determinar el estado actual que son como "animaciones"
para el escudo.

A cada turno, pone su posición a donde esté el jugador, apunta su ángulo hacia
la mirilla y despliega en esa posición todas las partes.

A la hora de ser representado, no tiene una representación propia, sino que
devolverá iterativamente la representación de cada una de sus partes, o NULL
cuando ya no queden. Antes de cada representación habrá que reiniciar el 
iterador interno.
*/

class Escudo:public Actor
{
	private:

	static const int DISTANCIA=48;
	static const unsigned int ANGULO=30; //22; //Ángulo inicial.
	static const unsigned int MAX_ANGULO=120;	//Ángulo máximo.
	static const unsigned int CANTIDAD_ANGULO_AMPLIADO=30; //Cantidad de ángulo que consigues por cada item.

	static const unsigned int CANTIDAD_PARTES=5;
	static const unsigned int MAX_CANTIDAD_PARTES=11;
	static const unsigned int CANTIDAD_PARTES_AMPLIADO=2;

	static const float MS_DESPLIEGUE_ESCUDO; //Tiempo que tarda en desplegarse plenamente.	
	static const unsigned int MAX_CARGA_INICIAL=700; //Máxima carga incial.
	static const unsigned int MAX_CARGA_AMPLIADA=1200; //Máxima carga que puedes tener.

	static const unsigned int CANTIDAD_CARGA_AMPLIADA=100; //Cantidad de carga que consigues por cada item.
	static const float GASTO_CARGA_SEG;	 //Carga gastada en un segundo.
	static const float RECUPERACION_CARGA_SEG; //Carga recuperada en un segundo.

	static const unsigned int MIN_CARGA_USO=100;	//Mínimo de carga que puede usarse.

	enum estados{
	E_DESPLIEGUE,
	E_ACTIVO,
	E_CIERRE};

	typedef std::vector<Escudo_parte *> t_vector_partes;

	//Información del escudo en si.

	float angulo; //De posición...
	float angulo_despliegue; //De despliegue...
	float multiplicador_despliegue;
	unsigned short int partes_actuales;
	unsigned short int estado;

	//Información de la carga del escudo...
	unsigned int max_carga_escudo;
	float carga_escudo_actual;
	bool activo;
	
	t_vector_partes partes;
	t_vector_partes::iterator iterador_representacion;
	t_vector_partes::iterator iterador_colision;
	t_vector_partes::iterator iterador_turno;
	t_vector_partes::iterator iterador_fin;

	void establecer_posicion();
	void construir_vector_de_partes();
	void destruir_vector_de_partes();
	void establecer_transparencia_partes(unsigned short int);

	public:

	Escudo();
	~Escudo();

	void establecer_posicion_y_angulo_desde_actores(const Actor&, const Actor&);
	void procesar_turno(float);
	void procesar_turno_escudos(float);

	void reiniciar_pasos_representacion();
	DLibV::Representacion * acc_representacion();

	bool comprobar_colision_con(const Proyectil&);
	void cuentame() {std::cout<<"SOY EL ESCUDO!"<<std::endl;}

	bool sumar_duracion();
	bool sumar_escudos();
	void reiniciar();
	bool activar();
	bool desactivar();
	bool es_usable() const {return this->carga_escudo_actual > MIN_CARGA_USO;}
	bool es_activo() const {return this->activo;}
	unsigned int acc_max_carga_escudo() const {return this->max_carga_escudo;}
	float acc_carga_escudo_actual() const {return this->carga_escudo_actual;}

	

};

#endif
