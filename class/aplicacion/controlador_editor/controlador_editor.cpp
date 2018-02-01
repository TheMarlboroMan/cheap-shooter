#include "controlador_editor.h"

unsigned int Controlador_editor::REJILLA_X=32;
unsigned int Controlador_editor::REJILLA_Y=32;

#include "controlador_editor.representaciones.cpp"
#include "controlador_editor.controles.cpp"
#include "controlador_editor.io.cpp"

Controlador_editor::Controlador_editor(std::string p_fichero, DLibV::Pantalla &p, DLibI::Controles_SDL &c)
	:nombre_fichero(p_fichero), pantalla(p), controles_sdl(c), 
	escritor(c), camara(0,0, Medidas::ANCHO_VISTA , Medidas::ALTO_VISTA ,0,0),
	INFO_NIVEL(),
	posicion_y(Controlador_editor::LIMITE_CAMARA_INFERIOR), 
	tipo_actor_actual(0), grupo_tipo_actual(0), bloquear_a_rejilla(true), mostrar_rejilla(true),
	mostrar_info(true),
	filtro_profundidad_actores(Actor_mapa::P_FONDO | Actor_mapa::P_NORMAL | Actor_mapa::P_FRENTE),
	mostrar_ocultos(true), indice_actual_listado(0),
	mensaje(""), cadena_parametros(""), cadena_tag(""), cadena_acciones(""),
	estado_actual(E_EDITAR_NIVEL), tipo_modificar_actor(T_NORMAL), editando_propiedad(P_NIVEL_VECTOR_Y),
	indice_editando_tabla(0), indice_parametro(0), indice_accion(0),
	indice_parametro_accion(0), maximo_tag(0)
{
	INFO_NIVEL.flags_artefactos_nivel=Definiciones::P_CANON | Definiciones::P_TELETRANSPORTADOR | Definiciones::P_EMISOR_DENSIDAD_UNIFICADA | Definiciones::P_ESCUDO;

	this->rep_txt_info=NULL;
	this->rep_txt_editar_propiedades=NULL;
	this->rep_txt_editar_tablas=NULL;
	this->rep_txt_parametros=NULL;
	this->rep_txt_tag=NULL;
	this->rep_txt_acciones=NULL;
	this->rep_txt_listado=NULL;
	this->seleccion_actual=NULL;

	Actor_mapa::activar_en_editor();

	this->inicializar();
	this->cargar();


}

Controlador_editor::~Controlador_editor()
{
	this->desmontar();
}

void Controlador_editor::desplazar_camara(int p_factor)
{
	this->posicion_y+=p_factor*REJILLA_Y;

	if(this->posicion_y > LIMITE_CAMARA_INFERIOR) this->posicion_y=LIMITE_CAMARA_INFERIOR;
	else if(this->posicion_y <= LIMITE_CAMARA_SUPERIOR) this->posicion_y=LIMITE_CAMARA_SUPERIOR;

	this->camara.enfocar_a(0, this->posicion_y);
}

void Controlador_editor::escoger_tipo(unsigned int p_factor)
{
	int total=this->TIPOS[this->grupo_tipo_actual].TIPOS.size();
	this->tipo_actor_actual+=p_factor;

	if(this->tipo_actor_actual < 0) this->tipo_actor_actual=total-1;
	else if(this->tipo_actor_actual >= total) this->tipo_actor_actual=0;
}

void Controlador_editor::escoger_grupo(unsigned int p_factor)
{
	this->tipo_actor_actual=0;
	int total=this->TIPOS.size();
	this->grupo_tipo_actual+=p_factor;

	if(this->grupo_tipo_actual < 0) this->grupo_tipo_actual=total-1;
	else if(this->grupo_tipo_actual >= total) this->grupo_tipo_actual=0;
}

void Controlador_editor::modificar_rejilla(int p_factor)
{
	unsigned int valor=REJILLA_X;

	if(p_factor > 0) valor*=2;
	else if(p_factor < 0) valor/=2;

	if(valor < MIN_REJILLA || valor > MAX_REJILLA) return;
	else
	{
		REJILLA_X=valor;
		REJILLA_Y=valor;
	}

}

void Controlador_editor::desmontar()
{
	if(this->rep_txt_info) delete this->rep_txt_info;
	if(this->rep_txt_editar_propiedades) delete this->rep_txt_editar_propiedades;
	if(this->rep_txt_editar_tablas) delete this->rep_txt_editar_tablas;
	if(this->rep_txt_parametros) delete this->rep_txt_parametros;
	if(this->rep_txt_tag) delete this->rep_txt_tag;
	if(this->rep_txt_acciones) delete this->rep_txt_acciones;
	this->vaciar_actores_mapa();
}

void Controlador_editor::vaciar_actores_mapa()
{
	this->actores_mapa.clear();
}

