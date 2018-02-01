#ifndef TABLA_PROBABILIDAD_H
#define TABLA_PROBABILIDAD_H

#include <vector>
#include <fstream>
#include <cmath>
#include <cstdlib>

class Tabla_probabilidad
{
	typedef uint32_t U_ENTERO_32;
	typedef uint16_t U_ENTERO_16;
	typedef uint8_t U_ENTERO_8;

	private:

	struct Item_tabla
	{
		unsigned int tipo;
		unsigned short int probabilidad;

		Item_tabla(unsigned int p_tipo, unsigned short int p_prob)
			:tipo(p_tipo), probabilidad(p_prob) 
		{}
	};

	std::vector<Item_tabla> TABLA;

	public:

	static const int TIRADA_INVALIDA=-1;

	void cargar_tabla(std::ifstream &);
	int generar_tirada();
};

#endif
