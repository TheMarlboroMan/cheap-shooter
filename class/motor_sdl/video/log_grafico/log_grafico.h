#ifndef LOG_GRAFICO_H
#define LOG_GRAFICO_H

#include <string>
#include <vector>

#include "../../herramientas/herramientas/herramientas.h"
#include "../representacion/representacion_grafica/representacion_texto/representacion_texto_fija.h"

namespace DLibV
{

class Log_grafico
{
	public:

	typedef std::string t_log_grafico_tipo;

	struct Entrada_log
	{
		unsigned int lineas;
		t_log_grafico_tipo cadena;
	
		Entrada_log(const t_log_grafico_tipo& c, unsigned int l):
			lineas(l), cadena(c) {};
	};

	typedef std::vector<Entrada_log> t_log_grafico_contenedor;

	private:

	unsigned int proximo_id;
	unsigned int indice_actual;

	unsigned int lineas_vertical;
	unsigned int max_caracteres_linea;

	Representacion_texto_fija_estatica rep;	//La representación en si,
	t_log_grafico_contenedor contenedor;	//El vector...

	Entrada_log preparar_entrada(const t_log_grafico_tipo&);
	Log_grafico& operator=(const Log_grafico&);	//Está aquí para que no puedas usarlo.
	Log_grafico(const Log_grafico&); 		//Idem eadem.
	void redibujar();
	void realizar_calculos_iniciales();

	public:

	Log_grafico(const Recurso_grafico *, const SDL_Rect&);
	~Log_grafico();
	void insertar(const t_log_grafico_tipo&);
	void limpiar();
	bool avanzar();			//Intenta retroceder en el indice actual.	
	bool retroceder();		//Idem, pero para adelante.
	bool seleccionar(unsigned int);	//Idem, pero para escoger el deseado.
	unsigned int obtener_indice_actual() const {return this->indice_actual;}
	unsigned int obtener_cantidad() const {return this->contenedor.size();}

	void intercambiar_visibilidad() {this->rep.intercambiar_visibilidad();}
	void mut_alpha(Uint8 a) {this->rep.establecer_alpha(a);}
	void mut_color_fondo(Uint32 c) {this->rep.mut_color_fondo(c);}
	Representacion& acc_representacion() {return this->rep;}
};

}
#endif
