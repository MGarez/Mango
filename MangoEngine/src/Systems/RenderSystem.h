#pragma once
#include "core.h"
#include <memory>

class RenderSystem: public System
{
public:

	RenderSystem();
	void Update(struct SDL_Renderer* renderer, std::unique_ptr<class AssetStore>& assetStore);
};






// #nota Checkear vcpkg
// #nota Checkear build system

