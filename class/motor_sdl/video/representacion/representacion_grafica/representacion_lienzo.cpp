#include "representacion_lienzo.h"

using namespace DLibV;

Representacion_lienzo::Representacion_lienzo(unsigned int p_w, unsigned int p_h)
	:Representacion_manual(), ancho(p_w), alto(p_h)
{
	this->iniciar_recurso();
}

Representacion_lienzo::Representacion_lienzo(const Representacion_lienzo& o)
	:Representacion_manual(o), ancho(o.ancho), alto(o.alto)
{
	iniciar_recurso();
}

Representacion_lienzo& Representacion_lienzo::operator=(const Representacion_lienzo& p_otro)
{
	//Esta línea hace que el recurso se copie de uno a otro... Lo que
	//se copiará será el contenido del puntero.

	Representacion_manual::operator=(p_otro);

	this->ancho=p_otro.ancho;
	this->alto=p_otro.alto;

	//Esto puede resultar un poco triposo pero lo único que está haciendo
	//es una copia profunda del lienzo subyacente.

	Lienzo * l=static_cast <Lienzo *>(p_otro.ref_recurso());
	Lienzo * n=new Lienzo(*l);
	Representacion_grafica::establecer_recurso(n);


	return *this;
}

Representacion_lienzo::~Representacion_lienzo()
{
	this->destruir_recurso();
}

void Representacion_lienzo::destruir_recurso()
{
	Representacion_grafica::liberar_recurso();
}

void Representacion_lienzo::iniciar_recurso()
{
	//Generar un lienzo. Sobre este lienzo actuarán las cosas de turno.
	Lienzo * l=Lienzo::generar_nuevo(this->ancho, this->alto); 			
	l->rellenar(0,0,0);
	Representacion_grafica::establecer_recurso(l);
}

void Representacion_lienzo::reiniciar_recurso(unsigned int p_w, unsigned int p_h)
{
	this->ancho=p_w;
	this->alto=p_h;	
	Representacion_grafica::liberar_recurso();
	this->iniciar_recurso();
}

void Representacion_lienzo::exportar_bmp(const char * p_ruta)
{
	SDL_SaveBMP(ref_superficie(), p_ruta);
}
