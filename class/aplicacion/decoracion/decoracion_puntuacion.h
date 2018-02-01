#ifndef DECORACION_PUNTUACION_H
#define DECORACION_PUNTUACION_H

/*
Este objeto es particular. Para cada "puntuación" existiría un objeto maestro
guardado en un mapa del controlador de puntuación. Cuando instanciamos uno
desde el controlador llamamos al objeto maestro que hace una copia de si mismo.
Esta copia no elimina los recursos al generarse, sino que apunta a los recursos
del maestro.

Más concretamente, el recurso maestro tiene un "representacion_texto_auto_estatica"
mientras que los que no son maestros tienen "representacion_bitmap".
*/

#include "decoracion.h"

//class Controlador_puntuacion; //F-f-f-forward declaration!!!.

class Decoracion_puntuacion:public Decoracion
{
	private:
	
	int puntuacion;
	bool es_copia;

	//Esta es la representación real. Es de un tipo concreto que requiere una manipulación q otros tipos no permiten.
	//Asignaremos el puntero de "Actor" a esta.
	DLibV::Representacion_bitmap_estatica * representacion_real;

	//El constructor es privado pero la clase es amiga del controlador, que hará el trabajo.	
	void crear_representacion_desde_maestra(DLibV::Representacion *);
	Decoracion_puntuacion(); 

	protected:

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	unsigned short int tipo_decoracion() {return Decoracion::D_PUNTUACION;}
	static const int VELOCIDAD_VECTOR=200;
	static const int POS_FINAL_X=12;
	static const int POS_FINAL_Y=560;

	~Decoracion_puntuacion();

	int acc_puntuacion() const {return this->puntuacion;}
	void mut_puntuacion(int p_valor) {this->puntuacion=p_valor;}

	void preparar_maestra();
	void poner_en_marcha();
	void procesar_turno(float=0);
	bool es_finalizado();
	Decoracion_puntuacion * generar_copia();

	void cuentame() {std::cout<<"SOY DECORACION DE PUNTUACION"<<std::endl;}

	friend class Controlador_puntuacion;
};

#endif
