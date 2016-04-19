#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <vector>

#include "LTexture.h"

class CharacterModel {
public:
	CharacterModel(std::string path, int cols, int rows, int still) : TEX_COLS(cols), TEX_ROWS(rows), STILL_FRAME(still), TEX_PATH(path) {};

	bool loadMedia(SDL_Renderer* gRenderer) {
		//Loading success flag
		bool success = true;

		//Load Foo' texture
		if (!mTexture.loadFromFile(TEX_PATH, gRenderer)) {
			std::cout << "Failed to load texture image!" << std::endl;
			success = false;
		}
		else {
			bounds.w = mTexture.getWidth() / TEX_COLS;
			bounds.h = mTexture.getHeight() / TEX_ROWS;
			mTexture.setClipRects(bounds.w, bounds.h, TEX_ROWS, TEX_COLS);
		}
		return success;
	}

	void runAnim(int x, int y, int& frame, const int& FPU, SDL_Renderer* gRenderer) {
		if (frame > RUN_END*FPU) {
			frame = RUN_START*FPU;
		}
		mTexture.renderAnim(x, y, frame / FPU, gRenderer, 0.0, NULL, flip);
	}

	bool attackAnim(int x, int y, int& frame, const int& FPU, SDL_Renderer* gRenderer) {
		if (frame > ATTACK_END*FPU) {
			frame = ATTACK_START*FPU;
		}
		if (frame < ATTACK_START*FPU) {
			frame = ATTACK_START*FPU;
		}
		if (frame < ATTACK_END*FPU) {
			mTexture.renderAnim(x, y, frame / FPU, gRenderer, 0.0, NULL, flip);
			attacking = true;
		}
		else {
			attacking = false;
		}
		return attacking;
	}

	bool shootAnim(int x, int y, int& frame, const int& FPU, SDL_Renderer* gRenderer) {
		if (frame > SHOOT_END*FPU) {
			frame = SHOOT_START*FPU;
		}
		if (frame < SHOOT_START*FPU) {
			frame = SHOOT_START*FPU;
		}
		if (frame < SHOOT_END*FPU) {
			mTexture.renderAnim(x, y, frame / FPU, gRenderer, 0.0, NULL, flip);
			shooting = true;
		}
		else {
			shooting = false;
		}
		return shooting;
	}

	bool fallingAnim(int x, int y, SDL_Renderer* gRenderer) {
		if (bounds.y < yMax) {
			std::cout << bounds.y << std::endl;
			std::cout << yMax << std::endl;
			falling = true;
			mTexture.renderAnim(x, y, JUMP_END-1, gRenderer, 0.0, NULL, flip);
		}
		else {
			falling = false;
		}
		return falling;
	}

	std::vector<bool> jumpAnim(int x, int y, int& frame, const int& FPU, SDL_Renderer* gRenderer) {
		if (attacking == false) {
			if (frame > JUMP_END*FPU) {
				frame = JUMP_START*FPU;
			}
			if (frame < JUMP_START*FPU) {
				frame = JUMP_START*FPU;
			}
			if (frame < JUMP_END*FPU) {
				mTexture.renderAnim(x, y, frame / FPU, gRenderer, 0.0, NULL, flip);
				jumping = true;
			}
			//if (bounds.y < yMax)
			else {
				//setAcceleration(0, GRAV_ACCEL);
				//setVelocity(0, 0);
				//std::cout << Y_BASE << std::endl;
				//bounds.y = yMax;
				jumping = false;
				falling = true;
			}
			return std::vector<bool> {jumping, falling};
		}
	}

	void idleAnim(int x, int y, SDL_Renderer* gRenderer) {
		mTexture.renderAnim(x, y, STILL_FRAME, gRenderer, 0.0, NULL, flip);
	}

	void setRunFrames(int startFrame, int endFrame) {
		RUN_START = startFrame;
		RUN_END = endFrame;
	}

	void setJumpFrames(int startFrame, int endFrame) {
		JUMP_START = startFrame;
		JUMP_END = endFrame;
	}

	void setAttackFrames(int startFrame, int endFrame) {
		ATTACK_START = startFrame;
		ATTACK_END = endFrame;
	}

	void setShootFrames(int startFrame, int endFrame) {
		SHOOT_START = startFrame;
		SHOOT_END = endFrame;
	}

	void move() {
		accelerate();
		bounds.x += velocX;
		bounds.y += velocY;
		if (bounds.y > yMax) {
			bounds.y = yMax;
		}
		if (accelY < GRAV_ACCEL) {
			accelY = GRAV_ACCEL;
		}
	}

	SDL_Rect* getBounds() {
		return &bounds;
	}

	void setVelocity(int x, int y) {
		velocX = x;
		velocY = y;
	}

	std::vector<int> getVelocVec() {
		return std::vector<int> {velocX, velocY};
	}

	void accelerate() {
		velocX += accelX;
		if (bounds.y < yMax + bounds.h ) {
			velocY += accelY;
		}
	}

	void setAcceleration(int x, int y) {
		accelX = x;
		accelY = y;
	}

	int getPosX() {
		return bounds.x;
	}

	int getPosY() {
		return bounds.y;
	}

	void setPosX(int x) {
		bounds.x = x;
	}

	void setPosY(int y) {
		if (y == 0) {
			bounds.y = Y_BASE;
		}
		else {
			bounds.y = y;
		}
	}

	void setYMax(int y) {
		if (y == -1) {
			yMax = Y_BASE;
		}
		else {
			yMax = y;
		}
	}

	void setFlip(SDL_RendererFlip flipTo) {
		flip = flipTo;
	}

private:
	const int TEX_ROWS;
	const int TEX_COLS;
	const int STILL_FRAME;
	const std::string TEX_PATH;
	const int GRAV_ACCEL = 1;
	int Y_BASE = 720;
	int X_BASE = 640;

	LTexture mTexture;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	int RUN_START = NULL;
	int RUN_END = NULL;
	int JUMP_END = NULL;
	int JUMP_START = NULL;
	int ATTACK_START = NULL;
	int ATTACK_END = NULL;
	int SHOOT_START = NULL;
	int SHOOT_END = NULL;

	int accelY = GRAV_ACCEL;
	int velocY;
	int yMax = Y_BASE;
	int yMin = 0;

	int accelX;
	int velocX;

	SDL_Rect bounds = {0,0};


	bool attacking = NULL;
	bool jumping = NULL;
	bool shooting = NULL;
	bool falling = NULL;
};