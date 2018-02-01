#ifndef HERRAMIENTAS_H
#define HERRAMIENTAS_H

#include <cstring> //Para memset
#include <fstream>
#include <iostream>
#include <string>
#include <locale>
#include <cmath>
#include <cctype>
#include <sstream>
#include <vector>
#include <cstdio>

namespace DLibH
{

class Herramientas
{
	private:

	Herramientas() 
	{
		//MUhaahahaaaaaahaahaa!
	}

	static std::stringstream sstream;

	public:

	static unsigned int digitos_en_entero(const int&);
	static unsigned short int tipo_inicio_utf8(const char c);
	static bool es_inicio_utf8_2b(const char c);
	static bool es_inicio_utf8_3b(const char c);
	static bool es_inicio_utf8_4b(const char c);
	static bool es_inicio_utf8_5b(const char c);
	static bool es_inicio_utf8_6b(const char c);
	static unsigned char utf8_2b_a_uchar(char a, char b);
	static int abs(int &p_valor);
	static char * cadena_w_a_8(const wchar_t * p_cadena);
	static int cadena_a_entero(const wchar_t * p_cadena);
	static int cadena_a_entero(const char * p_cadena);
	static std::string entero_a_cadena(int numero);
	static std::string float_a_cadena(float numero);
	static std::string cadena_a_alfanumerica_normalizada(const char *, const char);
	static std::string cadena_a_alfanumerica_normalizada(std::string const &, const char);
	static std::string cadena_a_maximo_ancho(std::string const &, unsigned int);
	static std::string cadena_desde_fichero(const char *);
	static float grados_a_radianes(float);
	static float round(float);
	static float angulo_entre_puntos(float, float, float, float);
	static bool validar_email(std::string);
	static int cadena_a_entero(const std::string &);
	static std::vector<std::string> explotar(const std::string &, const char);
	static float porcentaje(float, float);
	static int obtener_aleatorio_tramo(int, int);
};

}
#endif
