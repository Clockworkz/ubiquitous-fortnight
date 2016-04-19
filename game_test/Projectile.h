#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <vector>

#include "LTexture.h"

class Projectile {
public:
	Projectile(std::vector<int> posVec, std::vector<int> accelVec) : posX(posVec[0]), posY(posVec[1]), velocX(accelVec[0]), velocY(accelVec[1]) {};

	void accelerate() {
		velocX += accelX;
		velocY += accelY;
		//if (accelY != GRAV_ACCEL) {
		//	accelY = GRAV_ACCEL;
		//}
	}

	void move() {
		posX += velocX;
		posY += accelY;
	}

private:
	int posX;
	int posY;
	int accelX;
	int accelY;
	int velocX = 0;
	int velocY = 0;
	const int GRAV_ACCEL = 1;

	SDL_RendererFlip flip = SDL_FLIP_NONE;
	SDL_Rect bounds;


	
};