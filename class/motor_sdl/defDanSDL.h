#ifndef DEFDANSDL_H
#define DEFDANSDL_H

SDL_PixelFormat * DLibV::Gestor_color::formato_pixeles=NULL;

DLibA::Controlador_audio_SDL * DLibA::Controlador_audio_SDL::instancia=NULL;

DLibA::Gestor_recursos_audio::Gestor_recursos_sonido DLibA::Gestor_recursos_audio::sonidos;
DLibA::Gestor_recursos_audio::Gestor_recursos_musica DLibA::Gestor_recursos_audio::musicas;

#endif
