#ifndef CAMPO_ESTRELLAS_H
#define CAMPO_ESTRELLAS_H

//#include <list>
#include <vector>
#include <algorithm>
#include "../definiciones/definiciones.h"
#include "../../motor_sdl/libDanSDL.h"

class Campo_estrellas
{
	unsigned int max_puntos;
	const static unsigned int MAX_ESTRELLAS=400;
	const static unsigned int VALOR_CAMBIO_ESTRELLAS_IGNORAR=401;
	float divisor_movimiento;
	float rojo, azul, verde;
	Uint8 nrojo, nazul, nverde;
	Uint8 salto_color;
	bool cambio_color;

	private:

	struct Punto
	{		
		float x;
		float y;
		float velocidad;
		Uint32 color;

		Punto(){}

		~Punto(){}

		bool fuera_de_juego() const
		{
			return this-> y >= Medidas::ALTO_TERRENO_JUEGO-1;
		}

/*
		Lo quitamos. Desenrollar este método es una optimización enorme.

		void turno(float p_delta=0)
		{
			this->y+=this->velocidad * p_delta;
			if(this->fuera_de_juego()) this->establecer_valores();
		}
*/

		void establecer_valores(bool y_aleatorio=false)
		{
			this->x=rand()% Medidas::ANCHO_TERRENO_JUEGO;
			this->y=y_aleatorio ? rand()% Medidas::ALTO_TERRENO_JUEGO-1 : 0;
			this->velocidad=( (rand()%20)+1 ) * 20;

			int r=rand()%255;
			int g=rand()%255;
			int b=rand()%255;
			this->color=DLibV::Gestor_color::color(r,g,b);
		}
	};

	typedef std::vector<Punto> t_lista_puntos;
	typedef std::vector<Punto>::iterator t_iterator_lista_puntos;

	t_lista_puntos puntos;
	DLibV::Representacion_pixeles_estatica * representacion;

	void establecer_representacion();
	void preparar_puntos();
	void generar_punto(bool=false);
	void aplicar_puntos();

	public:

	DLibV::Representacion_pixeles_estatica * acc_representacion() const {return this->representacion;}
	Campo_estrellas();
	~Campo_estrellas();

	void establecer_divisor_movimiento(float);
	void procesar_turno(float=0);
	void procesar_cambio_color(float=0);
	void mut_max_puntos(unsigned int p_valor) 
	{
		if(p_valor==VALOR_CAMBIO_ESTRELLAS_IGNORAR) return;
		else
		{
			this->max_puntos=p_valor;
			if(this->max_puntos > MAX_ESTRELLAS) this->max_puntos=MAX_ESTRELLAS;
		}
	}
	void establecer_color_fondo(Uint8, Uint8, Uint8);
	void transformar_color_fondo(Uint8, Uint8, Uint8, Uint8);
	void restaurar();
	bool es_cambio_color() const {return this->cambio_color;}
};

#endif
