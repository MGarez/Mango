#pragma once


#include "core.h"
#include <string>
#include <SDL.h>

struct SpriteComponent
{
	SpriteComponent(std::string assetId = "", int width = 0, int height = 0, int zindex = 0, int srcRectX = 0, int srcRectY = 0)
	{
		this->assetId = assetId;
		this->width = width;
		this->height = height;
		this->zindex = zindex;
		this->srcRest = SDL_Rect{ srcRectX, srcRectY, width, height };
	};

	std::string assetId;
	int width;
	int height;
	int zindex;
	SDL_Rect srcRest;

};

