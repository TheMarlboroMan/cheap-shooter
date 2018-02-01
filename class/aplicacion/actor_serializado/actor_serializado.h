#ifndef ACTOR_SERIALIZADO_H
#define ACTOR_SERIALIZADO_H

/*Esto es un struct que se usa en la carga y guardado de niveles... No tiene
mucha historia, realmente. El tipo se correspondería con 
Actor_mapa::tipo_actor_mapa y el resto con las coordenadas. Al no tener
mucha historia simplemente se queda como cabecera, sin archivo cpp y sin 
compilar a O.*/

#include "../parametros_actor/parametros_actor.h"

struct Actor_serializado
{
	typedef uint16_t U_ENTERO_16;
	typedef int16_t ENTERO_16;
	typedef uint8_t U_ENTERO_8;

	U_ENTERO_16 tipo;
	ENTERO_16 x;
	ENTERO_16 y;
	U_ENTERO_16 tag;
	Parametros_actor parametros;
	Parametros_actor_flexible params_rep;
	Parametros_actor_redimensionable params_redim;
	Parametros_actor_ordenable_profundidad params_profundidad;

	U_ENTERO_8 parametrizable;
	U_ENTERO_8 representacion_flexible;
	U_ENTERO_8 redimensionable;
	U_ENTERO_8 ordenable_profundidad;

	bool valido;

	Actor_serializado()
		:tipo(0), x(0), y(0), tag(0), valido(true)
	{}

	Actor_serializado(U_ENTERO_16 p_tipo, ENTERO_16 p_x, ENTERO_16 p_y, 
		U_ENTERO_16 p_tag, 
		const Parametros_actor& params,
		const Parametros_actor_flexible& p_params_rep,
		const Parametros_actor_redimensionable& p_params_redim,
		const Parametros_actor_ordenable_profundidad& p_params_profundidad
		)
		:tipo(p_tipo), x(p_x), y(p_y), tag(p_tag), parametros(params), 	
		params_rep(p_params_rep), params_redim(p_params_redim),
		params_profundidad(p_params_profundidad),
		parametrizable(0), representacion_flexible(0), 
		redimensionable(0), ordenable_profundidad(0), valido(true)
	{}

	Actor_serializado(const Actor_serializado& o)
		:tipo(o.tipo), x(o.x), y(o.y), tag(o.tag), 
		parametros(o.parametros), params_rep(o.params_rep), 
		params_redim(o.params_redim),
		params_profundidad(o.params_profundidad),  
		parametrizable(o.parametrizable), 
		representacion_flexible(o.representacion_flexible), 
		redimensionable(o.redimensionable), 
		ordenable_profundidad(o.ordenable_profundidad), valido(o.valido)
	{}

	Actor_serializado& operator=(const Actor_serializado& p_otro)
	{
		tipo=p_otro.tipo;
		x=p_otro.x;
		y=p_otro.y;
		tag=p_otro.tag;
		parametros=p_otro.parametros;
		params_rep=p_otro.params_rep;
		params_redim=p_otro.params_redim;
		params_profundidad=p_otro.params_profundidad;

		parametrizable=p_otro.parametrizable;
		representacion_flexible=p_otro.representacion_flexible;		
		redimensionable=p_otro.redimensionable;
		ordenable_profundidad=p_otro.ordenable_profundidad;

		valido=p_otro.valido;

		return *this;
	}

	void serializar(std::ofstream &archivo)
	{
		archivo.write((char *) &this->tipo, sizeof(U_ENTERO_16));
		archivo.write((char *) &this->x, sizeof(ENTERO_16));
		archivo.write((char *) &this->y, sizeof(ENTERO_16));
		archivo.write((char *) &this->tag, sizeof(U_ENTERO_16));

		archivo.write((char *) &this->parametrizable, sizeof(U_ENTERO_8));
		if(this->parametrizable!=0) this->parametros.serializar(archivo);

		archivo.write((char *) &this->representacion_flexible, sizeof(U_ENTERO_8));
		if(this->representacion_flexible!=0) this->params_rep.serializar(archivo);

		archivo.write((char *) &this->redimensionable, sizeof(U_ENTERO_8));
		if(this->redimensionable!=0) this->params_redim.serializar(archivo);

		archivo.write((char *) &this->ordenable_profundidad, sizeof(U_ENTERO_8));
		if(this->ordenable_profundidad!=0) this->params_profundidad.serializar(archivo);
	}


	static Actor_serializado deserializar(std::ifstream &archivo)
	{
		Actor_serializado temp;

                archivo.read((char *) &temp.tipo, sizeof(U_ENTERO_16));
                archivo.read((char *) &temp.x, sizeof(ENTERO_16));
                archivo.read((char *) &temp.y, sizeof(ENTERO_16));
                archivo.read((char *) &temp.tag, sizeof(U_ENTERO_16));

                archivo.read((char *) &temp.parametrizable, sizeof(U_ENTERO_8));
		if(temp.parametrizable!=0) temp.parametros.deserializar(archivo);

                archivo.read((char *) &temp.representacion_flexible, sizeof(U_ENTERO_8));
		if(temp.representacion_flexible!=0) temp.params_rep.deserializar(archivo);

                archivo.read((char *) &temp.redimensionable, sizeof(U_ENTERO_8));
		if(temp.redimensionable!=0) temp.params_redim.deserializar(archivo);

                archivo.read((char *) &temp.ordenable_profundidad, sizeof(U_ENTERO_8));
		if(temp.ordenable_profundidad!=0) temp.params_profundidad.deserializar(archivo);

		return temp;
	}

	void invalidar()
	{
		this->valido=false;
		this->parametrizable=0;
		this->representacion_flexible=0;
		this->redimensionable=0;
		this->ordenable_profundidad=0;
		this->tipo=0;
		this->x=0;
		this->y=0;
		this->tag=0;
	}
};

#endif
