#ifndef LIENZO_H
#define LIENZO_H

#include "recurso_grafico.h"
#include "../gestor_color/gestor_color.h"

/*
Esto es un caso especial... Básicamente un recurso gráfico que va a generar
una superificie de video vacía para poderla manipular.
*/

namespace DLibV
{

class Lienzo:public Recurso_grafico
{
	private:
	bool generado;

	public:
	
	Lienzo();
	~Lienzo();
	Lienzo(const Lienzo&);
	Lienzo& operator=(const Lienzo&);
	static Lienzo * generar_nuevo(int=400, int=400); 	//Se genera un recurso que NO se registraría... No me termina de gustar.
	static Lienzo * copiar_lienzo(const Lienzo&);

	bool es_generado() const {return this->generado;}
	int obtener_tipo(){return Recurso_grafico::T_LIENZO;}
	bool es_preparado() {return this->generado;}
};

} //Fin namespace DLibV

#endif
