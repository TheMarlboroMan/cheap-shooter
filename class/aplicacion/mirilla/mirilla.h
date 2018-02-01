#ifndef MIRILLA_H
#define MIRILLA_H

#include "../actor/actor.h"
#include "../representable/representable.h"

class Mirilla:public Actor, public Representable
{
	private:
	
	bool activa_gamepad;
	float input_gamepad_x;
	float input_gamepad_y;

	protected:

	void establecer_posicion();
	void establecer_representacion();
	void inicializar();
	
	public:

	void procesar_valores_gamepad(float, float);
	void guardar_valores_gamepad(float, float);
	void procesar_turno(float=0);
	bool es_activa_gamepad() const {return this->activa_gamepad;}
	void gamepad_activo(bool p_valor) {this->activa_gamepad=p_valor;}

	Mirilla();
	virtual ~Mirilla();
	void ir_a(float, float);
	void cuentame() {std::cout<<"SOY MIRILLA"<<std::endl;}
	void establecer_estado_disparo(bool);
	void mostrar_como_escudo();
};

#endif
