#pragma once
#include <SDL.h>
#include "CharacterModel.h"

class Camera {
private:
	SDL_Rect cam;
	const int SCREEN_WIDTH;
	const int SCREEN_HEIGHT;
public:
	Camera(const int* width, const int* height):SCREEN_WIDTH(*width),SCREEN_HEIGHT(*height){
		cam = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	}

	void moveCam(CharacterModel* toFollow, const int* LEVEL_WIDTH, const int* LEVEL_HEIGHT) {
		cam.x = (toFollow->getPosX() + toFollow->getBounds()->w / 2) - SCREEN_WIDTH / 2;
		cam.y = (toFollow->getPosY() + toFollow->getBounds()->h / 2) - SCREEN_HEIGHT / 2;
		checkPos(LEVEL_WIDTH, LEVEL_HEIGHT);
	}

	void checkPos(const int* LEVEL_WIDTH, const int* LEVEL_HEIGHT) {
		//Keep the camera in bounds
		if (cam.x < 0) {
			cam.x = 0;
		}
		if (cam.y < 0) {
			cam.y = 0;
		}
		if (cam.x > *LEVEL_WIDTH - cam.w) {
			cam.x = *LEVEL_WIDTH - cam.w;
		}
		if (cam.y > *LEVEL_HEIGHT - cam.h) {
			cam.y = *LEVEL_HEIGHT - cam.h;
		}
	}

	SDL_Rect getRect() {
		return cam;
	}
};