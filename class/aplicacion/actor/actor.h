#ifndef ACTOR_H
#define ACTOR_H

#include <iostream>
#include "../../motor_sdl/libDanSDL.h"
#include "../recursos/recursos.h"
#include "../definiciones/definiciones.h"
#include "../gestor_mapas_colision/gestor_mapas_colision.h"
#include "../receptor_trigger/receptor_trigger.h"
#include "../parametros_actor/parametros_actor.h"
#include "../accion_trigger/accion_trigger.h" //Para las definiciones de los tipos, mostly.
#include "../accion_trigger_datos/accion_trigger_datos.h"


class Actor
{
	private:

	//Esto podría ser como... la cámara. En todo el juego se usa la pantalla
	//como el límite para controlar cuando algún elemento está fuera de 
	//juego. En los niveles en movimiento podemos usar esta caja para 
	//determinar la posiciones de las cosas. Es un desastre y un pensamiento
	//de última hora para acomoda una nueva funcionalidad, pero es lo que
	//hay.
	static SDL_Rect caja_terreno_juego;

	protected:

	SDL_Rect posicion;
	DLibV::Mapa_colision_bitmap const * mapa_colision;
	virtual void procesar_turno(float)=0;

	//Algunos tipos de actor pueden tener colisión mediante mapa.
	virtual const DLibV::Mapa_colision_bitmap * obtener_mapa_colision() const {return this->mapa_colision;}
	
	float x;
	float y;
	unsigned int w;
	unsigned int h;
	mutable unsigned int superficie;

	DLibH::Vector_2d vector_mov;

	//El actor obliga a implementar un método de este tipo, por si acaso hay que hacer llamadas a representaciones, por ejemplo.
	virtual void establecer_posicion()=0;
	void establecer_posicion(int, int);
	void posicion_desde_coordenadas_propias();
	void recargar_posicion();
	void aplicar_vector(float);
	void aplicar_vector_externo(const DLibH::Vector_2d&, float);
	virtual void controlar_posiciones();

	int acc_int_x() const {return DLibH::Herramientas::round(this->x);}
	int acc_int_y() const {return DLibH::Herramientas::round(this->y);}

	public:

	bool comprobar_mapa_colision(unsigned int x, unsigned int y) const
	{
		if(!mapa_colision) return false;
		else return mapa_colision->comprobar(x, y);
	}

	//Público porque a veces el controlador te dice dónde debes ir.
	void ir_a(float, float);
	float acc_x() const {return this->x;}
	float acc_y() const {return this->y;}
	unsigned int acc_w() const {return this->w;}
	unsigned int acc_h() const {return this->h;}
	void mut_w(unsigned int p_valor) 
	{
		this->posicion.w=p_valor;
		this->w=p_valor;
	}
	void mut_h(unsigned int p_valor) 
	{
		this->posicion.h=p_valor;
		this->h=p_valor;
	}
	float acc_x_centro() const {return this->x + (this->w/2);}
	float acc_y_centro() const {return this->y + (this->h/2);}	

	//Esto es un hack como un castillo. Dado que ancho y alto no se
	//pasan al constructor tenemos que hacer mutable la superficie
	//y calcularla la primera vez en caso de ser solicitada.

	unsigned int acc_superficie() const 
	{
		if(this->superficie==0) this->superficie=this->w*this->h;
		return this->superficie;
	}
	void sumar_vector(DLibH::Vector_2d, float=1);
	void restar_vector(DLibH::Vector_2d, float=1);
	void detener_vector();
	void establecer_vector(DLibH::Vector_2d, float=1);
	void mut_vector_x(float p_valor) {this->vector_mov.x=p_valor;}
	void mut_vector_y(float p_valor) {this->vector_mov.y=p_valor;}

	//Esto es un caso raro: si alguna derivada sobreescribe el método de colisión necesitará esto.
	SDL_Rect acc_posicion() const {return this->posicion;}

	DLibH::Vector_2d acc_vector() const {return this->vector_mov;}
	float acc_vector_mov_x() const {return this->vector_mov.x;}
	float acc_vector_mov_y() const {return this->vector_mov.y;}

	void establecer_vector_desde_actor(const Actor& , float=1);
	void establecer_posicion_desde_actor(const Actor&, float=0, float=0);
	DLibH::Vector_2d obtener_vector_unidad_hasta_actor(const Actor&);
	DLibH::Vector_2d obtener_vector_unidad_hasta_posicion(float, float);
//	static DLibH::Vector_2d obtener_vector_unidad_desde_hasta(Actor *, Actor *);
	static DLibH::Vector_2d obtener_vector_unidad_desde_hasta(const Actor&, const Actor&);

	bool es_en_pantalla() const;
	
	static unsigned int acc_terreno_juego_x() {return Actor::caja_terreno_juego.x;}
	static unsigned int acc_terreno_juego_y() {return Actor::caja_terreno_juego.y;}
	static unsigned int acc_terreno_juego_w() {return Actor::caja_terreno_juego.w;}
	static unsigned int acc_terreno_juego_h() {return Actor::caja_terreno_juego.h;}

	void static establecer_terreno_juego(SDL_Rect const p_valor)
	{
		Actor::caja_terreno_juego.x=p_valor.x;
		Actor::caja_terreno_juego.y=p_valor.y;
		Actor::caja_terreno_juego.w=p_valor.w;
		Actor::caja_terreno_juego.h=p_valor.h;
	}

	Actor();
	virtual ~Actor();

	//Ojo al virtual: nos permitirá redefinirla más adelante (???).
	virtual bool en_colision_con(Actor const&) const;
	virtual bool en_colision_con(const SDL_Rect &) const;
	virtual void cuentame()=0;
	virtual bool recibir_senal_trigger(const Accion_trigger_datos&);

	/*Esto es una ñapa como un castillo: algunos actores (los jefes de fin
	de nivel) chocarán de forma distinta con el jugador y los proyectiles...
	como la colisión se define en esta clase y aún no podemos importar la
	clase proyectil (deriva de esta) la solución es redefinir la colisión
	en las clases de turno preguntando por el tipo de colisión.*/

	enum tipos_colision{
		T_TIPO_COLISION_ESTANDAR=0,
		T_TIPO_COLISION_PROYECTIL
	};

	virtual unsigned short int obtener_tipo_colision() const {return T_TIPO_COLISION_ESTANDAR;}
};

#endif