bool Controlador_editor::inicializar()
{
	//Rellenar cadenas varias...

	this->cadenas_propiedades.push_back("VECTOR Y");
	this->cadenas_propiedades.push_back("FLAGS ARTEFACTOS (cañón)");
	this->cadenas_propiedades.push_back("FLAGS ARTEFACTOS (teletransporte)");
	this->cadenas_propiedades.push_back("FLAGS ARTEFACTOS (e.d.u)");
	this->cadenas_propiedades.push_back("FLAGS ARTEFACTOS (escudo)");
	this->cadenas_propiedades.push_back("MÁXIMO OBSTÁCULOS (modo aleatorio)");
	this->cadenas_propiedades.push_back("MÁXIMO BONUS (modo aleatorio)");
	this->cadenas_propiedades.push_back("TOTAL DE GEMAS PARA FINALIZAR (0 si no es aplicable)");
	this->cadenas_propiedades.push_back("MÁXIMO DE GEMAS A PERDER (0 si no es aplicable)");
	this->cadenas_propiedades.push_back("PROXIMO NIVEL");
	this->cadenas_propiedades.push_back("PROXIMO NIVEL PARA SALIDA SECRETA");
	this->cadenas_propiedades.push_back("INDICE MÚSICA");

	std::vector<Tipo_actor> VBONUS;
	VBONUS.push_back(Tipo_actor("BONUS DISPARO", 	Actor_mapa::T_BONUS_DISPARO));
	VBONUS.push_back(Tipo_actor("BONUS TIEMPO", 	Actor_mapa::T_BONUS_TIEMPO));
	VBONUS.push_back(Tipo_actor("BONUS VIDA", 		Actor_mapa::T_BONUS_VIDA));
	VBONUS.push_back(Tipo_actor("BONUS PIMIENTO", 	Actor_mapa::T_BONUS_PIMIENTO));
	VBONUS.push_back(Tipo_actor("BONUS NORMAL", 	Actor_mapa::T_BONUS_NORMAL));
	VBONUS.push_back(Tipo_actor("BONUS CARGA ESCUDO", 	Actor_mapa::T_BONUS_CARGA_ESCUDO));
	VBONUS.push_back(Tipo_actor("BONUS COMPONENTE ESCUDO", 	Actor_mapa::T_BONUS_COMPONENTE_ESCUDO));
	this->TIPOS.push_back(Grupo_tipo("BONUS", VBONUS));

	std::vector<Tipo_actor> VOBSTACULOS;
	VOBSTACULOS.push_back(Tipo_actor("OBSTACULO 26x26", 	Actor_mapa::T_OBSTACULO_26_26));
	VOBSTACULOS.push_back(Tipo_actor("OBSTACULO 32x64", 	Actor_mapa::T_OBSTACULO_32_64));
	VOBSTACULOS.push_back(Tipo_actor("OBSTACULO 64x26", 	Actor_mapa::T_OBSTACULO_64_26));
	VOBSTACULOS.push_back(Tipo_actor("OBSTACULO 128x32", 	Actor_mapa::T_OBSTACULO_128_32));
	VOBSTACULOS.push_back(Tipo_actor("OBSTACULO 800x32", 	Actor_mapa::T_OBSTACULO_800_32));
	VOBSTACULOS.push_back(Tipo_actor("OBSTACULO BONUS", 	Actor_mapa::T_OBSTACULO_BONUS));
	VOBSTACULOS.push_back(Tipo_actor("OBSTACULO DESTRUIBLE", Actor_mapa::T_OBSTACULO_DESTRUIBLE));
	VOBSTACULOS.push_back(Tipo_actor("OBSTACULO DESTRUIBLE SECRETO", Actor_mapa::T_OBSTACULO_DESTRUIBLE_SECRETO));
	this->TIPOS.push_back(Grupo_tipo("OBSTACULOS", VOBSTACULOS));

	std::vector<Tipo_actor> VACTIVIDAD;
	VACTIVIDAD.push_back(Tipo_actor("ACTIVIDAD GRAVEDAD", 	Actor_mapa::T_OBSTACULO_ACTIVIDAD_GRAVEDAD));
	VACTIVIDAD.push_back(Tipo_actor("ACTIVIDAD DISPARO", 	Actor_mapa::T_OBSTACULO_ACTIVIDAD_DISPARADOR));
	VACTIVIDAD.push_back(Tipo_actor("ACTIVIDAD EXPLOSIVO", Actor_mapa::T_OBSTACULO_ACTIVIDAD_EXPLOSIVO));
	VACTIVIDAD.push_back(Tipo_actor("ACTIVIDAD CAÑON PROGRAMABLE", Actor_mapa::T_OBSTACULO_ACTIVIDAD_CANON_PROGRAMABLE));
	VACTIVIDAD.push_back(Tipo_actor("ACTIVIDAD CAÑON GIRATORIO", Actor_mapa::T_OBSTACULO_ACTIVIDAD_CANON_GIRATORIO));
	VACTIVIDAD.push_back(Tipo_actor("ACTIVIDAD CAÑON APUNTADO", Actor_mapa::T_OBSTACULO_ACTIVIDAD_CANON_APUNTADO));
	VACTIVIDAD.push_back(Tipo_actor("ACTIVIDAD PUERTA DISPARABLE", Actor_mapa::T_OBSTACULO_ACTIVIDAD_PUERTA_DISPARAR));
	VACTIVIDAD.push_back(Tipo_actor("ACTIVIDAD PUERTA AUTOMÁTICA", Actor_mapa::T_OBSTACULO_ACTIVIDAD_PUERTA_AUTOMATICA));
	VACTIVIDAD.push_back(Tipo_actor("ACTIVIDAD PUERTA REMOTA", Actor_mapa::T_OBSTACULO_ACTIVIDAD_PUERTA_REMOTA));
	VACTIVIDAD.push_back(Tipo_actor("ACTIVIDAD GENERADOR OBSTACULOS", Actor_mapa::T_OBSTACULO_ACTIVIDAD_GENERADOR_OBSTACULOS));
	VACTIVIDAD.push_back(Tipo_actor("ACTIVIDAD TUTORIAL", Actor_mapa::T_OBSTACULO_TUTORIAL));
	VACTIVIDAD.push_back(Tipo_actor("BOSS 01", Actor_mapa::T_OBSTACULO_ACTIVIDAD_BOSS_01));
	VACTIVIDAD.push_back(Tipo_actor("BOSS 02", Actor_mapa::T_OBSTACULO_ACTIVIDAD_BOSS_02));
	this->TIPOS.push_back(Grupo_tipo("ACTIVIDAD", VACTIVIDAD));

	std::vector<Tipo_actor> VTRIGGERS;
	VTRIGGERS.push_back(Tipo_actor("PROPIEDADES NIVEL", Actor_mapa::T_OBSTACULO_PROPIEDADES));
	VTRIGGERS.push_back(Tipo_actor("PROPIEDADES FONDO", Actor_mapa::T_OBSTACULO_PROPIEDADES_FONDO));
	VTRIGGERS.push_back(Tipo_actor("PROPIEDADES VECTOR", Actor_mapa::T_OBSTACULO_VECTOR));
	VTRIGGERS.push_back(Tipo_actor("PROPIEDADES MOVIMIENTO RELATIVO", Actor_mapa::T_OBSTACULO_PROPIEDADES_MOVIMIENTO_RELATIVO));
	VTRIGGERS.push_back(Tipo_actor("GENERADOR DECORACIONES", Actor_mapa::T_OBSTACULO_ACTIVIDAD_GENERADOR_DECORACION));
	VTRIGGERS.push_back(Tipo_actor("TRIGGER CONTACTO", Actor_mapa::T_OBSTACULO_TRIGGER_CONTACTO));
	VTRIGGERS.push_back(Tipo_actor("TRIGGER INTERRUPTOR", Actor_mapa::T_OBSTACULO_TRIGGER_INTERRUPTOR));
	VTRIGGERS.push_back(Tipo_actor("TRIGGER AUTO", Actor_mapa::T_OBSTACULO_TRIGGER_AUTO));
	VTRIGGERS.push_back(Tipo_actor("PUNTO RUTA", Actor_mapa::T_PUNTO_RUTA));
	VTRIGGERS.push_back(Tipo_actor("COLOCADOR JUGADOR", Actor_mapa::T_OBSTACULO_COLOCADOR_JUGADOR));
	this->TIPOS.push_back(Grupo_tipo("TRIGGERS Y PROPIEDADES", VTRIGGERS));

	std::vector<Tipo_actor> VINDUSTRIAL;
	VINDUSTRIAL.push_back(Tipo_actor("OBSTACULO FLEXIBLE ", Actor_mapa::T_OBSTACULO_FLEX));
	VINDUSTRIAL.push_back(Tipo_actor("OBSTACULO IND. DIAGONAL 1 32x32", Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_1));
	VINDUSTRIAL.push_back(Tipo_actor("OBSTACULO IND. DIAGONAL 2 32x32", Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_2));
	VINDUSTRIAL.push_back(Tipo_actor("OBSTACULO IND. DIAGONAL 3 32x32", Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_3));
	VINDUSTRIAL.push_back(Tipo_actor("OBSTACULO IND. DIAGONAL 4 32x32", Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_4));
	VINDUSTRIAL.push_back(Tipo_actor("OBSTACULO IND. DIAGONAL 1 64x64", Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_1));
	VINDUSTRIAL.push_back(Tipo_actor("OBSTACULO IND. DIAGONAL 2 64x64", Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_2));
	VINDUSTRIAL.push_back(Tipo_actor("OBSTACULO IND. DIAGONAL 3 64x64", Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_3));
	VINDUSTRIAL.push_back(Tipo_actor("OBSTACULO IND. DIAGONAL 4 64x64", Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_4));
	VINDUSTRIAL.push_back(Tipo_actor("OBSTACULO IND. DIAGONAL 1 32x64", Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_64_1));
	VINDUSTRIAL.push_back(Tipo_actor("OBSTACULO IND. DIAGONAL 2 32x64", Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_64_2));
	VINDUSTRIAL.push_back(Tipo_actor("OBSTACULO IND. DIAGONAL 3 32x64", Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_64_3));
	VINDUSTRIAL.push_back(Tipo_actor("OBSTACULO IND. DIAGONAL 4 32x64", Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_64_4));
	VINDUSTRIAL.push_back(Tipo_actor("OBSTACULO IND. DIAGONAL 1 64x32", Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_32_1));
	VINDUSTRIAL.push_back(Tipo_actor("OBSTACULO IND. DIAGONAL 2 64x32", Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_32_2));
	VINDUSTRIAL.push_back(Tipo_actor("OBSTACULO IND. DIAGONAL 3 64x32", Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_32_3));
	VINDUSTRIAL.push_back(Tipo_actor("OBSTACULO IND. DIAGONAL 4 64x32", Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_32_4));
	this->TIPOS.push_back(Grupo_tipo("FLEXIBLE", VINDUSTRIAL));

	std::vector<Tipo_actor> VDECORACION;
	VDECORACION.push_back(Tipo_actor("DECORACION - FONDO ", Actor_mapa::T_DECORACION_NIVEL_FONDO));
	VDECORACION.push_back(Tipo_actor("DECORACION - FRENTE ", Actor_mapa::T_DECORACION_NIVEL_FRENTE));
	this->TIPOS.push_back(Grupo_tipo("DECORACION", VDECORACION));


	/////Tabla de probabilidad de obstáculos..
	this->TABLA_OBSTACULOS.push_back(Item_tabla_probabilidad("OBSTACULO 26x26", Actor_mapa::T_OBSTACULO_26_26, 0));
	this->TABLA_OBSTACULOS.push_back(Item_tabla_probabilidad("OBSTACULO 32x64", Actor_mapa::T_OBSTACULO_32_64, 0));
	this->TABLA_OBSTACULOS.push_back(Item_tabla_probabilidad("OBSTACULO 64x26", Actor_mapa::T_OBSTACULO_64_26, 0));
	this->TABLA_OBSTACULOS.push_back(Item_tabla_probabilidad("OBSTACULO 128x32", Actor_mapa::T_OBSTACULO_128_32, 0));
	this->TABLA_OBSTACULOS.push_back(Item_tabla_probabilidad("OBSTACULO 800x32", Actor_mapa::T_OBSTACULO_800_32, 0));
	this->TABLA_OBSTACULOS.push_back(Item_tabla_probabilidad("OBSTACULO BONUS", Actor_mapa::T_OBSTACULO_BONUS, 0));
	this->TABLA_OBSTACULOS.push_back(Item_tabla_probabilidad("OBSTACULO DESTRUIBLE", Actor_mapa::T_OBSTACULO_DESTRUIBLE, 0));
	this->TABLA_OBSTACULOS.push_back(Item_tabla_probabilidad("OBSTACULO GRAVEDAD", Actor_mapa::T_OBSTACULO_ACTIVIDAD_GRAVEDAD, 0));
	this->TABLA_OBSTACULOS.push_back(Item_tabla_probabilidad("OBSTACULO DISPARADOR", Actor_mapa::T_OBSTACULO_ACTIVIDAD_DISPARADOR, 0));
	this->TABLA_OBSTACULOS.push_back(Item_tabla_probabilidad("OBSTACULO EXPLOSIVO", Actor_mapa::T_OBSTACULO_ACTIVIDAD_EXPLOSIVO, 0));

	/////Tabla de probabilidad de bonus espontáneo...
	this->TABLA_BONUS.push_back(Item_tabla_probabilidad("BONUS DISPARO", Actor_mapa::T_BONUS_DISPARO, 0));
	this->TABLA_BONUS.push_back(Item_tabla_probabilidad("BONUS TIEMPO", Actor_mapa::T_BONUS_TIEMPO, 0));
	this->TABLA_BONUS.push_back(Item_tabla_probabilidad("BONUS VIDA", Actor_mapa::T_BONUS_VIDA, 0));
	this->TABLA_BONUS.push_back(Item_tabla_probabilidad("BONUS PIMIENTO", Actor_mapa::T_BONUS_PIMIENTO, 0));
	this->TABLA_BONUS.push_back(Item_tabla_probabilidad("BONUS CARGA ESCUDO", Actor_mapa::T_BONUS_CARGA_ESCUDO, 0));
	this->TABLA_BONUS.push_back(Item_tabla_probabilidad("BONUS COMPONENTE ESCUDO", Actor_mapa::T_BONUS_COMPONENTE_ESCUDO, 0));
	this->TABLA_BONUS.push_back(Item_tabla_probabilidad("GEMA", Actor_mapa::T_BONUS_NORMAL, 0));

	/////Tabla de probabilidad de bonus en caja bonus...
	this->TABLA_DESTRUIR_CAJA.push_back(Item_tabla_probabilidad("NADA", Actor_mapa::T_NADA, 0));
	this->TABLA_DESTRUIR_CAJA.push_back(Item_tabla_probabilidad("BONUS DISPARO", Actor_mapa::T_BONUS_DISPARO, 0));
	this->TABLA_DESTRUIR_CAJA.push_back(Item_tabla_probabilidad("BONUS TIEMPO", Actor_mapa::T_BONUS_TIEMPO, 0));
	this->TABLA_DESTRUIR_CAJA.push_back(Item_tabla_probabilidad("BONUS VIDA", Actor_mapa::T_BONUS_VIDA, 0));
	this->TABLA_DESTRUIR_CAJA.push_back(Item_tabla_probabilidad("BONUS PIMIENTO", Actor_mapa::T_BONUS_PIMIENTO, 0));
	this->TABLA_DESTRUIR_CAJA.push_back(Item_tabla_probabilidad("BONUS CARGA ESCUDO", Actor_mapa::T_BONUS_CARGA_ESCUDO, 0));
	this->TABLA_DESTRUIR_CAJA.push_back(Item_tabla_probabilidad("BONUS COMPONENTE ESCUDO", Actor_mapa::T_BONUS_COMPONENTE_ESCUDO, 0));
	this->TABLA_DESTRUIR_CAJA.push_back(Item_tabla_probabilidad("GEMA", Actor_mapa::T_BONUS_NORMAL, 0));

	/////Tabla de probabilidad de bonus en obstáculo destruible...
	this->TABLA_DESTRUIR_OBSTACULO.push_back(Item_tabla_probabilidad("NADA", Actor_mapa::T_NADA, 0));
	this->TABLA_DESTRUIR_OBSTACULO.push_back(Item_tabla_probabilidad("BONUS DISPARO", Actor_mapa::T_BONUS_DISPARO, 0));
	this->TABLA_DESTRUIR_OBSTACULO.push_back(Item_tabla_probabilidad("BONUS TIEMPO", Actor_mapa::T_BONUS_TIEMPO, 0));
	this->TABLA_DESTRUIR_OBSTACULO.push_back(Item_tabla_probabilidad("BONUS VIDA", Actor_mapa::T_BONUS_VIDA, 0));
	this->TABLA_DESTRUIR_OBSTACULO.push_back(Item_tabla_probabilidad("BONUS PIMIENTO", Actor_mapa::T_BONUS_PIMIENTO, 0));
	this->TABLA_DESTRUIR_OBSTACULO.push_back(Item_tabla_probabilidad("BONUS CARGA ESCUDO", Actor_mapa::T_BONUS_CARGA_ESCUDO, 0));
	this->TABLA_DESTRUIR_OBSTACULO.push_back(Item_tabla_probabilidad("BONUS COMPONENTE ESCUDO", Actor_mapa::T_BONUS_COMPONENTE_ESCUDO, 0));
	this->TABLA_DESTRUIR_OBSTACULO.push_back(Item_tabla_probabilidad("GEMA", Actor_mapa::T_BONUS_NORMAL, 0));

//	this->TIPOS.push_back(Tipo_actor("PROYECTIL OBSTACULO", Actor_mapa::T_PROYECTIL_OBSTACULO));

/*
	VACTIVIDAD.push_back(Tipo_actor("ACTIVIDAD CAÑON PROGRAMABLE", Actor_mapa::T_OBSTACULO_ACTIVIDAD_CANON_PROGRAMABLE));
	VACTIVIDAD.push_back(Tipo_actor("ACTIVIDAD PUERTA DISPARABLE", Actor_mapa::T_OBSTACULO_ACTIVIDAD_PUERTA_DISPARAR));
	VACTIVIDAD.push_back(Tipo_actor("ACTIVIDAD PUERTA AUTOMÁTICA", Actor_mapa::T_OBSTACULO_ACTIVIDAD_PUERTA_AUTOMATICA));
*/

	//Determinamos los tipos de parámetro para cada actor...
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_BONUS]=Estructura_parametros("Tipo bonus");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_ACTIVIDAD_GRAVEDAD]=Estructura_parametros("Tiempo total ms");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_ACTIVIDAD_DISPARADOR]=Estructura_parametros("Tiempo total ms", "Resistencia (20)", "Modo", "Vector X", "Vector Y", "Vector X neg", "Vector Y neg", "Ruido punteria");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_ACTIVIDAD_EXPLOSIVO]=Estructura_parametros("Tiempo total ms", "Cantidad disparos", "Ángulo inicial", "Actúa fuera de pantalla 0-1");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_ACTIVIDAD_CANON_PROGRAMABLE]=Estructura_parametros("Tiempo activo ms", "Tiempo descanso ms", "Ángulo", "Actúa fuera de pantalla 0-1", "Encendido 0-1");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_ACTIVIDAD_CANON_GIRATORIO]=Estructura_parametros("Tiempo activo ms", "Tiempo descanso ms", "Ángulo", "Grados/100 por segundo", "Cantidad de cañones", "Grados entre cañones", "Actúa fuera de pantalla 0-1", "Encendido 0-1");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_ACTIVIDAD_CANON_APUNTADO]=Estructura_parametros("Tiempo activo ms", "Tiempo descanso ms", "Encendido 0-1");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_ACTIVIDAD_PUERTA_DISPARAR]=Estructura_parametros("Tiempo abierta ms");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_ACTIVIDAD_PUERTA_REMOTA]=Estructura_parametros("Tiempo abierta ms", "Autocierre 0-1");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_ACTIVIDAD_PUERTA_AUTOMATICA]=Estructura_parametros("Tiempo abierta ms", "Tiempo cerrada ms", "Actúa fuera de pantalla 0-1");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_PROPIEDADES]=Estructura_parametros("Propiedades (flags 1-canon, 2-transp, 4-e.d.u, 8-Escudo)", "Nueva Velocidad", "Velocidad cambio (0=instantáneo)", "Es fin nivel (0-1)");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_PROPIEDADES_FONDO]=Estructura_parametros("Rojo", "Verde", "Azul", "Velocidad (por segundo, 0=no cambiar)", "Estrellas (401=ignorar)");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_TRIGGER_CONTACTO]=Estructura_parametros("Tag", "Repetible (0-1)", "Flags activado por (1- Jugador, 2- Fantasma)");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_TRIGGER_INTERRUPTOR]=Estructura_parametros("Tag", "Repetible (0-1)");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_TRIGGER_AUTO]=Estructura_parametros("Tag", "Repetible (0-1)", "Encendido");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_ACTIVIDAD_GENERADOR_DECORACION]=Estructura_parametros("Explosion", "Chispas", "Chatarra", "Humo de explosión");
	this->mapa_parametros[Actor_mapa::T_PUNTO_RUTA]=Estructura_parametros("Agrupacion", "Identificador", "Siguiente", "Velocidad", "Instantáneo (0-1)");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_VECTOR]=Estructura_parametros("X (*100)", "Y (*100)", "X negativo", "Y negativo");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_ACTIVIDAD_GENERADOR_OBSTACULOS]=Estructura_parametros("Tipo obstaculo", "Dimension completa", "Angulo ini", "Angulo fin", "Tiempo descanso", "Factor aleatorio", "Multiplicador vector", "Encendido");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_TUTORIAL]=Estructura_parametros("Mensaje", "Tiempo mínimo en ms", "Finaliza tutoriales");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_PROPIEDADES_MOVIMIENTO_RELATIVO]=Estructura_parametros("Desplazamiento x", "Desplazamiento y", "X negativo", "Y negativo", "Velocidad");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_ACTIVIDAD_BOSS_01]=Estructura_parametros("Tag accion al destruir");
	this->mapa_parametros[Actor_mapa::T_OBSTACULO_ACTIVIDAD_BOSS_02]=Estructura_parametros("Tag accion al destruir");

	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_JUEGO_IND, "Bloques"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_NIV_01_A, "01 - Roca - Solido"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_NIV_01_B, "01 - Roca - transparente"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_NIV_01_A_ALT_01, "01 - Roca - Alt solido"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_NIV_01_B_ALT_01, "01 - Roca - Alt transparente"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_NIV_02_A, "02 - Metal - Solido"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_NIV_02_B, "02 - Metal - Transparente"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_NIV_02_C, "02 - Metal - Detalles"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_NIV_02_A_ALT_01, "02 - Metal - Alt Solido"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_NIV_02_B_ALT_01, "02 - Metal - Alt Transparente"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_NIV_02_C_ALT_01, "02 - Metal - Alt detalles"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_NIV_03_A, "03 - Roca verde - Solido"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_NIV_03_B, "03 - Roca verde - Transparente"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_NIV_03_A_ALT_01, "03 - Roca verde - Alt Solido"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_NIV_03_B_ALT_01, "03 - Roca verde - Alt Transparente"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_NIV_04_A, "03 - Hielo - Solido"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_NIV_04_B, "03 - Hielo - Transparente"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_NIV_04_C, "03 - Hielo - Roca"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_NIV_04_A_ALT_01, "03 - Hielo - Alt Solido"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_NIV_04_B_ALT_01, "03 - Hielo - Alt Transparente"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_NIV_04_C_ALT_01, "03 - Hielo - Alt Roca"));
	this->recursos.push_back(T_RECURSO(Recursos::G_TILE_JUEGO, "Juego"));

	DLibV::Utilidades_graficas_SDL::mostrar_ocultar_cursor(true);
	this->pantalla.establecer_titulo("CHEAP SHOOTER - EDITOR");	

	this->rep_txt_info=new DLibV::Representacion_texto_fija_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), "", Medidas::ANCHO_VISTA, 100);
	this->rep_txt_info->establecer_posicion(0, 0);

	this->rep_txt_editar_propiedades=new DLibV::Representacion_texto_fija_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), "", Medidas::ANCHO_VISTA, 480);
	this->rep_txt_editar_propiedades->establecer_posicion(0, 120);

	this->rep_txt_editar_tablas=new DLibV::Representacion_texto_fija_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), "", Medidas::ANCHO_VISTA, 380);
	this->rep_txt_editar_tablas->establecer_posicion(0, 220);

	this->rep_txt_parametros=new DLibV::Representacion_texto_fija_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), "", Medidas::ANCHO_VISTA, 380);
	this->rep_txt_parametros->establecer_posicion(0, 220);

	this->rep_txt_tag=new DLibV::Representacion_texto_fija_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), "", Medidas::ANCHO_VISTA, 380);
	this->rep_txt_tag->establecer_posicion(0, 220);

	this->rep_txt_acciones=new DLibV::Representacion_texto_fija_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), "", Medidas::ANCHO_VISTA, 380);
	this->rep_txt_acciones->establecer_posicion(20, 120);

	this->rep_txt_listado=new DLibV::Representacion_texto_fija_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), "", Medidas::ANCHO_VISTA, 480);
	this->rep_txt_listado->establecer_posicion(0, 120);

	std::string cadena_info=DLibH::Herramientas::cadena_a_maximo_ancho(DLibH::Herramientas::cadena_desde_fichero("data/ayuda/editor.dat"), 80);
	this->rep_txt_ayuda=new DLibV::Representacion_texto_auto_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), cadena_info);
	this->rep_txt_ayuda->establecer_posicion(0, 120);

	return true;
}

