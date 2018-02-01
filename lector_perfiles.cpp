#include <iostream>
#include <fstream>
#include <string>
#include "class/motor_sdl/herramientas/herramientas/herramientas.h"
#include "class/motor_sdl/herramientas/herramientas/herramientas.cpp"
#include "class/aplicacion/perfil/perfil.h"
#include "class/aplicacion/perfil/perfil.cpp"
#include "class/aplicacion/perfil_datos/perfil_datos.h"
#include "class/aplicacion/perfil_datos/perfil_datos.cpp"

void mostrar(std::string archivo)
{
	try
	{
		Perfil p=Perfil::cargar_desde_archivo(archivo);
		Perfil_datos * d=p.obtener_datos_juego();
		
		std::cout<<"SESIONES INICIADAS "<<d->acc_sesiones_iniciadas()<<std::endl;
		std::cout<<"PUNTUACION ACUMULADA "<<d->acc_puntuacion_acumulada()<<std::endl;
		std::cout<<"MAX TIPO COMBO "<<d->acc_max_tipo_combo()<<std::endl;
		std::cout<<"DISPAROS EFECTUADOS "<<d->acc_disparos_efectuados()<<std::endl;
		std::cout<<"PARTIDAS INICIADAS "<<d->acc_partidas_iniciadas()<<std::endl;
		std::cout<<"PARTIDAS ABORTADAS "<<d->acc_partidas_abortadas()<<std::endl;
		std::cout<<"PARTIDAS GANADAS "<<d->acc_partidas_ganadas()<<std::endl;
		std::cout<<"PARTIDAS PERDIDAS "<<d->acc_partidas_perdidas()<<std::endl;
		std::cout<<"GEMAS RECOGIDAS "<<d->acc_total_gemas_recogidas()<<std::endl;
		std::cout<<"SEGUNDOS TIEMPO DETENIDO "<<d->acc_total_segundos_tiempo_detenido()<<std::endl;
		std::cout<<"OBSTACULOS DESTRUIDOS "<<d->acc_total_obstaculos_destruidos()<<std::endl;
		std::cout<<"VIDAS PERDIDAS "<<d->acc_total_vidas_perdidas()<<std::endl;
		std::cout<<"PIXELES TELETRANSPORTADOS "<<d->acc_total_pixeles_teletransportados()<<std::endl;
		std::cout<<"POTENCIA E.D.U USADA "<<d->acc_total_potencia_edu_usada()<<std::endl;
		std::cout<<"NIVEL SECRETO 1 "<<d->es_finalizado_nivel_secreto(0)<<std::endl;
		std::cout<<"NIVEL SECRETO 2 "<<d->es_finalizado_nivel_secreto(1)<<std::endl;
		std::cout<<"NIVEL SECRETO 3 "<<d->es_finalizado_nivel_secreto(2)<<std::endl;

		unsigned int i=0;

		while(i < Definicion_retos::TOTAL_NIVELES+Definicion_retos::TOTAL_NIVELES_SECRETOS)
		{
			try
			{
				Perfil_datos::Info_nivel in=d->obtener_datos_nivel(i);
				std::cout<<" *** INFO NIVEL "<<i<<std::endl;
				std::cout<<"NIVEL: "<<(int)in.nivel<<std::endl;
				std::cout<<"PUNTUACION: "<<(int)in.maxima_puntuacion<<std::endl;
				std::cout<<"VECES INICIADO: "<<(int)in.veces_iniciado<<std::endl;
				std::cout<<"VIDAS PERDIDAS: "<<(int)in.vidas_perdidas<<std::endl;
				std::cout<<"FINALIZADO EN : "<<(int)in.dia_finalizacion<<"-"<<(int)in.mes_finalizacion<<"-"<<(int)in.anho_finalizacion<<std::endl;
				std::cout<<"RECOGIDO : "<<(int)in.porcentaje_recogible<<"%"<<std::endl;
				std::cout<<"DESTRUIDO : "<<(int)in.porcentaje_destruible<<"%"<<std::endl;
			}
			catch(Perfil_datos::Info_nivel_excepcion e)
			{
				std::cout<<"EXCEPCION AL CARGAR INFO NIVEL!!!!"<<std::endl;
			}

			++i;
		}	

		return;
	}
	catch(Perfil_excepcion e)
	{
		std::cout<<"El perfil "<<archivo<<" no existe"<<std::endl;
		return;
	}	
}

void marcar(std::string archivo, int tipo, int nivel)
{
	try
	{
		Perfil p=Perfil::cargar_desde_archivo(archivo);
		Perfil_datos * d=p.obtener_datos_juego();
		Estructura_datos_perfil_nivel datos(10, 10, 10, 10, 10, 10);
		d->obtener_datos_nivel(nivel).procesar_datos_fin_nivel(datos, tipo==1);
		p.guardar();

		return;
	}
	catch(Perfil_excepcion e)
	{
		std::cout<<"El perfil "<<archivo<<" no existe"<<std::endl;
		return;
	}
}

int main(int argc, char ** argv)
{
	if(argc !=2)
	{
		std::cout<<"USO: leerperfil nombreperfil"<<std::endl;
	}
	else
	{
		std::string archivo=argv[1];
		mostrar(archivo);
		
		int menu=-1;
		int aux=-1;

		while(menu)
		{
			std::cout<<"0 - Salir"<<std::endl;
			std::cout<<"1 - Marcar nivel finalizado"<<std::endl;
			std::cout<<"2 - Marcar nivel NO finalizado"<<std::endl;
			std::cout<<"3 - Mostrar de nuevo"<<std::endl;
			std::cin>>menu;

			switch(menu)
			{
				case 1:
				case 2:
					std::cout<<"NIVEL: 0-11"<<std::endl;
					std::cin>>aux;
					if(aux > 0 && aux < 12) marcar(archivo, menu, aux);
				break;
				
				case 3: mostrar(archivo); break;
			}
		}
	}

	return 0;
}
