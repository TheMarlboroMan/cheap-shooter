#ifndef TUTORIAL_H
#define TUTORIAL_H

#include "../caja_texto/caja_texto.h"
#include "../recursos/recursos.h"

/*
Define la clase tutorial... Un tutorial es la combinación de una caja de 
texto con algo de estado. En este caso es la caja de texto con un contador de 
cuenta atrás.
La caja de texto tiene siempre el mismo modelado de modo que lo único que el 
tutorial recibe es la cadena de texto y la duración.
*/

class Tutorial
{
	private:

	enum estados{
		E_MOSTRANDO,
		E_ESPERANDO,
		E_DESVANECIENDO,
		E_FINALIZADO
	};

	float opacidad;
	float tiempo_restante;
	bool mostrable;
	unsigned int estado;
	Caja_texto caja_texto;

	public:

	Tutorial(const std::string&, const std::string&, float, float, bool=false);
	Tutorial(const Tutorial&);
	~Tutorial();
	Tutorial& operator=(const Tutorial&);

	bool espera_input() const {return estado==E_ESPERANDO;}
	bool es_mostrable() const {return mostrable;}
	void activar() {mostrable=true;}
	void desactivar() {mostrable=false;}
	void procesar_turno(float);
	void desvanecer();
	Caja_texto& acc_caja_texto() {return caja_texto;}
	void test_intercambiar_visibilidad(unsigned int t) {caja_texto.test_intercambiar_visibilidad(t);}
};

#endif
