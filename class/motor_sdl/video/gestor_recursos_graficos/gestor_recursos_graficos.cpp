#include "gestor_recursos_graficos.h"

using namespace DLibV;

int Gestor_recursos_graficos::insertar(int p_clave, Recurso_grafico * p_rec)
{
	int resultado=DLibH::Gestor_mapa_static<int, Recurso_grafico *>::insertar(p_clave, p_rec);

	if(p_rec->es_preparado()) return resultado;
	else return -1;
}

/*Ojo: cosa puede ser NULL una vez que todo esto haya finalizado. Habría que hacer
control de errores en el código cliente*/

Recurso_grafico * Gestor_recursos_graficos::obtener(int p_clave)
{
	Recurso_grafico * cosa=DLibH::Gestor_mapa_static<int, Recurso_grafico *>::obtener(p_clave);
	return cosa;
}

void Gestor_recursos_graficos::liberar()
{
	if(DLibH::Gestor_mapa_static<int, Recurso_grafico *>::cantidad()) 
		DLibH::Gestor_mapa_static<int, Recurso_grafico *>::vaciar();
}

int Gestor_recursos_graficos::cantidad()
{
	return DLibH::Gestor_mapa_static<int, Recurso_grafico *>::cantidad();
}
