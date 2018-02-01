#ifndef REPRESENTACION_TEXTO_FIJA_H
#define REPRESENTACION_TEXTO_FIJA_H

#include "representacion_texto.h"

namespace DLibV
{


class Representacion_texto_fija:public Representacion_texto
{
	public:

	Representacion_texto_fija(Recurso_grafico const * p_rep, const char p_char, unsigned int p_ancho, unsigned int p_alto)
		:Representacion_texto(p_rep)
	{
		this->asignar(p_char);

		this->establecer_dimensiones_area(p_ancho, p_alto);
		this->iniciar_recurso();
	}

	Representacion_texto_fija(Recurso_grafico const * p_rep, const char * p_char, unsigned int p_ancho, unsigned int p_alto)
		:Representacion_texto(p_rep)
	{
		if(p_char) this->asignar(p_char);

		this->establecer_dimensiones_area(p_ancho, p_alto);
		this->iniciar_recurso();
	}

	Representacion_texto_fija(Recurso_grafico const * p_rep, std::string const& p_cad,unsigned int p_ancho, unsigned int p_alto):
		Representacion_texto(p_rep)
	{
		this->asignar(p_cad);

		this->establecer_dimensiones_area(p_ancho, p_alto);
		this->iniciar_recurso();
	}

	Representacion_texto_fija(const Representacion_texto_fija&);	
	virtual ~Representacion_texto_fija() {}

	Representacion_texto_fija& operator=(const Representacion_texto_fija&);

	bool es_estatica() const=0;
};

class Representacion_texto_fija_dinamica:public Representacion_texto_fija
{
	public:

	Representacion_texto_fija_dinamica(Recurso_grafico const * p_rep, const char p_char, unsigned int p_ancho, unsigned int p_alto)
		:Representacion_texto_fija(p_rep, p_char, p_ancho, p_alto)
	{}

	Representacion_texto_fija_dinamica(Recurso_grafico const * p_rep, const char * p_char, unsigned int p_ancho, unsigned int p_alto)
		:Representacion_texto_fija(p_rep, p_char, p_ancho, p_alto)
	{}

	Representacion_texto_fija_dinamica(Recurso_grafico const * p_rep, std::string const& p_cad, unsigned int p_ancho, unsigned int p_alto)
		:Representacion_texto_fija(p_rep, p_cad, p_ancho, p_alto)
	{}

	Representacion_texto_fija_dinamica(const Representacion_texto_fija_dinamica& p_otra):Representacion_texto_fija(p_otra) {}
	~Representacion_texto_fija_dinamica() {}
	Representacion_texto_fija_dinamica& operator=(const Representacion_texto_fija_dinamica& p_otra)
	{
		Representacion_texto_fija::operator=(p_otra);
		return *this;
	}

	bool es_estatica() const {return false;}
};

class Representacion_texto_fija_estatica:public Representacion_texto_fija
{
	public:

	Representacion_texto_fija_estatica(Recurso_grafico const * p_rep, const char p_char, unsigned int p_ancho, unsigned int p_alto)	
		:Representacion_texto_fija(p_rep, p_char, p_ancho, p_alto)
	{}

	Representacion_texto_fija_estatica(Recurso_grafico const * p_rep, const char * p_char, unsigned int p_ancho, unsigned int p_alto)
		:Representacion_texto_fija(p_rep, p_char, p_ancho, p_alto)
	{}

	Representacion_texto_fija_estatica(Recurso_grafico const * p_rep, std::string const& p_cad, unsigned int p_ancho, unsigned int p_alto)
		:Representacion_texto_fija(p_rep, p_cad, p_ancho, p_alto)
	{}

	Representacion_texto_fija_estatica(const Representacion_texto_fija_estatica& p_otra):Representacion_texto_fija(p_otra) {}
	~Representacion_texto_fija_estatica(){}
	Representacion_texto_fija_estatica& operator=(const Representacion_texto_fija_estatica& p_otra)
	{
		Representacion_texto_fija::operator=(p_otra);
		return *this;
	}

	bool es_estatica() const {return true;}
};

} //Fin namespace DLibV

#endif
