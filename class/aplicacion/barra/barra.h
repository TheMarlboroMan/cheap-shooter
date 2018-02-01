#ifndef BARRA_H
#define BARRA_H

#include "../../motor_sdl/video/representacion/representacion_grafica/representacion_pixeles/representacion_pixeles.h"
#include <algorithm>

class Barra
{
	private:

	DLibV::Representacion_pixeles_estatica * rep;	//El lienzo de dibujo, vamos a darle unas dimensiones máximas.

	unsigned int ancho;	
	unsigned int altura;
	unsigned int valor_maximo;
	unsigned int valor_actual;
	unsigned int orientacion;
	unsigned int modelo_relleno;


	Uint32 color_barra;
	Uint32 color_fondo;

	protected:

	void dibujar(Uint32);
	void dibujar(Uint32, int);
	void rellenar(Uint32);

	public:

	enum orientaciones{
		O_HORIZONTAL,
		O_VERTICAL
	};

	enum modelos_relleno{
		M_IZQUIERDA_A_DERECHA,
		M_DERECHA_A_IZQUIERDA
	};
		
	Barra(unsigned int, unsigned int);
	virtual ~Barra();

	DLibV::Representacion * acc_representacion() {return this->rep;}
	void establecer_valor_actual(unsigned int p_valor) {this->valor_actual=p_valor;}
	void establecer_valor_maximo(unsigned int p_valor) {this->valor_maximo=p_valor;	}
	void establecer_orientacion(unsigned int o) {if(o==O_HORIZONTAL || o==O_VERTICAL) orientacion=o;}
	void establecer_modelo_relleno(unsigned int m) {if(m==M_IZQUIERDA_A_DERECHA || m==M_DERECHA_A_IZQUIERDA) modelo_relleno=m;}
	void establecer_color_fondo(Uint32 p_param){this->color_fondo=p_param;}
	void establecer_color_barra(Uint32 p_param){this->color_barra=p_param;}
	void establecer_posicion(int p_x, int p_y){this->rep->establecer_posicion(p_x, p_y);}

	virtual void actualizar();	

	unsigned int acc_ancho() const {return ancho;}
	unsigned int acc_altura() const {return altura;}
	unsigned int acc_valor_maximo() const {return valor_maximo;}
	unsigned int acc_valor_actual() const {return valor_actual;}
};


#endif
