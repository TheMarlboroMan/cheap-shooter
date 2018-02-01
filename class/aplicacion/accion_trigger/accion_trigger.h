#ifndef ACCION_TRIGGER_H
#define ACCION_TRIGGER_H

#include <string>
#include <fstream>
#include <cstdint>

/*
Las acciones se graban con cada nivel. La acción tiene un tiempo en ms antes
de ser ejecutada y otro después. Tiene un tag propio, al finalizar puede llamar
a otra acción que tenga "tag_siguiente" y tiene además el tag_actores para
indicar a los actores que afecta.

Hay un poco de cajón desastre en que meteremos parámetros que igual no se
usan, como el tag de actores secundario (que sería sólo
para las acciones de ruta, por ejemplo).

De por si la acción no es nada, es sólo el patrón que se guarda en disco. Las
acciones se harán concretas en otra clase que se instanciaría tantas veces
como la acción se ejecute, conteniendo los vectores para los actores y demás,
la referencia al trigger que lo ha ejecutado y alguna cosa más.

De por si las acciones siempre actúan sobre un actor de mapa que ya existe ya
que al "actuar" llaman al método "recibir acción trigger" de cada uno de los
actores que tengan el tag correspondiente. Para actuar sobre otros tipos de
elementos (como el propio mapa, el fondo y cosas así) hay que usar "obstáculos
de actividad puente". Por ejemplo, hay obstáculos para controlar el fondo 
o las propiedades del nivel que al recibir el trigger se activan, al activarse
se recogen por el controlador y al procesar su actividad afectan al nivel.
*/

class Accion_trigger
{
	private:

	typedef uint16_t U_ENTERO_16;
	typedef int16_t S_ENTERO_16;
	typedef uint8_t U_ENTERO_8;

	std::string descripcion;

	U_ENTERO_16 ms_antes;
	U_ENTERO_16 ms_despues;
	U_ENTERO_16 tag;
	U_ENTERO_16 tag_actores;
	U_ENTERO_16 tag_actores_secundario;
	U_ENTERO_16 tag_siguiente;
	S_ENTERO_16 desp_x;
	S_ENTERO_16 desp_y;

	U_ENTERO_8 tipo;

	public:

	enum TIPOS{
	T_ACTIVAR=0,
	T_DESACTIVAR,
	T_ACTIVAR_DESACTIVAR,
	T_RUTA,
	T_DESACTIVAR_RUTA,
	T_IR_A,
	T_PROPIEDADES_FONDO,
	T_PROPIEDADES_NIVEL,
	T_HACER_VISIBLE,
	T_HACER_INVISIBLE,
	T_INTERCAMBIAR_VISIBILIDAD,
	T_CAMBIAR_VECTOR,
	T_GENERAR_DECORACION,
	T_MOVIMIENTO_RELATIVO,
	T_DETENER_MOVIMIENTO_RELATIVO
	};

	void serializar(std::ofstream&) const;
	static Accion_trigger deserializar(std::ifstream&);

	void mut_descripcion(const char * p_param) {this->descripcion=p_param;}
	void mut_ms_antes(U_ENTERO_16 p_param) {this->ms_antes=p_param;}
	void mut_ms_despues(U_ENTERO_16 p_param) {this->ms_despues=p_param;}
	void mut_tag(U_ENTERO_16 p_param) {this->tag=p_param;}
	void mut_tag_actores(U_ENTERO_16 p_param) {this->tag_actores=p_param;}
	void mut_tag_actores_secundario(U_ENTERO_16 p_param) {this->tag_actores_secundario=p_param;}
	void mut_tag_siguiente(U_ENTERO_16 p_param) {this->tag_siguiente=p_param;}
	void mut_desp_x(S_ENTERO_16 p_param) {this->desp_x=p_param;}
	void mut_desp_y(S_ENTERO_16 p_param) {this->desp_y=p_param;}
	void mut_tipo(U_ENTERO_16 p_param) {this->tipo=p_param;}

	std::string acc_descripcion() const {return this->descripcion;}
	U_ENTERO_16 acc_ms_antes() const {return this->ms_antes;}
	U_ENTERO_16 acc_ms_despues() const {return this->ms_despues;}
	U_ENTERO_16 acc_tag() const {return this->tag;}
	U_ENTERO_16 acc_tag_actores() const {return this->tag_actores;}
	U_ENTERO_16 acc_tag_actores_secundario() const {return this->tag_actores_secundario;}
	U_ENTERO_16 acc_tag_siguiente() const {return this->tag_siguiente;}
	S_ENTERO_16 acc_desp_x() const {return this->desp_x;}
	S_ENTERO_16 acc_desp_y() const {return this->desp_y;}
	U_ENTERO_8 acc_tipo() const {return this->tipo;}

	Accion_trigger();
	Accion_trigger& operator=(const Accion_trigger&);
	~Accion_trigger();
};

#endif
