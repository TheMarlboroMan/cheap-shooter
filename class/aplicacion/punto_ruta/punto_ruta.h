#ifndef PUNTO_RUTA_H
#define PUNTO_RUTA_H

/*Un punto de ruta.*/

#include "../obstaculo/obstaculo.h"

class Punto_ruta:public Obstaculo
{
	private:

	unsigned int agrupacion;
	unsigned int identificador;
	unsigned int siguiente;
	float velocidad;
	bool instantaneo;

	protected:

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	//Una estructura para ordenarlos... En realidad no sirve de mucho a
	//no ser que prometas que vas a meter el identificador de uno en uno
	//y que el siguiente siempre será el siguiente número. Perfectamente
	//podemos hacer que el orden sea 1, 4, 2, 3, 5... Con lo que ordenarlos
	//es para nada.

	struct Ordenacion
	{
		bool operator() (Punto_ruta * a,Punto_ruta *b) 
		{
			return a->identificador < b->identificador;
		}
	};

	Punto_ruta();
	virtual ~Punto_ruta();

	void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);
	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_PUNTO_RUTA;}

	unsigned int tipo_a_colision_jugador() const {return T_NADA;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_TRIGGER;}
	bool es_afectado_por_edu() const {return false;}

	void cuentame() {std::cout<<"SOY PUNTO DE RUTA"<<std::endl;}
	bool es_representable() const {return false;}
	bool es_parametrizable() const {return true;}
	unsigned int acc_identificador() const {return this->identificador;}
	unsigned int acc_siguiente() const {return this->siguiente;}
	unsigned int acc_agrupacion() const {return this->agrupacion;}
	float acc_velocidad() const {return this->velocidad;}
	virtual bool es_fuera_de_juego() const {return false;}
	bool es_instantaneo() const {return true;}
};

#endif
