#ifndef CONFIGURACION_CONTROLES
#define CONFIGURACION_CONTROLES

/*
La configuración de controles sería un proxy entre el juego
y los controles SDL.

En lugar de preguntar si se ha pulsado tal tecla en el juego
o tal botón podría ser como:

Configuracion_controles.es_input_abajo(Controles_SDL).

La cosa es que supuestamente se soportan dos modelos de control y 
que preguntar por los ejes del joystick puede ser horrible
de modo que vamos a definir sólo aquellas cosas que sabemos que
pueden cambiar. Serían:

usar_artefacto
parar_tiempo
cambiar_artefacto_up
cambiar_artefacto_down

movimiento_arriba
movimiento_abajo
movimiento_izquierda
movimiento_derecha

pausa

Algunas cosas estarían fijas: como usar los ejes del joystick para
moverse o el ratón para mover la mirilla. El resto sería seleccionable.

Problemas técnicos que puede plantear esto?. Básicamente guardar la
asociación de datos. Podría ser una estructura así:

configuracion_controles_datos
{
	enum tipo{teclado, joystick, raton}
	unsigned int valor;
}	

Y a la que preguntaríamos así:

switch(this->tipo)
{
	case teclado:
		return controles_sdl.es_tecla_pulsada(valor);
	break;

	case joystick...
	case raton...
}

Necesitaríamos preguntar (para cada tipo) por el estado up, down o pulsado.
*/

#include <fstream>
#include <string>
#include <cstdint>
#include "../../motor_sdl/def_input.h"
#include "../../motor_sdl/herramientas/herramientas/herramientas.h"

class Configuracion_controles
{
	public:

	enum tipo{
	T_NADA=0,
	T_TECLADO,
	T_BOTON_RATON,
	T_BOTON_JOYSTICK};

	enum modo{
	T_UP,
	T_DOWN,
	T_PULSADO};

	enum joystick{
	JABAJO=14, 
	JIZQUIERDA=15, 
	JARRIBA=12, 
	JDERECHA=13, 
	JX=2, 
	JCIRCULO=1, 
	JTRIANGULO=0, 
	JCUADRADO=3, 
	JSTART=8, 
	JSELECT=9, 
	JR1=5, 
	JR2=7, 
	JL1=4, 
	JL2=6};

	enum codigos_aprendizaje{
	C_CANCELAR=-1,
	C_NADA=0,
	C_OK=1};

	typedef uint8_t U_ENTERO_8;
	typedef uint16_t U_ENTERO_16;
	typedef uint32_t U_ENTERO_32;

	typedef U_ENTERO_16 t_valor_interno;
	typedef U_ENTERO_8 t_valor_corto;

	private:

	struct Nodo_control
	{
		t_valor_corto tipo;
		t_valor_interno valor;

		Nodo_control():
			tipo(T_NADA), valor(0)
		{}

		Nodo_control(t_valor_corto p_tipo, t_valor_interno p_valor):
			tipo(p_tipo), valor(p_valor)
		{}
	
		void configurar(t_valor_corto p_tipo, t_valor_interno p_valor)
		{
			tipo=p_tipo;
			valor=p_valor;
		}

		bool comprobar(const DLibI::Controles_SDL &c, unsigned int p_tipo) const
		{
			switch(this->tipo)
			{
				case T_TECLADO:
					switch(p_tipo)
					{
						case T_UP: return c.es_tecla_up(this->valor); break;
						case T_DOWN: return c.es_tecla_down(this->valor); break;
						case T_PULSADO: return c.es_tecla_pulsada(this->valor); break;
						default: return false; break;
					}
				break;

				case T_BOTON_RATON:
					switch(p_tipo)
					{
						case T_UP: return c.es_boton_up(this->valor); break;
						case T_DOWN: return c.es_boton_down(this->valor); break;
						case T_PULSADO: return c.es_boton_pulsado(this->valor); break;
						default: return false; break;
					}
				break;

				case T_BOTON_JOYSTICK:
					switch(p_tipo)
					{
						case T_UP: return c.es_joystick_boton_up(this->valor); break;
						case T_DOWN: return c.es_joystick_boton_down(this->valor); break;
						case T_PULSADO: return c.es_joystick_boton_pulsado(this->valor); break;
						default: return false; break;
					}
				break;

				default:
				case T_NADA: return false; break;				
			}
		}

		short int aprender(SDL_Event &evento)
		{
			switch(evento.type)
			{
				case SDL_QUIT:
					return C_CANCELAR;
				break;

				case SDL_MOUSEBUTTONDOWN:
					this->tipo=T_BOTON_RATON;
					this->valor=evento.button.button;
					return C_OK;
				break;

				case SDL_JOYBUTTONDOWN:
					this->tipo=T_BOTON_JOYSTICK;
					this->valor=evento.jbutton.button;
					return C_OK;
				break;

				case SDL_KEYDOWN:
					this->tipo=T_TECLADO;
					this->valor=evento.key.keysym.sym;
					return C_OK;
				break;

				default: return C_NADA; break;
			}
		}

