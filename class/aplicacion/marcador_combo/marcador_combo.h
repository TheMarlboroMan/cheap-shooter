#ifndef MARCADOR_COMBO_H
#define MARCADOR_COMBO_H

#include "../../motor_sdl/video/representacion/representacion_grafica/representacion_bitmap/representacion_bitmap.h"
#include "../controlador_puntuacion/controlador_puntuacion.h"

/*No tiene nada que ver con las clases de actores ni nada de eso. Es una clase
simple, hecha desde cero...

Representa a cualquiera de los números que aparecen en el medidor de combo.

Tiene tres estados: estoy entrando en la pantalla, estoy como marcador activo
y estoy saliendo. Al construir se activa automáticamente el primer estado.
Lo siguiente que podemos hacer es activar llamar al método iniciar_salida y
una vez que haya terminado, destruirlo. Sabremos que hay que destruirlo cuando 
el método "procesar_turno" devuelva "false".

Al construirlo llamamos con un número que se corresponde con la cantidad de 
combo según el enum TIPOS_COMBO.
*/

class Marcador_combo
{
	private:
	
	unsigned short int tipo_combo;
	unsigned short int estado;
	float x;
	float y;

	DLibV::Representacion_bitmap_estatica representacion;

	static const int X_INICIAL=840;
	static const int Y_INICIAL=6;
	static const int X_ACTIVA=740;
	static const int Y_ACTIVA=6;
	static const int X_FINAL=740;
	static const int Y_FINAL=-40;

	static const int ANCHO=30;
	static const int ALTO=80;

	static const float CANTIDAD_MOVIMIENTO;

	enum ESTADOS {E_ENTRADA, E_ACTIVO, E_SALIDA};

	//Métodos privados.
	void establecer_recorte();

	public:

	//Constructor y destructor.
	Marcador_combo(unsigned int, DLibV::Recurso_grafico *);
	Marcador_combo(const Marcador_combo&);
	Marcador_combo& operator=(const Marcador_combo&);
	~Marcador_combo();

	DLibV::Representacion_bitmap_estatica& acc_representacion() {return this->representacion;}	
	bool procesar_turno(float);
	void iniciar_salida();
	unsigned int acc_tipo_combo() const {return this->tipo_combo;}
};

#endif
