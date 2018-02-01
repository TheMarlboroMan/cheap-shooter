#ifndef DEFINICIONES_H
#define DEFINICIONES_H

/*Absurda colección de enums, por conveniencia... Las que estén aquí es porque
más de una clase las usa.*/

struct Definiciones
{
	//Define los artefactos del jugador.
	enum ARTEFACTOS{A_MIN_ARTEFACTO, A_CANON, A_TELETRANSPORTADOR, A_EMISOR_DENSIDAD_UNIFICADA, A_ESCUDO, A_MAX_ARTEFACTO};	

	//Como flags, servirá para que "estructura_info_nivel" determine los artefactos que puede usar.
	enum ARTEFACTOS_PERMITIDOS{P_MIN_ARTEFACTO=0, P_CANON=1, P_TELETRANSPORTADOR=2, P_EMISOR_DENSIDAD_UNIFICADA=4, P_ESCUDO=8, P_MAX_ARTEFACTO=16};

	//Las facciones se usan para ver a qué tipo de actor puede atravesar
	//un proyectil. Básicamente pueden atravesar al jugador, a actores no
	//preparados y a actores de nivel preparado.

	enum FACCIONES{F_NADA=0, F_JUGADOR=1, F_NO_PREPARADO=2, F_PREPARADO=4, F_BONUS=8, F_TRIGGER=16};

	static const float T_MAX_DELTA;
};

struct Medidas
{
	static const int ANCHO_VISTA=800;
	static const int ALTO_VISTA=600;
	static const unsigned int U_ANCHO_VISTA=800;

	static const int ANCHO_TERRENO_JUEGO=800;
	static const int ALTO_TERRENO_JUEGO=600;
	static const unsigned int U_ANCHO_TERRENO_JUEGO=800;
};

//Para no hacer "exit" a saco...

struct Excepcion_salida
{ 
	enum t_error
	{
		E_IMPOSIBLE_ABRIR_FICHERO_NIVEL,
		E_FICHERO_CERRADO_AL_REBOBINAR,
		E_LISTA_NIVELES_CORRUPTA
	};

	int cod; 

	Excepcion_salida(int p_c):cod(p_c) {}

	static const char * traducir(int p_cod)
	{
		switch(p_cod)
		{
			case E_IMPOSIBLE_ABRIR_FICHERO_NIVEL: return "IMPOSIBLE ABRIR FICHERO DE NIVEL"; break;
			case E_FICHERO_CERRADO_AL_REBOBINAR: return "FICHERO CERRADO AL REBOBINARLO"; break;
			case E_LISTA_NIVELES_CORRUPTA: return "LISTA DE NIVELES CORRUPTA O INCORRECTA"; break;						
		}

		return "DESCONOCIDO";
	} 
};

#endif
