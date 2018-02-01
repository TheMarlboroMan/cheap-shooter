#include "representacion_texto_auto.h"

using namespace DLibV;

Representacion_texto_auto::Representacion_texto_auto(const Representacion_texto_auto& p_otra)
	:Representacion_texto(p_otra)
{
	
}

Representacion_texto_auto& Representacion_texto_auto::operator=(const Representacion_texto_auto& p_otra)
{
	Representacion_texto::operator=(p_otra);
	return *this;
}

void Representacion_texto_auto::asignar(const char * p_param)
{
	Representacion_texto::asignar(p_param);
	this->establecer_dimensiones_por_contenido();
}

void Representacion_texto_auto::asignar(const char p_param)
{
	Representacion_texto::asignar(p_param);
	this->establecer_dimensiones_por_contenido();
}

void Representacion_texto_auto::asignar(std::string const& p_param)
{
	Representacion_texto::asignar(p_param);
	this->establecer_dimensiones_por_contenido();
}

/*
Sabiendo el ancho y alto de los glifos podemos recorrer la cadena y hacer
que se establezcan los valores sólos. No podemos olvidar tampoco el espacio entre
líneas.
*/

void Representacion_texto_auto::establecer_dimensiones_por_contenido()
{
	unsigned int cantidad_lineas=0, ancho_maximo=0, pos=0, ini=0, tam=0;
	bool salir=false;
	std::string c_aux;

	if(!this->cadena.size()) 
	{
		DLibH::Log_motor::L()<<DLibH::Log_base_n(1)<<DLibH::Log_base_t()<<"Representacion_texto_auto::establecer_dimensiones_por_contenido() NO HAY CADENA"<<std::endl;
		ancho_maximo=1;
		cantidad_lineas=1;
	}
	else
	{
		do
		{
			pos=this->cadena.find('\n', ini);
		
			//Esto nos garantiza salir en la última iteración.
			salir=(pos==std::string::npos);
			if(salir)
			{
				//Si nos queda una sóla simplemente usamos el total.
				pos=this->cadena.size();
			}

			c_aux=this->cadena.substr(ini, pos-ini);
			ini=pos+1;
			cantidad_lineas++;
			tam=c_aux.size();
			ancho_maximo=tam >= ancho_maximo ? tam : ancho_maximo;
		}while(!salir);

	}

	this->establecer_dimensiones_area(ancho_maximo * this->ancho_glifo, cantidad_lineas * (this->alto_glifo+this->interlineado));	
	this->destruir_recurso();
	this->iniciar_recurso();
}

