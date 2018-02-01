#ifndef CONTROLES_SDL_H
#define CONTROLES_SDL_H

#include <SDL/SDL.h>
#include <iostream>
#include <cstring> //Para memset.
#include <vector>
#include <string>

namespace DLibI
{

class Controles_SDL
{
	private:

	//El bloque de control es quien interactúa realmente con la capa inferior de la SDL.
	class Teclado
	{
		private:
		Uint8 * teclas_pulsadas; //Lo manejaremos todo manualmente...

		public:
		Teclado() {teclas_pulsadas=NULL;}
		~Teclado() {teclas_pulsadas=NULL;}
		void mut_teclas_pulsadas(Uint8 * teclas) {teclas_pulsadas=teclas;}
		Uint8 const * acc_teclas_pulsadas() const {return teclas_pulsadas;}
		bool es_tecla_pulsada(int p_tecla) const {return this->teclas_pulsadas[p_tecla];}
	};

	//Aquí dentro se guarda todo lo que tiene que ver con el raton.

	class Raton
	{
		public:	//Public porque esto si se devuelve es como "const" y es más cómodo que andar con accedentes a este nivel.
		int x, y;

		//Para los puristas, versiones acc...
		int acc_x() const {return this->x;}
		int acc_y() const {return this->y;}

		bool movimiento;

		char * botones_up;
		char * botones_down;
		char * botones_pulsados;

		static const unsigned int MAX_BOTONES=6;	
	
		Raton()	
		{
			this->x=0; 
			this->y=0;
			this->movimiento=false;

			this->botones_up=new char[MAX_BOTONES];	//0 no es nada, 5 es la última rueda...
			this->botones_down=new char[MAX_BOTONES];

			this->botones_pulsados=new char[MAX_BOTONES];
			memset(this->botones_pulsados, 0, MAX_BOTONES);

		}

		~Raton()
		{
			delete [] this->botones_up;
			delete [] this->botones_down;
			delete [] this->botones_pulsados;
		}

		void inicializar_estado()
		{
			memset(this->botones_up, 0, MAX_BOTONES);
			memset(this->botones_down, 0, MAX_BOTONES);
			this->movimiento=false;
		}
	};

	class Joystick
	{
		public:

		unsigned int numero;
		unsigned int botones;
		unsigned int cantidad_ejes;
		SDL_Joystick * estructura;
		char * botones_up;
		char * botones_down;
		char * botones_pulsados;
		char * botones_soltados;
		Sint16 * ejes;
		std::string nombre;

		Joystick(unsigned int p_i)
		{
			this->numero=p_i;

			this->botones=0;
			this->cantidad_ejes=0;

			this->estructura=NULL;	

			this->botones_up=NULL;
			this->botones_down=NULL;
			this->botones_pulsados=NULL;
			this->botones_soltados=NULL;
			this->ejes=NULL;

			this->inicializar();
		}

		void inicializar()
		{
			this->estructura=SDL_JoystickOpen(this->numero);	//De alguna manera el valgrind saca aquí una pérdida.
			this->botones=SDL_JoystickNumButtons(this->estructura);
			this->cantidad_ejes=SDL_JoystickNumAxes(this->estructura);
			this->nombre=SDL_JoystickName(this->numero);				

			if(this->botones)
			{
				this->botones_up=new char[this->botones];
				this->botones_down=new char[this->botones];
				this->botones_pulsados=new char[this->botones];
				this->botones_soltados=new char[this->botones];
			}

			if(this->cantidad_ejes)
			{
				this->ejes=new Sint16[this->cantidad_ejes];
			}

			this->inicializar_estado();	

			/*OJO: No vamos a mover esto a "inicializar estado!!!",
			si lo hacemos se perderá el estado del input de botones
			pulsados o movimientos de ejes!!!.*/

			memset(this->botones_pulsados, 0, this->botones);
			memset(this->botones_soltados, 1, this->botones);

			if(this->cantidad_ejes) 
			{
				memset(this->ejes, 0, this->cantidad_ejes);
			}

		}

