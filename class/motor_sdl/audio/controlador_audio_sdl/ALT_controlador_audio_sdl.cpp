#include "controlador_audio_sdl.h"

using namespace DLibA;

/*
Canal_audio::Canal_audio(const Canal_audio& p_otro):
	indice(p_otro.indice),
	repeticiones(p_otro.repeticiones),
	volumen(p_otro.volumen),
	reproduciendo(p_otro.reproduciendo),
	ocupado(p_otro.ocupado),
	monitoreado(p_otro.monitoreado),
	pausado(p_otro.pausado),
	audio_reproduciendo(p_otro.audio_reproduciendo)
{}

Canal_audio& Canal_audio::operator=(const Canal_audio& p_otro)
{
	indice=p_otro.indice;
	repeticiones=p_otro.repeticiones;
	volumen=p_otro.volumen;
	reproduciendo=p_otro.reproduciendo;
	ocupado=p_otro.ocupado;
	monitoreado=p_otro.monitoreado;
	pausado=p_otro.pausado;
	audio_reproduciendo=p_otro.audio_reproduciendo;

	return *this;
}*/

void Canal_audio::liberar()
{
	std::cout<<"LIBERANDO"<<std::endl;
	audio_reproduciendo=NULL;
	repeticiones=0;
	ocupado=false;
	monitoreado=false;
}

void Canal_audio::forzar_parada()
{
	std::cout<<"FORZAR PARADA"<<std::endl;
	Mix_HaltChannel(indice);
	reproduciendo=false;
	liberar();
}

void Canal_audio::establecer_volumen(int v)
{
	volumen=v;
	Mix_Volume(indice, volumen);
}

void Canal_audio::recibir_callback_finalizacion()
{
	std::cout<<"RECIBIDO CALLBACK FIN ["<<indice<<"]"<<std::endl;
	reproduciendo=false;
	if(!monitoreado) liberar();
}

int Canal_audio::iniciar_reproduccion(Sonido& p_s, int p_vol, int p_rep)
{
	Estructura_sonido e(p_s, p_vol, p_rep);
	return iniciar_reproduccion(e);	
}

int Canal_audio::iniciar_reproduccion(Estructura_sonido& e)
{
	audio_reproduciendo=e.sonido;
	repeticiones=e.repeticiones;
	if(e.volumen!=-1) Mix_Volume(indice, e.volumen);
	reproduciendo=true;

	std::cout<<"INICIANDO SONIDO ["<<indice<<" "<<this<<"] -> REPRODUCIENDO "<<reproduciendo<<std::endl;

	return Mix_PlayChannel(indice, audio_reproduciendo->acc_datos(), repeticiones);
}

int Canal_audio::iniciar_fade_in(Sonido& p_s, int p_ms, int p_vol, int p_rep)
{
	Estructura_sonido e(p_s, p_vol, p_rep);
	return iniciar_fade_in(e, p_ms);	
}

int Canal_audio::iniciar_fade_in(Estructura_sonido& e, int p_ms)
{
	audio_reproduciendo=e.sonido;
	repeticiones=e.repeticiones;
	reproduciendo=true;

	std::cout<<"INICIANDO FADE IN ["<<indice<<" "<<this<<"] -> "<<p_ms<<" REPRODUCIENDO "<<reproduciendo<<std::endl;

	return Mix_FadeInChannel(indice, audio_reproduciendo->acc_datos(), repeticiones, p_ms);
}

int Canal_audio::iniciar_fade_out(int p_ms)
{
	return Mix_FadeOutChannel(indice, p_ms);
}

void Canal_audio::pausar()
{
	Mix_Pause(indice);
	pausado=true;
}

void Canal_audio::reanudar()
{
	Mix_Resume(indice);
	pausado=false;
}

Controlador_audio_SDL::Controlador_audio_SDL():
	ratio(44100), //22050), 
	salidas(2), 
	buffers(1024), //1024), //2048), 
	formato(AUDIO_S16SYS), 
	canales_audio(8), 
	estado_musica(true)
{

}

