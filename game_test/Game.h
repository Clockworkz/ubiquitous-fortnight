#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>

#include "LTexture.h"
#include "CharacterModel.h"
#include "Camera.h"

class Game {
public:
	Game () {
		// initialise sdl
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		}
		else { // create window
			window = SDL_CreateWindow("Test window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
			if (window == nullptr) {
				std::cout << "Failed to initialise window." << std::endl;
			}
			else {
				//Create renderer for window
				renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
				if (renderer == nullptr)
				{
					std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
				}
				else
				{
					//Initialize renderer color
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					// initialise png loader
					if (!(IMG_Init(imgFlags) & imgFlags)) {
						std::cout << "Coudlnt initialise SDL_image" << IMG_GetError() << imgFlags << std::endl;
					}
					// create surface
					else {
						screenSurface = SDL_GetWindowSurface(window);
						if (screenSurface == nullptr) {
							std::cout << "Failed to initialise surface." << std::endl;
						}
					}
				}
			}
		}
	}

	bool checkCollision2(SDL_Rect* A, std::vector<SDL_Rect*> rects) {
		SDL_Rect* B;
		int leftA, leftB;
		int rightA, rightB;
		int topA, topB;
		int bottomA, bottomB;
		bool collision = true;
		for (auto n : rects) {
			B = n;
			leftA = A->x;
			rightA = A->x + A->w;
			topA = A->y;
			bottomA = A->y + A->h;

			//Calculate the sides of rect B
			leftB = B->x;
			rightB = B->x + B->w;
			topB = B->y;
			bottomB = B->y + B->h;

			//If any of the sides from A are outside of B
			if (bottomA <= topB)
			{
				collision = false;
			}

			if (topA >= bottomB)
			{
				collision = false;
			}

			if (rightA <= leftB)
			{
				collision = false;
			}

			if (leftA >= rightB)
			{
				collision = false;
			}
		}
		return collision;
	}

	bool checkCollision(SDL_Rect* rect1, std::vector<int> velocVec1, SDL_Rect* rect2, std::vector<int> velocVec2) {
		
		// Be able to determine which axis the collision is on, if on Y, stop Y movement, if on X stop X movement etc.
		SDL_Rect collision;
		int leftA, leftB;
		int rightA, rightB;
		int topA, topB;
		int bottomA, bottomB;

		SDL_Rect A = *rect1;
		A.x += velocVec1[0];
		A.y += velocVec1[1];
		SDL_Rect B = *rect2;
		B.x += velocVec2[0];
		B.y += velocVec2[1];

		leftA = A.x;
		rightA = A.x + A.w;
		topA = A.y;
		bottomA = A.y + A.h;

		//Calculate the sides of rect B
		leftB = B.x;
		rightB = B.x + B.w;
		topB = B.y;
		bottomB = B.y + B.h;

		//If any of the sides from A are outside of B
		if (bottomA <= topB)
		{
			return false;
		}

		if (topA >= bottomB)
		{
			return false;
		}

		if (rightA <= leftB)
		{
			return false;
		}

		if (leftA >= rightB)
		{
			return false;
		}

		//If none of the sides from A are outside B
		return true;


		//if (SDL_IntersectRect(&rectA, &rectB, &collision) == SDL_TRUE) {
		//	std::cout << collision.x <<" "<< collision.y << std::endl;
		//	return true;
		//}
		//return false;
	}

	void processCollision(CharacterModel model1, CharacterModel model2, Camera camera1) {
		// use if platform
		if (model1.getBounds()->y + model1.getBounds()->h <= model2.getBounds()->y + model2.getBounds()->h) {
			model1.setVelocity(model1.getVelocVec()[0], 0);
			model1.setAcceleration(0, 0);
			model1.setPosY(model2.getBounds()->y - model1.getBounds()->h - camera1.getRect().y);
			model1.setYMax(model2.getBounds()->y - model1.getBounds()->h - camera1.getRect().y);
		}
			// use if wall
		else if (model1.getBounds()->x + model1.getBounds()->w >= model2.getBounds()->x) {
			model1.setVelocity(0,model1.getVelocVec()[1]);
			model1.setPosX(model2.getBounds()->x - camera1.getRect().y - model1.getBounds()->w);
			model1.setAcceleration(0, 0);
			}
		}