void Controlador_editor::cambiar_estado(unsigned int p_valor)
{
	this->estado_actual=p_valor;
	this->indice_editando_tabla=0;
}

void Controlador_editor::cambiar_propiedad(short int factor)
{
	editando_propiedad+=factor;

	if(editando_propiedad >= P_MAX_PROPIEDADES) editando_propiedad=0;
	else if(editando_propiedad < 0) editando_propiedad=P_MAX_PROPIEDADES-1;	
}

std::string Controlador_editor::traducir_tipo_actor(unsigned int p_tipo)
{
	std::vector<Grupo_tipo >::iterator 	tini=TIPOS.begin(),
						tfin=TIPOS.end();

	std::vector<Tipo_actor>::iterator ini, fin;

	while(tini < tfin)
	{
		ini=(*tini).TIPOS.begin();
		fin=(*tini).TIPOS.end();

		while(ini < fin)
		{
			if( (*ini).tipo==p_tipo) return (*ini).nombre;
			ini++;
		}

		++tini;
	}

	return "Desconocido";
}

void Controlador_editor::duplicar_actor_actual()
{
	if(!this->seleccion_actual) return;
	else
	{
		int x=this->seleccion_actual->actor->acc_x();
		int y=this->seleccion_actual->actor->acc_y();
		int tipo=this->seleccion_actual->actor->tipo_actor_mapa();
		int tag=this->seleccion_actual->actor->acc_tag();

		insertar_actor_en_posicion(x, y, tipo, tag, 
		this->seleccion_actual->parametros, 
		this->seleccion_actual->params_rep,
		this->seleccion_actual->params_redim,
		this->seleccion_actual->params_profundidad,
		true, true);
	}
}

