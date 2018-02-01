#ifndef RETO_H
#define RETO_H

#include <string>
#include "../../motor_sdl/video/representacion/representacion_grafica/representacion_bitmap/representacion_bitmap.h"
#include "../../motor_sdl/video/representacion/representacion_grafica/representacion_compuesta/representacion_compuesta.h"
//#include "../../motor_sdl/video/representacion/representacion_grafica/representacion_texto/representacion_texto_auto_dinamica.h"
#include "../../motor_sdl/video/representacion/representacion_grafica/representacion_texto/representacion_texto_fija.h"
#include "../../motor_sdl/video/gestor_recursos_graficos/gestor_recursos_graficos.h"
#include "../../motor_sdl/video/proceso_superficie/proceso_superficie_blanco_negro.h"
#include "../../motor_sdl/herramientas/vector_2d/vector_2d.h"
#include "../recursos/recursos.h"
#include "../definiciones/definicion_retos.h"

/*
La clase reto tiene aquí un par de funcionalidades... En primer lugar tiene un 
enum con los tipos de reto que se usarán para obtener su representación. En 
segundo lugar, tiene una representación compuesta que se podrá servir a la
pantalla.

El funcionamiento es como sigue: cuando se consigue un reto, se sirve un objeto
de esta clase a la clase Controlador_retos. Este objeto se
almacena en un vector del controlador y mientras este lleno, se usará el método
de turno para sólo el primer elemento. El método turno irá moviendo la 
representación por la pantalla. 

A cada turno preguntaremos si hemos finalizado (cuando ya estamos fuera de la
pantalla) y cuando eso ocurra
*/

class Reto
{
	private:

	static const int W_CAJA=336;
	static const int H_CAJA=60;

	static const int X_CAJA_ICONO=8;
	static const int Y_CAJA_ICONO=6;
	static const int W_CAJA_ICONO=48;
	static const int H_CAJA_ICONO=48;

	static const int X_TEXTO=67;
	static const int Y_TEXTO=4;
	static const int W_TEXTO=256;
	static const int H_TEXTO=52;

	static const int POS_X_INICIAL=231;
	static const int POS_Y_INICIAL=-60;

	static const int POS_X_PAUSA=231;
	static const int POS_Y_PAUSA=0;

	static const int VECTOR_X_ENTRADA=0;
	static const int VECTOR_Y_ENTRADA=60;

	static const int VECTOR_X_SALIDA=0;
	static const int VECTOR_Y_SALIDA=-60;
	static const unsigned short int LONGITUD_PAUSA=6;
	
	DLibV::Representacion_compuesta_estatica * representacion;
	DLibV::Representacion_compuesta_estatica * representacion_off;

//	DLibV::Representacion_texto_fija_estatica * representacion;
	std::string titulo;
	std::string texto;
	unsigned short int identificador;
	unsigned short int fase_animacion;
	float tiempo_pausa;
	float x, y;
	DLibH::Vector_2d vector;
	bool finalizado;
	
	bool animar(float, int, int);
	
	public:

	enum animacion{
	A_ENTRADA,
	A_PAUSA,
	A_SALIDA};

	void static debug_reto(unsigned int p_reto, unsigned int p_a, unsigned int p_b)
	{
		std::cout<<p_reto<<" -> ";

		switch(p_reto)
		{
case Definicion_retos::R_N_PARTIDAS: 			std::cout<<"RETO "<<p_a<<" PARTIDAS: "<<p_b<<std::endl; break;
case Definicion_retos::R_N_DISPAROS: 			std::cout<<"RETO "<<p_a<<" DISPAROS: "<<p_b<<std::endl; break;
case Definicion_retos::R_N_OBSTACULOS_DESTRUIDOS: 	std::cout<<"RETO "<<p_a<<" OBSTACULOS DESTRUIDOS: "<<p_b<<std::endl; break;
case Definicion_retos::R_N_GEMAS: 			std::cout<<"RETO "<<p_a<<" GEMAS: "<<p_b<<std::endl; break;
case Definicion_retos::R_N_COMBO: 			std::cout<<"RETO "<<p_a<<" COMBO: "<<p_b<<std::endl; break;
case Definicion_retos::R_N_VIDAS_PERDIDAS: 		std::cout<<"RETO "<<p_a<<" VIDAS PERDIDAS: "<<p_b<<std::endl; break;
case Definicion_retos::R_NIVEL_PACIFISTA: 		std::cout<<"RETO PACIFISTA "<<std::endl; break;
case Definicion_retos::R_N_PARAR_TIEMPO: 		std::cout<<"RETO "<<p_a<<" PARAR TIEMPO: "<<p_b<<std::endl; break;
case Definicion_retos::R_N_TELETRANSPORTE: 		std::cout<<"RETO "<<p_a<<" TELETRANSPORTE: "<<p_b<<std::endl; break;
case Definicion_retos::R_N_EDU: 			std::cout<<"RETO "<<p_a<<" EDU: "<<p_b<<std::endl; break;
case Definicion_retos::R_N_MINAS_DESTRUIDAS: 		std::cout<<"RETO "<<p_a<<" MINAS: "<<p_b<<std::endl; break;
case Definicion_retos::R_ASTEROIDE_SECRETO: 		std::cout<<"RETO ASTEROIDE SECRETO "<<std::endl; break;
case Definicion_retos::R_NIVELES_SECRETOS: 		std::cout<<"RETO NIVELES SECRETOS "<<std::endl; break;
case Definicion_retos::R_FINALIZAR_JUEGO: 		std::cout<<"RETO FINALIZAR JUEGO "<<std::endl; break;
case Definicion_retos::R_MAXIMIZAR_CANON: 		std::cout<<"RETO MAX CANON "<<std::endl; break;
case Definicion_retos::R_NIVEL_CON_TODAS_GEMAS: 	std::cout<<"RETO NIVEL TODAS GEMAS "<<std::endl; break;
		}
	}

	DLibV::Representacion * acc_representacion() {return this->representacion;}
	DLibV::Representacion * acc_representacion_off() {return this->representacion_off;}
	std::string acc_titulo() const {return this->titulo;}
	unsigned short int acc_identificador() const {return this->identificador;}

	void asignar_datos(unsigned int, std::string const &, std::string const &);
	Reto(const Reto&);
	Reto();
	~Reto();
	Reto& operator=(const Reto&);

	void turno(float);
	bool es_finalizado() const {return this->finalizado;}
};

#endif
