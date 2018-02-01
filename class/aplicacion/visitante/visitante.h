#ifndef VISITANTE_H
#define VISITANTE_H

#include "../obstaculo_actividad/obstaculo_actividad.h"

class Obstaculo_actividad;
class Obstaculo_canon_apuntado;
class Obstaculo_canon_giratorio;
class Obstaculo_canon_programable;
class Obstaculo_colocador_jugador;
class Obstaculo_disparador;
class Obstaculo_explosivo;
class Obstaculo_generador_decoracion;
class Obstaculo_generador_obstaculos;
class Obstaculo_gravedad;
class Obstaculo_propiedades;
class Obstaculo_propiedades_fondo;
class Obstaculo_puerta_automatica;
class Obstaculo_puerta_disparar;
class Obstaculo_puerta_remota;
class Obstaculo_trigger_auto;
class Obstaculo_tutorial;
class Obstaculo_vector;
class Obstaculo_propiedades_movimiento_relativo;
class Trazador_fantasma;
class Trazador_explosion;
class Boss_01;
class Boss_02;

class Visitante
{
	public:
	
	Visitante() {}
	virtual ~Visitante() {}

	virtual void visitar(Obstaculo_actividad&)=0;
	virtual void visitar(Obstaculo_canon_apuntado&)=0;
	virtual void visitar(Obstaculo_canon_giratorio&)=0;
	virtual void visitar(Obstaculo_canon_programable&)=0;
	virtual void visitar(Obstaculo_colocador_jugador&)=0;
	virtual void visitar(Obstaculo_disparador&)=0;
	virtual void visitar(Obstaculo_explosivo&)=0;
	virtual void visitar(Obstaculo_generador_decoracion&)=0;
	virtual void visitar(Obstaculo_generador_obstaculos&)=0;
	virtual void visitar(Obstaculo_gravedad&)=0;
	virtual void visitar(Obstaculo_propiedades&)=0;
	virtual void visitar(Obstaculo_propiedades_fondo&)=0;
	virtual void visitar(Obstaculo_puerta_automatica&)=0;
	virtual void visitar(Obstaculo_puerta_disparar&)=0;
	virtual void visitar(Obstaculo_puerta_remota&)=0;
	virtual void visitar(Obstaculo_trigger_auto&)=0;
	virtual void visitar(Obstaculo_tutorial&)=0;
	virtual void visitar(Obstaculo_vector&)=0;
	virtual void visitar(Obstaculo_propiedades_movimiento_relativo&)=0;
	virtual void visitar(Trazador_fantasma&)=0;
	virtual void visitar(Trazador_explosion&)=0;
	virtual void visitar(Boss_01&)=0;
	virtual void visitar(Boss_02&)=0;
};

#endif

