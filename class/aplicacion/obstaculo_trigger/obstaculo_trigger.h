#ifndef OBSTACULO_TRIGGER_H
#define OBSTACULO_TRIGGER_H

/*Un trigger.*/

#include "../obstaculo/obstaculo.h"
#include "../trigger/trigger.h"

class Obstaculo_trigger:public Obstaculo, public Trigger
{
	protected:

	virtual void establecer_representacion()=0;
	virtual void establecer_caracteristicas()=0;

	public:

	virtual float vel_max() {return 0;}
	virtual float vel_min() {return 0;}

	Obstaculo_trigger();
	virtual ~Obstaculo_trigger();

	virtual bool es_afectado_por_edu() const {return false;}
	virtual bool desaparece_tras_colision() const {return false;}
	virtual void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);

	virtual unsigned short int tipo_actor_mapa()=0;
	virtual void cuentame()=0;
	virtual bool es_representable() const=0;

	virtual bool es_fuera_de_juego() const {return false;} //No van a desaparecer: recordemos que los triggers son notificados del fin de las acciones. ¿Qué pasa si ha desaparecido?.
	bool es_parametrizable() const {return true;}
};

#endif