bool Controlador_editor::loop_principal()
{
	bool salir=false;

	while(!salir)
	{
		if(this->controles_sdl.es_senal_salida() || this->controles_sdl.es_tecla_down(SDLK_F4))
		{
			salir=true;
		}
		else
		{
			this->controles_general();
		}

		this->representar();
	}

	//Antes de salir, como se queda un par se segundos cerrando sistemas
	//vamos a poner la pantalla en negro...

	this->pantalla.rellenar(0,0,0);
	this->pantalla.flipar();

	return true;
}

void Controlador_editor::cambiar_dato_tabla(std::vector<Item_tabla_probabilidad> & p_vector, unsigned int p_indice, int factor)
{
	unsigned int l=p_vector.size();
	short int prob=0;

	if(p_indice >= 0 && p_indice < l)
	{
//		prob=p_vector[p_indice].probabilidad;
//		prob+=factor;

		prob=factor;
		if(prob < 0) prob=100;
		else if(prob > 100) prob=0;

		p_vector[p_indice].probabilidad=prob;
	}
	
}

void Controlador_editor::modificar_vector(int p_factor)
{
	INFO_NIVEL.vector_y_nivel=p_factor;
}

void Controlador_editor::modificar_max_obstaculos(int p_factor)
{
	INFO_NIVEL.max_obstaculos=p_factor;
}

