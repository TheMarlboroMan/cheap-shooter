#ifndef REPRESENTACION_MANUAL_H
#define REPRESENTACION_MANUAL_H
#include "representacion_grafica.h"

/*Clase base para todas las clases que permitan la creación de su superficie
de video propia. Sólo da los métodos abstractos para que se creen, preparen y
demás historias. Por lo demás, no tiene ninguna otra funcionalidad.

De esta deriva la representación lienzo, que permite crear una superficie de
unas dimensiones determinadas. Aunque se puede mover aquí el tema de crear
para ciertas dimensiones, creo que es mejor si lo dejamos así. Primero porque
ya funciona y segundo... por no tocar todo lo usaría la clase derivada.*/

namespace DLibV
{

class Representacion_manual:public Representacion_grafica
{
	private:

	Uint32 clave_color;
	bool clave_color_activa;

	protected:

	//No se da una implementación: debe darla la derivada. Es posible
	//que se haga una representación manual cuyo recurso gráfico no sea 
	//manejado por esta misma, de modo que no se genera.
	virtual void destruir_recurso()=0;

	public:

	Representacion_manual();
	Representacion_manual(const Representacion_manual&);
	virtual ~Representacion_manual();
	Representacion_manual& operator=(const Representacion_manual&);

	//Lo único que hace este método es dar una puerta de entrada para generar el recurso gráfico propio.
	//Lo que cada clase derivada pueda necesitar ya se sale de lo que esto puede facilitar y,
	//por tanto, debe proveerlo cada derivada.

	virtual void iniciar_recurso()=0;
	virtual void preparar()=0;
	virtual bool es_estatica() const=0; 

	//Estos métodos están aquí para manipular los recursos generados por estas
	//clases. No nos sirven, por ejemplo, para manipular representaciones bitmap.
	//Y eso es correcto. La próxima vez que pienses en quitarlo, please, 
	//piensa que por ejemplo el texto debe establecer la clave de color de
	//su recurso (no de su fuente) y eso lo hace por medio de estos métodos.

	void establecer_clave_color(int, int,int);
	void establecer_clave_color(Uint32);
	void establecer_clave_color(SDL_PixelFormat *);
	void establecer_clave_color(SDL_Surface *);
	void desactivar_clave_color();
	Uint32 acc_clave_color() const {return this->clave_color;}
};

} //Fin namespace DLibV

#endif
