/*Este archivo puede usarse como base para las pruebas de turno. Se
compilaría con -L. -lDanSDL -lSDL -lSDL_mixer*/

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include "../libDanSDL.h"
#include "../defDanSDL.h"

class Controlador
{
	private:

	DLibV::Pantalla pantalla;
	DLibI::Controles_SDL controles_sdl;
	DLibA::Controlador_audio_SDL * audio;
	
	public:

	Controlador():
		pantalla(), controles_sdl(), audio(DLibA::Controlador_audio_SDL::obtener())
	{
	
	}

	~Controlador()
	{
		DLibA::Gestor_recursos_audio::liberar();
		DLibV::Gestor_recursos_graficos::liberar();
		DLibA::Controlador_audio_SDL::desmontar();
	}

	void inicializar_pantalla(int p_w, int p_h, const char * p_titulo)
	{
		this->pantalla.inicializar(p_w, p_h);
		this->pantalla.establecer_titulo(p_titulo);
		DLibV::Gestor_color::establecer_formato(this->pantalla.obtener_formato_pixeles());
	}

	void rellenar_pantalla(unsigned int r, unsigned int g, unsigned int b)
	{
		this->pantalla.rellenar(r,g,b);
	}

	void volcar_en_pantalla(DLibV::Representacion& R)
	{
		this->pantalla.volcar(R);
	} 

	void flipar_pantalla()	
	{
		this->pantalla.flipar();
	}

	DLibI::Controles_SDL procesar_eventos()
	{
		this->controles_sdl.recoger();
		
		return this->controles_sdl;
	}

	DLibV::Pantalla & acc_pantalla() {return this->pantalla;}
	DLibI::Controles_SDL & acc_controles_sdl() {return this->controles_sdl;}
};

class Gestor_recursos
{
	public:

	static bool insertar(unsigned int indice, std::string p_ruta)
	{
		return DLibV::Gestor_recursos_graficos::insertar(indice, new DLibV::Imagen(p_ruta.c_str()))!=-1;	
	}
	
	static bool transparencia(unsigned int indice, unsigned int r, unsigned int g, unsigned int b)
	{
		DLibV::Gestor_recursos_graficos::obtener(indice)->establecer_transparencia(r,g,b);
	}

	static bool alpha(unsigned int indice, unsigned int alpha)
	{
		DLibV::Gestor_recursos_graficos::obtener(indice)->establecer_alpha(alpha);
	}

	static DLibV::Recurso_grafico * obtener(unsigned int indice)
	{
		return DLibV::Gestor_recursos_graficos::obtener(indice);
	}
};
	

int main(int argc, char ** argv)
{
	if(!DLibH::Herramientas_SDL::iniciar_SDL(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK))
	{
		DLibH::Herramientas_SDL::error();
		return 1;
	}
	else
	{
		//Este orden siempre sería el mismo...

		Controlador C;
		C.inicializar_pantalla(1024, 768, "TEST");

		//Insertar los recursos gráficos aquí...

		//Gestor_recursos::insertar(1, "ruta.bmp");
		//Gestor_recursos::transparencia(1, 0, 255, 255);
		//Gestor_recursos::alpha(1, 0);		

		//Insertar aquí la lógica...
		//C.rellenar_pantalla(124, 245, 123);
		//C.flipar_pantalla();
		//SDL_Delay(1000);		
	}

	DLibH::Herramientas_SDL::apagar_SDL();	
	return 0;
}
