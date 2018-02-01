#include "decoracion.h"

Decoracion::Decoracion():No_jugador()
{

}

Decoracion::~Decoracion()
{

}

void Decoracion::procesar_turno(float p_delta)
{
	No_jugador::procesar_turno(p_delta);
}