		std::string traducir() const
		{
			std::string resultado="";

			switch(this->tipo)
			{
				case T_TECLADO: resultado=SDL_GetKeyName( (SDLKey) this->valor); break;
				case T_BOTON_RATON: resultado="MOUSE BTN "+DLibH::Herramientas::entero_a_cadena(this->valor); break;
				case T_BOTON_JOYSTICK: resultado="JOYSTICK BTN "+DLibH::Herramientas::entero_a_cadena(this->valor); break;
				case T_NADA: break;				
			}

			return resultado;
		}

		void guardar(std::ofstream& p_archivo)
		{
			p_archivo.write( (char *) &this->tipo, sizeof(t_valor_corto));
			p_archivo.write( (char *) &this->valor, sizeof(t_valor_interno)); 
		}

		void cargar(std::ifstream& p_archivo)
		{
			p_archivo.read( (char *) &this->tipo, sizeof(t_valor_corto));
			p_archivo.read( (char *) &this->valor, sizeof(t_valor_interno)); 
		}
	};

	static const std::string RUTA_ARCHIVO;
	static const std::string EXTENSION_ARCHIVO;

	Nodo_control nodo_pausa;
	Nodo_control nodo_usar_artefacto;
	Nodo_control nodo_parar_tiempo;
	Nodo_control nodo_ciclar_artefacto_siguiente;
	Nodo_control nodo_ciclar_artefacto_anterior;
	Nodo_control nodo_movimiento_arriba;
	Nodo_control nodo_movimiento_derecha;
	Nodo_control nodo_movimiento_abajo;
	Nodo_control nodo_movimiento_izquierda;

//	DLibI::Controles_SDL& ref_controles;
	std::string extra_ruta;

	bool aprender(DLibI::Controles_SDL&, Nodo_control &);
	bool comprobar_nodo(DLibI::Controles_SDL&, const Nodo_control &, unsigned int) const;
	std::string generar_ruta();

	public:

	Configuracion_controles(const std::string&);
	~Configuracion_controles();

	void generar_por_defecto_teclado();
	void generar_por_defecto_joystick();

	bool es_pausa(DLibI::Controles_SDL& c, unsigned int t_tipo) const 			{return this->comprobar_nodo(c, nodo_pausa, t_tipo);}
	bool es_usar_artefacto(DLibI::Controles_SDL& c, unsigned int t_tipo) const 		{return this->comprobar_nodo(c, nodo_usar_artefacto, t_tipo);}
	bool es_parar_tiempo(DLibI::Controles_SDL& c, unsigned int t_tipo) const		{return this->comprobar_nodo(c, nodo_parar_tiempo, t_tipo);}
	bool es_ciclar_artefacto_siguiente(DLibI::Controles_SDL& c, unsigned int t_tipo) const	{return this->comprobar_nodo(c, nodo_ciclar_artefacto_siguiente, t_tipo);}
	bool es_ciclar_artefacto_anterior(DLibI::Controles_SDL& c, unsigned int t_tipo) const	{return this->comprobar_nodo(c, nodo_ciclar_artefacto_anterior, t_tipo);}
	bool es_movimiento_arriba(DLibI::Controles_SDL& c, unsigned int t_tipo) const		{return this->comprobar_nodo(c, nodo_movimiento_arriba, t_tipo);}
	bool es_movimiento_derecha(DLibI::Controles_SDL& c, unsigned int t_tipo) const		{return this->comprobar_nodo(c, nodo_movimiento_derecha, t_tipo);}
	bool es_movimiento_abajo(DLibI::Controles_SDL& c, unsigned int t_tipo) const		{return this->comprobar_nodo(c, nodo_movimiento_abajo, t_tipo);}
	bool es_movimiento_izquierda(DLibI::Controles_SDL& c, unsigned int t_tipo) const	{return this->comprobar_nodo(c, nodo_movimiento_izquierda, t_tipo);}

	bool aprender_pausa(DLibI::Controles_SDL& c)				{return this->aprender(c, nodo_pausa);}
	bool aprender_usar_artefacto(DLibI::Controles_SDL& c)			{return this->aprender(c, nodo_usar_artefacto);}
	bool aprender_parar_tiempo(DLibI::Controles_SDL& c)			{return this->aprender(c, nodo_parar_tiempo);}
	bool aprender_ciclar_artefacto_siguiente(DLibI::Controles_SDL& c)	{return this->aprender(c, nodo_ciclar_artefacto_siguiente);}
	bool aprender_ciclar_artefacto_anterior(DLibI::Controles_SDL& c)	{return this->aprender(c, nodo_ciclar_artefacto_anterior);}
	bool aprender_movimiento_arriba(DLibI::Controles_SDL& c)		{return this->aprender(c, nodo_movimiento_arriba);}
	bool aprender_movimiento_abajo(DLibI::Controles_SDL& c)			{return this->aprender(c, nodo_movimiento_abajo);}
	bool aprender_movimiento_izquierda(DLibI::Controles_SDL& c)		{return this->aprender(c, nodo_movimiento_izquierda);}
	bool aprender_movimiento_derecha(DLibI::Controles_SDL& c)		{return this->aprender(c, nodo_movimiento_derecha);}

	bool cargar();
	void guardar();
	std::string generar_cadena_configuracion() const;
};

#endif
