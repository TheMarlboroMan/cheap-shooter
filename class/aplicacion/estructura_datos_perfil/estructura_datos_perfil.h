#ifndef ESTRUCTURA_DATOS_PERFIL_H
#define ESTRUCTURA_DATOS_PERFIL_H

/*Esta estructura se usaría entre cada intervalo de guardar los datos
de perfil para ir almacenando los progresos que el jugador ha hecho.
En lugar de manipular directamente los datos del perfil, desde el juego
manipulamos esta estructura y luego la usamos para contrastarla con
los datos del perfil. A cada actualización se reiniciarían los datos
de turno, para poder volver a sumarlos con comodidad.

La vamos a usar también para controlar que los retos se consiguen.

Más abajo hay una segunda.
*/

struct Estructura_datos_perfil
{
	public:

	unsigned int puntuacion;
	unsigned int gemas_recogidas;
	unsigned int disparos_efectuados;
	unsigned int obstaculos_destruidos;
	unsigned int minas_destruidas;
	unsigned int vidas_perdidas;
	unsigned int pixeles_teletransportados;
	unsigned short int max_tipo_combo;
	unsigned int potencia_edu_usada;
	float segundos_tiempo_detenido;
	unsigned short int max_canon;

	unsigned int nivel_gemas_perdidas;
	unsigned int nivel_disparos_efectuados;

	bool encontrado_asteroide_secreto;
	unsigned int encontrados_niveles_secretos;


	Estructura_datos_perfil()
		:puntuacion(0), 
		gemas_recogidas(0), 
		disparos_efectuados(0),
		obstaculos_destruidos(0),
		minas_destruidas(0),  
		vidas_perdidas(0), 
		pixeles_teletransportados(0), 
		max_tipo_combo(0),
		potencia_edu_usada(0),
		segundos_tiempo_detenido(0),
		max_canon(0),
		nivel_gemas_perdidas(0),
		nivel_disparos_efectuados(0),
		encontrado_asteroide_secreto(false),
		encontrados_niveles_secretos(0)
	{}

	void sumar_puntuacion(unsigned int p_valor) {this->puntuacion+=p_valor;}
	void sumar_gema() {++this->gemas_recogidas;}
	void sumar_obstaculo_destruido() {++this->obstaculos_destruidos;}
	void sumar_mina_destruida() {++this->minas_destruidas;}
	void sumar_disparo_efectuado() 
	{
		++this->disparos_efectuados;
		++this->nivel_disparos_efectuados;
	}
	void perder_vida() {++this->vidas_perdidas;}
	void perder_gema() {++this->nivel_gemas_perdidas;}
	void sumar_tiempo_detenido(float p_valor) {this->segundos_tiempo_detenido+=p_valor;}
	void sumar_pixeles_teletransportados(unsigned int p_valor) {this->pixeles_teletransportados+=p_valor;}
	void sumar_potencia_edu(unsigned int p_valor) {this->potencia_edu_usada+=p_valor;}
	void actualizar_combo(unsigned short int p_valor)
	{
		if(p_valor > this->max_tipo_combo) this->max_tipo_combo=p_valor;
	}

	void actualizar_canon(unsigned short int p_valor)
	{
		if(p_valor > this->max_canon) this->max_canon=p_valor;
	}

	//Información que se reinicia cada vez que se guarda en disco. Básicamente es la información acumulable.
	void reiniciar() 
	{
		this->puntuacion=0;
		this->gemas_recogidas=0;
		this->disparos_efectuados=0;
		this->obstaculos_destruidos=0;
		this->vidas_perdidas=0;	
		this->pixeles_teletransportados=0;
		this->potencia_edu_usada=0;
		if(this->segundos_tiempo_detenido >= 1)	this->segundos_tiempo_detenido=0;
	}

	//Información que se reinicia al iniciar un nuevo nivel.
	void reiniciar_nivel()
	{
		this->nivel_gemas_perdidas=0;
		this->nivel_disparos_efectuados=0;
	}

	//Información que se reinicia al iniciar una nueva partida.
/*
	void reiniciar_partida()
	{
		this->encontrado_asteroide_secreto=false;
		this->encontrados_niveles_secretos=0;
	}
*/
		
};

struct Estructura_datos_perfil_nivel
{
	unsigned int recogibles_total;
	unsigned int recogibles_cuenta;
	unsigned int destruibles_total;
	unsigned int destruibles_cuenta;
	unsigned int vidas_perdidas;
	unsigned int puntuacion;

	Estructura_datos_perfil_nivel(unsigned int rt, unsigned int rc,
		unsigned int dt, unsigned int dc, unsigned int vp, unsigned int p)
	{
		recogibles_total=rt;
		recogibles_cuenta=rc;
		destruibles_total=dt;
		destruibles_cuenta=dc;
		vidas_perdidas=vp;
		puntuacion=p;
	}
};

#endif