void Controlador_editor::modificar_max_bonus(int p_factor)
{
	INFO_NIVEL.max_bonus=p_factor;
}

void Controlador_editor::modificar_gemas_en_nivel(int p_factor)
{
	INFO_NIVEL.gemas_en_nivel=p_factor;
}

void Controlador_editor::modificar_gemas_max_nivel(int p_factor)
{
	INFO_NIVEL.gemas_max_nivel=p_factor;
}

void Controlador_editor::modificar_artefactos(unsigned short int p_valor, bool activo)
{
	if(activo) INFO_NIVEL.flags_artefactos_nivel=INFO_NIVEL.flags_artefactos_nivel | p_valor;
	else INFO_NIVEL.flags_artefactos_nivel=INFO_NIVEL.flags_artefactos_nivel ^ p_valor;
}

void Controlador_editor::desplazar_seleccion_actual(float p_x, float p_y)
{
	auto desplazar=[p_x, p_y](T_TIPO_ACTOR * a)
	{
		float x=a->actor->acc_x();
		float y=a->actor->acc_y();

		x=x+(p_x * REJILLA_X);
		y=y+(p_y * REJILLA_Y);

		a->actor->ir_a(x, y);
	};

	if(this->seleccion_multiple.size())
	{
		for_each(seleccion_multiple.begin(), seleccion_multiple.end(), desplazar);
	}
	else if(this->seleccion_actual)
	{
		desplazar(this->seleccion_actual);
	}	
}

void Controlador_editor::modificar_profundidad_seleccion_actual(int prof)
{
	auto desplazar=[prof](T_TIPO_ACTOR * a)
	{
		int p=a->actor->acc_profundidad_representacion()+prof;

		if(p < 0) p=1;
		else if(p > 255) p=Actor_mapa::P_MAX_PROFUNDIDAD;

		Parametros_actor_ordenable_profundidad param(p);
		a->actor->recibir_valores_actor_ordenable_profundidad(param);
		a->params_profundidad=param;
	};

	if(this->seleccion_multiple.size())
	{
		for_each(seleccion_multiple.begin(), seleccion_multiple.end(), desplazar);
	}
	else if(this->seleccion_actual)
	{
		desplazar(this->seleccion_actual);
	}	
}

void Controlador_editor::seleccionar_indice(int indice)
{
	this->seleccion_actual=*(this->actores_mapa.begin()+indice);
	this->posicion_y=this->seleccion_actual->actor->acc_y()-300;
	this->camara.enfocar_a(0, this->posicion_y);
}

void Controlador_editor::seleccionar_actual(int p_factor)
{
	if(!this->seleccion_actual)
	{
		return;
	}
	else
	{
		std::vector<T_TIPO_ACTOR*>::iterator 	ini=this->actores_mapa.begin(),
							fin=this->actores_mapa.end();
		
		while(ini < fin)
		{
			//Localizar la selección actual...
			if((*ini)==this->seleccion_actual)
			{
				//Transformarla...

				if(p_factor > 0)
				{
					ini++;
					if(ini!=fin) this->seleccion_actual=(*ini);
				}
				else if(p_factor < 0)
				{
					if(ini!=this->actores_mapa.begin())
					{	
						ini--;
						if(ini!=fin) this->seleccion_actual=(*ini);
					}
				}								
		
				break;
			}

			ini++;
		}

		this->posicion_y=this->seleccion_actual->actor->acc_y()-300;
		this->camara.enfocar_a(0, this->posicion_y);
	}
}