	void processCollision(CharacterModel model1, SDL_Rect rect1, Camera camera1) {
		// use if platform
		if (model1.getBounds()->y + model1.getBounds()->h <= rect1.y + rect1.h) {
			model1.setVelocity(model1.getVelocVec()[0], 0);
			model1.setAcceleration(0, 0);
			model1.setPosY(rect1.y - model1.getBounds()->h - camera1.getRect().y);
			model1.setYMax(rect1.y - model1.getBounds()->h - camera1.getRect().y);
			// use if wall
			/*	else if (playerChar.getBounds()->x + playerChar.getBounds()->w >= testRect.x) {
			playerChar.setVelocity(0,playerChar.getVelocVec()[1]);
			playerChar.setPosX(testRect.x - camera.getRect().y - playerChar.getBounds()->w);
			playerChar.setAcceleration(0, 0);
			}*/
		}
	}

	void setFrames() {
		playerChar.setJumpFrames(41, 48);
		playerChar.setRunFrames(4, 11);
		playerChar.setAttackFrames(12, 16);
		playerChar.setShootFrames(28,33);

		skeleChar.setJumpFrames(17,19);
		skeleChar.setRunFrames(1,6);
	}

	void handleInput(SDL_Event* e) {

		if (e->type == SDL_QUIT) {
			quit = true;
		}

		// abstract non-quit key actions to key states instead of reading PollEvent (saves needing both key_down and key_up switch)
		else if (e->type == SDL_KEYDOWN) {
			switch (e->key.keysym.sym) {
			case SDLK_ESCAPE:
				quit = true;
				break;
			case SDLK_RIGHT:
				walk = true;
				flip = SDL_FLIP_NONE;
				break;
			case SDLK_LEFT:
				walk = true;
				flip = SDL_FLIP_HORIZONTAL;
				break;
			case SDLK_f:
				if (attack == false) {
					attack = true;
				}
				break;
			case SDLK_r:
				if (shoot == false) {
					shoot = true;
				}
				break;
			case SDLK_SPACE:
				if (jump == false & fall == false) {
					jump = true;
					if (flip == SDL_FLIP_HORIZONTAL) {
						playerChar.setVelocity(playerChar.getVelocVec()[0], -5);
						playerChar.setAcceleration(0, -10);
					}
					else {
						playerChar.setVelocity(playerChar.getVelocVec()[0], -5);
						playerChar.setAcceleration(0, -10);
					}
				}
				break;
			}
		}
		else if (e->type == SDL_KEYUP) {
			switch (e->key.keysym.sym) {
			case SDLK_RIGHT:
				walk = false;
				break;
			case SDLK_LEFT:
				walk = false;
				break;
			}
		}
		playerChar.setFlip(flip);
	}

	void processInput(int* frame) {
		// abstract these to function
		int xPos = playerChar.getPosX() - camera.getRect().x;
		int yPos = playerChar.getPosY() - camera.getRect().y;
		
		if (shoot == true) {
			jump = false;
			attack = false;
			shoot = playerChar.shootAnim(xPos, yPos, *frame, FRAMES_PER_UPDATE, renderer);
		}
		else if (attack == true) {
			jump = false;
			attack = playerChar.attackAnim(xPos, yPos, *frame, FRAMES_PER_UPDATE, renderer);
		}
		else if (fall == true) {
			fall = playerChar.fallingAnim(xPos, yPos, renderer);
		}
		else if (jump == true) {
			temp = playerChar.jumpAnim(xPos, yPos, *frame, FRAMES_PER_UPDATE, renderer);
			jump = temp[0];
			fall = temp[1];
		}
		else if (walk == true) {
			if (flip == SDL_FLIP_HORIZONTAL) {
				playerChar.setVelocity(-5, playerChar.getVelocVec()[1]);
			}
			else {
				playerChar.setVelocity(5, playerChar.getVelocVec()[1]);
			}
			playerChar.runAnim(xPos, yPos, *frame, FRAMES_PER_UPDATE, renderer);
		}
		else{ //(jump == false && walk == false && attack == false) {
			playerChar.setVelocity(0, playerChar.getVelocVec()[1]);
			playerChar.idleAnim(xPos, yPos, renderer);
		}
	}

