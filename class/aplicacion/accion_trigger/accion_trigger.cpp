#include "accion_trigger.h"

Accion_trigger::Accion_trigger():
	descripcion(""), ms_antes(0), ms_despues(0),
	tag(0), tag_actores(0), tag_actores_secundario(0), tag_siguiente(0),
	desp_x(0), desp_y(0), tipo(0)
{

}

Accion_trigger::~Accion_trigger()
{

}

Accion_trigger& Accion_trigger::operator=(const Accion_trigger& p_otro)
{
	this->ms_antes=p_otro.ms_antes;
	this->ms_despues=p_otro.ms_despues;
	this->tag=p_otro.tag;
	this->tag_actores=p_otro.tag_actores;
	this->tag_actores_secundario=p_otro.tag_actores_secundario;
	this->tag_siguiente=p_otro.tag_siguiente;
	this->desp_x=p_otro.desp_x;
	this->desp_y=p_otro.desp_y;
	this->tipo=p_otro.tipo;

	return *this;
}

void Accion_trigger::serializar(std::ofstream& archivo) const
{
	U_ENTERO_8 	longitud_descripcion=this->descripcion.size(),
			i=0;

	archivo.write((char *) &longitud_descripcion, sizeof(U_ENTERO_8));
	while(i < longitud_descripcion) 
	{
		archivo.write((char *) &this->descripcion[i], sizeof(char));
		++i;
	}
	
	archivo.write((char *) &this->ms_antes, sizeof(U_ENTERO_16));
	archivo.write((char *) &this->ms_despues, sizeof(U_ENTERO_16));
	archivo.write((char *) &this->tag, sizeof(U_ENTERO_16));
	archivo.write((char *) &this->tag_actores, sizeof(U_ENTERO_16));
	archivo.write((char *) &this->tag_actores_secundario, sizeof(U_ENTERO_16));
	archivo.write((char *) &this->tag_siguiente, sizeof(U_ENTERO_16));
	archivo.write((char *) &this->desp_x, sizeof(S_ENTERO_16));
	archivo.write((char *) &this->desp_y, sizeof(S_ENTERO_16));

	archivo.write((char *) &this->tipo, sizeof(U_ENTERO_8));
}

Accion_trigger Accion_trigger::deserializar(std::ifstream& archivo)
{
	Accion_trigger temp;

	U_ENTERO_8 	longitud_descripcion;
	archivo.read((char *) &longitud_descripcion, sizeof(U_ENTERO_8));

	char * buffer=new char[longitud_descripcion+1];
	archivo.read(buffer, sizeof(char)*longitud_descripcion);
	buffer[longitud_descripcion]='\0';

	temp.descripcion=buffer;
	
	archivo.read((char *) &temp.ms_antes, sizeof(U_ENTERO_16));
	archivo.read((char *) &temp.ms_despues, sizeof(U_ENTERO_16));
	archivo.read((char *) &temp.tag, sizeof(U_ENTERO_16));
	archivo.read((char *) &temp.tag_actores, sizeof(U_ENTERO_16));
	archivo.read((char *) &temp.tag_actores_secundario, sizeof(U_ENTERO_16));
	archivo.read((char *) &temp.tag_siguiente, sizeof(U_ENTERO_16));
	archivo.read((char *) &temp.desp_x, sizeof(S_ENTERO_16));
	archivo.read((char *) &temp.desp_y, sizeof(S_ENTERO_16));

	archivo.read((char *) &temp.tipo, sizeof(U_ENTERO_8));

	delete [] buffer;

	return temp;
}
