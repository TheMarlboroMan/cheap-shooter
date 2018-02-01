#ifndef ESCRITOR_TEXTO_H
#define ESCRITOR_TEXTO_H

#include <iostream>
#include <string>
#include <cctype>
#include "../controles_sdl/controles_sdl.h"

namespace DLibI
{

class Escritor_texto
{
	//Puntero a método llamado callback_tecla que coge un int
	//y devuelve otro an int
//	typedef int (Escritor_texto::*callback_tecla)(int);
//	typedef bool (*callback_tecla)(int);  

	private:

	static const int VALOR_SIN_LIMITAR=-1;

	std::string cadena;
	Controles_SDL & controles;
	unsigned short int codigo_salida;
	int limite_longitud;
	bool obtenida_salida;
	bool modificada_cadena;

	public:

	Escritor_texto(Controles_SDL &p_controles, unsigned short int p_codigo_salida=13):
		controles(p_controles), codigo_salida(p_codigo_salida), 
		limite_longitud(VALOR_SIN_LIMITAR),
		obtenida_salida(false), modificada_cadena(false)
	{}

	~Escritor_texto()
	{}

	void loop_control(bool=true);
	void mut_cadena(std::string const &);
	std::string acc_cadena() const;
	const char * acc_cadena_c() const;
	void limpiar();
	bool es_obtenida_salida() const {return this->obtenida_salida;}
	void reiniciar_salida() {this->obtenida_salida=false;}
	void forzar_salida() {this->obtenida_salida=true;}
	bool es_modificada_cadena() const {return this->modificada_cadena;}
	void establecer_limite_longitud(int p_valor) {this->limite_longitud=p_valor;}
	void limpiar_limite_longitud() {this->limite_longitud=VALOR_SIN_LIMITAR;}
};

} //Fin namespace DLibI

#endif
