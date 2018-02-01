#ifndef OBSTACULO_FLEXIBLE_H
#define OBSTACULO_FLEXIBLE_H

#include "obstaculo.h"

/*
Son los bloques de construcción para los diferentes niveles. Invisibles,
pero colisionables.
*/

class Obstaculo_flexible:public Obstaculo
{
	protected:

	unsigned int tipo_actor_flexible;
	bool desaparece_colision;
	bool afectado_edu;

	void establecer_representacion();
	void establecer_caracteristicas();
	void establecer_mapa_colision();

	void crear_representacion(const SDL_Rect&);
	void actualizar_representacion();

	public:

	Obstaculo_flexible(unsigned int, unsigned int, bool, bool);
	virtual ~Obstaculo_flexible();

	
	void refrescar_propiedades();

	unsigned short int tipo_actor_mapa() {return this->tipo_actor_flexible;}
	bool es_afectado_por_edu() const {return this->afectado_edu;}
	bool desaparece_tras_colision() const {return this->desaparece_colision;}
	bool es_flexible() const {return false;}
	unsigned int tipo_a_colision_proyectil() const {return T_DESTRUYE_PROYECTIL;}
	bool es_redimensionable() const {return tipo_actor_flexible==T_OBSTACULO_FLEX;}
//	void recibir_valores_actor_flexible(const Parametros_actor_flexible&);
	void recibir_valores_actor_redimensionable(const Parametros_actor_redimensionable&);
	void procesar_cambio_dimensiones();
	virtual bool es_fuera_de_juego() const {return false;} //El obstáculo flexible nunca desaparecerá.

	/*Esto de "EN EDITOR" tiene su sentido... Esto sólo es visible desde
	el editor. Fuera del editor no se muestra, para conservar algo de 
	FPS... Es de "ACTOR MAPA"*/

	bool es_representable() const {return EN_EDITOR();} //En juego NO es representable. En editor si.
	//TODO: Quitar cuando hayamos finalizado.
//	bool es_representable() const {return true;} //En juego NO es representable. En editor si.
	void cuentame() {std::cout<<"SOY ACTOR FLEXIBLE "<<this->tipo_actor_flexible<<std::endl;}	
};

#endif
