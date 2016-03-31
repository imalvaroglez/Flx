#ifndef _FLX_H_
#define _FLX_H_

#include <iostream>
#include <iomanip>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_net.h>

using namespace std;

class Flx{
private:
	//mixer
	Mix_Music *bso;
	int volumen;

	//main
	SDL_Event evento;

	//video
	SDL_Surface *pantalla;

	//image
	SDL_Surface *nave;
	SDL_Rect posicion;
public:
	Flx();

	void main(void);

	//métodos inicializadores
	void Init_Subsistemas(void);

	void mixer(void);
	void video(void);
	void image(void);

	//juego
	int gameloop(void);
};

#endif