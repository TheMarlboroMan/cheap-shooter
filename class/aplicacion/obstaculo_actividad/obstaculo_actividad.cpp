#include "obstaculo_actividad.h"

Obstaculo_actividad::Obstaculo_actividad()
	:Actor_mapa(), actua_fuera_de_pantalla(false)
{

}

Obstaculo_actividad::~Obstaculo_actividad()
{

}

void Obstaculo_actividad::procesar_turno(float p_delta)
{
	Actor_mapa::procesar_turno(p_delta);
}

/*Sólo se registra actividad cuando estás dentro del terreno de juego. Puede
parecer una chorrada en modo aleatorio pero para niveles preparados es
muy importante. Implica no meter en el vector de actividades a actores que 
aún no se han visto.*/

bool Obstaculo_actividad::realiza_actividad() const
{
	if(this->actua_fuera_de_pantalla) return true;
	else return this->es_en_pantalla();
}

/*Parecido al anterior, pero implica que los actores sólo empiezan a procesar
su turno cuando son visibles.*/

bool Obstaculo_actividad::procesa_turno() const
{
	if(actua_fuera_de_pantalla) return true;
	else return this->es_en_pantalla();
}
