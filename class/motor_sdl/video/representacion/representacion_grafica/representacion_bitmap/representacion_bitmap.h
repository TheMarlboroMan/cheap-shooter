#ifndef REPRESENTACION_BITMAP
#define REPRESENTACION_BITMAP

#include "../representacion_grafica.h"

namespace DLibV
{


class Representacion_bitmap:public Representacion_grafica
{
	public:

	Representacion_bitmap();
	Representacion_bitmap(const Representacion_bitmap&);
	Representacion_bitmap(const Recurso_grafico *);
	virtual ~Representacion_bitmap();
	Representacion_bitmap& operator=(const Representacion_bitmap &);

	virtual void establecer_recurso(Recurso_grafico const *);
	void preparar() {marcar_como_preparada();}
	virtual bool es_estatica() const=0;
};

class Representacion_bitmap_dinamica:public Representacion_bitmap
{
	public:

	Representacion_bitmap_dinamica();
	Representacion_bitmap_dinamica(const Representacion_bitmap_dinamica&);
	Representacion_bitmap_dinamica& operator=(const Representacion_bitmap_dinamica&);
	Representacion_bitmap_dinamica(const Recurso_grafico *);
	virtual ~Representacion_bitmap_dinamica() {}

	bool es_estatica() const{return false;}
};

class Representacion_bitmap_estatica:public Representacion_bitmap
{
	public:

	Representacion_bitmap_estatica();
	Representacion_bitmap_estatica(const Representacion_bitmap_estatica&);
	Representacion_bitmap_estatica& operator=(const Representacion_bitmap_estatica&);
	Representacion_bitmap_estatica(const Recurso_grafico *);
	virtual ~Representacion_bitmap_estatica() {}
	bool es_estatica() const {return true;}
};

} //Fin namespace DLibV

#endif
