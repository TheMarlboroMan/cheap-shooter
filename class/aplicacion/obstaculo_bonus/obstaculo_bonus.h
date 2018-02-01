#ifndef OBSTACULO_BONUS_H
#define OBSTACULO_BONUS_H
#include "../actor_mapa/actor_mapa.h"


class Obstaculo_bonus:public Actor_mapa
{
	private:
	
	unsigned int tipo_bonus;

	protected:

	void establecer_representacion();
	void establecer_caracteristicas();

	void procesar_tiempo_herido_herido() {this->establecer_recorte(32,0,32,32);}
	void procesar_tiempo_herido_ok() {this->establecer_recorte(0,0,32,32);}


	public:

	//Enumeración para los tipos de bonus que puede dar.
	enum TIPO_BONUS_OTORGADO {
		B_ALEATORIO,
		B_DISPARO,
		B_TIEMPO,
		B_VIDA,
		B_PIMIENTO,
		B_NORMAL,
		B_CARGA_ESCUDO,
		B_COMPONENTE_ESCUDO};

	Obstaculo_bonus(unsigned int);
	virtual ~Obstaculo_bonus();

	short int obtener_tirada() const;
	float vel_max() {return 150;}
	float vel_min() {return 50;}
	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_BONUS;}
	short int puntuacion_por_destruir() {return 20;}
	unsigned int tipo_a_colision_proyectil() const {return T_DESTRUYE_PROYECTIL_RESTA_RESISTENCIA_ACTOR_DA_ITEM;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_NO_PREPARADO;}
	void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);
	virtual bool es_parametrizable() const {return true;}
	bool procesar_colision_proyectil(Proyectil *);
	void cuentame() {std::cout<<"SOY OBSTACULO BONUS"<<std::endl;}
	unsigned int acc_columnas_humo_explosion() const {return 3;}
};

#endif
