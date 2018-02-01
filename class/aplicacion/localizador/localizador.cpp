#include "localizador.h"

DLibH::t_lista_nombres_archivo Localizador::obtener_lista_archivos()
{
	DLibH::t_lista_nombres_archivo resultado;

	resultado.push_back(DLibH::t_cadena("data/localizacion/loc"));

	return resultado;
}

DLibH::t_cadena const& Localizador::cadena_no_cargado() {return this->cadenas[C_NO_CARGADO];}
DLibH::t_cadena const& Localizador::cadena_no_encontrado() {return this->cadenas[C_NO_ENCONTRADO];}

void Localizador::inicializar(unsigned short int p_idioma)
{		
	DLibH::Localizador_base::cambiar_idioma(p_idioma);

	this->insertar_cadena(0, "NO SE HA ENCONTRADO EL ARCHIVO");	
	this->insertar_cadena(1, "NO SE HA ENCONTRADO LA CLAVE");
//	Localizador_base::inicializar();
}
