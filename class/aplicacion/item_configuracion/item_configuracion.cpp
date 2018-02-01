#include "item_configuracion.h"

Item_configuracion::Item_configuracion(std::string p_titulo)
{
	this->titulo=p_titulo;	
	this->iterador_opciones=this->opciones.begin();
}

Item_configuracion::~Item_configuracion()
{
	this->opciones.clear();
}
