#ifndef ACTOR_JUGABLE_H
#define ACTOR_JUGABLE_H

#include "../actor/actor.h"
#include "../representable/representable.h"

class Actor_jugable:public Actor, public Representable
{
	protected:

	enum estados_aceleracion{
	EA_FRENANDO,
	EA_NADA,
	EA_ACELERANDO
		};

	virtual void establecer_representacion()=0;
	virtual float factor_retraccion_vectores()=0;
	virtual float maximo_vector_desplazamiento()=0;
	virtual float factor_multiplicador_input()=0;

	int input_x;
	int input_y;
	unsigned int estado_aceleracion;
	bool retraer_vectores;
	bool debe_controlar_posiciones;

	void procesar_retraccion_vectores(float);
	void controlar_posiciones();
	void establecer_posicion();

	public:

	Actor_jugable();
	virtual ~Actor_jugable();
	void recibir_input(int, int, float);
	virtual void procesar_turno(float)=0;
	void mut_retraer_vectores(bool p_valor) {this->retraer_vectores=p_valor;}
	void mut_debe_controlar_posiciones(bool p_valor) {this->debe_controlar_posiciones=p_valor;}
};
#endif
