#pragma once
#include <cstdint>
#include <memory>
#include "core.h"
#include "AssetStore/AssetStore.h"

static constexpr uint32_t FPS = 60;
static constexpr double MS_PER_FRAME = 1000 / FPS;

class Game
{
public:
	Game();
	~Game();
	void Initialize();
	void Run();
	void LoadLevel(int level);
	void Setup();
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();

public:
	uint32_t m_windowWidth;
	uint32_t m_windowHeight;
private:

	struct SDL_Window* m_pWindow;
	struct SDL_Renderer* m_pRenderer;

	int32_t m_millisecsPreviousFrame;

	bool m_isRunning;

	std::unique_ptr<Registry> m_pRegistry;
	std::unique_ptr<AssetStore> m_pAssetStore;
};

