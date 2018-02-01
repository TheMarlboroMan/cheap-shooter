#include "tabla_probabilidad.h"

void Tabla_probabilidad::cargar_tabla(std::ifstream &p_archivo)
{
	this->TABLA.clear();

	unsigned int cantidad=0;
	unsigned int i=0;
	unsigned int tipo=0;
	unsigned short int probabilidad=0; 

	//Leer la cantidad
	p_archivo.read((char *) &cantidad, sizeof(U_ENTERO_16));

	//Leer tantos items como la cantidad indique...

	while(i < cantidad)
	{
		p_archivo.read((char *) &tipo, sizeof(U_ENTERO_16));
		p_archivo.read((char *) &probabilidad, sizeof(U_ENTERO_8));

		this->TABLA.push_back(Item_tabla(tipo, probabilidad));
		++i;
	}
}

int Tabla_probabilidad::generar_tirada()
{
	unsigned short int TOTAL_ITEMS=this->TABLA.size();

	if(!TOTAL_ITEMS) 
	{
		return TIRADA_INVALIDA;
	}
	else
	{
		unsigned short int porcentaje=rand() % 100;
		unsigned short int acum=0;
		unsigned short int i=0;
		unsigned short int tirada=0;

		std::vector<Item_tabla>::iterator	ini=this->TABLA.begin(),
							fin=this->TABLA.end();

		while(ini < fin)
		{
			tirada=(*ini).probabilidad;

			if(tirada)
			{
				acum+=tirada;
				if(porcentaje < acum) 
				{
					return (*ini).tipo;
				}
			}

			ini++;
		}

		return i;
	}
}
