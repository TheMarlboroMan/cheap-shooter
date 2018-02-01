#ifndef PERFIL_DATOS
#define PERFIL_DATOS

#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <cstdint>
#include "../estructura_datos_perfil/estructura_datos_perfil.h"
#include "../definiciones/definicion_retos.h"

/*
Esto podría ser parte del perfil pero lo vamos a separar para más adelante poder
reescribir y reusar si hace falta... El Perfil contiene un objeto Perfil_datos
al que le pasa también el archivo para escribir y leer. Perfil_datos contiene
datos relativos al juego por el jugador, por ejemplo, veces que ha iniciado
sesión, puntuación total acumulada...

Importante: en esta clase no tendremos punteros a nada!. Se va a copiar de un 
lado a otro libremente, de modo que todo lo que tendremos son variables y tal.
*/

class Perfil_datos
{
	private:

	//Se guardan.
	unsigned int version;

	typedef uint8_t U_ENTERO_8;
	typedef uint16_t U_ENTERO_16;
	typedef uint32_t U_ENTERO_32;

	/*En esta estructura se almacena información de los distintos niveles
	para este perfil. Se meterán tantas como niveles haya en un vector
	y se irán rellenando cuando sea necesario. Tienen sus propios métodos
	para guardar y cargar de disco.*/

	public: 

	struct Info_nivel_excepcion
	{
		enum codigos{C_FUERA_DE_RANGO};
		unsigned int codigo;
		Info_nivel_excepcion(unsigned int c) {codigo=c;}
	};

	struct Info_nivel
	{
		U_ENTERO_8 nivel;			//El índice del nivel.
		U_ENTERO_32 maxima_puntuacion;		
		U_ENTERO_16 veces_iniciado;		//Veces que se ha iniciado el nivel, contando veces que lo inicias desde la selección y veces que lo alcanzas, no veces que te liquidan y reinicias.
		U_ENTERO_16 vidas_perdidas;		//Vidas que se han perdido en el nivel.
		U_ENTERO_8 dia_finalizacion;		//La fecha la guardamos en tres partes.
		U_ENTERO_8 mes_finalizacion;
		U_ENTERO_16 anho_finalizacion;
		U_ENTERO_8 porcentaje_recogible;
		U_ENTERO_8 porcentaje_destruible;

		Info_nivel(unsigned int n=0):
			nivel(n), maxima_puntuacion(0), veces_iniciado(0), vidas_perdidas(0), 
			dia_finalizacion(0), mes_finalizacion(0), 
			anho_finalizacion(0), porcentaje_recogible(0), 
			porcentaje_destruible(0)
		{}

		void procesar_datos_fin_nivel(Estructura_datos_perfil_nivel& p_datos, bool p_finalizado)
		{
			++veces_iniciado;
			maxima_puntuacion=(U_ENTERO_32)p_datos.puntuacion > maxima_puntuacion ? (U_ENTERO_32)p_datos.puntuacion : maxima_puntuacion;
			vidas_perdidas+=(U_ENTERO_16)p_datos.vidas_perdidas;

			int val_porcentaje_recogible=p_datos.recogibles_total 
				? (p_datos.recogibles_cuenta * 100) / p_datos.recogibles_total
				: 0;

			int val_porcentaje_destruible=p_datos.destruibles_total
				? (p_datos.destruibles_cuenta * 100) / p_datos.destruibles_total
				: 0;

			porcentaje_recogible=(U_ENTERO_8)val_porcentaje_recogible > porcentaje_recogible 
				? (U_ENTERO_8)val_porcentaje_recogible
				: porcentaje_recogible;

			porcentaje_destruible=(U_ENTERO_8)val_porcentaje_destruible > porcentaje_destruible 
				? (U_ENTERO_8)val_porcentaje_destruible
				: porcentaje_destruible;

			if(p_finalizado && !es_finalizado())	//Si no está finalizado se sella la fecha a hoy.
			{
				char * t=new char[5];
				memset(t, '\0', 5);
				time_t tiempo=time(NULL);

				strftime(t, 5, "%d", localtime(&tiempo));
				dia_finalizacion=(U_ENTERO_8)DLibH::Herramientas::cadena_a_entero(t);

				strftime(t, 5, "%m", localtime(&tiempo));
				mes_finalizacion=(U_ENTERO_8)DLibH::Herramientas::cadena_a_entero(t);

				strftime(t, 5, "%Y", localtime(&tiempo));
				anho_finalizacion=(U_ENTERO_16)DLibH::Herramientas::cadena_a_entero(t);
			}
		}

		void cargar(std::ifstream& p_archivo)
		{
			p_archivo.read( (char *) &nivel, sizeof(U_ENTERO_8));
			p_archivo.read( (char *) &maxima_puntuacion, sizeof(U_ENTERO_32));
			p_archivo.read( (char *) &veces_iniciado, sizeof(U_ENTERO_16));
			p_archivo.read( (char *) &vidas_perdidas, sizeof(U_ENTERO_16));
			p_archivo.read( (char *) &dia_finalizacion, sizeof(U_ENTERO_8));
			p_archivo.read( (char *) &mes_finalizacion, sizeof(U_ENTERO_8));
			p_archivo.read( (char *) &anho_finalizacion, sizeof(U_ENTERO_16));
			p_archivo.read( (char *) &porcentaje_recogible, sizeof(U_ENTERO_8));
			p_archivo.read( (char *) &porcentaje_destruible, sizeof(U_ENTERO_8));
		}

