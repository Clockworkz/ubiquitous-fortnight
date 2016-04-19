#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <vector>

class LTexture{
public:
	LTexture() {
		//Initialize
		mTexture = nullptr;
		mWidth = 0;
		mHeight = 0;
	}

	~LTexture() {
		//Deallocate
		free();
	}

	bool loadFromFile(std::string path, SDL_Renderer*& gRenderer) {
		//Get rid of preexisting texture
		free();
		//The final texture
		SDL_Surface* tempSurface = nullptr;
		SDL_Texture* newTexture = nullptr;
		bool success = true;

		if (path.find(".bmp") != std::string::npos) {
			tempSurface = SDL_LoadBMP(path.c_str());
		}
		else {
			tempSurface = IMG_Load(path.c_str());
		}
		if (tempSurface == nullptr) {
			std::cout << "Couldnt load image." << SDL_GetError() << std::endl;
			success = false;
		}
		else {
			//Color key image
			SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 0xFF, 0xFF, 0xFF));

			//Create texture from surface pixels
			newTexture = SDL_CreateTextureFromSurface(gRenderer, tempSurface);
			if (newTexture == nullptr) {
				std::cout << "Unable to create texture from " << path.c_str() << "SDL_Error: " << SDL_GetError() << std::endl;
			}
			else {
				//Get image dimensions
				mWidth = tempSurface->w;
				mHeight = tempSurface->h;
			}

			//Get rid of old loaded surface
			SDL_FreeSurface(tempSurface);
		}

		//Return success
		mTexture = newTexture;
		return mTexture != nullptr;
	}

	void free() {
		//Free texture if it exists
		if (mTexture != NULL)
		{
			SDL_DestroyTexture(mTexture);
			mTexture = NULL;
			mWidth = 0;
			mHeight = 0;
		}
	}

	bool setClipRects(int width, int height, int rows, int cols) {
		bool success = true;
		int ySplit = 0;
		for (int i = 0; i < rows; i++) {
			int xSplit = 0;
			for (int j = 0; j < cols; j++) {
				SDL_Rect tempRect;
				tempRect.h = height;
				tempRect.w = width;
				tempRect.x = xSplit;
				tempRect.y = ySplit;
				clipRects.push_back(tempRect);
				xSplit += width;
			}
			ySplit += height;
		}
		if (clipRects.size() != rows*cols) {
			success = false;
			std::cout << "Failed to initialised list of clip rects:" << clipRects.size() << std::endl;
		}
		return success;
	}

	void render(int x, int y, SDL_Rect* clip, SDL_Renderer*& gRenderer) {
		//Set rendering space and render to screen
		SDL_Rect renderQuad = { x, y, mWidth, mHeight };
		if (clip != NULL) {
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}

		SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
	}

	void renderAnim(int x, int y, int frameNum, SDL_Renderer*& gRenderer, double angle, SDL_Point* centre, SDL_RendererFlip flip) {
		//Set rendering space and render to screen
		SDL_Rect renderQuad = { x, y, mWidth, mHeight };
		SDL_Rect* clip;
		if (clipRects.empty()) {
			clip = NULL;
		}
		else if (frameNum > clipRects.size()) {
			std::cout << "Frame does not exist" << std::endl;
			clip = NULL;
		}
		else {
			clip = &clipRects[frameNum];
			if (clip != NULL) {
				renderQuad.w = clip->w;
				renderQuad.h = clip->h;
			}
			SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, centre, flip);
		}
	}

	int getWidth() {
		return mWidth;
	}

	int getHeight() {
		return mHeight;
	}
private:
	//The actual hardware texture
	SDL_Texture* mTexture = nullptr;

	//Image dimensions
	int mWidth = 0;
	int mHeight = 0;

	//Set clip rect list
	std::vector<SDL_Rect> clipRects;
};