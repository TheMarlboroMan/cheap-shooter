#ifndef OBSTACULO_ACTIVIDAD_H
#define OBSTACULO_ACTIVIDAD_H

#include "../actor_mapa/actor_mapa.h"

/*
Esto será una clase para actores del mapa que pueden llevar a cabo
alguna actividad que afecte al resto como por ejemplo, disparar algo o
afectar a sus vectores. Siendo el motor el que tiene que procesar toda la 
actividad tendrá un método "es_activo" para saber si puede actuar. El tipo de
actividad lo podemos sacar del tipo "Actor_mapa" que tenga.

Ojo con esto: desde Actor_mapa y No_jugador venimos ya arrastando unos cuantos
de métodos que implementar al final:

protected: virtual void No_jugador::establecer_representacion()
public: virtual void Obstaculo_actividad::procesar_turno(float)
public: virtual void Actor::cuentame()
protected: virtual void No_jugador::establecer_caracteristicas()
protected: virtual float Actor_mapa::vel_max()
protected: virtual float Actor_mapa::vel_min()
public: virtual short unsigned int Actor_mapa::tipo_actor_mapa()
public: virtual bool Obstaculo_actividad::es_activo()
public: virtual short unsigned int Obstaculo_actividad::codigo_actividad()

Vamos a intentar definirla también como una interface "Visitable" para implementar
el patrón visitante en el controlador de juego...

*/

class Visitante;

class Obstaculo_actividad:public Actor_mapa
{
	private:

	bool actua_fuera_de_pantalla;

	protected:

	bool es_actua_fuera_de_pantalla() const {return actua_fuera_de_pantalla;}
	void mut_actua_fuera_de_pantalla(bool v) {actua_fuera_de_pantalla=v;}
	void limitar_valor_minimo_tiempo(float &p_valor)
	{
		if(p_valor < Definiciones::T_MAX_DELTA) p_valor=Definiciones::T_MAX_DELTA;
	}

	bool restar_y_restaurar_valor(float &p_valor, float p_max, float p_delta)
	{
		p_valor-=p_delta;

		if(p_valor > 0)
		{
			return false;
		}
		else
		{
			p_valor=p_max + (-p_valor);
			return true;
		}
	}
	
	public:

	Obstaculo_actividad();
	virtual ~Obstaculo_actividad();

	virtual bool realiza_actividad() const;
	virtual bool procesa_turno() const;

	//Toda esta es la interface a implementar.

	virtual void parametrizar_actor_en_nivel_preparado(const Parametros_actor&)=0;
	virtual void procesar_turno(float=0);
	virtual bool es_activo() const=0;
	bool es_parametrizable() const {return true;}
	bool desaparece_tras_colision() const {return false;}

	virtual unsigned short int actuar_sonido()=0;
	virtual void aceptar_visitante(Visitante&)=0;	//Este método define la interfaz para el patrón visitante.
};

#endif
