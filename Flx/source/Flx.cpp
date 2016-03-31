#include "Flx.h"

#include <iostream>
#include <iomanip>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_net.h>

Flx :: Flx(void){
	main();
}

void Flx :: main(void){
	Init_Subsistemas();

	video();
	image();
	mixer();

	gameloop();	
}

void Flx :: Init_Subsistemas(void){
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
		cerr << "No se pudo iniciar SDL: " << SDL_GetError() << endl;
		exit(1);
	}

	atexit(SDL_Quit);
}

void Flx :: mixer(void){
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) < 0){
		cerr << "Subsistema de audio no disponible" << endl;
		exit(1);
	}

	atexit(Mix_CloseAudio);

	bso = Mix_LoadMUS("../resources/soundtrack/omt.wav");

	if(bso == NULL){
		cerr << "No se puede cargar el fichero 'omt.wav': ";
		cerr << Mix_GetError() << endl;
		exit(1);
	}

	volumen = 100;

	/*cout << "\nPusa ESC para salir\n" << endl;
	cout << "Pulse 'Q' para subir el volumen" << endl;
	cout << "Pulse 'A' para bajar el volumen" << endl;
	cout << "Pulse 'I' para reproducir la musica" << endl;
	cout << "Pulse 'P' para pausar la reproduccion" << endl;
	cout << "Pulse 'R' para reproducir la musica debsoues del pause" << endl;
	cout << "Pulse 'W' para poner la música al inicio" << endl;
	cout << "Pulse 'H' para mostrar la ayuda" << endl;*/
	
	Mix_PlayMusic(bso, -1);
}

void Flx :: video(void){
	if(SDL_VideoModeOK(640, 480, 24, SDL_HWSURFACE|SDL_DOUBLEBUF) == 0){
		cerr << "Modo no soportado: " << SDL_GetError() << endl;
		exit(1);
	}

	SDL_WM_SetCaption("Eterno resplandor de una mente inmaculada: Flx, el maldito", NULL);

	pantalla = SDL_SetVideoMode(640, 480, 24, SDL_HWSURFACE|SDL_DOUBLEBUF);

	if(pantalla == NULL){
		cerr << "No se puedo establecer el modo de video: "
		 << SDL_GetError();
		exit(1);
	}
}

void Flx :: image(void){
	nave = SDL_LoadBMP("../resources/images/nave.bmp");
	
	if(nave == NULL){
		cerr << "Error al cargar la nave: " << SDL_GetError() << endl;
		exit(1);
	}

	posicion.x = 150;
	posicion.y = 120;
	posicion.w = nave -> w;
	posicion.h = nave -> h;

	SDL_BlitSurface(nave, NULL, pantalla, &posicion);
	SDL_Flip(pantalla);
}

int Flx :: gameloop(void){
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	for(;;){
		while(SDL_PollEvent(&evento)){
			if(evento.type == SDL_KEYDOWN){
				switch(evento.key.keysym.sym){
				//case para mixer
				case SDLK_q:
					volumen += 1;
					if(volumen < 128)
						Mix_VolumeMusic(volumen);
					else
						volumen = 128;
						
					cout << "Volumen actual: " << volumen << endl;
					break;
				case SDLK_a:
					volumen -= 1;
					
					if(volumen > -1)
						Mix_VolumeMusic(volumen);
					else
						volumen = 0;
						
					cout << "Volumen actual: " << volumen << endl;
					break;
				
				case SDLK_i:
					Mix_PlayMusic(bso, -1);
					cout << "Musica iniciada" << endl;
					break;

				case SDLK_p:
					if(Mix_PlayingMusic() == 1){
						Mix_PauseMusic();

						cout << "Musica en pausa" << endl;
					}
					else{
						cout<< "La musica no esta en reproduccion" << endl;
					}					
					break;	
				
				case SDLK_r:
					if(Mix_PausedMusic() == 1){
						Mix_ResumeMusic();

						cout << "Musica en reproduccion" << endl;
					}
					else{
						cout<< "La musica ya esta en reproduccion" << endl;
					}	
					break;
				
				case SDLK_w:
					Mix_RewindMusic();
					cout << "Musica al principio" << endl;
					break;

				case SDLK_h:
					cout << "\nPusa ESC para salir\n" << endl;
					cout << "Pulse 'Q' para subir el volumen" << endl;
					cout << "Pulse 'A' para bajar el volumen" << endl;
					cout << "Pulse 'I' para reproducir la musica" << endl;
					cout << "Pulse 'P' para pausar la reproduccion" << endl;
					cout << "Pulse 'R' para reproducir la musica debsoues del pause" << endl;
					cout << "Pulse 'W' para poner la música al inicio" << endl;
					cout << "Pulse 'H' para mostrar la ayuda" << endl;
					break;

				//case para movimiento
				case SDLK_UP:
					posicion.y -= 4;
					
					if(posicion.y < 0)
						posicion.y = 0;
					break;

				case SDLK_DOWN:
					posicion.y += 4;
					
					if(posicion.y > 444)
						posicion.y = 444;
					break;

				case SDLK_RIGHT:
					posicion.x += 4;
					
					if(posicion.x > 610)
						posicion.x = 610;
					break;

				case SDLK_LEFT:
					posicion.x -= 4;
					
					if(posicion.x < 0)
						posicion.x = 0;
					break;

				case SDLK_ESCAPE:
					SDL_FreeSurface(nave);
					Mix_FreeMusic(bso);
					
					atexit(Mix_CloseAudio);
					
					cout << "Gracias" << endl;

					return 0;

				case SDLK_f:
					SDL_WM_ToggleFullScreen(pantalla);
					break;

				default:
					cout << "Ha pulsado otra tecla" << endl;
				}
				
				SDL_FillRect(pantalla, NULL, 0);
				
				SDL_BlitSurface(nave, NULL, pantalla, &posicion);
				
				SDL_Flip(pantalla);
				
				cout << " Valor x: " << setw(3) << posicion.x 
				     << " Valor y: " << setw(3) << posicion.y << endl;
				}
			}
		}
	}