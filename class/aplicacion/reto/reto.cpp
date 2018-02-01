#include "reto.h"

Reto::Reto(const Reto& p_otro)
{
	this->representacion=p_otro.representacion;
	this->titulo=p_otro.titulo;
	this->texto=p_otro.texto;
	this->identificador=p_otro.identificador;
	this->fase_animacion=p_otro.fase_animacion;
	this->tiempo_pausa=p_otro.tiempo_pausa;
	this->finalizado=p_otro.finalizado;
}

Reto::Reto()
	:representacion(NULL), representacion_off(NULL),
	titulo(""), texto(""), 
	identificador(0), fase_animacion(A_ENTRADA),
	tiempo_pausa(0), x(POS_X_INICIAL), y(POS_Y_INICIAL), 
	vector((float)VECTOR_X_ENTRADA, (float)VECTOR_Y_ENTRADA),
	finalizado(false)
{

}

Reto::~Reto()
{
	if(this->representacion)
	{
		delete this->representacion;
	}

	if(this->representacion_off)
	{
		delete this->representacion_off;
	}
}

Reto& Reto::operator=(const Reto& p_otro)
{
	this->representacion=p_otro.representacion;
	this->representacion_off=p_otro.representacion_off;
	this->titulo=p_otro.titulo;
	this->texto=p_otro.texto;
	this->identificador=p_otro.identificador;
	this->fase_animacion=p_otro.fase_animacion;
	this->tiempo_pausa=p_otro.tiempo_pausa;
	this->finalizado=false;

	return *this;
}

void Reto::asignar_datos(unsigned int p_id, std::string const &p_titulo, 
			std::string const &p_texto)
{
	this->identificador=p_id;
	this->titulo=p_titulo;
	this->texto=p_texto;

	std::string texto_final=this->titulo+"\n\n"+this->texto;

	//Generar las representaciones...
	this->representacion=new DLibV::Representacion_compuesta_estatica(W_CAJA, H_CAJA, 0, 0, 0);
	this->representacion->establecer_posicion(POS_X_INICIAL, POS_Y_INICIAL);

	this->representacion_off=new DLibV::Representacion_compuesta_estatica(W_CAJA, H_CAJA, 0, 0, 0);
	this->representacion_off->establecer_posicion(POS_X_INICIAL, POS_Y_INICIAL);

	//Generar el fondo...
	DLibV::Representacion_bitmap_estatica * fondo=new DLibV::Representacion_bitmap_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FONDO_RETOS));

	//Generar el icono...
	DLibV::Representacion_bitmap_estatica * icono=new DLibV::Representacion_bitmap_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_ICONO_RETOS));
	icono->establecer_recorte(0, 0, 48, 48);
	SDL_Rect caja_icono=DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CAJA_ICONO, Y_CAJA_ICONO, W_CAJA_ICONO, H_CAJA_ICONO);

	//Ahora el texto...
	SDL_Rect caja_texto=DLibH::Herramientas_SDL::nuevo_sdl_rect(X_TEXTO, Y_TEXTO, W_TEXTO, H_TEXTO);
	DLibV::Representacion_texto_fija_estatica * txt=new DLibV::Representacion_texto_fija_estatica(
			DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_2), 
			texto_final, W_TEXTO, H_TEXTO);

	//Volcar fondo, texto e icono.

	this->representacion->volcar_representacion(*fondo);
	this->representacion->volcar_representacion(*txt, caja_texto);
	this->representacion->volcar_representacion(*icono, caja_icono);

	//El off es igual, pero sin icono.
	this->representacion_off->volcar_representacion(*fondo);
	this->representacion_off->volcar_representacion(*txt, caja_texto);

	//Copiar al off. Ojo con la deferencia de punteros, que si no copiamos la dirección a la que apuntamos.
//	*this->representacion_off=*this->representacion;

	//Borrar las sobras.
	delete txt;
	delete fondo;
	delete icono;
}

void Reto::turno(float p_delta)
{
	switch(this->fase_animacion)
	{
		case A_ENTRADA:

			if(animar(p_delta, POS_X_PAUSA, POS_Y_PAUSA))
			{
				this->fase_animacion=A_PAUSA;
				this->vector.x=0;
				this->vector.y=0;
			}

		break;

		case A_PAUSA:
			this->tiempo_pausa+=p_delta;

			if(this->tiempo_pausa >= LONGITUD_PAUSA)
			{
				this->vector.x=(float)VECTOR_X_SALIDA;
				this->vector.y=(float)VECTOR_Y_SALIDA;
				this->fase_animacion=A_SALIDA;
			}
		break;

		case A_SALIDA:
			if(animar(p_delta, POS_X_INICIAL, POS_Y_INICIAL))
			{
				this->finalizado=true;
			}	

		break;
	}
}

bool Reto::animar(float p_delta, int destino_x, int destino_y)
{
	x+=this->vector.x*p_delta;
	y+=this->vector.y*p_delta;

	if( (vector.x > 0 && x > destino_x) || (vector.x < 0 && x < destino_x) ) {x=destino_x;}
	if( (vector.y > 0 && y > destino_y) || (vector.y < 0 && y < destino_y) ) {y=destino_y;}

	this->representacion->establecer_posicion(x, y);

	//Devolvemos true cuando hemos llegado.
	return x==destino_x && y==destino_y;
}
