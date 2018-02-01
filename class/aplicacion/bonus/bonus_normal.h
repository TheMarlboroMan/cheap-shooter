#ifndef BONUS_NORMAL_H
#define BONUS_NORMAL_H

#include "bonus.h"

class Bonus_normal:public Bonus
{
	private:

	static const float BRILLO;

	protected:
	float brillo;
	void establecer_representacion();
	void establecer_caracteristicas();
	bool recogido;

	public:

	Bonus_normal(unsigned int);
	virtual ~Bonus_normal();

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_BONUS_NORMAL;}
	void cuentame() {std::cout<<"SOY BONUS"<<std::endl;}
	bool es_recogido() const {return this->recogido;}
	void procesar_turno(float=0);
	void recoger() {this->recogido=true;}
	unsigned int tipo_a_colision_proyectil() const {return T_DESTRUYE_PROYECTIL_ACTOR;}
	unsigned int tipo_a_colision_jugador() const {return T_AUMENTA_CONTADOR_NIVEL;}
	unsigned int tipo_a_cuenta() const {return T_CUENTA_GEMA;}

//	bool en_colision_con(Actor const&);
};

#endif
