#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "common.hpp"
#include "SDL2/SDL.h"
#include <string>

class Button;

struct fingerID{
	SDL_FingerID id;
	char key;
};

class Controller{
	private:
	SDL_Event event;
	SDL_Window *gameWindow;
	
	struct fingerID fingerID[8];
	
	
	public:
	Button *button[8];
	Button *exit, *saveANDexit, *save;
	
	Controller(SDL_Renderer *renderer, SDL_Window *window);
	void Drow(SDL_Renderer *renerer);
	void Click(int *quitFlg, char *key);
	
	
	
};


#endif