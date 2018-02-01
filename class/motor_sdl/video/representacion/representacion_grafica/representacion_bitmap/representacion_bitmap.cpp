#include "representacion_bitmap.h"

using namespace DLibV;

Representacion_bitmap::Representacion_bitmap():Representacion_grafica()
{

}

Representacion_bitmap::Representacion_bitmap(const Recurso_grafico * p_rec):Representacion_grafica()
{
	this->establecer_recurso(p_rec);
	this->establecer_dimensiones_posicion_por_recorte();
}

Representacion_bitmap::Representacion_bitmap(const Representacion_bitmap& o)
	:Representacion_grafica(o)
{

}

Representacion_bitmap::~Representacion_bitmap()
{

}

Representacion_bitmap& Representacion_bitmap::operator=(const Representacion_bitmap & p_otra)
{
	Representacion_grafica::operator=(p_otra);
	return *this;
}

void Representacion_bitmap::establecer_recurso(Recurso_grafico const * p_recurso)
{
	Representacion_grafica::establecer_recurso(p_recurso);
	this->recorte_a_medidas_recurso();
}

/*** Dinámica...*/

Representacion_bitmap_dinamica::Representacion_bitmap_dinamica():Representacion_bitmap()
{
	this->preparar();
}

Representacion_bitmap_dinamica::Representacion_bitmap_dinamica(const Representacion_bitmap_dinamica& o)
	:Representacion_bitmap(o)
{
	this->preparar();
}

Representacion_bitmap_dinamica::Representacion_bitmap_dinamica(const Recurso_grafico * p_rec)
	:Representacion_bitmap(p_rec)
{
	this->preparar();
}

Representacion_bitmap_dinamica& Representacion_bitmap_dinamica::operator=(const Representacion_bitmap_dinamica& o)
{
	Representacion_bitmap::operator=(o);
	return * this;
}

/** Estática **/

Representacion_bitmap_estatica::Representacion_bitmap_estatica():Representacion_bitmap()
{
	this->preparar();
}

Representacion_bitmap_estatica::Representacion_bitmap_estatica(const Representacion_bitmap_estatica& o)
	:Representacion_bitmap(o)
{
	this->preparar();
}

Representacion_bitmap_estatica::Representacion_bitmap_estatica(const Recurso_grafico * p_rec):Representacion_bitmap(p_rec)
{
	this->preparar();
}

Representacion_bitmap_estatica& Representacion_bitmap_estatica::operator=(const Representacion_bitmap_estatica& o)
{
	Representacion_bitmap::operator=(o);
	return * this;
}
