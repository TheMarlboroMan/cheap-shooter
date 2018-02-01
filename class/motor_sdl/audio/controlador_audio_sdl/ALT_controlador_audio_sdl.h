#ifndef CONTROLADOR_AUDIO_SDL_H
#define CONTROLADOR_AUDIO_SDL_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "../sonido/sonido.h"
#include "../musica/musica.h"
#include <vector>
#include <map>
#include <stdexcept>
#include <iostream>

/*
Esta clase está implementada como un singleton... No es por gusto, es porque
la función de callback para cuando un sonido termina de escucharse necesita
recibir un método global y no habría otra forma de obtener qué controlador
de audio es el que ha finalizado.

Para implementar toda esta historia hemos creado un método amigo de esta clase,
que es el callback en si No es tan vergonzoso en realidad.
*/

namespace DLibA
{

void callback_fin_reproduccion(int);	//F-f-f-forward!.
class Cola_sonido;			//Re-f-f-f-forward!.

/*El canal de audio lo podríamos usar para darles a otros elementos referencias
a los canales de audio originales que esté usando el controlador. El controlador
tendría un vector de tantos objetos como canales. Al solicitar la reproducción 
podríamos devolver un asa a uno de estos de forma que se pueda siempre
inspeccionar y actuar desde la propia entidad sin tener que hacer muchos puentes
entre clases.

Estos objetos serían válidos mientras el controlador de audio estuviera en 
marcha.

Esto reemplazaría a los vectores de estado de canales y demás y se convertiría
en el nuevo punto de acceso.

La implementación original inluía un vector de Canales de audio en el 
controlador pero la actual es un vector de punteros a canales de audio. Esto es
así porque la interfaz pública hacía copia al devolver el objeto y no permitía
realmente llevar la cuenta de los mismos si se usan desde fuera del controlador.
Por ejemplo, al sacar un canal libre y usarlo desde el código cliente se hacía
una copia del mismo y al usarlo dejaba de reflejarse en debug_canales.
*/

struct Excepcion_controlador_audio
{
	enum codigos
	{
		ERROR_RANGO=0
	};

	unsigned short int cod;
	std::string mensaje;

	Excepcion_controlador_audio(unsigned short int p_c, std::string p_m)
		:cod(p_c), mensaje(p_m)
	{}	
};

class Canal_audio
{
	private:

	int indice;
	int repeticiones;
	int volumen;
	bool reproduciendo; //Reproduciendo es que la onda se está reproduciendo.
	bool ocupado;	//Activo es que el canal está ocupado, ya sea porque reproduce o porque está monitoreado.
	bool monitoreado; //Un canal monitoreado no se libera del estado ocupado de forma automática.
	bool pausado;
	Sonido * audio_reproduciendo;

	public:

	void forzar_parada();	//Forzaría la parada del canal en el controlador.
	void recibir_callback_finalizacion(); 	//Se pondría como "no_reproduciendo" y quitaría el puntero del audio.
	int iniciar_reproduccion(Sonido&, int=-1, int=0);	//Pondría el puntero al audio y si está o no en loop.
	int iniciar_reproduccion(Estructura_sonido&);	//Pondría el puntero al audio y si está o no en loop.
	int iniciar_fade_in(Sonido&, int, int=1, int=0);
	int iniciar_fade_in(Estructura_sonido&, int);
	int iniciar_fade_out(int);


	bool es_en_loop() const {return repeticiones==-1;}
	bool es_ocupado() const {return ocupado;}
	bool es_monitoreado() const {return monitoreado;}
	bool es_reproduciendo() const {return reproduciendo;}
	bool es_pausado() const {return pausado;}

	int acc_indice() const {return indice;}
	int acc_repeticiones() const {return repeticiones;}
	int acc_volumen() const {return volumen;}

	void establecer_volumen(int val);
	void activar_monitoreado() {monitoreado=true;}
	void desactivar_monitoreado() {monitoreado=false;}
	void liberar(); //Es público para poder llamarlo si haces monitoreado manual.
	void pausar();
	void reanudar();

	Canal_audio(int i):
		indice(i), repeticiones(0), volumen(128), reproduciendo(false), 
		ocupado(false), monitoreado(false), pausado(false), 
		audio_reproduciendo(NULL)
	{}

	Canal_audio(const Canal_audio&)=delete;
	Canal_audio& operator=(const Canal_audio&)=delete;

	~Canal_audio()
	{
		audio_reproduciendo=NULL;
	}
};

class Controlador_audio_SDL
{
	
	/////////////////////////////////////
	// Propiedades...

	private:

	int ratio;
	int salidas;	//1 mono, 2 estéreo.
	int buffers;
	Uint16 formato;
	unsigned int canales_audio;
	std::vector<Canal_audio *> canales;
	bool estado_musica; 			//True=libre, false=ocupado.
	static Controlador_audio_SDL * instancia;

	/////////////////////////////////////
	// Métodos...

	private:

	Controlador_audio_SDL();
	void preparar_controles(unsigned int);
	int obtener_indice_canal_libre(int=0, int=-1);


	public:

	~Controlador_audio_SDL();
	static Controlador_audio_SDL * obtener();
	static void desmontar();

	void mut_estado_musica(bool p_valor) {this->estado_musica=p_valor;}
	int reproducir_sonido(Sonido&, int=-1, int=-1, int=0);
	int reproducir_sonido(Estructura_sonido&, int=-1);
	int reproducir_fade_in(Sonido&, int, int=-1, int=-1, int=0);
	int reproducir_fade_in(Estructura_sonido&, int, int=-1);
	void detener_sonido(int);
	bool reproducir_musica(Musica&, int=-1);
	void detener_musica();
	void establecer_volumen(int, int=-1); //p_vol de 0 a 128.
	void establecer_volumen_musica(int);
	void debug_estado_canales();
	bool iniciar();
	unsigned int acc_canales_audio() const {return canales_audio;}
	Canal_audio& obtener_canal(int) throw();
	Canal_audio& obtener_canal_libre() throw();
	void pausar_sonido();
	void reanudar_sonido();
	void detener_sonido();

	///////////////////////////////////////
	// Amigos.

	friend void callback_fin_reproduccion(int);
	friend class Cola_sonido;
	friend class Canal_audio;
};
}
#endif
