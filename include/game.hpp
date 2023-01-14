#ifndef GAME_HPP
#define GAME_HPP

#include "common.hpp"
#include "SDL2/SDL.h"
#include <string>

class Cartridge;
class Cpu;
class Graphics;
class Controller;
class Label;

class Game{
	private:
	int counter = 0;
	char img[winX + 17];
	SDL_Rect rect = {posX, posY, winX * magni, winY * magni};
	
	Cartridge *cartridge;
	Cpu *cpu;
	Graphics *graphics;
	Controller *controller;
	Label *label;
	
	SDL_Texture *texture, *wholeTexture;
	SDL_Renderer *renderer;
	
	void Drow(int line);
	void Dummy(int dummy);
	void Sequence(void (Graphics::*Graphics)(char*), void (Game::*Drown)(int));
	
	public:
	Game(std::string romName, SDL_Renderer *renderer, SDL_Window *window);
	void Play();
	~Game();

};

#endif