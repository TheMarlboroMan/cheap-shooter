#ifndef FACTORIA_ACTORES_MAPA_H
#define FACTORIA_ACTORES_MAPA_H

#include "../actor_mapa/actor_mapa.h"
#include "../obstaculo/obstaculo.h"
#include "../obstaculo/obstaculo_flexible.h"
#include "../obstaculo/obstaculo_aleatorio.h"
#include "../obstaculo/obstaculo_800_32.h"
#include "../obstaculo_destruible/obstaculo_destruible.h"
#include "../obstaculo_destruible_secreto/obstaculo_destruible_secreto.h"
#include "../obstaculo_bonus/obstaculo_bonus.h"
#include "../obstaculo_actividad/obstaculo_actividad.h"
#include "../obstaculo_actividad/obstaculo_gravedad.h"
#include "../obstaculo_actividad/obstaculo_disparador.h"
#include "../obstaculo_actividad/obstaculo_explosivo.h"
#include "../obstaculo_actividad/obstaculo_canon_programable.h"
#include "../obstaculo_actividad/obstaculo_canon_giratorio.h"
#include "../obstaculo_actividad/obstaculo_canon_apuntado.h"
#include "../obstaculo_actividad/obstaculo_puerta.h"
#include "../obstaculo_actividad/obstaculo_puerta_disparar.h"
#include "../obstaculo_actividad/obstaculo_puerta_automatica.h"
#include "../obstaculo_actividad/obstaculo_puerta_remota.h"
#include "../obstaculo_actividad/obstaculo_propiedades.h"
#include "../obstaculo_actividad/obstaculo_trigger_auto.h"
#include "../obstaculo_actividad/obstaculo_propiedades_fondo.h"
#include "../obstaculo_actividad/obstaculo_propiedades_movimiento_relativo.h"
#include "../obstaculo_actividad/obstaculo_vector.h"
#include "../obstaculo_actividad/obstaculo_tutorial.h"
#include "../obstaculo_actividad/obstaculo_generador_decoracion.h"
#include "../obstaculo_actividad/obstaculo_generador_obstaculos.h"
#include "../obstaculo_actividad/obstaculo_colocador_jugador.h"
#include "../boss/boss_01.h"
#include "../boss/boss_02.h"
#include "../punto_ruta/punto_ruta.h"
#include "../decoracion_nivel/decoracion_nivel.h"
#include "../obstaculo_actividad/trazador_fantasma.h"
#include "../obstaculo_actividad/trazador_explosion.h"
#include "../obstaculo_trigger/obstaculo_trigger.h"
#include "../obstaculo_trigger/obstaculo_trigger_contacto.h"
#include "../obstaculo_trigger/obstaculo_trigger_interruptor.h"
//#include "../proyectil_obstaculo/proyectil_obstaculo.h"
#include "../bonus/bonus_normal.h"
#include "../bonus/bonus_vida.h"
#include "../bonus/bonus_pimiento.h"
#include "../bonus/bonus_disparo.h"
#include "../bonus/bonus_tiempo.h"
#include "../bonus/bonus_componente_escudo.h"
#include "../bonus/bonus_carga_escudo.h"

class Factoria_actores_mapa
{
	private:

	Factoria_actores_mapa(){}
	~Factoria_actores_mapa(){}

	public:

	static Actor_mapa * fabricar(unsigned int, unsigned int);
};

#endif
