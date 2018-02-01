#ifndef OBSTACULO_TRIGGER_CONTACTO_H
#define OBSTACULO_TRIGGER_CONTACTO_H

/*Un trigger.*/

#include "../obstaculo_trigger/obstaculo_trigger.h"

class Obstaculo_trigger_contacto:public Obstaculo_trigger
{
	private:

	unsigned int activable_por;

	enum ACTIVABLE {
		A_JUGADOR=1,
		A_FANTASMA=2
	};

	protected:

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	Obstaculo_trigger_contacto();
	virtual ~Obstaculo_trigger_contacto();

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_TRIGGER_CONTACTO;}
	void cuentame() {std::cout<<"SOY OBSTACULO TRIGGER CONTACTO"<<std::endl;}
	bool es_representable() const {return false;}
	bool es_redimensionable() const {return true;}
	unsigned int tipo_a_colision_jugador() const {return T_DISPARA_ACCION;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_TRIGGER;}

	bool es_activable_por_jugador() const {return activable_por & A_JUGADOR;}
	bool es_activable_por_fantasma() const {return activable_por & A_FANTASMA;}

	void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);

};

#endif
