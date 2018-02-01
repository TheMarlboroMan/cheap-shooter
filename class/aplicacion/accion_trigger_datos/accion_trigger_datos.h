#ifndef ACCION_TRIGGER_DATOS_H
#define ACCION_TRIGGER_DATOS_H

#include <map>
#include <iostream>

/*Esta es la estructura de datos que reciben los actores para cuando tienen
que procesar una acción de trigger. Es un cajón desastre: dentro está lo que
tiene que ver con los tipos, con los "ir_a" y con las rutas a seguir. 

Posteriormente hemos metido aquí también otras estructuras de datos: las que
tienen que ver con los cambios de color del fondo, por ejemplo.

Podríamos hacer varias subclases, cada una con su especialidad, pero mejor
vamos a dejarlo o no terminaremos jamás.*/

class Accion_trigger_datos
{
	private:

	unsigned short int tipo;
	
	public:

	struct Datos_posicion
	{
		float x;
		float y;
		float vel;
		bool instantaneo;
		unsigned int identificador;	//Esto es redundante: cuando esté metido en un mapa la clave es el identificador...
		unsigned int siguiente;

		Datos_posicion(float p_x=0, float p_y=0, float p_vel=0, bool p_ins=false, unsigned int p_id=0, unsigned int p_sig=0):
			x(p_x), y(p_y), vel(p_vel), instantaneo(p_ins), identificador(p_id), siguiente(p_sig)
		{}

		void asignar(float p_x=0, float p_y=0, float p_vel=0, bool p_ins=false, unsigned int p_id=0, unsigned int p_sig=0)
		{
			this->x=p_x;
			this->y=p_y;
			this->vel=p_vel;
			this->instantaneo=p_ins;
			this->identificador=p_id;
			this->siguiente=p_sig;
		}
	};

	//Información acerca de los datos de la ruta.
	
	struct Datos_ruta
	{
		std::map<unsigned int, Datos_posicion> mapa_puntos;
		unsigned int punto_actual;

		Datos_ruta(): punto_actual(0)
		{}

		Datos_ruta& operator=(const Datos_ruta& p_otro)
		{
			this->mapa_puntos=p_otro.mapa_puntos;
			this->punto_actual=p_otro.punto_actual;
			return *this;
		}
	};
/*
	//Información acerca de los cambios de color...
	
	struct Datos_propiedades_fondo
	{
		unsigned short int rojo;
		unsigned short int verde;
		unsigned short int azul;
		unsigned short int velocidad;
		unsigned int estrellas;
		bool iniciado;

		Datos_propiedades_fondo():
			rojo(0), verde(0), azul(0), velocidad(0), estrellas(0),
			inciado(false)
		{}

		&Datos_propiedades_fondo operator=(const Datos_propiedades_fondo& p_otro)
		{
			this->rojo=p_otro.rojo;
			this->verde=p_otro.verde;
			this->azul=p_otro.azul;
			this->velocidad=p_otro.velocidad;
			this->estrellas=p_otro.estrellas;
			this->iniciado=p_otro.iniciado;
			return *this;
		}

		void asignar(unsigned short int pr, unsigned short int pg, 
			unsigned short int pb, unsigned short int pv, unsigned int pe)
		{
			this->rojo=pr;
			this->verde=pg;
			this->azul=pb;
			this->velocidad=pv;
			this->estrellas=pe;
		}
	}
*/	
	private:

	Datos_posicion datos_posicion;
	Datos_ruta datos_ruta;
	//Datos_propiedades_fondo datos_propiedades_fondo;
	int desp_x;
	int desp_y;

	public:

	Accion_trigger_datos(unsigned short int p_tipo): tipo(p_tipo)
	{}

	~Accion_trigger_datos()
	{}

	Accion_trigger_datos& operator=(const Accion_trigger_datos& p_otro)
	{
		this->tipo=p_otro.tipo;
		this->datos_posicion=p_otro.datos_posicion;
		this->datos_ruta=p_otro.datos_ruta;
//		this->datos_propiedades_fondo=p_otro.datos_propiedades_fondo;
		return *this; 
	}

	void asignar_datos_posicion(float p_x, float p_y, float p_vel=0) {this->datos_posicion.asignar(p_x, p_y, p_vel);}
	void insertar_datos_ruta(float p_x, float p_y, float p_vel, bool p_ins, unsigned int p_id, unsigned int p_sig) 
	{
		this->datos_ruta.mapa_puntos.insert(std::pair<unsigned int, Datos_posicion>(p_id, Datos_posicion(p_x, p_y, p_vel, p_ins, p_id, p_sig)));
	}
	void asignar_desplazamientos(unsigned int px, unsigned int py) 
	{
		this->desp_x=px; 
		this->desp_y=py;
	}
	void escoger_punto_ruta(unsigned int p_punto){this->datos_ruta.punto_actual=p_punto;}

	unsigned int acc_tipo() const {return this->tipo;}

	const Datos_posicion& acc_datos_posicion() const {return this->datos_posicion;}
	unsigned int obtener_total_puntos_ruta() const {return this->datos_ruta.mapa_puntos.size();}
	unsigned int obtener_punto_inicial() const {return this->datos_ruta.punto_actual;}

	const Datos_posicion& obtener_punto_ruta(const unsigned int p_valor) const 
	{
		auto res=this->datos_ruta.mapa_puntos.find(p_valor);
		return res->second;
	}

	int acc_desp_x() const {return this->desp_x;}
	int acc_desp_y() const {return this->desp_y;}
	
//	void asignar_propiedades_fondo(unsigned short int pr, unsigned short int pg, unsigned short int pb, unsigned short int pv, unsigned int pe) {this->datos_propiedades_fondo.asignar(pr, pg, pb, pv, pe);}
//	bool es_cambio_fondo_iniciado() const {return this->datos_propiedades_fondo.iniciado;}
//	void iniciar_cambio_fondo() {this->datos_propiedades_fondo.iniciado=true;}
//	const Datos_propiedades_fondo& acc_datos_propiedades_fondo() const {return this->datos_propiedades_fondo;}
};

#endif
