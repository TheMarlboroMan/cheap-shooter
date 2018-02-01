#include "decoracion_puntuacion.h"

Decoracion_puntuacion::Decoracion_puntuacion():Decoracion()
{	
	//Empieza con un vector parado. En otro método la pondremos en 
	//marcha una vez que conozcamos su posición.
	this->vector_mov=DLibH::Vector_2d(0, 0);

	//Sin miedo, el controlador le dará luego las posiciones.

	this->x=0;
	this->y=0;

	this->es_copia=false;
	this->representacion_real=NULL;

	this->establecer_caracteristicas(); 
}

/*
La preparación sólo la lleva a cabo el objeto maestro. Sus copias obtienen una
copia de los recursos necesarios.
*/

void Decoracion_puntuacion::preparar_maestra()
{
	this->establecer_representacion();
}

Decoracion_puntuacion::~Decoracion_puntuacion()
{
	this->representacion_real=NULL;
}

/*En su turno se irá acercando a la esquina superior izquierda. Nada más.*/

void Decoracion_puntuacion::procesar_turno(float p_delta)
{
	Decoracion::procesar_turno(p_delta);

	//Primero intenta ir hacia la izquierda... Cuando llega se detiene todo
	//el movimiento hacia este lado y se pone hacia abajo.
	if(this->x < POS_FINAL_X) 
	{
		this->x=POS_FINAL_X;
		this->vector_mov.x=0;
		this->vector_mov.y=+VELOCIDAD_VECTOR;
	}
	
	//Cuando llega aquí no hay nada más que hacer.
	
	if(this->y > POS_FINAL_Y) 
	{
		this->y=POS_FINAL_Y;
		this->vector_mov.y=0;
	}
}

/*Dirigirla hacia la posición 0, 0. Primero se dirigirá hacia la izquierda y
luego adquirirá vector y*/

void Decoracion_puntuacion::poner_en_marcha()
{
	this->vector_mov.x=this->x > 0 ? -VELOCIDAD_VECTOR : VELOCIDAD_VECTOR;
}

void Decoracion_puntuacion::establecer_caracteristicas()
{
	//Esto no tiene mucha validez porque realmente "no existe".

	this->w=32;
	this->h=32;
	this->posicion=DLibH::Herramientas_SDL::nuevo_sdl_rect(this->x, this->y, this->w, this->h);
}

/*Debe tener algún tipo de representación para eliminarse desde el destructor de
la clase Actor, de la que es pariente lejano.*/

void Decoracion_puntuacion::establecer_representacion()
{
	std::string cadena_puntuacion=DLibH::Herramientas::entero_a_cadena(this->puntuacion);
	
	DLibV::Representacion_texto_auto_estatica * rep_maestra=new DLibV::Representacion_texto_auto_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), cadena_puntuacion);
	rep_maestra->establecer_posicion(100, 10);
	rep_maestra->preparar();

	Representable::establecer_representacion(rep_maestra);
}

/*La representación gráfica simplemente se crea. Como no podemos pasar parámetros
haremos en otro método que se copie del maestro... Dicho de otro
modo: la primera representación que se crea con esta "puntuación" es la
que genera el mapa de bits. El resto simplemente lo referencian.
*/

void Decoracion_puntuacion::crear_representacion_desde_maestra(DLibV::Representacion * maestra)
{
	//Sabemos que "Maestra" es del tipo "Representación_grafica", con superficie.
	//Este cast tenemos que ponerlo si o si.

	DLibV::Representacion_grafica * c_maestra=static_cast<DLibV::Representacion_grafica *> (maestra);
	this->representacion_real=new DLibV::Representacion_bitmap_estatica(c_maestra->ref_recurso());
	this->representacion_real->establecer_posicion(this->posicion.x, this->posicion.y);

	int 	w=c_maestra->ref_recurso()->acc_superficie()->w,
		h=c_maestra->ref_recurso()->acc_superficie()->h;	

	//Las posiciones es historias se ponen completamente en cero. Da igual, luego se pondrán en su sitio cuando toque.
	this->representacion_real->establecer_posicion(0, 0, w, h);
	this->representacion_real->establecer_recorte(0,0, w, h);
	this->representacion_real->ref_recurso()->establecer_alpha(127);

	Representable::establecer_representacion(this->representacion_real);
//	this->representacion=this->representacion_real;
}

/*Habrá finalizado cuando haya llegado a la esquina superior izquierda de la
pantalla*/	

bool Decoracion_puntuacion::es_finalizado()
{	
	return this->x==POS_FINAL_X && this->y==POS_FINAL_Y;
}

/*El maestro usa este método para generar una instancia copia.*/

Decoracion_puntuacion * Decoracion_puntuacion::generar_copia()
{
	Decoracion_puntuacion * resultado=new Decoracion_puntuacion();

	resultado->puntuacion=this->puntuacion;	
	resultado->es_copia=true;
	resultado->crear_representacion_desde_maestra(Representable::acc_representacion());

	return resultado;
}
