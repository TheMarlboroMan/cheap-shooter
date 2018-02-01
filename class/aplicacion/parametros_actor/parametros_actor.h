#ifndef PARAMETROS_ACTOR_H
#define PARAMETROS_ACTOR_H

typedef uint16_t U_ENTERO_16;
typedef uint8_t U_ENTERO_8;

struct Parametros_actor_redimensionable
{
	U_ENTERO_16 w;
	U_ENTERO_16 h;

	Parametros_actor_redimensionable(U_ENTERO_16 pw, U_ENTERO_16 ph)
		:w(pw), h(ph)
	{} 

	Parametros_actor_redimensionable():
		w(0), h(0)
	{}

	Parametros_actor_redimensionable(const Parametros_actor_redimensionable& o)
		:w(o.w), h(o.h)
	{}

	Parametros_actor_redimensionable& operator=(const Parametros_actor_redimensionable& o)
	{
		w=o.w;
		h=o.h;

		return *this;
	}

	void serializar(std::ofstream& archivo)
	{
		archivo.write((char *) &this->w, sizeof(U_ENTERO_16));
		archivo.write((char *) &this->h, sizeof(U_ENTERO_16));

	}

	void deserializar(std::ifstream& archivo)
	{
		archivo.read((char *) &this->w, sizeof(U_ENTERO_16));
		archivo.read((char *) &this->h, sizeof(U_ENTERO_16));
	}
};

struct Parametros_actor_ordenable_profundidad
{
	U_ENTERO_8 p;

	Parametros_actor_ordenable_profundidad(U_ENTERO_8 pp)
		:p(pp)
	{} 

	Parametros_actor_ordenable_profundidad():p(0)
	{}

	Parametros_actor_ordenable_profundidad(const Parametros_actor_ordenable_profundidad& o):p(o.p)
	{}

	Parametros_actor_ordenable_profundidad& operator=(const Parametros_actor_ordenable_profundidad& o)
	{
		p=o.p;
		return *this;
	}

	void serializar(std::ofstream& archivo)
	{
		archivo.write((char *) &this->p, sizeof(U_ENTERO_8));

	}

	void deserializar(std::ifstream& archivo)
	{
		archivo.read((char *) &this->p, sizeof(U_ENTERO_8));
	}
};

struct Parametros_actor_flexible
{
	U_ENTERO_16 rec_x;
	U_ENTERO_16 rec_y;
	U_ENTERO_16 rec_w;
	U_ENTERO_16 rec_h;
	U_ENTERO_16 alx;	//Alineamiento.
	U_ENTERO_16 aly;
	U_ENTERO_8 indice_recurso;

	Parametros_actor_flexible(U_ENTERO_16 p1, U_ENTERO_16 p2, 
				U_ENTERO_16 p3, U_ENTERO_16 p4, 
				U_ENTERO_16 p5, U_ENTERO_16 p6,
				U_ENTERO_8 p7):
		rec_x(p1), rec_y(p2), rec_w(p3), rec_h(p4), alx(p5), aly(p6), indice_recurso(p7)
	{}

	Parametros_actor_flexible():
		rec_x(0), rec_y(0), rec_w(0), rec_h(0), alx(0), aly(0), indice_recurso(0)
	{}

	Parametros_actor_flexible(const Parametros_actor_flexible& o)
		:rec_x(o.rec_x), rec_y(o.rec_y), rec_w(o.rec_w), rec_h(o.rec_h),
		alx(o.alx), aly(o.aly), indice_recurso(o.indice_recurso)
	{}

	Parametros_actor_flexible& operator=(const Parametros_actor_flexible& o)
	{
		rec_x=o.rec_x;
		rec_y=o.rec_y;
		rec_w=o.rec_w;
		rec_h=o.rec_h;
		alx=o.alx;
		aly=o.aly;
		indice_recurso=o.indice_recurso;

		return *this;
	}

