#ifndef ESTRUCTURA_INFO_NIVEL_H
#define ESTRUCTURA_INFO_NIVEL_H

/*Esta clase tiene como finalidad controlar los aspectos relativos
a los niveles para no tener aún más variables sueltas en
el controlador. Se crea uno con el controlador y se rellena según
sea conveniente.

Todo es un poco retorcido. Por ejemplo: la misma estructura se usa para 
controlar los niveles aleatorios (se especifican porque no se crean actores en 
el mapa) que los niveles preparados... Esto no es casual, sino consecuencia
de no haber previsto el tema de los niveles prehechos antes.

Siempre se leerán los datos de un archivo .dat. En función de un tipo de nivel
se seguirá usando esta clase para hacer streaming o se usarán las propiedades
que ya se hayan leido para determinar lo que sale y no.

Ojo: las tablas de probabilidad siguen en uso para los niveles aleatorios. Los
niveles prehechos simplemente tendrán todos los valores a cero (simplemente no 
se usan las tablas en ese caso).

*/

#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdint>
#include "../actor_serializado/actor_serializado.h"
#include "../accion_trigger/accion_trigger.h"
#include "../definiciones/definiciones.h"
#include "../tabla_probabilidad/tabla_probabilidad.h"
#include "../accion_trigger_compuesta/accion_trigger_compuesta.h"

/*Esta estructura define los datos de un nivel que se guardan en disco
y son independientes. Básicamente son aspectos como velocidad, flags
o siguiente nivel a cargar...*/

struct Info_base_nivel
{
	U_ENTERO_16 vector_y_nivel;
	U_ENTERO_16 max_obstaculos;
	U_ENTERO_16 max_bonus;
	U_ENTERO_16 gemas_en_nivel;
	U_ENTERO_16 gemas_max_nivel;
	U_ENTERO_16 gemas_max_nivel_original;
	U_ENTERO_8 flags_artefactos_nivel;
	U_ENTERO_8 indice_maxima_puntuacion;
	U_ENTERO_8 indice_proximo_nivel;
	U_ENTERO_8 indice_proximo_nivel_secreto;
	U_ENTERO_8 indice_musica;

	Info_base_nivel():vector_y_nivel(0), 
		max_obstaculos(0), max_bonus(0), gemas_en_nivel(0),
		gemas_max_nivel(0), gemas_max_nivel_original(0), 
		flags_artefactos_nivel(0), indice_proximo_nivel(0),
		indice_proximo_nivel_secreto(0), indice_musica(0)
	{}

	void serializar(std::ofstream&);
	void deserializar(std::ifstream&);
	void reiniciar_gemas() {gemas_max_nivel=gemas_max_nivel_original;}
};

/*Esta estructura define valores para el nivel una vez que está en
marcha...*/

struct Info_marcha_nivel
{
	float distancia;	//La distancia recorrida.	
	float vector_y;			//Ojo... Es float para que al acelerarse pueda ir paso a paso, pero se lee como entero sin signo.
	U_ENTERO_16 nuevo_vector_y; 	//El de destino, si cambia la velocidad...
	U_ENTERO_16 velocidad_cambio_vector_y;
	float divisor_movimiento;	//Divisor de movimiento para usar el "parar tiempo".
	float acumulador_camara;	//Un acumulador para la cámara.
	int avance_este_turno;	//Guarda el movimiento para la cámara en el turno corriente.
	bool destruido_asteroide_secreto;	//Guarda si se ha destruido un asteroide secreto.
	bool comprobar_finalizacion_nivel_preparado;
	bool cambiando_velocidad;
	U_ENTERO_16 cantidad_actores_cargados;
	U_ENTERO_8 flags_artefactos;

	Info_marcha_nivel(): distancia(0.0f), vector_y(0.0f),
	nuevo_vector_y(0), velocidad_cambio_vector_y(0), divisor_movimiento(1.0f),
	acumulador_camara(0.0f), avance_este_turno(0), destruido_asteroide_secreto(false),
	comprobar_finalizacion_nivel_preparado(false), cambiando_velocidad(false),
	cantidad_actores_cargados(0), 
	flags_artefactos(Definiciones::P_CANON | Definiciones::P_TELETRANSPORTADOR | Definiciones::P_EMISOR_DENSIDAD_UNIFICADA)
	{}