Controlador_audio_SDL::~Controlador_audio_SDL()
{
	for(Canal_audio * c : canales)
	{
		delete c;
	}
}

Controlador_audio_SDL * Controlador_audio_SDL::obtener()
{
	if(!Controlador_audio_SDL::instancia)
	{
		Controlador_audio_SDL::instancia=new Controlador_audio_SDL;
	}	

	return Controlador_audio_SDL::instancia;
}

void Controlador_audio_SDL::desmontar()
{
	Controlador_audio_SDL::instancia->detener_sonido(-1);
	Controlador_audio_SDL::instancia->detener_musica();
	Mix_CloseAudio();

	if(Controlador_audio_SDL::instancia)
	{
		delete Controlador_audio_SDL::instancia;
		Controlador_audio_SDL::instancia=NULL;
	}	
}

int Controlador_audio_SDL::reproducir_sonido(Sonido& p_sonido, int p_canal, int p_vol, int p_repeticiones)
{
	Estructura_sonido e(p_sonido, p_vol, p_repeticiones);
	return reproducir_sonido(e, p_canal);
}

int Controlador_audio_SDL::reproducir_sonido(Estructura_sonido& p_estructura, int p_canal)
{
	int resultado=-1;

	if(p_estructura.es_preparado())
	{
		if(p_canal==-1) p_canal=obtener_indice_canal_libre();

		try
		{
//			resultado=canales.at(p_canal).iniciar_reproduccion(p_estructura);
			resultado=canales.at(p_canal)->iniciar_reproduccion(p_estructura);
		}
		catch (const std::out_of_range& e) 
		{
			return -1;
		}
	}

	return resultado;
}

int Controlador_audio_SDL::reproducir_fade_in(Sonido& p_sonido, int p_ms, int p_canal, int p_vol, int p_repeticiones)
{
	Estructura_sonido e(p_sonido, p_vol, p_repeticiones);
	return reproducir_fade_in(e, p_ms, p_canal);
}

int Controlador_audio_SDL::reproducir_fade_in(Estructura_sonido& p_estructura, int p_ms, int p_canal)
{
	int resultado=-1;

	if(p_estructura.es_preparado())
	{
		if(p_canal==-1) p_canal=obtener_indice_canal_libre();

		try
		{
//			resultado=canales.at(p_canal).iniciar_fade_in(p_estructura, p_ms);
			resultado=canales.at(p_canal)->iniciar_fade_in(p_estructura, p_ms);
		}
		catch (const std::out_of_range& e) 
		{
			return -1;
		}
	}

	return resultado;
}

/*Obtiene el primer canal libre a partir del canal especificado.  Si se da
un segundo parámetro busca hasta el canal especificado. Si no se da ninguno 
devuelve realmente el primer canal libre. Aparementemente devuelve -1 si
no hay canales disponibles.*/

int Controlador_audio_SDL::obtener_indice_canal_libre(int p_desde, int p_hasta)
{
	int resultado=-1;
	unsigned int i=p_desde;
	unsigned int l=(p_hasta==-1) ? canales.size() : p_hasta;
	
	//Un control adicional de que el segundo parámetro no sea mayor que la lista de canales disponibles.
	if(l > canales.size()) l=canales.size();

	for(;i<l;i++)
	{
//		Canal_audio c=canales.at(i);
		Canal_audio * c=canales.at(i);
	
//		if(!c.es_ocupado() && !c.es_reproduciendo())
		if(!c->es_ocupado() && !c->es_reproduciendo())
		{
			resultado=i;
			break;
		}
	}

	return resultado;
}

void Controlador_audio_SDL::detener_sonido(int p_canal)
{
	try
	{
//		canales.at(p_canal).forzar_parada();
		canales.at(p_canal)->forzar_parada();
	}
	catch(const std::out_of_range& e)
	{

	}
}

bool Controlador_audio_SDL::reproducir_musica(Musica &p_musica, int p_repeticiones)
{
	bool resultado=false;

	if(p_musica.es_preparada())
	{
		Mix_PlayMusic(p_musica.acc_datos(), p_repeticiones);
		this->estado_musica=false;
		resultado=true;
	}

	return resultado;
}

