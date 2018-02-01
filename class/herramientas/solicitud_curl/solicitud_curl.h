#ifndef SOLICITUD_CURL_H
#define SOLICITUD_CURL_H

#include <string>
#include <curl/curl.h>
#include "../../motor_sdl/herramientas/herramientas/herramientas.h"

class Solicitud_CURL
{
	private:

	static const char CURL_EXPECT[];

	CURL *curl;
	CURLcode res;

	struct curl_httppost *formpost;
	struct curl_httppost *lastptr;
	struct curl_slist *headerlist;

	std::string url;
	std::string respuesta;

	bool con_post;

	public:

	Solicitud_CURL();
	~Solicitud_CURL();

	bool inicializar(std::string const&);
	void post(std::string const&, std::string const&);
	void post(std::string const&, int);

	bool lanzar();
	static size_t callback(void *, size_t , size_t , void *);
	std::string acc_respuesta() const {return this->respuesta;}
};

#endif