	void reiniciar();
	void copiar_de_datos_nivel(const Info_base_nivel&);
	void debug_forzar_distancia(float p) {distancia=p;}
};

/*Esta otra estructura es información relacionada con el archivo en el 
que se guarda el nivel...*/

struct Info_fichero
{
	std::string nombre_fichero;
	U_ENTERO_16 cantidad_actores;
	U_ENTERO_16 cantidad_acciones;
	bool archivo_cargado;
	bool finalizada_lectura;
	int posicion_inicio_datos;

	Info_fichero():
	nombre_fichero(""), cantidad_actores(0),
	cantidad_acciones(0), archivo_cargado(false), finalizada_lectura(false),
	posicion_inicio_datos(0)
	{}

	void reiniciar();
};

/*En esta estructura se guarda información referente a la puntuación obtenida
en el nivel, las cosas destruidas y recogidas y demás. Estas cuentas se ven
sólo afectadas por aquellos elementos que estaban en un nivel preparado.
Se guarda además la puntuación acumulada en esa partida para ese nivel.
*/

struct Info_perfil_nivel
{
	unsigned int total_destruible;	//Total de objetos destruibles y recogibles.
	unsigned int total_recogible;
	unsigned int cuenta_destruible;	//Cantidad de objetos destruibles y recogidos en esta "vida" y nivel.
	unsigned int cuenta_recogible;
	unsigned int puntuacion_acumulada;	//Puntuación acumulada en este nivel y esta vida.
	unsigned int vidas_perdidas;		//Vidas perdidas en este nivel.
	
	Info_perfil_nivel():
		total_destruible(0), total_recogible(0), cuenta_destruible(0),
		cuenta_recogible(0), puntuacion_acumulada(0), vidas_perdidas(0)
{

}

	void reiniciar_nivel()
	{
		cuenta_destruible=0;
		cuenta_recogible=0;
		puntuacion_acumulada=0;
	}

	void cargar_informacion(int td, int tr)
	{
		total_destruible=td;
		total_recogible=tr;
	}
	
	void sumar_puntuacion(int v) {puntuacion_acumulada+=v;}
	void sumar_recogible() {++cuenta_recogible;}
	void sumar_destruible() {++cuenta_destruible;}
	void perder_vida() {++vidas_perdidas;}
};

/*Esta es la clase en la que todos se unen.*/

class Estructura_info_nivel
{
	private:

	typedef uint16_t U_ENTERO_16;
	typedef uint8_t U_ENTERO_8;

	unsigned int modo_juego;
	std::ifstream fichero;
	
	Info_base_nivel INFO_NIVEL; 
	Info_marcha_nivel INFO_MARCHA;
	Info_fichero INFO_FICHERO;
	Info_perfil_nivel INFO_PERFIL;

	std::vector<Accion_trigger> acciones_trigger;

	//Estos son objetos propios.
	Tabla_probabilidad TABLA_OBSTACULOS;
	Tabla_probabilidad TABLA_BONUS;
	Tabla_probabilidad TABLA_DESTRUIR_CAJA;
	Tabla_probabilidad TABLA_DESTRUIR_OBSTACULO;

	//Métodos.

	void turno_modo_aleatorio(float);
	void turno_modo_preparado(float);

	public:

	enum modos{M_ALEATORIO, M_PREPARADO};
	enum tablas{T_OBSTACULOS, T_BONUS, T_DESTRUIR_CAJA, T_DESTRUIR_OBSTACULO};

	Estructura_info_nivel();
	~Estructura_info_nivel();

	int generar_tirada_tabla(unsigned int);
	void reiniciar();
	void rebobinar();
	bool abrir_fichero(std::string const &);
	void turno(float);
	bool debe_cargar();
	Actor_serializado obtener();

	float acc_acumulador_camara() const {return INFO_MARCHA.acumulador_camara;}
	float acc_distancia() const {return INFO_MARCHA.distancia;}
	U_ENTERO_16 acc_vector_y() const {return INFO_MARCHA.vector_y;}
	int acc_avance_este_turno() const {return INFO_MARCHA.avance_este_turno;}
	unsigned int acc_modo_juego() const {return this->modo_juego;}
	void muU_ENTERO_16(float p_valor) {INFO_MARCHA.vector_y=p_valor;}
	void establecer_divisor_movimiento(float p_valor) {INFO_MARCHA.divisor_movimiento=p_valor;}
	bool es_archivo_cargado() const {return INFO_FICHERO.archivo_cargado;}
	bool es_comprobar_finalizacion_nivel_preparado() const {return INFO_MARCHA.comprobar_finalizacion_nivel_preparado;}

