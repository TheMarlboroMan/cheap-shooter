#ifndef GESTOR_RECURSOS_GRAFICOS_H
#define GESTOR_RECURSOS_GRAFICOS_H

#include "../recurso_grafico/recurso_grafico.h"
#include "../../herramientas/templates/gestor_mapa_static.h"

namespace DLibV
{


class Gestor_recursos_graficos:public DLibH::Gestor_mapa_static<int, Recurso_grafico *>
{
	public:

	static int insertar(int, Recurso_grafico *);
	static Recurso_grafico * obtener(int);
	static void liberar();
	static int cantidad();
};

} //Fin namespace DLibV

#endif