		void registrar_boton(unsigned int v_tipo, unsigned int v_boton)
		{
			if(v_tipo==0)
			{
				this->botones_down[v_boton]=1;
				this->botones_pulsados[v_boton]=1;
				this->botones_soltados[v_boton]=0;
			}
			else
			{
				this->botones_up[v_boton]=1;
				this->botones_soltados[v_boton]=1;
				this->botones_pulsados[v_boton]=0;
			}
		}
	
		void registrar_eje(unsigned int v_eje, Sint16 v_valor)
		{
			this->ejes[v_eje]=v_valor;
		}	

		void inicializar_estado()
		{
			if(this->botones)
			{
				memset(this->botones_up, 0, this->botones);
				memset(this->botones_down, 0, this->botones);
			}
		}

		~Joystick()
		{
			if(this->botones)
			{
				delete [] this->botones_up;
				delete [] this->botones_down;
				delete [] this->botones_pulsados;
				delete [] this->botones_soltados;	
			}

			if(this->cantidad_ejes)
			{
				delete [] this->ejes;
			}

			SDL_JoystickClose(this->estructura);
			this->estructura=NULL;
		}		
	};

	/*Esta clase controla los eventos de actividad (minimizar, maximizar,
	pérdida de foco...). Será un miembro de la clase Controles de modo que
	siempre estará ahí con los últimos valores que tuviera. Para usarla
	correctamente preguntaremos siempre si hay algún evento de actividad
	y luego, si lo hay, preguntaremos ya por el estado y el foco.

	Conservamos una sana actitud escéptica a que se den varios eventos de
	actividad a la vez. Si lo hubiera tendríamos que cambiar toda la 
	interface pública por una cola de eventos.
	*/

	class Evento_actividad
	{
		private:

		Uint8 estado;
		bool foco;
		bool registrado_evento_actividad;

		public:

		Evento_actividad():estado(0), foco(false), registrado_evento_actividad(false){}
		~Evento_actividad(){}

		void recibir_input(bool p_foco, Uint8 p_estado)
		{
			this->foco=p_foco;
			this->estado=p_estado;
			this->registrado_evento_actividad=true;
		}

		void reiniciar()
		{
			this->registrado_evento_actividad=false;
			this->foco=false;
			this->estado=0;
		}
	
		bool es_registrado_evento_actividad() const {return this->registrado_evento_actividad;}
		bool es_foco() const {return this->foco;}
		Uint8 acc_estado() const {return this->estado;}
	};

	private:

	SDL_Event eventos; //Las cosas que ocurran, como cerrar la ventana
	Teclado teclado;
	Raton raton;		//Siempre hay un ratón, no?.
	Evento_actividad evento_actividad;
	std::vector<Joystick *> joysticks;

	bool senal_salida; //SDL_QUIT; básicamente...
	unsigned short int joystick_defecto;
	unsigned short int cantidad_joysticks;

	char * teclas_down;
	char * teclas_up;

	void inicializar_teclas();
	void inicializar_joysticks();
	void limpiar_estado_joysticks();
	void limpiar_estado_eventos_actividad();
	void cerrar_joysticks();
	bool comprobacion_boton_joystick(unsigned int, unsigned int) const;
	bool comprobacion_eje_joystick(unsigned int, unsigned int) const;

	/*Esto registra simplemente si hay algún evento de este tipo por cada
	ciclo de eventos.*/

	bool hay_eventos_movimiento_raton;
	bool hay_eventos_boton_raton;
	bool hay_eventos_teclado;
	bool hay_eventos_eje_joystick;
	bool hay_eventos_boton_joystick;

	/*Registra el foco...*/

	public:

	enum ESTADOS{E_FOCO_RATON=SDL_APPMOUSEFOCUS, E_FOCO_INPUT=SDL_APPINPUTFOCUS, E_FOCO_ACTIVIDAD=SDL_APPACTIVE};	
	enum VALORES_EJES{MIN_EJE=-32768, MAX_EJE=32767, MIN_RUIDO=-3200, MAX_RUIDO=3200};

