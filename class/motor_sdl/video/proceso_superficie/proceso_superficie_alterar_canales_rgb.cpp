#include "proceso_superficie_alterar_canales_rgb.h"

using namespace DLibV;

Proceso_superficie_alterar_canales_rgb::Proceso_superficie_alterar_canales_rgb(int cr, int cg, int cb, bool p_trans)
	:Proceso_superficie_base_por_pixel(),
	cambio_r(cr), cambio_g(cg), cambio_b(cb), respetar_transparencia(p_trans)
{

}

bool Proceso_superficie_alterar_canales_rgb::procesar_pixel(Uint32 original, Uint32 &resultado, Uint32 trans)
{
	if(original==trans && this->respetar_transparencia)
	{
		return false;
	}
	else
	{
		auto cambio=[](int color, int cambio) -> short int
		{
			color+=cambio;
			if(color <= 0) return 0;
			else if(color >= 255) return 255;
			else return color;
		};

		SDL_GetRGBA(original, this->acc_formato(), &r, &g, &b, &a);

		tr=cambio(r, cambio_r);
		tg=cambio(g, cambio_g);
		tb=cambio(b, cambio_b);	

		resultado=DLibV::Gestor_color::color(tr, tg, tb);

		return true;
	}
}

Proceso_superficie_alterar_canales_rgb::~Proceso_superficie_alterar_canales_rgb()
{

}

Proceso_superficie_alterar_canales_rgb * Proceso_superficie_alterar_canales_rgb::generar(int pr, int pg, int pb, bool p_trans)
{
	Proceso_superficie_alterar_canales_rgb * temp=NULL;
	temp=new Proceso_superficie_alterar_canales_rgb(pr, pg, pb, p_trans);
	return temp;
}
