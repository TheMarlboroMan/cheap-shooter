#include "gestor_recursos_audio.h"
#include <iostream>

using namespace DLibA;

/*Se realizan inserciones y se devuelve -1 si no están preparadas correctamente.
Un fallo no significa nada: realmente se vuelven a hacer comprobaciones con 
cada reproducción que se haga. Atención a que realmente SIEMPRE se hace una 
inserción en la lista!*/

int Gestor_recursos_audio::Gestor_recursos_sonido::insertar(int p_clave, const char * p_ruta)
{
	int resultado=p_clave;

	Sonido * s=new Sonido(p_ruta);
	DLibH::Gestor_mapa_static<int, Sonido *>::insertar(p_clave, s);

	if(!s->es_preparado()) resultado=-1;
	return resultado;
}

/*Idem que en el anterior. Se realiza la inserción y se devuelve -1 si ha
fallado algo.*/

int Gestor_recursos_audio::Gestor_recursos_musica::insertar(int p_clave, const char * p_ruta)
{
	int resultado=p_clave;

	Musica * m=new Musica(p_ruta);
	DLibH::Gestor_mapa_static<int, Musica *>::insertar(p_clave, m);
	
	if(!m->es_preparada()) resultado=-1;
	return resultado;
}

/*Ojo con el control de errores: cosa puede ser perfectamente NULL*/

Sonido * Gestor_recursos_audio::Gestor_recursos_sonido::obtener(int p_clave)
{
	Sonido * cosa=DLibH::Gestor_mapa_static<int, Sonido *>::obtener(p_clave);
	return cosa;
}

/*Ojo con el control de errores: cosa puede ser perfectamente NULL otra vez...*/

Musica * Gestor_recursos_audio::Gestor_recursos_musica::obtener(int p_clave)
{
	Musica * cosa=DLibH::Gestor_mapa_static<int, Musica *>::obtener(p_clave);
	return cosa;
}

void Gestor_recursos_audio::liberar()
{
	Gestor_recursos_audio::sonidos.vaciar();
	Gestor_recursos_audio::musicas.vaciar();
} 

int Gestor_recursos_audio::insertar_sonido(int p_clave, const char * p_ruta) {return Gestor_recursos_audio::sonidos.insertar(p_clave, p_ruta);}
int Gestor_recursos_audio::insertar_musica(int p_clave, const char * p_ruta) {return Gestor_recursos_audio::musicas.insertar(p_clave, p_ruta);}
Sonido * Gestor_recursos_audio::obtener_sonido(int p_clave) {return Gestor_recursos_audio::sonidos.obtener(p_clave);}
Musica * Gestor_recursos_audio::obtener_musica(int p_clave) {return Gestor_recursos_audio::musicas.obtener(p_clave);}
