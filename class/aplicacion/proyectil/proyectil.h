#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "../no_jugador/no_jugador.h"

class Proyectil:public No_jugador
{
	private:

	const Actor * origen;

/*
Cuando un proyecil es de una facción NO chocará con ella.	
*/


	unsigned short int potencia;
	unsigned short int color;
	unsigned short int faccion;
	float velocidad;
	
//	bool rebota;

	//El constructor es privado: para crearlos los traemos por los métodos estáticos que hay.
	Proyectil();
	void establecer_dimensiones(unsigned short int);
	void actualizar_representacion();
	static Proyectil * nuevo_proyectil(unsigned short int=D_NORMAL);

	protected:

	void establecer_representacion();
	void establecer_caracteristicas();

	public:
//	void establecer_potencia(unsigned short int=1, unsigned short int=0);
	void establecer_potencia(unsigned short int v) {potencia=v;}
	void establecer_velocidad(float);
	void establecer_color(unsigned short int);
	void establecer_faccion(unsigned short int p_faccion) {this->faccion=p_faccion;}

	enum colores{
	C_ROJO,
	C_AZUL,
	C_VERDE,
	C_VIOLETA,
		C_MAX_COLOR};

	enum dimensiones{
	D_PEQUENO=8,
	D_NORMAL=16,
	D_GRANDE=32,
		D_MAX_DIMENSIONES};

	virtual ~Proyectil();

	unsigned short int acc_potencia() const {return this->potencia;}
	virtual bool puede_rebotar() const {return false;}
	bool es_disparado_por(Actor * p_actor) const {return this->origen==p_actor;}
	bool es_de_faccion(unsigned int p_facc) const 
	{
		return p_facc & this->faccion;
	}
	void cuentame() {std::cout<<"SOY PROYECTIL"<<std::endl;}

	virtual unsigned short int obtener_tipo_colision() const {return Actor::T_TIPO_COLISION_PROYECTIL;}

	static Proyectil * generar(const Actor&, DLibH::Vector_2d, unsigned short int=D_NORMAL, int=0, int=0);
	static Proyectil * generar(const Actor&, const Actor&, unsigned short int=D_NORMAL, int=0, int=0, int=0);
	static Proyectil * generar(int, int, const Actor&, DLibH::Vector_2d, unsigned short int=D_NORMAL);
};

#endif
