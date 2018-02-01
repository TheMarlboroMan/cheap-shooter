#ifndef ACCION_TRIGGER_COMPUESTA_H
#define ACCION_TRIGGER_COMPUESTA_H

#include <vector>
#include <algorithm>
#include "../accion_trigger/accion_trigger.h"
#include "../actor_mapa/actor_mapa.h"
#include "../trigger/trigger.h"

/*La acción de trigger compuesta es un estructura que contiene una acción
de trigger, los actores que tienen ese trigger y todas las cosas que nos hagan
falta... 

En algún momento nos hemos metido aquí en un embolado terrible al crear 
dependencias circulares entre esta clase y el actor mapa, que debería recibir
un objeto de esta clase para ejecutar sus acciones.

Para resolverlo introduciremos un elemento intermedio "Accion_trigger_datos"
o algo así.
*/

struct Accion_trigger_compuesta
{
	private:

	enum estados{
	E_CUENTA_PRE,
	E_ACCION,
	E_CUENTA_POST,
	E_FINALIZADA
	};

	public:

	Accion_trigger const  * accion;
	Accion_trigger_datos datos;
	Trigger * trigger;
	std::vector<Actor_mapa *> actores;	//Actores a los que se está aplicando.

	unsigned short int estado;	
	int ms_antes;
	int ms_despues;
	bool iniciada;
	bool invalidada;

	bool es_finalizada() const;
	bool es_invalidada() const {return this->invalidada;}
	void invalidar() {this->invalidada=true;}
	void turno(float);
	void eliminar_actor(Actor_mapa *);
	void forzar_finalizacion();

	Accion_trigger_compuesta& operator=(const Accion_trigger_compuesta&);
	Accion_trigger_compuesta(const Accion_trigger * p_accion);
	~Accion_trigger_compuesta();
};

#endif