	U_ENTERO_16 acc_max_obstaculos() const {return INFO_NIVEL.max_obstaculos;}
	U_ENTERO_16 acc_max_bonus() const {return INFO_NIVEL.max_bonus;}
	U_ENTERO_16 acc_gemas_en_nivel() const {return INFO_NIVEL.gemas_en_nivel;}
	U_ENTERO_16 acc_gemas_max_nivel() const {return INFO_NIVEL.gemas_max_nivel;}
//	U_ENTERO_8 acc_flags_artefactos() const {return INFO_MARCHA.flags_artefactos;}
	unsigned int acc_proximo_nivel() const {return INFO_NIVEL.indice_proximo_nivel;}
	unsigned int acc_proximo_nivel_secreto() const {return INFO_NIVEL.indice_proximo_nivel_secreto;}
	unsigned int acc_musica() const {return INFO_NIVEL.indice_musica;}

	//Mucho ojo... Estos dos son enteros CON signo y pueden dárse la vuelta fácilmente.
	void incrementar_gemas_max_nivel() 
	{
		++INFO_NIVEL.gemas_max_nivel;
	}

	void decrementar_gemas_max_nivel() 
	{
		if(INFO_NIVEL.gemas_max_nivel!=0)
		{
			--INFO_NIVEL.gemas_max_nivel;
		}
	}

	bool es_estado_gemas_peligro() {return INFO_NIVEL.gemas_max_nivel <= INFO_NIVEL.gemas_en_nivel;}
	bool es_destruido_asteroide_secreto() const {return INFO_MARCHA.destruido_asteroide_secreto;}
	void marcar_asteroide_secreto_como_destruido() {INFO_MARCHA.destruido_asteroide_secreto=true;}

	void cargar_valores_para_nivel(unsigned short int);
	void preparar_para_final_juego();

	void modificar_artefactos(unsigned short int);
	void modificar_vector_y(U_ENTERO_16 p_param, unsigned int p_velocidad) 
	{
		if(!p_velocidad) 
		{
			INFO_MARCHA.vector_y=p_param;
		}
		else 
		{
			if(p_param != INFO_MARCHA.vector_y)
			{
				INFO_MARCHA.nuevo_vector_y=p_param;
				INFO_MARCHA.velocidad_cambio_vector_y=p_velocidad;
				INFO_MARCHA.cambiando_velocidad=true;
			}
		}
	}

	void habilitar_comprobacion_finalizacion_nivel_preparado() {INFO_MARCHA.comprobar_finalizacion_nivel_preparado=true;}
	bool es_artefacto_permitido(unsigned short int);
	unsigned short int ciclar_artefacto(int, unsigned short int);
	std::vector<Accion_trigger_compuesta> obtener_composicion_accion_trigger_por_tag(unsigned int);
	void debug_forzar_distancia(float p) {INFO_MARCHA.distancia=p;}

	//Estas son para la info de perfil...
	void establecer_info_perfil(int pd, int pr) {INFO_PERFIL.cargar_informacion(pd, pr);}
	void reiniciar_info_perfil() {INFO_PERFIL.reiniciar_nivel();}
	void aumentar_destruible() {INFO_PERFIL.sumar_destruible();}
	void aumentar_recogible() {INFO_PERFIL.sumar_recogible();}
	int total_destruible() const {return INFO_PERFIL.total_destruible;}
	int total_recogible() const {return INFO_PERFIL.total_recogible;}
	int cuenta_destruible() const {return INFO_PERFIL.cuenta_destruible;}
	int cuenta_recogible() const {return INFO_PERFIL.cuenta_recogible;}
	void perder_vida() {return INFO_PERFIL.perder_vida();}
	void sumar_puntuacion(int v) {INFO_PERFIL.sumar_puntuacion(v);}
	int acc_puntuacion_acumulada() const {return INFO_PERFIL.puntuacion_acumulada;}
	int acc_vidas_perdidas() const {return INFO_PERFIL.vidas_perdidas;}
};

#endif
