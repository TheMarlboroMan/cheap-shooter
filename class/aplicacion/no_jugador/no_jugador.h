#ifndef NO_JUGADOR_H
#define NO_JUGADOR_H

#include "../actor/actor.h"
#include "../representable/representable.h"
#include "../definiciones/definiciones.h"

class No_jugador:public Actor, public Representable
{
	private:

	static bool permitir_vector_cero;

	protected:

	static float divisor_movimiento;
	bool marcado_para_borrar;
	bool movimiento_natural;
	short int rebotes_restantes;

	virtual void establecer_representacion()=0;
	void establecer_posicion();
	virtual void establecer_caracteristicas()=0;
	virtual bool puede_rebotar() const=0;

	void establecer_movimiento_natural() {movimiento_natural=true;}
	void anular_movimiento_natural() {movimiento_natural=false;}

		//Con respecto a las heridas...
	static const unsigned int T_HERIDO=100;

	float tiempo_herido;

	//Todo podemos redefinir por completo todo este set... sobre todo para los bosses.
	virtual void procesar_tiempo_herido(float);
	virtual void herir(){tiempo_herido=(float)T_HERIDO / 1000;}
	virtual void procesar_tiempo_herido_ok() {}	//Estas dos están aquí para poder compilar... En aquellas clases
	virtual void procesar_tiempo_herido_herido() {} //que requieren hacer algo cuando están heridas las podemos implementar.

	public:

	No_jugador();
	virtual ~No_jugador();
	virtual void procesar_turno(float=0);
	virtual bool es_fuera_de_juego() const;
	virtual bool es_afectado_por_edu() const {return true;}
	static void establecer_divisor_movimiento(float);
	static void mut_permitir_vector_cero(bool);
	bool es_marcado_para_borrar() const {return this->marcado_para_borrar;}
	void marcar_para_borrar() {this->marcado_para_borrar=true;}
};

//float No_jugador::divisor_movimiento=1;

#endif