	bool bombear_eventos_manual(SDL_Event &, bool=true);
	Controles_SDL();
	~Controles_SDL();
	Teclado const & acc_teclado() const {return this->teclado;}
	SDL_Event const & acc_eventos() const {return this->eventos;}
	Raton const & acc_raton() const {return raton;}
	Joystick const & acc_joystick(int indice) const {return *(joysticks[indice]);}
	void recoger();
	void limpiar_para_nueva_recogida();
	void procesar_evento(SDL_Event &);
	void mover_raton(unsigned int p_x, unsigned int p_y) {SDL_WarpMouse(p_x, p_y);}
	bool es_tecla_pulsada(int p_tecla) const {return this->teclado.es_tecla_pulsada(p_tecla);}
	bool es_tecla_down(int p_tecla) const {return this->teclas_down[p_tecla];}
	bool es_tecla_up(int p_tecla) const {return this->teclas_up[p_tecla];}
	bool es_boton_up(int p_boton) const {return this->raton.botones_up[p_boton];}
	bool es_boton_down(int p_boton) const {return this->raton.botones_down[p_boton];}
	bool es_boton_pulsado(int p_boton) const {return this->raton.botones_pulsados[p_boton];}
	bool es_movimiento_raton() const {return this->raton.movimiento;}
	bool es_joystick_boton_down(unsigned int, unsigned int) const;
	bool es_joystick_boton_up(unsigned int, unsigned int) const;
	bool es_joystick_boton_pulsado(unsigned int, unsigned int) const;
	bool es_joystick_boton_soltado(unsigned int, unsigned int) const;
	bool es_joystick_boton_down(unsigned int) const;
	bool es_joystick_boton_up(unsigned int) const;
	bool es_joystick_boton_pulsado(unsigned int) const;
	bool es_joystick_boton_soltado(unsigned int) const;
	Sint16 joystick_eje(unsigned int, unsigned int) const;
	Sint16 joystick_eje(unsigned int) const;
	unsigned short int acc_cantidad_joysticks() const {return this->cantidad_joysticks;}

	bool es_evento_quit(SDL_Event &) const;
	bool es_evento_mousemotion(SDL_Event &) const;
	bool es_evento_mousebuttondown(SDL_Event &) const;
	bool es_evento_mousebuttonup(SDL_Event &) const;
	bool es_evento_keydown(SDL_Event &) const;
	bool es_evento_keyup(SDL_Event &) const; 
	bool es_senal_salida() const {return senal_salida;}
	void establecer_joystick_defecto(unsigned int p_valor) {this->joystick_defecto=p_valor;}

	bool recibe_eventos_raton() const {return this->hay_eventos_movimiento_raton || this->hay_eventos_boton_raton;}
	bool recibe_eventos_movimiento_raton() const {return this->hay_eventos_movimiento_raton;}
	bool recibe_eventos_boton_raton() const {return this->hay_eventos_boton_raton;}
	bool recibe_eventos_teclado() const {return this->hay_eventos_teclado;}
	bool recibe_eventos_joystick() const {return this->hay_eventos_eje_joystick || this->hay_eventos_boton_joystick;}
	bool recibe_eventos_eje_joystick() const {return this->hay_eventos_eje_joystick;}
	bool recibe_eventos_boton_joystick() const {return this->hay_eventos_boton_joystick;}
	bool recibe_eventos_input() const {return this->recibe_eventos_raton() || this->recibe_eventos_teclado() || this->recibe_eventos_joystick();}
	bool es_evento_actividad() const {return this->evento_actividad.es_registrado_evento_actividad();}
	bool es_foco_evento_actividad() const {return this->evento_actividad.es_foco();}
	Uint8 acc_estado_evento_actividad() const {return this->evento_actividad.acc_estado();}
};

} //Fin namespace DLibI

#endif
