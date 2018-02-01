#include "obstaculo_trigger_interruptor.h"

Obstaculo_trigger_interruptor::Obstaculo_trigger_interruptor()
	:Obstaculo_trigger()
{
	this->preparar_actor(0);
}

Obstaculo_trigger_interruptor::~Obstaculo_trigger_interruptor()
{

}

void Obstaculo_trigger_interruptor::establecer_caracteristicas()
{
	this->w=16;
	this->h=16;
}

/* Realmente no tiene representación... Tan fácil como eso.*/

void Obstaculo_trigger_interruptor::establecer_representacion()
{
	this->mut_indice_recurso(Recursos::G_TILE_JUEGO);
	this->establecer_representacion_bitmap_dinamica();
	this->establecer_recorte(32,208,16,16);
//	this->representacion=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(), 0, 0); //this->posicion.x, this->posicion.y);
//	this->representacion->establecer_recorte(32,208,16,16);
}

bool Obstaculo_trigger_interruptor::procesar_colision_proyectil(Proyectil * proyectil)
{
	if(!proyectil->es_de_faccion(Definiciones::F_JUGADOR)) 
	{
		return false;
	}
	else 
	{
		return true;
	}
}
