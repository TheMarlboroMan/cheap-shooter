#ifndef REPRESENTACION_LIENZO_H
#define REPRESENTACION_LIENZO_H

/*Es la clase que provee con una superficie SDL que manipular manualmente. 
Al crearla se generan el recurso y la superficie subyacente.

La principal diferencia con la manual es que permite decir las dimensiones
del recurso.

TODO: Mover el tema de las dimensiones a la manual. Que la manual genere un
lienzo...
*/

#include "representacion_manual.h"
#include "../../recurso_grafico/lienzo.h"

namespace DLibV
{


class Representacion_lienzo:public Representacion_manual
{
	protected:

	unsigned int ancho;
	unsigned int alto;

	void destruir_recurso();

	public:

	Representacion_lienzo(unsigned int, unsigned int);
	Representacion_lienzo& operator=(const Representacion_lienzo&);
	Representacion_lienzo(const Representacion_lienzo&);

	virtual ~Representacion_lienzo();
	void iniciar_recurso();
	void reiniciar_recurso(unsigned int, unsigned int); 

	virtual void preparar()=0;
	virtual bool es_estatica() const=0; 

	void exportar_bmp(const char *);
};

} //Fin namespace DLibV

#endif
