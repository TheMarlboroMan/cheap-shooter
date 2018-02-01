#include "solicitud_curl.h"

const char Solicitud_CURL::CURL_EXPECT[]="Expect:";

Solicitud_CURL::Solicitud_CURL()
	:curl(NULL), formpost(NULL), lastptr(NULL), headerlist(NULL), con_post(false)
{

}

Solicitud_CURL::~Solicitud_CURL()
{

}

bool Solicitud_CURL::inicializar(std::string const& p_url)
{
	this->url=p_url;
	return curl_global_init(CURL_GLOBAL_ALL)==0;
}

void Solicitud_CURL::post(std::string const& p_campo, std::string const& p_valor)
{
	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, p_campo.c_str(), CURLFORM_COPYCONTENTS, p_valor.c_str(), CURLFORM_END);
	this->con_post=true;
}


void Solicitud_CURL::post(std::string const& p_campo, int p_valor)
{
	std::string cad=DLibH::Herramientas::entero_a_cadena(p_valor);
	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, p_campo.c_str(), CURLFORM_COPYCONTENTS, cad.c_str(), CURLFORM_END);
	this->con_post=true;
}

size_t Solicitud_CURL::callback(void *ptr, size_t size, size_t count, void *stream)
{
	((std::string*)stream)->append((char*)ptr, 0, size* count);
	return size* count;
}

bool Solicitud_CURL::lanzar()
{
	this->curl = curl_easy_init();
	this->headerlist = curl_slist_append(this->headerlist, Solicitud_CURL::CURL_EXPECT);

	if(!this->curl) 
	{
		return false;
	}
	else
	{
		if(this->con_post) curl_easy_setopt(this->curl, CURLOPT_HTTPPOST, this->formpost); //Esto debe ser para añadir los datos que se van a mandar por POST.

		curl_easy_setopt(this->curl, CURLOPT_URL, this->url.c_str());	//Decimos a dónde vamos.
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist); //Añadir las cabeceras.
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);	//Función de callback para la respuesta del server.
		curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &respuesta);	//Datos que se pasan a la función de callback.

		this->res=curl_easy_perform(this->curl);	//Esto es lo que hace realmente la llamada.
	
		if(this->res != CURLE_OK)
		{
			return false;
		}
		else
		{
			curl_easy_cleanup(this->curl);
			if(this->con_post) curl_formfree(this->formpost);
			curl_slist_free_all(this->headerlist);
			return true;
		}
	}	
}
