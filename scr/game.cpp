#include "game.hpp"
#include "cartridge.hpp"
#include "cpu.hpp"
#include "widget.hpp"
#include "graphics.hpp"
#include "controller.hpp"
#include <iostream>

void Game::Dummy(int dummy){
	
}

Game::Game(std::string romName, SDL_Renderer *render, SDL_Window *window)
{
	renderer = render;
	
	cartridge = new Cartridge(romName);
	cpu = new Cpu(cartridge);
	graphics = new Graphics(cpu->vRam, cpu->ioReg, cpu->oam, cpu->palette);
	controller = new Controller(renderer, window);
	
	label = new Label(romName, renderer, winX * magni, posY >> 1);
	
	wholeTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, dispX, dispY);
	
	if (wholeTexture == NULL)
	{
		std::cout<< "SDL_CreateTexture Error" << std::endl;
		return;
	}
	SDL_SetRenderTarget(renderer, wholeTexture);
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	
	label->Drow(renderer, posX, posY >> 2);
	controller->Drow(renderer);
	SDL_SetRenderTarget(renderer, NULL);
	
	SDL_Delay(500);
	SDL_Event event;
	while( SDL_PollEvent(&event)){
	}

}

void Game::Play(){

	int skipFlg = 1;
		std::chrono::high_resolution_clock::time_point start, end;
	
	int quitFlg = 0;
	int timeStep = 0;
	
	SDL_Rect wholeRect = {0, 0, dispX, dispY};
	
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, winX, winY);
	
	if (texture == NULL)
	{
		std::cout<< "SDL_CreateTexture Error" << std::endl;
		return;
	}
	
	while(quitFlg == 0){
		
		while(!CheckBit(cpu->ioReg[0x40], 7) && (quitFlg != 0)){
			graphics->Mode0();
			cpu->ioReg[0x44] = 0x00;
			counter = 0;
			counter += cpu->HDMA();
			counter = cpu->Sequence();
			controller->Click(&quitFlg, cpu->key);
			timeStep = 0;
		}
		
		start = std::chrono::high_resolution_clock::now();
		controller->Click(&quitFlg, cpu->key);

		if (skipFlg != 0){
			Sequence(&Graphics::Dummy, &Game::Dummy);

		}else{
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, wholeTexture, NULL, &wholeRect);
			Sequence(&Graphics::Sequence, &Game::Drow);
			SDL_RenderPresent(renderer);
		}

		if ((quitFlg & 0x80) == 0x80){
			if (cpu->ioReg[0x50] != 0){
				cartridge->WriteSaveDate();
			}
			quitFlg &= 0x7f;
		}

		end = std::chrono::high_resolution_clock::now();
		auto millisec = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		timeStep += static_cast<int>(millisec);
		if (timeStep <= 16){
			SDL_Delay(16 - timeStep);
			timeStep = 0;
		}else{
			timeStep -= 16;
			timeStep &=0xff;
		}
		skipFlg = timeStep;
	}
}

void Game::Drow(int line){
	int bgR, bgG, bgB, num;

	num = static_cast<int>(cpu->ioReg[0x47] & 0x03) | 0x20;
	
	bgR = cpu->colorR[num];
	bgG = cpu->colorG[num];
	bgB = cpu->colorB[num];

	SDL_SetRenderDrawColor(renderer, bgR, bgG, bgB, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, 0, line, winX, line);
		
	for (int j = 0; j < winX; ++j){
		num = static_cast<int>(img[j + 8] & 0x3f);
		if ((cpu->colorR[num] != bgR) || (cpu->colorG[num] != bgG) || (cpu->colorB[num] != bgB)){
			SDL_SetRenderDrawColor(renderer, cpu->colorR[num], cpu->colorG[num], cpu->colorB[num], SDL_ALPHA_OPAQUE);
			SDL_RenderDrawPoint(renderer, j, line);
		}
	}
}

void Game::Sequence(void (Graphics::*Graphics)(char*), void (Game::*Drow)(int)){

	SDL_SetRenderTarget(renderer, texture);
	cpu->ioReg[0x44] = 0xff;
	graphics->windowCounter = 0;

	for(int i = 0; i < winY; ++i){
		graphics->NextLine(2);
//		(graphics->*Graphics)(img);
//		(this->*Drow)(i);
		while (counter < 80){
			counter += cpu->Sequence();
		}
		counter -= 80;
		
		(graphics->*Graphics)(img);
		(this->*Drow)(i);
		
		graphics->Mode3();
		while (counter < 172){
			counter += cpu->Sequence();
		}
		counter -= 172;
		
		graphics->Mode0();
//		counter += cpu->Sequence();
		counter += cpu->HDMA();
		while(counter < 204){
			counter += cpu->Sequence();
		}
		counter -= 204;

	}
	
	cpu->IF |= 0x01;
	for (int i = 0; i < 10; ++i){
		graphics->NextLine(1);
		while(counter < 456){
			counter += cpu->Sequence();
		}
		counter -= 456;
	}
	
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, texture, NULL, &rect);

}


Game::~Game(){
	delete cartridge;
	delete cpu;
	delete graphics;
	delete label;
}