		void guardar(std::fstream& p_archivo)
		{
			p_archivo.write( (char *) &nivel, sizeof(U_ENTERO_8));
			p_archivo.write( (char *) &maxima_puntuacion, sizeof(U_ENTERO_32));
			p_archivo.write( (char *) &veces_iniciado, sizeof(U_ENTERO_16));
			p_archivo.write( (char *) &vidas_perdidas, sizeof(U_ENTERO_16));
			p_archivo.write( (char *) &dia_finalizacion, sizeof(U_ENTERO_8));
			p_archivo.write( (char *) &mes_finalizacion, sizeof(U_ENTERO_8));
			p_archivo.write( (char *) &anho_finalizacion, sizeof(U_ENTERO_16));
			p_archivo.write( (char *) &porcentaje_recogible, sizeof(U_ENTERO_8));
			p_archivo.write( (char *) &porcentaje_destruible, sizeof(U_ENTERO_8));
		}

		//Para saber si es finalizado podemos comprobar si el día es distinto de cero, que lo es por defecto.
		bool es_finalizado() const {return dia_finalizacion!=0;}
	};

	private:
	
	U_ENTERO_16 sesiones_iniciadas;
	U_ENTERO_32 puntuacion_acumulada;
	U_ENTERO_8 max_tipo_combo;
	U_ENTERO_32 disparos_efectuados;
	U_ENTERO_16 partidas_iniciadas;
	U_ENTERO_16 partidas_abortadas;
	U_ENTERO_16 partidas_ganadas;
	U_ENTERO_16 partidas_perdidas;
	U_ENTERO_32 total_gemas_recogidas;
	U_ENTERO_16 total_segundos_tiempo_detenido;	//En enteros. Ya veremos como lo contamos.
	U_ENTERO_32 total_obstaculos_destruidos;
	U_ENTERO_16 total_vidas_perdidas;
	U_ENTERO_16 total_pixeles_teletransportados;
	U_ENTERO_16 total_potencia_edu_usada;
	U_ENTERO_16 total_minas_destruidas;
	U_ENTERO_8 mostrar_tutoriales;

	std::vector<bool> retos_obtenidos;
	std::vector<Info_nivel> info_niveles;

	//No se guardan...
	unsigned short int suma_retos_obtenidos;
	unsigned short int total_niveles_secretos_finalizados;	
	unsigned short int cuenta_niveles_finalizados;

	public:	

	Perfil_datos(unsigned int);
	~Perfil_datos();
	Perfil_datos(const Perfil_datos&);
	Perfil_datos& operator=(const Perfil_datos &);

	bool cargar(std::ifstream&);
	bool guardar(std::fstream&);
	
	void iniciar_partida() {this->partidas_iniciadas++;}
	void perder_partida() {this->partidas_perdidas++;}
	void ganar_partida() {this->partidas_ganadas++;}
	void abortar_partida() {this->partidas_abortadas++;}
	void agregar_inicio_sesion() {this->sesiones_iniciadas++;}
	void actualizar_por_estructura(Estructura_datos_perfil &);
	void conseguir_reto(unsigned int);
	bool es_reto_conseguido(unsigned int);
	bool es_finalizado_todos_niveles_secretos() const;
	bool es_finalizado_nivel_secreto(unsigned int) const;
	void desactivar_tutoriales() {mostrar_tutoriales=0;}
	void activar_tutoriales() {mostrar_tutoriales=1;}
	unsigned short int acc_cuenta_niveles_finalizados() {return cuenta_niveles_finalizados;}

	U_ENTERO_16 acc_sesiones_iniciadas() const {return this->sesiones_iniciadas;}
	U_ENTERO_32 acc_puntuacion_acumulada() const {return this->puntuacion_acumulada;}
	U_ENTERO_8 acc_max_tipo_combo() const {return this->max_tipo_combo;}
	U_ENTERO_32 acc_disparos_efectuados() const {return this->disparos_efectuados;}
	U_ENTERO_16 acc_partidas_iniciadas() const {return this->partidas_iniciadas;}
	U_ENTERO_16 acc_partidas_abortadas() const {return this->partidas_abortadas;}
	U_ENTERO_16 acc_partidas_ganadas() const {return this->partidas_ganadas;}
	U_ENTERO_16 acc_partidas_perdidas() const {return this->partidas_perdidas;}
	U_ENTERO_32 acc_total_gemas_recogidas() const {return this->total_gemas_recogidas;}
	U_ENTERO_16 acc_total_segundos_tiempo_detenido() const {return this->total_segundos_tiempo_detenido;}
	U_ENTERO_32 acc_total_obstaculos_destruidos() const {return this->total_obstaculos_destruidos;}
	U_ENTERO_16 acc_total_vidas_perdidas() const {return this->total_vidas_perdidas;}
	U_ENTERO_16 acc_total_pixeles_teletransportados() const {return this->total_pixeles_teletransportados;}
	U_ENTERO_16 acc_total_potencia_edu_usada() const {return this->total_potencia_edu_usada;}
	U_ENTERO_16 acc_total_minas_destruidas() const {return this->total_minas_destruidas;}
	bool es_mostrar_tutoriales() const {return (bool)mostrar_tutoriales;}

	Estructura_intercambio_retos obtener_estructura_retos_rellena();
	void rellenar_datos_retos_desde_servidor(const Estructura_intercambio_retos&);

	Info_nivel& obtener_datos_nivel(unsigned int p_valor) throw ();
	void refrescar_cuenta_niveles_finalizados();
};

#endif
