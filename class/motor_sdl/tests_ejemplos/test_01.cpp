/*
Test 1: Funcionamiento de Utilidades_graficas_SDL::posicion_dimension_segmentos

Una vez superpuestos dos segmentos hay cinco casos de posicionamiento:

1) A está a la izquierda de B -> solución parcial
2) B está a la izquierda de A -> solución parcial
3) A está dentro de B -> solución completa
4) B está dentro de A -> solución completa
5) A y B son iguales -> solución completa (identidad)

Dónde solución parcial y completa son métodos de resolución que están 
implementados más abajo.

Para esto hemos hecho un banco de pruebas con estos segmentos, que recogen todas
las posibilidades:

numero	aini	afin	bini	bfin	tipo

0	-8, 	-4, 	0, 	4	A fuera de B, a la izquierda
1	0, 	4, 	-8, 	-4	A fuera de B, a la derecha.
2	-8, 	-4, 	-5, 	-1	A parcialmente dentro de B, a la izquierda
3	4, 	9, 	0, 	5	A parcialmente dentro de B, a la derecha
4	-1, 	3, 	-4, 	6	A dentro de B por completo
5	2, 	13, 	3, 	11	B dentro de A por completo
6	-6, 	1, 	-7, 	1	A dentro de B por completo (coincide un extremo)
7	2, 	8, 	2, 	10	A dentro de B por completo (coincide el otro extremo)
8	-8, 	-3, 	-6, 	-3	B dentro de A por completo (coincide un extremo)
9	5, 	11, 	5, 	8	B dentro de A por completo (coincide el otro extremo)
10	3, 	8, 	3, 	8	A y B son iguales.

En estos segmentos se han medido seis criterios:

c1=aini < bini;
c2=afin < bfin;
c3=bini < aini;
c4=bfin < afin;
c5=aini == bini;
c6=afin == bfin;

Los resultados son una tabla tal que así:
encima podemos usar una tabla así:

caso /criterio	1	2	3	4	5	6
0		-	-	-	-	-	-	
1		-	-	-	-	-	-
2		v	v	x	x	x	x
3		x	x	v	v	x	x
4		x	v	v	x	x	x
5		v	x	x	v	x	x
6		x	x	v	x	x	v
7		x	v	x	x	v	x
8		v	x	x	x	x	v
9		x	x	x	v	v	x
10		x	x	x	x	v	v

A partir de la tabla sacamos que c1&&c2 y c3&&c4 se identifican como parciales.
c5 && c6 son identicos.
cualquier caso donde !c1 && !c4 tiene a A dentro de B
cualquier caso donde !c2 && !c3 tiene a B dentro de A.

Y esas son las soluciones.
*/

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include "class/motor_sdl/libDanSDL.h"
#include "class/motor_sdl/defDanSDL.h"

class Auto_test
{
	public: 
	std::string nombre;
	int aini, afin, bini, bfin, ra, rb;

	Auto_test(const char * p_nombre, int paini, int pafin, int pbini, int pbfin)
	{
		this->nombre=p_nombre;
		this->aini=paini;
		this->afin=pafin;
		this->bini=pbini;
		this->bfin=pbfin;
		this->ra=0;
		this->rb=0;		
	}

	void representar()
	{
		std::cout<<"("<<aini<<" "<<afin<<") ("<<bini<<" "<<bfin<<")"<<std::endl;
		std::cout<<" 8765432101234567890123456 "<<std::endl;
		std::cout<<"|";
		for(int i=-8; i<16; i++)
		{
			if(i >= aini && i < afin) std::cout<<"-";
			else std::cout<<" ";
		}
		std::cout<<"|";

		std::cout<<std::endl;

		std::cout<<"|";
		for(int i=-8; i<16; i++)
		{
			if(i >= bini && i < bfin) std::cout<<"-";
			else std::cout<<" ";
		}
		std::cout<<"|";
		std::cout<<std::endl;
	}
};

void procesar_test(Auto_test & v)
{
	std::cout<<v.nombre<<std::endl;
	v.representar();

	if(!Utilidades_graficas_SDL::segmentos_superpuestos(v.aini, v.afin, v.bini, v.bfin))
	{
		std::cout<<"NO SUPERPUESTOS!!"<<std::endl;
	}
	else
	{
		Utilidades_graficas_SDL::posicion_dimension_segmentos(v.aini, v.afin, v.bini, v.bfin, v.ra, v.rb);
		std::cout<<"SOLUCION DESDE "<<v.ra<<" HASTA "<<v.rb<<std::endl;
	}
}

int main(int argc, char ** argv)
{	
	std::vector<Auto_test> v;
	v.push_back(Auto_test("0)", -8, -4, 0, 4));
	v.push_back(Auto_test("1)", 0, 4, -8, -4));
	v.push_back(Auto_test("2)", -8, -4, -5, -1));
	v.push_back(Auto_test("3)", 4, 9, 0, 5));
	v.push_back(Auto_test("4)", -1, 3, -4, 6));
	v.push_back(Auto_test("5)", 2, 13, 3, 11));
	v.push_back(Auto_test("6)", -6, 1, -7, 1));
	v.push_back(Auto_test("7)", 2, 8, 2, 10));
	v.push_back(Auto_test("8)", -8, -3, -6, -3));
	v.push_back(Auto_test("9)", 5, 11, 5, 8));
	v.push_back(Auto_test("10)", 3, 8, 3, 8));

	int i=1;
	
	while(i!=-1)
	{
		std::cout<<">>";
		std::cin>>i;

		if(i >= 0 && i < v.size())
		{
			procesar_test(v[i]);
		}
	}	
}
