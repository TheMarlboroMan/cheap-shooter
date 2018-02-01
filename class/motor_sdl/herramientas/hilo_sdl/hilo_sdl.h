#ifndef HILO_SDL_H
#define HILO_SDL_H
#include "SDL/SDL_thread.h"

/*Para entender esta clase vamos a revisar sus distintos estados:

- Creado: nada, simplemente lo hemos instanciado.
- Iniciado: se llama al método iniciar y devuelve Ok: a partir de este momento 
	se está ejecutando el código cliente en el hilo.
- Finalizado: el cliente indica al hilo que ha finalizado su proceso. Aún queda
	realizar limpieza.
- Completo: se realiza la limpieza del hilo y se obtiene el resultado.

Otra cosa importante para entender esta clase es la estructura 
Hilo_SDL::Hilo_estructura_datos. Esta estructura es lo que se pasará como
parámetro (casteado a void *) al código que ejecute el hilo. Desde ese
código se debe primero obtener el puntero al propio hilo y luego el parámetro
original con el casteo deseado. Es IMPRESCINDIBLE que cuando se haya
terminado con el código cliente este último diga al propio hilo que
ya está finalizado usando el método "finalizar".

El loop deberá comprobar a cada paso si el hilo ha finalizado usando el
método "es_finalizado". En el momento en que esté finalizado ya podrá usar
el método completar para finalizar el ciclo. 

Supuestamente el destructor del hilo se encargaría de detenerlo, por si tenemos
prisa.
*/


namespace DLibH
{

class Hilo_SDL
{
	public:

	struct Hilo_estructura_datos
	{
		Hilo_SDL * ref_hilo;
		void * datos;
	};


	private:
	SDL_Thread * hilo;
	bool iniciado;
	bool finalizado;
	bool completo;
	int resultado;
	Hilo_estructura_datos ed;

	public: 

	Hilo_SDL()
	{
		this->iniciado=false;
		this->finalizado=false;
		this->completo=false;
		this->hilo=NULL;

	}

	~Hilo_SDL()
	{
		this->completar();

		this->ed.ref_hilo=NULL;
		this->ed.datos=NULL;
	}

	bool es_iniciado() const {return this->iniciado;}
	bool es_finalizado() const {return this->finalizado;}
	bool es_completo() const {return this->completo;}
	int acc_resultado() const {return this->resultado;}
	
	bool iniciar(int (*funcion)(void *), void * datos);
	void finalizar();
	void static finalizar_y_destruir_hilo(Hilo_SDL *&);
	void static destruir_hilo(Hilo_SDL *&);
	void static completar_y_destruir_hilo(Hilo_SDL *&);
	int completar();
};

}
#endif
