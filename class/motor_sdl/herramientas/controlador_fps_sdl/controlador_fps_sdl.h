#ifndef CONTROLADOR_FPS_SDL_H
#define CONTROLADOR_FPS_SDL_H

//temporizador.Encender_Apagar(true);
//if(temporizador.CortarFps()) temporizador.Pausar_CortarFPS();

#include <SDL/SDL.h>
#include <vector>
#include <algorithm>

/*
Incluye el controlador de frames y el cronómetro. El controlador nos servirá
para controlar los FPS y para obtener la cantidad de delta desde el último 
frame.

El crónometro_SDL está muy ligado al Controlador de FPS. A un controlador le
podemos asignar referencias a uno o más cronómetros y por cada turno del 
controlador se irá pasando el tiempo al crono. Más sencillo imposible. Estos
cronómetros deben mantener referencias fuera de la clase: sin ellas no 
podremos manejarlos correctamente.

OJO: En el momento en que adjuntamos un cronómetro al control de frames ese 
cronómetro no puede "salirse" de ámbito, por así decirlo. Allí donde exista
el control deben existir aún los cronómetros.
*/

namespace DLibH
{

class Cronometro_SDL 
{
	private:

	static const int LIMITE_INACTIVO=-1;

	bool activo;
	float cuenta;
	float limite;

	public:

	Cronometro_SDL(bool p_activo=true, float p_limite=LIMITE_INACTIVO)
		:activo(p_activo), cuenta(0), limite(p_limite) 
	{

	}

	float acc_limite() const {return this->limite;}
	float acc_cuenta() const {return this->cuenta;}
	bool es_activo() const {return this->activo;}
	bool es_pasado_limite() const {return this->limite != LIMITE_INACTIVO && this->cuenta > this->limite;}
	void establecer_limite(float p_valor) {this->limite=p_valor;} 
	void eliminar_limite() {this->limite=LIMITE_INACTIVO;}
	void turno(float p_delta) {this->cuenta+=p_delta;}
	void reiniciar() {this->cuenta=0;}
	void activar() {this->activo=true;}
	void desactivar() {this->activo=false;}
};

class Controlador_fps_SDL
{
	private:

	bool aplicar;
	Uint32 ticks_cuenta;
	Uint32 ticks_fin;
	Uint32 ticks_inicio;
	Uint32 dif;
	unsigned int frames_contados;	//Este es el "visible"...
	unsigned int frames_contados_interno;	//Este es "no visible"...
	unsigned int duracion_frame;
	unsigned int frames_deseados;
	float delta;

	std::vector<Cronometro_SDL *> cronos;

	void calcular_duracion_frame();

	public:

	Controlador_fps_SDL(unsigned int p_f=30);
	~Controlador_fps_SDL();

	unsigned int acc_frames_contados() const {return this->frames_contados;}
	unsigned int acc_frames_deseados() const {return this->frames_deseados;}

	Cronometro_SDL * insertar_crono(Cronometro_SDL *);
	Cronometro_SDL * insertar_crono(Cronometro_SDL &);
	bool retirar_crono(Cronometro_SDL *);
	bool retirar_crono(Cronometro_SDL &);

	void procesar_cronos();
	void vaciar_cronos();

	void incrementar_framerate();
	void decrementar_framerate();
	void establecer_framerate(unsigned int);
	void mut_aplicar(bool);

	void inicializar();	//Justo antes del loop principal llamamos aquí.
	void turno();		//Esto lo llamamos justo después del render (o antes, lo que sea).
	void limitar_fps();	//Esto es para aplicar el tope de FPS después de turno();
	float obtener_delta();	//Esto es para obtener el valor delta después de turno();
	float obtener_delta_para_tiempo(Uint32);	//Esto es para obtener el valor delta después de turno();
};

}
#endif