	void serializar(std::ofstream& archivo)
	{
		archivo.write((char *) &this->rec_x, sizeof(U_ENTERO_16));
		archivo.write((char *) &this->rec_y, sizeof(U_ENTERO_16));
		archivo.write((char *) &this->rec_w, sizeof(U_ENTERO_16));
		archivo.write((char *) &this->rec_h, sizeof(U_ENTERO_16));
		archivo.write((char *) &this->alx, sizeof(U_ENTERO_16));
		archivo.write((char *) &this->aly, sizeof(U_ENTERO_16));
		archivo.write((char *) &this->indice_recurso, sizeof(U_ENTERO_8));
	} 

	void deserializar(std::ifstream& archivo)
	{
		archivo.read((char *) &this->rec_x, sizeof(U_ENTERO_16));
		archivo.read((char *) &this->rec_y, sizeof(U_ENTERO_16));
		archivo.read((char *) &this->rec_w, sizeof(U_ENTERO_16));
		archivo.read((char *) &this->rec_h, sizeof(U_ENTERO_16));
		archivo.read((char *) &this->alx, sizeof(U_ENTERO_16));
		archivo.read((char *) &this->aly, sizeof(U_ENTERO_16));
		archivo.read((char *) &this->indice_recurso, sizeof(U_ENTERO_8));
	}
};

struct Parametros_actor
{
	U_ENTERO_16 param1;
	U_ENTERO_16 param2;
	U_ENTERO_16 param3;
	U_ENTERO_16 param4;
	U_ENTERO_16 param5;
	U_ENTERO_16 param6;
	U_ENTERO_16 param7;
	U_ENTERO_16 param8;

	Parametros_actor(U_ENTERO_16 p1, U_ENTERO_16 p2, U_ENTERO_16 p3, U_ENTERO_16 p4, U_ENTERO_16 p5, U_ENTERO_16 p6, U_ENTERO_16 p7, U_ENTERO_16 p8):
		param1(p1), param2(p2), param3(p3), param4(p4), param5(p5),
		param6(p6), param7(p7), param8(p8)
	{
		
	}

	Parametros_actor():
		param1(0), param2(0), param3(0), param4(0), param5(0),
		param6(0), param7(0), param8(0)
	{
		
	}

	Parametros_actor(const Parametros_actor& o)
		:param1(o.param1), param2(o.param2), param3(o.param3),
		param4(o.param4), param5(o.param5), param6(o.param6), 
		param7(o.param7), param8(o.param8) 
	{}

	Parametros_actor& operator=(const Parametros_actor& o)
	{
		param1=o.param1;
		param2=o.param2;
		param3=o.param3;
		param4=o.param4;
		param5=o.param5;
		param6=o.param6;
		param7=o.param7;
		param8=o.param8;

		return *this;
	}

	void serializar(std::ofstream& archivo)
	{
		archivo.write((char *) &this->param1, sizeof(U_ENTERO_16));
		archivo.write((char *) &this->param2, sizeof(U_ENTERO_16));
		archivo.write((char *) &this->param3, sizeof(U_ENTERO_16));
		archivo.write((char *) &this->param4, sizeof(U_ENTERO_16));
		archivo.write((char *) &this->param5, sizeof(U_ENTERO_16));
		archivo.write((char *) &this->param6, sizeof(U_ENTERO_16));
		archivo.write((char *) &this->param7, sizeof(U_ENTERO_16));
		archivo.write((char *) &this->param8, sizeof(U_ENTERO_16));
	} 

	void deserializar(std::ifstream& archivo)
	{
		archivo.read((char *) &this->param1, sizeof(U_ENTERO_16));
		archivo.read((char *) &this->param2, sizeof(U_ENTERO_16));
		archivo.read((char *) &this->param3, sizeof(U_ENTERO_16));
		archivo.read((char *) &this->param4, sizeof(U_ENTERO_16));
		archivo.read((char *) &this->param5, sizeof(U_ENTERO_16));
		archivo.read((char *) &this->param6, sizeof(U_ENTERO_16));
		archivo.read((char *) &this->param7, sizeof(U_ENTERO_16));
		archivo.read((char *) &this->param8, sizeof(U_ENTERO_16));
	}
};

#endif
