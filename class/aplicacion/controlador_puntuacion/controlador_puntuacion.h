#ifndef CONTROLADOR_PUNTUACION_H
#define CONTROLADOR_PUNTUACION_H

#include <map>
#include <vector>
#include "../debug_log_header.h"
#include "../decoracion/decoracion_puntuacion.h"


class Controlador_puntuacion
{
	private:

	const static unsigned int CANTIDAD_NIVEL_1_5;
	const static unsigned int CANTIDAD_NIVEL_2;
	const static unsigned int CANTIDAD_NIVEL_3;
	const static unsigned int CANTIDAD_NIVEL_4;
	const static unsigned int CANTIDAD_NIVEL_5;
	const static unsigned int MAX_MULTIPLICADOR;

	float multiplicador_combo;

	unsigned int contador_combo;
	unsigned int cantidad_restante_combo;

	bool al_maximo;

	static const unsigned long int MAX_PUNTUACION=999999999;

	unsigned long int puntuacion;
	unsigned long int puntuacion_representada;
	unsigned int tipo_combo;

	std::map<unsigned int, Decoracion_puntuacion *> decoraciones;
	std::vector<unsigned int> cantidades_necesarias;

	void calcular_multiplicador_combo();
	void calcular_cantidad_restante_combo();

	public:
	
	//Cada uno de estos tipos de combo se corresponde con un recorte.
	enum TIPOS_COMBO {X1=0, X1_5, X2, X3, X4, X5};

	Controlador_puntuacion();
	~Controlador_puntuacion();

	Decoracion_puntuacion * obtener_decoracion_puntuacion(int, float=0, float=0);

	void sumar_puntuacion(int);
	void sumar_puntuacion_representada(int);

	void reiniciar_combo();
	void bajar_combo();
	bool sumar_combo();
	void igualar_representada_a_real() {puntuacion_representada=puntuacion;}

	float acc_multiplicador_combo() const {return this->multiplicador_combo;}
	unsigned long int acc_puntuacion() const {return this->puntuacion;}
	int acc_puntuacion_representada() const {return this->puntuacion_representada;}
	unsigned int acc_tipo_combo() const {return this->tipo_combo;}

	unsigned int acc_cantidad_restante_combo() const {return cantidad_restante_combo;}
	unsigned int acc_contador_combo() const {return contador_combo;}
	bool es_al_maximo() const {return al_maximo;}
};
#endif
