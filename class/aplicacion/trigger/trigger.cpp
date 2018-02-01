#include "trigger.h"

Trigger::Trigger():
	tag_accion(0), repetible(false),
	veces_activado(0), en_curso(false)
{

}

Trigger::~Trigger()
{

}

bool Trigger::puede_ser_activado() const
{
	if(this->repetible) 
	{
		return !this->en_curso;
	}
	else 
	{
		return !this->veces_activado; 
	}
}

void Trigger::activar() const
{
	++this->veces_activado;
}

void Trigger::recibir_inicio_accion()
{
	this->en_curso=true;
}

void Trigger::recibir_finalizacion_accion()
{
	this->en_curso=false;
}