bool Controlador_editor::actor_supera_filtro_profundidad(T_TIPO_ACTOR * ini)
{
/*
P_FONDO=50,	//Hasta 50 son "fondo"... Podemos ordenar hasta 50 niveles.
P_NORMAL=79,	//De 51 a 79 son niveles propios del motor para mostrar actores.
P_FRENTE=80,	//De 80 en adelante son "frente". Podemos ordenar hasta... muuuchos niveles.
P_MAX_PROFUNDIDAD=130
*/

	//Fondo...
	if(
		ini->actor->acc_profundidad_representacion() >= 0 &&	
		ini->actor->acc_profundidad_representacion() <= Actor_mapa::P_FONDO &&			
		this->filtro_profundidad_actores & F_FILTRO_FONDO)
	{
		return true;
	}
	//Normal...
	else if(
		ini->actor->acc_profundidad_representacion() > Actor_mapa::P_FONDO &&
		ini->actor->acc_profundidad_representacion() <= Actor_mapa::P_NORMAL &&  
		this->filtro_profundidad_actores & F_FILTRO_NORMAL)
	{
		return true;
	}
	//Frente...			
	else if(
		ini->actor->acc_profundidad_representacion() >= Actor_mapa::P_FRENTE &&
		ini->actor->acc_profundidad_representacion() <= Actor_mapa::P_MAX_PROFUNDIDAD &&   
		this->filtro_profundidad_actores & F_FILTRO_FRENTE)
	{
		return true;
	}

	return false;
}

Controlador_editor::T_TIPO_ACTOR * Controlador_editor::obtener_actor_en_posicion(int x, int y)
{
	y+=this->posicion_y;

	std::vector<T_TIPO_ACTOR*>::reverse_iterator	ini=this->actores_mapa.rbegin(),
							fin=this->actores_mapa.rend();

	while(ini < fin)
	{
		if(
			DLibH::Herramientas_SDL::punto_en_caja((*ini)->actor->acc_posicion(), x, y) 
			&& actor_supera_filtro_profundidad(*ini)
		)
		{
			return (*ini);
		}
		else
		{
			++ini;
		}
	}

	return NULL;
}

void Controlador_editor::insertar_actor_en_posicion(int x, int y, int tipo, unsigned int tag, const Parametros_actor& params, const Parametros_actor_flexible& p_params_rep, const Parametros_actor_redimensionable& p_params_red, const Parametros_actor_ordenable_profundidad& p_params_prof, bool cargando, bool seleccionar)
{
	/*Si no estamos cargando consideramos añadir a la Y del ratón la posición
	de la cámara y contar con el bloqueo de rejilla...*/

	if(!cargando)
	{
		y+=this->posicion_y;

		if(this->bloquear_a_rejilla)
		{
			x=REJILLA_X * (x/REJILLA_X);
			y=REJILLA_Y * (y/REJILLA_Y);
		}
	}

	if(tipo==Actor_mapa::T_OBSTACULO_800_32) x=0; //El obstáculo enorme siempre va a la izquierda...

	//Actor_mapa * obstaculo=Factoria_actores_mapa::fabricar(tipo, 0);
	std::shared_ptr<Actor_mapa> obstaculo(Factoria_actores_mapa::fabricar(tipo, 0));
	obstaculo->preparar_actor_mapa_manual(x, y, 0, 0);
	obstaculo->mut_tag(tag);

	if(obstaculo)
	{		
		T_TIPO_ACTOR * contenedor=new T_TIPO_ACTOR;
		contenedor->actor=obstaculo;
		contenedor->parametros=params;
		contenedor->params_rep=p_params_rep;
		contenedor->params_redim=p_params_red;
		contenedor->params_profundidad=p_params_prof;


//		contenedor->params_profundidad.p=obstaculo->acc_profundidad_representacion();

//		switch(tipo)
//		{
//			case Actor_mapa::T_DECORACION_NIVEL_FONDO: contenedor->params_profundidad.p=Actor_mapa::P_NORMAL-1; break;
//			case Actor_mapa::T_DECORACION_NIVEL_FRENTE: contenedor->params_profundidad.p=Actor_mapa::P_FRENTE; break;
//			default: contenedor->params_profundidad.p=Actor_mapa::P_NORMAL-1; break;
//		}


		//La representación ahora...
		if(obstaculo->es_flexible()) //Es flexible es... Raro.
		{
			Obstaculo_flexible * ac_flex=static_cast<Obstaculo_flexible *>(obstaculo.get());
//			contenedor->params_rep=p_params_rep;

			if(!cargando)
			{
				//Ojo con esto: en este caso dice que es del tipo 100% flexible... No
				//que sea redimensionable: es flexible y además redimensionable.
				if(ac_flex->es_redimensionable())
				{
					contenedor->params_rep.rec_x=ac_flex->acc_caja_patron().x;
					contenedor->params_rep.rec_y=ac_flex->acc_caja_patron().y;
					contenedor->params_rep.rec_w=ac_flex->acc_caja_patron().w;
					contenedor->params_rep.rec_h=ac_flex->acc_caja_patron().h;
					contenedor->params_rep.alx=ac_flex->acc_alineamiento_x();
					contenedor->params_rep.aly=ac_flex->acc_alineamiento_y();
				}
			}
			else
			{
				if(ac_flex->es_redimensionable())
				{
					ac_flex->establecer_caja_patron(contenedor->params_rep.rec_x, 
							contenedor->params_rep.rec_y,
							contenedor->params_rep.rec_w,
							contenedor->params_rep.rec_h);
					ac_flex->establecer_alineamiento(contenedor->params_rep.alx, contenedor->params_rep.aly);
					ac_flex->mut_indice_patron(contenedor->params_rep.indice_recurso);
				}

				ac_flex->refrescar_propiedades();
			}
		}

		if(obstaculo->es_redimensionable())
		{
			if(!cargando)
			{
				contenedor->params_redim.w=obstaculo->acc_w();
				contenedor->params_redim.h=obstaculo->acc_h();
			}
			else
			{
				obstaculo->mut_w(contenedor->params_redim.w);
				obstaculo->mut_h(contenedor->params_redim.h);
				obstaculo->procesar_cambio_dimensiones();
			}
		}
	
		if(obstaculo->es_ordenable_profundidad())
		{
			contenedor->params_profundidad.p=p_params_prof.p;
			obstaculo->recibir_valores_actor_ordenable_profundidad(p_params_prof);
		}

		this->actores_mapa.push_back(contenedor);

		if(seleccionar)
		{
			this->seleccion_actual=contenedor;
		}
	}

	Ordenador_actores_profundidad_editor ord;
	std::sort(this->actores_mapa.begin(), this->actores_mapa.end(), ord);
}

void Controlador_editor::eliminar_actor_seleccionado()
{
	if(this->seleccion_multiple.size())
	{
		for(T_TIPO_ACTOR * a : seleccion_multiple)
		{
			auto f=std::find(actores_mapa.begin(), actores_mapa.end(), a);	
			if(f!=actores_mapa.end()) 
			{				
				actores_mapa.erase(f);
			}
		}

		this->seleccion_multiple.clear();
	}
	else if(this->seleccion_actual)
	{
		auto f=std::find(actores_mapa.begin(), actores_mapa.end(), seleccion_actual);	
		if(f!=seleccion_multiple.end()) actores_mapa.erase(f);
		this->seleccion_actual=NULL;
	}
}