	void close() {
		SDL_FreeSurface(screenSurface);
		screenSurface = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;
		//mTexture.free();

		SDL_Quit();
		IMG_Quit();
	}

	int run() {
		SDL_Event e;
		playerChar.loadMedia(renderer);
		skeleChar.loadMedia(renderer);
		skeleChar.setPosX(0);
		skeleChar.setVelocity(2, 0);
		bgTexture.loadFromFile("Sprites/intro-background-sheet.png", renderer);
		SDL_Rect testRect = { 500,500,100,10 };
		SDL_Rect testRect2;
		SDL_Rect baseLine = { 0,0,LEVEL_WIDTH,10 };

		setFrames();
		// game loop
		int frame = 0;
		int frame2 = 0;
		while (quit == false) {
			//SDL_UpdateWindowSurface(window);
			while (SDL_PollEvent(&e) != 0) {
				handleInput(&e);
			}
			SDL_RenderClear(renderer);
			camera.moveCam(&playerChar, &LEVEL_WIDTH, &LEVEL_HEIGHT);


			// Create level builder class/function
			testRect.x = 500 - camera.getRect().x;
			testRect.y = 550 - camera.getRect().y;
			testRect2.x = playerChar.getPosX()-camera.getRect().x;
			testRect2.h = playerChar.getBounds()->h;
			testRect2.w = playerChar.getBounds()->w;
			testRect2.y = playerChar.getPosY()-camera.getRect().y;
			baseLine.x = 0 - camera.getRect().x;
			baseLine.y = 609 - camera.getRect().y;
			baseLine.w = LEVEL_WIDTH + camera.getRect().w;

			bgTexture.render(0, 0, &camera.getRect(), renderer);
			SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
			SDL_RenderDrawRect(renderer, &testRect);
			SDL_RenderDrawRect(renderer, &testRect2);
			SDL_RenderDrawRect(renderer, &baseLine);

			// Move collision checks to a handler, maintain a vector of collision objects
			if (checkCollision2(playerChar.getBounds(), std::vector<SDL_Rect*>{skeleChar.getBounds(), &testRect}) == false) {
				std::cout << "no collision" << std::endl;
				playerChar.setYMax(baseLine.y - playerChar.getBounds()->h);
			}

			if (checkCollision(playerChar.getBounds(), playerChar.getVelocVec(), skeleChar.getBounds(), std::vector<int> {0, 0})) {
				playerChar.setVelocity(0, 0);
				playerChar.setAcceleration(0, 0);
				skeleChar.setVelocity(0, 0);
				playerChar.setPosY(skeleChar.getBounds()->y - playerChar.getBounds()->h - camera.getRect().y - 2);
				playerChar.setYMax(skeleChar.getBounds()->y - playerChar.getBounds()->h - camera.getRect().y - 2);
			}

			if (checkCollision(playerChar.getBounds(), playerChar.getVelocVec(), &testRect, std::vector<int> {0, 0})) {
				std::cout << "Collision on platform" << std::endl;
				std::cout << "Base Y: " << playerChar.getPosY() << std::endl;
				// use if platform
				if (playerChar.getBounds()->y + playerChar.getBounds()->h <= testRect.y + testRect.h) {
					playerChar.setVelocity(playerChar.getVelocVec()[0], 0);
					playerChar.setAcceleration(NULL, 0);
					playerChar.setPosY(testRect.y - playerChar.getBounds()->h - camera.getRect().y);
					playerChar.setYMax(testRect.y - playerChar.getBounds()->h - camera.getRect().y);
				}
				// use if wall
			/*	else if (playerChar.getBounds()->x + playerChar.getBounds()->w >= testRect.x) {
					playerChar.setVelocity(0,playerChar.getVelocVec()[1]);
					playerChar.setPosX(testRect.x - camera.getRect().y - playerChar.getBounds()->w);
					playerChar.setAcceleration(0, 0);
				}*/

			}
			if (checkCollision(playerChar.getBounds(), playerChar.getVelocVec(), &baseLine, std::vector<int> {0, 0})) {
				std::cout << "Collision on base" << std::endl;
				std::cout << "Base Y: " << baseLine.y << std::endl;
				playerChar.setVelocity(0, 0);
				playerChar.setAcceleration(0, 0);
				playerChar.setPosY(baseLine.y - playerChar.getBounds()->h - camera.getRect().y );
				playerChar.setYMax(baseLine.y - playerChar.getBounds()->h - camera.getRect().y );
			}
			if (checkCollision(skeleChar.getBounds(), skeleChar.getVelocVec(), &baseLine, std::vector<int> {0, 0})) {
				std::cout << "Collision on base" << std::endl;
				std::cout << "Base Y: " << baseLine.y << std::endl;
				skeleChar.setVelocity(0, 0);
				skeleChar.setAcceleration(0, 0);
				skeleChar.setPosY(baseLine.y - skeleChar.getBounds()->h - camera.getRect().y);
				skeleChar.setYMax(baseLine.y - skeleChar.getBounds()->h - camera.getRect().y);
			}


			skeleChar.runAnim(skeleChar.getPosX() - camera.getRect().x, skeleChar.getPosY() - camera.getRect().y,frame2,FRAMES_PER_UPDATE*2, renderer);
			
			skeleChar.move();
			if (skeleChar.getPosX() + skeleChar.getBounds()->w >= LEVEL_WIDTH) {
				skeleChar.setVelocity(-2, 0);
				skeleChar.setFlip(SDL_FLIP_HORIZONTAL);
				skeleChar.move();
			}
			else if (skeleChar.getPosX() <= 0) {
				skeleChar.setVelocity(2, 0);
				skeleChar.setFlip(SDL_FLIP_NONE);
				skeleChar.move();
			}
			
			// abstract these checks to a handler
			processInput(&frame);
			playerChar.move();

			if (playerChar.getPosX()+ playerChar.getBounds()->w >= LEVEL_WIDTH) {
				playerChar.setVelocity(-5, playerChar.getVelocVec()[1]);
				playerChar.move();
			}
			else if (playerChar.getPosX() <= 0) {
				playerChar.setVelocity(5, playerChar.getVelocVec()[1]);
				playerChar.move();
			}
			SDL_RenderPresent(renderer);
			frame++;
			frame2++;
		}
		close();
		return 0;
	}

private:
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;

	const int LEVEL_WIDTH = 2560;
	const int LEVEL_HEIGHT = 720;

	const int FRAMES_PER_UPDATE = 4;

	SDL_Window* window = nullptr;
	SDL_Surface* screenSurface = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	Camera camera = { &SCREEN_WIDTH, &SCREEN_HEIGHT };

	CharacterModel playerChar{ "Sprites/platformer_sprites_pixelized2.png", 8, 9, 64 };
	CharacterModel skeleChar{ "Sprites/bloodSkeletonBase.png",10,6,1 };

	LTexture bgTexture;
	int imgFlags = IMG_INIT_JPG|IMG_INIT_PNG;

	bool quit = false;
	bool walk = false;
	bool jump = false;
	bool fall = false;
	bool attack = false;
	bool shoot = false;
	std::vector<bool> temp;
};