void Controlador_audio_SDL::detener_musica()
{
	if(!this->estado_musica)
	{
		this->estado_musica=true;
		Mix_HaltMusic();
	}
}

//De la documentación...
//All channels default to a volume of 128, which is the max. 
//Newly allocated channels will have the max volume set, 
//so setting all channels volumes does not affect subsequent channel allocations. 

void Controlador_audio_SDL::establecer_volumen(int p_vol, int p_canal)	//p_vol de 0 a 128.
{
	try
	{
//		canales.at(p_canal).establecer_volumen(p_vol);
		canales.at(p_canal)->establecer_volumen(p_vol);
	}
	catch(const std::out_of_range& e)
	{

	}
}

void Controlador_audio_SDL::establecer_volumen_musica(int p_vol)
{
	Mix_VolumeMusic(p_vol);
}

void Controlador_audio_SDL::preparar_controles(unsigned int p_can)
{
	canales.clear();
	unsigned int i=0;
	
	while(i < p_can)
		canales.push_back(new Canal_audio(i++));
//		canales.push_back(Canal_audio(i++));
}

bool Controlador_audio_SDL::iniciar()
{
	this->preparar_controles(this->canales_audio);

	bool resultado=true;

	//Comprobar que el audio está arrancado.
	if(SDL_WasInit(SDL_INIT_AUDIO)==0) 
	{
		if(SDL_InitSubSystem(SDL_INIT_AUDIO)==-1)
		{
			resultado=false;
		}
	}

	if(resultado)
	{	
		if(Mix_OpenAudio(this->ratio, this->formato, this->salidas, this->buffers) == -1) 
		{
			resultado=false;
		}
		else
		{
			Mix_ChannelFinished(callback_fin_reproduccion);
		}
	}

	return resultado;
}

void Controlador_audio_SDL::debug_estado_canales()
{
	std::cout<<"C["<<canales.size()<<"] = ";

//	for(Canal_audio& c : canales)
//	{
//		if(c.es_pausado()) std::cout<<"P"<<std::endl;
//		else if(c.es_reproduciendo()) std::cout<<"R";
//		else if(c.es_ocupado()) std::cout<<"O";
//		else std::cout<<"_";
//	}

	for(Canal_audio * c : canales)
	{
		if(c->es_pausado()) std::cout<<"P"<<std::endl;
		else if(c->es_reproduciendo()) std::cout<<"R";
		else if(c->es_ocupado()) std::cout<<"O";
		else std::cout<<"_";
	}

	std::cout<<std::endl;
}

Canal_audio& Controlador_audio_SDL::obtener_canal(int p_canal) throw()
{
	try
	{
//		return Canal_audio(canales.at(p_canal));
		return * canales.at(p_canal);
//		return canales.at(p_canal);
	}
	catch(const std::out_of_range& e)
	{
		throw Excepcion_controlador_audio(Excepcion_controlador_audio::ERROR_RANGO, "El canal no existe");
	}
}

Canal_audio& Controlador_audio_SDL::obtener_canal_libre() throw()
{
	return obtener_canal(obtener_indice_canal_libre());
}

void Controlador_audio_SDL::pausar_sonido()
{
//	for(Canal_audio c : canales) c.pausar();
	for(Canal_audio * c : canales) c->pausar();
}

void Controlador_audio_SDL::reanudar_sonido()
{
//	for(Canal_audio c : canales) c.reanudar();
	for(Canal_audio * c : canales) c->reanudar();
}

void Controlador_audio_SDL::detener_sonido()
{
//	for(Canal_audio c : canales) c.forzar_parada();
	for(Canal_audio * c : canales) c->forzar_parada();
}

void DLibA::callback_fin_reproduccion(int p_canal)
{
	Controlador_audio_SDL * ca=Controlador_audio_SDL::instancia;

	if(ca)
	{
		try
		{
//			ca->canales.at(p_canal).recibir_callback_finalizacion();
			ca->canales.at(p_canal)->recibir_callback_finalizacion();
		}
		catch (const std::out_of_range& e) 
		{}
	}
}
