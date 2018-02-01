#ifndef DEFINICION_RETOS_H
#define DEFINICION_RETOS_H

/*En esta estructura se almacenan como verdadero o falso los valores de los
retos conseguidos para un perfil que se enviarán al server o se recibirán del 
mismo. 
Los retos en el servidor tienen un id propio que es desconocido para el 
resto de la aplicación, de modo que hay que hacer traducciones.
*/

#include <vector>
#include <string>
#include <sstream>
#include "../../motor_sdl/herramientas/herramientas/herramientas.h"

struct Definicion_retos
{
	enum retos{
	R_N_PARTIDAS=0,			//Implementado 0
	R_N_DISPAROS,			//Implementado 1
	R_N_OBSTACULOS_DESTRUIDOS,	//Implementado 2 
	R_N_GEMAS,			//Implementado 3
	R_N_COMBO,			//Implementado 4
	R_N_VIDAS_PERDIDAS,		//Implementado 5
	R_NIVEL_PACIFISTA,		//Implementado 6 
	R_N_PARAR_TIEMPO,		//Implementado 7
	R_N_TELETRANSPORTE,		//Implementado 8 
	R_N_EDU,			//Implementado 9
	R_N_MINAS_DESTRUIDAS,		//Implementado 10
	R_ASTEROIDE_SECRETO,		//Implementado 11
	R_NIVELES_SECRETOS,		//Implementado 12
	R_FINALIZAR_JUEGO,		//Implementado 13
	R_MAXIMIZAR_CANON,		//Implementado 14
	R_NIVEL_CON_TODAS_GEMAS,	//Implementado 15
	R_N_PUNTUACION,			//Implementado 16
	TOTAL_RETOS};

	enum ids{
	ID_N_PARTIDAS=1,
	ID_N_DISPAROS,
	ID_N_OBSTACULOS_DESTRUIDOS, 
	ID_N_GEMAS,
	ID_N_COMBO,
	ID_N_VIDAS_PERDIDAS,
	ID_NIVEL_PACIFISTA,
	ID_N_PARAR_TIEMPO,
	ID_N_TELETRANSPORTE,
	ID_N_EDU,
	ID_N_MINAS_DESTRUIDAS,
	ID_ASTEROIDE_SECRETO,
	ID_NIVELES_SECRETOS,
	ID_FINALIZAR_JUEGO,
	ID_MAXIMIZAR_CANON,
	ID_NIVEL_CON_TODAS_GEMAS,
	ID_N_PUNTUACION
	};

	static const unsigned short int TOTAL_NIVELES=9;
	static const unsigned short int TOTAL_NIVELES_SECRETOS=3;

	static unsigned int id_a_reto(unsigned int p_id)
	{
		switch(p_id)
		{
			case ID_N_PARTIDAS: 			return R_N_PARTIDAS; break;
			case ID_N_DISPAROS: 			return R_N_DISPAROS; break;
			case ID_N_OBSTACULOS_DESTRUIDOS: 	return R_N_OBSTACULOS_DESTRUIDOS; break;
			case ID_N_GEMAS: 			return R_N_GEMAS; break;
			case ID_N_COMBO: 			return R_N_COMBO; break;
			case ID_N_VIDAS_PERDIDAS: 		return R_N_VIDAS_PERDIDAS; break;
			case ID_NIVEL_PACIFISTA: 		return R_NIVEL_PACIFISTA; break;
			case ID_N_PARAR_TIEMPO: 		return R_N_PARAR_TIEMPO;  break;
			case ID_N_TELETRANSPORTE: 		return R_N_TELETRANSPORTE; break;
			case ID_N_EDU: 				return R_N_EDU; break;
			case ID_N_MINAS_DESTRUIDAS: 		return R_N_MINAS_DESTRUIDAS; break;
			case ID_ASTEROIDE_SECRETO: 		return R_ASTEROIDE_SECRETO; break;
			case ID_NIVELES_SECRETOS: 		return R_NIVELES_SECRETOS; break;
			case ID_FINALIZAR_JUEGO: 		return R_FINALIZAR_JUEGO; break;
			case ID_MAXIMIZAR_CANON: 		return R_MAXIMIZAR_CANON; break;
			case ID_NIVEL_CON_TODAS_GEMAS:	 	return R_NIVEL_CON_TODAS_GEMAS; break;
			case ID_N_PUNTUACION: 			return R_N_PUNTUACION;  break;
			default: return 0;
		}
	}

	static unsigned int reto_a_id(unsigned int p_reto)
	{
		switch(p_reto)
		{
			case R_N_PARTIDAS: 			return ID_N_PARTIDAS; break;
			case R_N_DISPAROS: 			return ID_N_DISPAROS; break;
			case R_N_OBSTACULOS_DESTRUIDOS: 	return ID_N_OBSTACULOS_DESTRUIDOS; break;
			case R_N_GEMAS: 			return ID_N_GEMAS; break;
			case R_N_COMBO: 			return ID_N_COMBO; break;
			case R_N_VIDAS_PERDIDAS: 		return ID_N_VIDAS_PERDIDAS; break;
			case R_NIVEL_PACIFISTA: 		return ID_NIVEL_PACIFISTA; break;
			case R_N_PARAR_TIEMPO: 			return ID_N_PARAR_TIEMPO;  break;
			case R_N_TELETRANSPORTE: 		return ID_N_TELETRANSPORTE; break;
			case R_N_EDU: 				return ID_N_EDU; break;
			case R_N_MINAS_DESTRUIDAS: 		return ID_N_MINAS_DESTRUIDAS; break;
			case R_ASTEROIDE_SECRETO: 		return ID_ASTEROIDE_SECRETO; break;
			case R_NIVELES_SECRETOS: 		return ID_NIVELES_SECRETOS; break;
			case R_FINALIZAR_JUEGO: 		return ID_FINALIZAR_JUEGO; break;
			case R_MAXIMIZAR_CANON: 		return ID_MAXIMIZAR_CANON; break;
			case R_NIVEL_CON_TODAS_GEMAS:	 	return ID_NIVEL_CON_TODAS_GEMAS; break;
			case R_N_PUNTUACION: 			return ID_N_PUNTUACION;  break;
			default: return 0; break;
		}
	}
};

struct Estructura_intercambio_retos
{
	bool carga_correcta;
	std::vector<bool> datos;

	Estructura_intercambio_retos(): carga_correcta(false)
	{
		this->datos.insert(this->datos.begin(), Definicion_retos::TOTAL_RETOS, false);
	}

	/*La cadena viene en este formato: 
	id_base_datos.[0/1]|id_base_datos.[0/1]|id_base_datos.[0/1]|...*/

	void cargar_desde_cadena(const std::string & cadena)
	{
		carga_correcta=true;

		const char SEPARADOR_ENTRADA='|';
		const char SEPARADOR_DATO='.';
		
		unsigned int id=0, valor=0;

		std::string linea;
		std::istringstream stream(cadena);
		std::getline(stream, linea);

		std::vector<std::string> partes=DLibH::Herramientas::explotar(linea, SEPARADOR_ENTRADA);
		std::vector<std::string>::iterator ini=partes.begin(), fin=partes.end();

		while(ini < fin)
		{
			std::vector<std::string> pieza=DLibH::Herramientas::explotar((*ini), SEPARADOR_DATO);
			id=DLibH::Herramientas::cadena_a_entero(pieza[0]);
			valor=DLibH::Herramientas::cadena_a_entero(pieza[1]);

			this->datos[Definicion_retos::id_a_reto(id)]=bool(valor);

			ini++;
		}
	}
};

#endif
