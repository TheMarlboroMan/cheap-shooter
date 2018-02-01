#ifndef DECORACION_NIVEL_H
#define DECORACION_NIVEL_H

#include "../obstaculo/obstaculo.h"

class Decoracion_nivel:public Obstaculo
{
	private:

	unsigned short int profundidad;

	protected:

	static const int W_DEFECTO=32;
	static const int H_DEFECTO=32; 

	void establecer_representacion();
	void establecer_caracteristicas();
	void actualizar_representacion();

	public:

	Decoracion_nivel(unsigned int); 
	virtual ~Decoracion_nivel();

	void refrescar_propiedades();

	bool es_afectado_por_edu() const {return false;}
	bool desaparece_tras_colision() const {return false;}
	bool es_flexible() const {return true;}
	bool es_redimensionable() const {return true;}
	bool es_colisionable() const {return false;}
	bool es_ordenable_profundidad() const {return true;}
	void recibir_valores_actor_flexible(const Parametros_actor_flexible&);
	void recibir_valores_actor_redimensionable(const Parametros_actor_redimensionable&);
	void recibir_valores_actor_ordenable_profundidad(const Parametros_actor_ordenable_profundidad&);
	void procesar_cambio_dimensiones();
	unsigned int tipo_a_colision_jugador() const {return T_NADA;}
	virtual bool es_fuera_de_juego() const {return false;} //El obstáculo flexible nunca desaparecerá.
	virtual unsigned short int acc_profundidad_representacion() const {return profundidad;}

	virtual void cuentame()=0;
	virtual unsigned short int tipo_actor_mapa()=0;
	virtual unsigned short int limite_min_profundidad() const =0;
	virtual unsigned short int limite_max_profundidad() const =0;


};

//Definimos aquí las dos subclases.

class Decoracion_nivel_fondo:public Decoracion_nivel
{
	public:

	Decoracion_nivel_fondo():Decoracion_nivel(Actor_mapa::P_FONDO) {}
	virtual ~Decoracion_nivel_fondo() {}
	virtual void cuentame() {std::cout<<"SOY DECORACION NIVEL DE FONDO"<<std::endl;}
	virtual unsigned short int tipo_actor_mapa() {return Actor_mapa::T_DECORACION_NIVEL_FONDO;}
	virtual unsigned short int limite_min_profundidad() const {return 0;}
	virtual unsigned short int limite_max_profundidad() const {return Actor_mapa::P_NORMAL-1;}
};

class Decoracion_nivel_frente:public Decoracion_nivel
{
	public:

	Decoracion_nivel_frente():Decoracion_nivel(Actor_mapa::P_FRENTE) {}
	virtual ~Decoracion_nivel_frente() {}
	virtual void cuentame() {std::cout<<"SOY DECORACION NIVEL DE FRENTE"<<std::endl;}
	virtual unsigned short int tipo_actor_mapa() {return Actor_mapa::T_DECORACION_NIVEL_FRENTE;}
	virtual unsigned short int limite_min_profundidad() const {return Actor_mapa::P_FRENTE;}
	virtual unsigned short int limite_max_profundidad() const {return Actor_mapa::P_MAX_PROFUNDIDAD;}
};

#endif
