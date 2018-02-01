#ifndef HUD_ARTEFACTOS_H
#define HUD_ARTEFACTOS_H

/*Esta clase controla el HUD que indica los artefactos disponibles y activos.*/

#include <map>
#include "../../motor_sdl/libDanSDL.h"
#include "../recursos/recursos.h"
#include "../definiciones/definiciones.h"

class Hud_artefactos
{
	private:

	struct Parte_hud
	{
		enum estados {
			E_INACTIVO,
			E_ACTIVO,
			E_NO_DISPONIBLE
		};

		static const int W=6;
		static const int H=6;
		static const int X_ACTIVO=160;
		static const int Y_ACTIVO=48;
		static const int X_INACTIVO=166;
		static const int Y_INACTIVO=48;
		static const int X_NO_DISPONIBLE=160;
		static const int Y_NO_DISPONIBLE=54;


		unsigned int estado;
		DLibV::Representacion_bitmap_estatica rep;

		Parte_hud():
			estado(E_INACTIVO),
			rep(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_JUGADOR))
		{
			rep.establecer_posicion(0,0,6,6);
			desactivar();
		}

		void posicionar(int x){rep.establecer_posicion(x, 0);}

		void activar(){rep.establecer_recorte(X_ACTIVO, Y_ACTIVO, W, H);}
		void desactivar(){rep.establecer_recorte(X_INACTIVO, Y_INACTIVO, W, H);}
		void anular(){rep.establecer_recorte(X_NO_DISPONIBLE, Y_NO_DISPONIBLE, W, H);}

		//Los constructores y operadores de copia por defecto nos servirán.
	};

	DLibV::Representacion_agrupada_estatica rep;
	typedef std::pair<unsigned int, Parte_hud> t_parte;
	std::map<unsigned int, Parte_hud> mapa;

	bool comprobar_artefacto(unsigned int);

	public:

	Hud_artefactos();
	~Hud_artefactos();

	void activar_artefacto(unsigned int);
	void desactivar_artefacto(unsigned int);
	void anular_artefacto(unsigned int);
	void establecer_posicion(int x, int y) {rep.establecer_posicion(x, y);}
	DLibV::Representacion& acc_representacion() {return rep;}
};

#endif