void Controlador_editor::reiniciar()
{
	this->vaciar_actores_mapa();
	this->vaciar_acciones();
	this->posicion_y=LIMITE_CAMARA_INFERIOR;
	this->camara.enfocar_a(0, this->posicion_y);
	this->actualizar_maximo_tag();

	this->mensaje="Reiniciando contenido";

}

void Controlador_editor::insertar_nueva_accion()
{
	this->acciones.push_back(Accion_trigger());
}

void Controlador_editor::eliminar_accion_actual()
{
	if(this->acciones.size())
	{
		this->acciones.erase(this->acciones.begin() + this->indice_accion);
		this->cambiar_accion_actual(-1);
	}
}

void Controlador_editor::cambiar_accion_actual(short int p_indice)
{
	unsigned int total=this->acciones.size();

	if(total)
	{
		if(p_indice < 0)
		{
			if(this->indice_accion==0) this->indice_accion=total-1;
			else --this->indice_accion;
		}
		else if(p_indice > 0)
		{
			++this->indice_accion;
			if(this->indice_accion>=total) this->indice_accion=0;
		}
	}
}

void Controlador_editor::vaciar_acciones()
{
	this->acciones.clear();
}

std::string Controlador_editor::traducir_recurso(unsigned int p_ind)
{
	std::vector<T_RECURSO>::iterator 	ini=this->recursos.begin(),
						fin=this->recursos.end();

	while(ini < fin)
	{
		if( (*ini).indice==p_ind) return (*ini).nombre;
		++ini;
	};

	return "DESCONOCIDO";
}

unsigned int Controlador_editor::indice_para_patron(unsigned int p_ind)
{
	std::vector<T_RECURSO>::iterator 	ini=this->recursos.begin(),
						fin=this->recursos.end();

	unsigned int i=0;

	while(ini < fin)
	{
		if( (*ini).indice==p_ind) return i;

		++i;
		++ini;
	};

	return 0;
}

//Este debe encargarse sólo del tema de la representación flexible.
void Controlador_editor::actualizar_representacion_flexible(short int p_x, short int p_y, short int p_w, short int p_h, short int p_alx, short int p_aly, short int p_rep)
{
	Obstaculo_flexible * ac_flex=static_cast<Obstaculo_flexible *>(this->seleccion_actual->actor.get());
	int rep_x=ac_flex->acc_caja_patron().x; 
	int rep_y=ac_flex->acc_caja_patron().y;
	int rep_w=ac_flex->acc_caja_patron().w; 
	int rep_h=ac_flex->acc_caja_patron().h;
	int rep_alx=ac_flex->acc_alineamiento_x();
	int rep_aly=ac_flex->acc_alineamiento_y();
	int rep_act=this->indice_para_patron(ac_flex->acc_indice_patron());

	int x=rep_x+(p_x*REJILLA_X);
	int y=rep_y+(p_y*REJILLA_Y);
	int w=rep_w+(p_w*REJILLA_X);
	int h=rep_h+(p_h*REJILLA_Y);
	int alx=rep_alx+(p_alx*REJILLA_X);
	int aly=rep_aly+(p_aly*REJILLA_Y);
	int rep_ind=rep_act+p_rep;

	int max=this->recursos.size();

	if(x<0) x=0;
	if(y<0) y=0;
	if(w<1) x=1;
	if(h<1) y=1;
	if(alx<0) alx=0;
	if(aly<0) aly=0;
	if(rep_ind<0) rep_ind=max-1;
	else if(rep_ind >= max) rep_ind=0;

//std::cout<<"REC rx"<<p_x<<" ry"<<p_y<<" rw"<<p_w<<" rh"<<p_h<<" aw"<<p_aw<<" ah"<<p_ah<<" rep"<<p_rep<<std::endl;
//std::cout<<"HAY x"<<rep_x<<" y"<<rep_y<<" w"<<rep_w<<" h"<<rep_h<<" aw"<<rep_aw<<" ah"<<rep_ah<<" rep"<<rep_act<<std::endl;
//std::cout<<"SAL x"<<x<<" y"<<y<<" w"<<w<<" h"<<h<<" aw"<<aw<<" h"<<ah<<" rep"<<rep_ind<<std::endl;

	this->seleccion_actual->params_rep.rec_x=x;
	this->seleccion_actual->params_rep.rec_y=y;
	this->seleccion_actual->params_rep.rec_w=w;
	this->seleccion_actual->params_rep.rec_h=h;
	this->seleccion_actual->params_rep.alx=alx;
	this->seleccion_actual->params_rep.aly=aly;
	this->seleccion_actual->params_rep.indice_recurso=this->recursos[rep_ind].indice;

	this->actualizar_representacion_seleccion_actual(x, y, w, h, alx, aly, this->recursos[rep_ind].indice);
}

void Controlador_editor::actualizar_representacion_seleccion_actual(int px, int py, int pw, int ph, int alx, int aly, int p_rep_ind)
{
	Obstaculo_flexible * ac_flex=static_cast<Obstaculo_flexible *>(this->seleccion_actual->actor.get());

	if(p_rep_ind != -1) {ac_flex->mut_indice_patron(p_rep_ind);}

	this->mensaje=
		this->traducir_recurso(p_rep_ind)+
		DLibH::Herramientas::entero_a_cadena(px)+","+
		DLibH::Herramientas::entero_a_cadena(py)+" ["+
		DLibH::Herramientas::entero_a_cadena(pw)+","+
		DLibH::Herramientas::entero_a_cadena(ph)+"] "+
		DLibH::Herramientas::entero_a_cadena(alx)+" / "+
		DLibH::Herramientas::entero_a_cadena(aly);

	ac_flex->establecer_caja_patron(px, py, pw, ph);
	ac_flex->establecer_alineamiento(alx, aly);
	ac_flex->refrescar_propiedades();
}

void Controlador_editor::actualizar_tamano(short int p_aw, short int p_ah)
{
	int rep_aw=this->seleccion_actual->actor->acc_w(); 
	int rep_ah=this->seleccion_actual->actor->acc_h(); 

	int aw=rep_aw+(p_aw*REJILLA_X);
	int ah=rep_ah+(p_ah*REJILLA_Y);

	if(aw<1) aw=1;
	if(ah<1) ah=1;

	this->seleccion_actual->params_redim.w=aw;
	this->seleccion_actual->params_redim.h=ah;

	this->actualizar_tamano_actual(aw, ah);
}

void Controlador_editor::actualizar_tamano_actual(int paw, int pah)
{
	if(paw!=-1) this->seleccion_actual->actor->mut_w(paw);
	if(pah!=-1) this->seleccion_actual->actor->mut_h(pah);

	if(paw!= -1 || pah!= -1)
	{
		this->seleccion_actual->actor->procesar_cambio_dimensiones();
	}
}

void Controlador_editor::actualizar_filtro_profundidad(unsigned int p_flag)
{
	if(this->filtro_profundidad_actores & p_flag)
	{
		this->filtro_profundidad_actores=this->filtro_profundidad_actores ^ p_flag;
	}
	else
	{
		this->filtro_profundidad_actores=this->filtro_profundidad_actores | p_flag;
	}
}

void Controlador_editor::insertar_en_seleccion_multiple(T_TIPO_ACTOR * p)
{
	seleccion_multiple.push_back(p);
	seleccion_actual=NULL;
}

