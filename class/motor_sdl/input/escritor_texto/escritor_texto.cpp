#include "escritor_texto.h"

using namespace DLibI;

/*El parámetro es para indicar al controlador que debe reiniciar el estado
como si estuviera al principio de un nuevo loop. Lo vamos a dejar controlable
porque nunca se sabe...*/

void Escritor_texto::loop_control(bool limpiar)
{
	SDL_Event evento;
	int tecla=0;
	this->modificada_cadena=false;

	//Se bombearán todos los eventos de la cola. Con el segundo parámetro
	//se pasan antes por el controlador.

	if(limpiar) 
	{
		this->controles.limpiar_para_nueva_recogida();	
	}

	while(this->controles.bombear_eventos_manual(evento, true))
	{
		if(this->controles.es_evento_keydown(evento))
		{
			tecla=evento.key.keysym.sym;

			if(tecla==this->codigo_salida) 
			{
				this->obtenida_salida=true;
				break;
			}
			else
			{
				//std::cout<<"SE HA PULSADO "<<tecla<<std::endl;

				if(isprint(tecla))
				{
					int longitud=this->cadena.size();
		
					//Mayúsculas importantes...
					if(evento.key.keysym.mod & KMOD_LSHIFT || evento.key.keysym.mod & KMOD_RSHIFT)
					{
						int insertar=0;

						switch(tecla)
						{
							case 44: insertar=';'; break;
							case 45: insertar='_'; break;
							case 46: insertar=':'; break;
							case 48: insertar='='; break;
							case 49: insertar='!'; break;
							case 50: insertar='"'; break;
							case 51: insertar='.'; break;
							case 52: insertar='$'; break;
							case 53: insertar='%'; break;
							case 54: insertar='&'; break;
							case 55: insertar='/'; break;
							case 56: insertar='('; break;
							case 57: insertar=')'; break;
							default: insertar=toupper(tecla); break;
						}								

						if(insertar && (this->limite_longitud==VALOR_SIN_LIMITAR || longitud < this->limite_longitud))
						{					
							this->modificada_cadena=true;
							this->cadena.append(1, (char)insertar);
						}
					}
					//ALTs importantes...
					else if(evento.key.keysym.mod & KMOD_LALT || evento.key.keysym.mod & KMOD_RALT)
					{
						int insertar=0;

						switch(tecla)
						{
							case 49: insertar='|'; break;
							case 50: insertar='@'; break;
							case 51: insertar='#'; break;
							case 52: insertar='~'; break;
							case 53: insertar='_'; break;
							case 54: insertar='_'; break;
							case 55: insertar='{'; break;
							case 56: insertar='['; break;
							case 57: insertar=']'; break;
						}								

						if(insertar && (this->limite_longitud==VALOR_SIN_LIMITAR || longitud < this->limite_longitud))
						{					
							this->modificada_cadena=true;
							this->cadena.append(1, (char)insertar);
						}
					}
					else
					{
						if(this->limite_longitud==VALOR_SIN_LIMITAR || longitud < this->limite_longitud)
						{
							this->modificada_cadena=true;
							this->cadena.append(1, (char)tecla);	
						}
					}
				}
				else
				{
					switch(tecla)
					{
						case 8:	
							if(this->cadena.size()) 
							{
								this->cadena.erase(this->cadena.size()-1); 
								this->modificada_cadena=true;
							}
						break; //Borrar...
						default: break;
					}
				}
			}
		}
		else if(this->controles.es_evento_quit(evento))
		{
			break;
		}
	}
}

std::string Escritor_texto::acc_cadena() const {return this->cadena;}
const char * Escritor_texto::acc_cadena_c() const  {return this->cadena.c_str();}
void Escritor_texto::mut_cadena(std::string const & p_cadena){this->cadena=p_cadena;}
void Escritor_texto::limpiar(){this->cadena="";}
