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

/*
Sobre el Controlador_audio:

Es un singleton... Podemos "montarlo" mediante el método "obtener", que nos
daría la instancia del controlador. Antes de montarlo se puede configurar
mediante los distintos métodos configurar_XXXX.

Sobre el Canal_audio.

El canal de audio lo podríamos usar para darles a otros elementos referencias
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
controlador. Una posterior es un vector de punteros a canales de audio. Esto es
así porque la interfaz pública hacía copia al devolver el objeto y no permitía
realmente llevar la cuenta de los mismos si se usan desde fuera del controlador.
Por ejemplo, al sacar un canal libre y usarlo desde el código cliente se hacía
una copia del mismo y al usarlo dejaba de reflejarse en debug_canales.

La implementación actual es bien diferente: el canal de audio que se guarda
en el controlador es del tipo Canal_audio_real, y no se guarda como un puntero.
Para obtener referencias a los mismos desde el código cliente y que todo
funcione bien, se ha envuelto en un patrón de proxy de forma que un objeto
Canal_audio tiene una referencia al Canal_audio_real. Canal_audio implementa
la misma interfaz pública que Canal_audio_real de forma que es transparente
al uso. Canal_audio puede ser copiable de un lado para otro sin problemas. 
Canal_audio es amiga de Canal_audio_real para poder acceder a sus propiedades
directamente.

El objeto del tipo Canal_audio es deshechable: no se devuelven por referencia
ni se guardan referencias al mismo, sino que se copian tantas veces como sea
necesario. Un detalle MUY importante es que puede crearse un objeto Canal_audio
de la nada pero estaría "desvinculado" y todo aquello que llamemos que tenga
que afectar un canal causaría un crash seguro. Tenemos el método "es_vinculado()"
para asegurarnos de que tiene un canal de audio real asociado. Asimismo se
puede desvincular con el método "desvicular".

Objetos de Canal_audio directamente obtenidos del controlador de audio estarían
siempre vinculados y listos. Objetos creados libremente estarán siempre
desvinculados a no ser que se creen mediante copia o constructor de copia de 
otro objeto Canal_audio.
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
		:cod(p_c), mensaje(p_m) {}

	Excepcion_controlador_audio(const Excepcion_controlador_audio& e)
		:cod(e.cod), mensaje(e.mensaje) {}

	Excepcion_controlador_audio& operator=(const Excepcion_controlador_audio& e)
	{
		cod=e.cod;
		mensaje=e.mensaje;
		return *this;
	}
};

class Canal_audio; //F-f-f-f-forward!.

class Controlador_audio_SDL
{
	/////////////////////////////////////
	// Propiedades...

	private:

	static int config_ratio;
	static int config_salidas;
	static int config_buffers;
	static Uint16 config_formato;
	static unsigned int config_canales_audio;

	int ratio;
	int salidas;	//1 mono, 2 estéreo.
	int buffers;
	Uint16 formato;
	unsigned int canales_audio;
	bool estado_musica; 			//True=libre, false=ocupado.
	static Controlador_audio_SDL * instancia;
//	std::vector<Canal_audio_real> canales; OJO: Esto no está definido aquí
//porque hemos definido Canal_audio_real un poco más adelante y no podemos
//hacer una forward declaration de una clase interna... Y debe ser interna y
//privada para que sea inaccesible. Simplemente pondremos el vector cuando
//hayamos definido la clase.

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
	static void configurar_ratio(int v) {config_ratio=v;}
	static void configurar_salidas(int v) {config_salidas=v;}
	static void configurar_buffers(int v) {config_buffers=v;}
	static void configurar_formato(Uint16 v) {config_formato=v;}
	static void configurar_canales_audio(int v) {config_canales_audio=v;}

	void mut_estado_musica(bool p_valor) {this->estado_musica=p_valor;}
	int reproducir_sonido(Estructura_sonido&, int=-1);
	void detener_sonido(int);
	bool reproducir_musica(Musica&, int=-1);
	void detener_musica();
	void establecer_volumen(int, int=-1); //p_vol de 0 a 128.
	void establecer_volumen_musica(int);
	void debug_estado_canales();
	bool iniciar();
	unsigned int acc_canales_audio() const {return canales_audio;}
	Canal_audio obtener_canal(int);// throw(); OJO: "A function with an empty exception specification doesn't allow any exceptions"
	Canal_audio obtener_canal_libre();// throw();
	void pausar_sonido();
	void reanudar_sonido();
	void detener_sonido();

	////////////////////////////////////
	// Clases...

	private:

	class Canal_audio_real
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
		int iniciar_reproduccion(const Estructura_sonido&);	//Pondría el puntero al audio y si está o no en loop.
		int iniciar_fade_out(int);

		bool es_en_loop() const {return repeticiones==-1;}
		bool es_ocupado() const {return ocupado;}
		bool es_monitoreado() const {return monitoreado;}
		bool es_reproduciendo() const {return reproduciendo;}
		bool es_pausado() const {return pausado;}
		int acc_indice() const {return indice;}
		int acc_repeticiones() const {return repeticiones;}
		int acc_volumen() const {return volumen;}
		void activar_monitoreado() {monitoreado=true;}
		void desactivar_monitoreado() {monitoreado=false;}
		void establecer_volumen(int val);

		void liberar(); //Es público para poder llamarlo si haces monitoreado manual.
		void pausar();
		void reanudar();
		void recibir_callback_finalizacion(); 	//Se pondría como "no_reproduciendo" y quitaría el puntero del audio.


		Canal_audio_real(int i):
			indice(i), repeticiones(0), volumen(128), reproduciendo(false), 
			ocupado(false), monitoreado(false), pausado(false), 
			audio_reproduciendo(NULL) {}
		Canal_audio_real(const Canal_audio_real&);
		Canal_audio_real& operator=(const Canal_audio_real&);
		~Canal_audio_real(){audio_reproduciendo=NULL;}

		friend class Canal_audio;
		friend class Controlador_audio_SDL; //Para poder llamar a la recepción de callback.
	};

	private:

	std::vector<Canal_audio_real> canales;

	///////////////////////////////////////
	// Amigos.

	friend void callback_fin_reproduccion(int);
	friend class Cola_sonido;
	friend class Canal_audio; //Para que pueda acceder a la definición de "Canal_audio_real".
};

//Exposición pública del canal de audio para usarlo fuera del controlador. 
//Tiene una referencia al canal real que asegura que todo se sincronice.

class Canal_audio
{
	private:

	Controlador_audio_SDL::Canal_audio_real * canal;

	public:

	void forzar_parada() {canal->forzar_parada();}
	//void recibir_callback_finalizacion() {canal.recibir_callback_finalizacion();}
	int iniciar_reproduccion(const Estructura_sonido& e) {return canal->iniciar_reproduccion(e);}
	int iniciar_fade_out(int ms) {return canal->iniciar_fade_out(ms);}

	bool es_en_loop() const {return canal->es_en_loop();}
	bool es_ocupado() const {return canal->ocupado;}
	bool es_monitoreado() const {return canal->monitoreado;}
	bool es_reproduciendo() const {return canal->reproduciendo;}
	bool es_pausado() const {return canal->pausado;}
	int acc_indice() const {return canal->indice;}
	int acc_repeticiones() const {return canal->repeticiones;}
	int acc_volumen() const {return canal->volumen;}

	void establecer_volumen(int val) {canal->establecer_volumen(val);}
	void activar_monitoreado() {canal->activar_monitoreado();}
	void desactivar_monitoreado() {canal->desactivar_monitoreado();}
	void liberar() {canal->liberar();}
	void pausar() {canal->pausar();}
	void reanudar() {canal->reanudar();}

	bool es_vinculado() const {return canal!=NULL;}
	void desvincular() {canal=NULL;}

	Canal_audio():canal(NULL){}
	Canal_audio(Controlador_audio_SDL::Canal_audio_real& c):canal(&c) {}
	Canal_audio(const Canal_audio& c):canal(c.canal){}

	Canal_audio& operator=(const Canal_audio& c)
	{
		canal=c.canal;
		return *this;
	}
};

}
#endif