void Controlador_editor::sacar_de_seleccion_multiple(T_TIPO_ACTOR * p)
{
	std::vector<T_TIPO_ACTOR *>::iterator it;
	it=find(seleccion_multiple.begin(), seleccion_multiple.end(), p);	
	if(it!=seleccion_multiple.end()) seleccion_multiple.erase(it);
}

bool Controlador_editor::existe_en_seleccion_multiple(T_TIPO_ACTOR * p)
{
	bool resultado=false;
	auto comprobacion=[&resultado, p](T_TIPO_ACTOR * a){if(p==a) resultado=true;};
	for_each(seleccion_multiple.begin(), seleccion_multiple.end(), comprobacion);
	return resultado;
}

void Controlador_editor::vaciar_seleccion_multiple()
{
	seleccion_multiple.clear();
}

void Controlador_editor::seleccionar_actores_en_caja(const SDL_Rect& p_caja)
{
	auto comprobar_e_insertar=[&p_caja, this](T_TIPO_ACTOR * a)
	{
		if(DLibH::Herramientas_SDL::rectangulos_superpuestos(p_caja, a->actor->acc_posicion()))
			this->insertar_en_seleccion_multiple(a);
	};

	for_each(actores_mapa.begin(), actores_mapa.end(), comprobar_e_insertar);
}

void Controlador_editor::actualizar_maximo_tag()
{
	auto accion=[this](T_TIPO_ACTOR * a)
	{
		unsigned int t=a->actor->acc_tag();
		if(t > maximo_tag) maximo_tag=t;
	};

	maximo_tag=0;
	for_each(actores_mapa.begin(), actores_mapa.end(), accion);
	++maximo_tag;

}

void Controlador_editor::guardar_archivo_listado_actores()
{
	std::ofstream archivo("listado_nivel.txt", std::ios::out);	

	if(!archivo)
	{
		this->mensaje="No se ha podido generar el fichero";
	}
	else
	{
		auto funcion_mostrado=[this, &archivo](T_TIPO_ACTOR * actor, unsigned int i)
		{
			unsigned int tipo_actor=actor->actor->tipo_actor_mapa();
		
			std::string indice=DLibH::Herramientas::entero_a_cadena(this->indice_actual_listado+i);
			std::string x=DLibH::Herramientas::entero_a_cadena(actor->actor->acc_x());
			std::string y=DLibH::Herramientas::entero_a_cadena(actor->actor->acc_y());
			std::string w=DLibH::Herramientas::entero_a_cadena(actor->actor->acc_w());
			std::string h=DLibH::Herramientas::entero_a_cadena(actor->actor->acc_h());
			std::string tag=DLibH::Herramientas::entero_a_cadena(actor->actor->acc_tag());

			std::string cadena=" "+indice+" : TAG "+tag+" :: "+
			traducir_tipo_actor(tipo_actor)+
			" ["+x+","+y+"] - ("+w+", "+h+")";

			archivo<<cadena.c_str()<<std::endl;
		};

		unsigned int i=1;

		std::vector<T_TIPO_ACTOR *>::iterator 	ini=actores_mapa.begin(),
							fin=actores_mapa.end();


		while(ini<fin)
		{
			funcion_mostrado((*ini), i);
			++i;
			++ini;
		}

		this->mensaje="Fichero generado correctamente";

	}
}

void Controlador_editor::eliminar_duplicados()
{
	seleccion_actual=NULL;
	vaciar_seleccion_multiple();

	//Primero los ordenamos...
	Comparador_actor_parametrizado ordenador;
	std::sort(actores_mapa.begin(), actores_mapa.end(), ordenador);

	unsigned int duplicados=0;
	Comparador_igualdad_actor_parametrizado comparador;
	std::vector<T_TIPO_ACTOR *>::iterator primer_duplicado=std::unique(actores_mapa.begin(), actores_mapa.end(), comparador);

	while(primer_duplicado < actores_mapa.end())
	{
		++duplicados;
		primer_duplicado=actores_mapa.erase(primer_duplicado);
	}

	std::sort(actores_mapa.begin(), actores_mapa.end(), ordenador);

	this->mensaje="Encontrados y eliminados "+DLibH::Herramientas::entero_a_cadena(duplicados)+" duplicados";
}

void Controlador_editor::duplicar_seleccion_multiple_obstaculo_flexible_en_decoracion(unsigned int tipo)
{
	for(T_TIPO_ACTOR * a : seleccion_multiple)
	{
		if(
			a->actor->tipo_actor_mapa()==Actor_mapa::T_OBSTACULO_FLEX)/* ||
			a->actor->tipo_actor_mapa()==Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_1 ||
			a->actor->tipo_actor_mapa()==Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_2 ||
			a->actor->tipo_actor_mapa()==Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_3 ||
			a->actor->tipo_actor_mapa()==Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_4 ||
			a->actor->tipo_actor_mapa()==Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_1 ||
			a->actor->tipo_actor_mapa()==Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_2 ||
			a->actor->tipo_actor_mapa()==Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_3 ||
			a->actor->tipo_actor_mapa()==Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_4 ||
			a->actor->tipo_actor_mapa()==Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_64_1 ||
			a->actor->tipo_actor_mapa()==Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_64_2 ||
			a->actor->tipo_actor_mapa()==Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_64_3 ||
			a->actor->tipo_actor_mapa()==Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_64_4 ||
			a->actor->tipo_actor_mapa()==Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_32_1 ||
			a->actor->tipo_actor_mapa()==Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_32_2 ||
			a->actor->tipo_actor_mapa()==Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_32_3 ||
			a->actor->tipo_actor_mapa()==Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_32_4)*/
		{
			int x=a->actor->acc_x();
			int y=a->actor->acc_y();
			int tag=a->actor->acc_tag();

			insertar_actor_en_posicion(x, y, tipo, tag, 
			Parametros_actor(), 
			Parametros_actor_flexible(0, 0, 32, 32, 0, 0, 0),
			Parametros_actor_redimensionable(a->actor->acc_w(), a->actor->acc_h()),
			Parametros_actor_ordenable_profundidad(a->actor->acc_profundidad_representacion()),
			true, true);
		}
	}
}

void Controlador_editor::exportar_imagen()
{
	int offset_y=0;

	for(T_TIPO_ACTOR * ini : actores_mapa)
	{	
		if((*ini).actor->acc_y() < offset_y)
		{
			offset_y=(*ini).actor->acc_y();
		}
	}

	offset_y=-offset_y;

	DLibV::Representacion_compuesta_estatica l=DLibV::Representacion_compuesta_estatica(Medidas::ANCHO_VISTA, offset_y);

	Ordenador_T_TIPO_ACTOR ord;
	sort(this->actores_mapa.begin(), 
		this->actores_mapa.end(),
		ord);

	Actor_mapa::desactivar_en_editor();

	for(T_TIPO_ACTOR * ini : actores_mapa)
	{	
		if((*ini).actor->es_representable())
		{
			int x=(*ini).actor->acc_x();
			int y=(*ini).actor->acc_y()+offset_y;
			int w=(*ini).actor->acc_representacion()->acc_posicion().w;
			int h=(*ini).actor->acc_representacion()->acc_posicion().h;
			SDL_Rect caja=SDL_Rect();

			caja.x=x;
			caja.y=y;
			caja.w=w;
			caja.h=h;

			l.volcar_representacion(*(*ini).actor->acc_representacion(), caja);		
		}
	}

	Actor_mapa::activar_en_editor();

	std::string fichero=nombre_fichero+".bmp";
	l.exportar_bmp(fichero.c_str());
	this->mensaje="Mapa exportado como imagen bmp.";
}
