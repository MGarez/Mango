#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <fstream>

#include "Game.h"
#include "Logger.h"
#include "Components/TransformComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/SpriteComponent.h"
#include "Systems/MovementSystem.h"
#include "Systems/RenderSystem.h"



Game::Game()
{
	m_isRunning = true;
	Logger::Log("Game constructor");
	m_pWindow = nullptr;
	m_pRenderer = nullptr;
	m_millisecsPreviousFrame = 0;
	m_windowWidth = 0;
	m_windowHeight = 0;
	m_pRegistry = std::make_unique<Registry>();
	m_pAssetStore = std::make_unique<AssetStore>();
}

Game::~Game()
{
	Logger::Log("Game destructor");
}

void Game::Initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		Logger::Err("Error initializing SDL");
		return;
	}

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	m_windowWidth = displayMode.w;
	m_windowHeight = displayMode.h;
	

	m_pWindow = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		m_windowWidth, m_windowHeight, SDL_WINDOW_BORDERLESS);

	if (!m_pWindow)
	{
		Logger::Err("Error creating SDL window");
		return;
	}

	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

	if (!m_pRenderer)
	{
		Logger::Err("Error creating SDL renderer");
		return;
	}

	//SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_FULLSCREEN);
	m_isRunning = true;
}


void Game::Run()
{
	Setup();
	while (m_isRunning)
	{
		ProcessInput();
		Update();
		Render();
	}
}

void Game::LoadLevel(int level)
{
	// Add the systems that need to be processed in our game
	// #TODO: ESTO EXPLOTA EN OTRO ORDEN!!!!!!!!!!!!!!!!!!!
	m_pRegistry->AddSystem<MovementSystem>();
	m_pRegistry->AddSystem<RenderSystem>();
	
	

	// Adding assets to the asset store
	m_pAssetStore->AddTexture(m_pRenderer, "tank-image", "./assets/images/tank-panther-right.png");
	m_pAssetStore->AddTexture(m_pRenderer, "truck-image", "./assets/images/truck-ford-right.png");
	

	// Load tilemap texture
	m_pAssetStore->AddTexture(m_pRenderer, "jungle-map", "./assets/tilemaps/jungle.png");
	
	// Load map file 
	std::fstream fs;
	fs.open("./assets/tilemaps/jungle.map");

	int tileSize = 32;
	double tileScale = 3.;
	int mapRows = 20;
	int mapCols = 25;
	int row = 0;

	for (int i = 0; i < mapRows; ++i)
	{
		for (int j = 0; j < mapCols; ++j)
		{
			char ch = fs.get();
			int tY = std::atoi(&ch) * tileSize;
			ch = fs.get();
			int tX = std::atoi(&ch) * tileSize;
			fs.ignore();

			Entity tile = m_pRegistry->CreateEntity();
			tile.AddComponent<TransformComponent>(glm::vec2(j * tileScale * tileSize, i * tileScale * tileSize), glm::vec2(tileScale, tileScale), 0.);
			tile.AddComponent<SpriteComponent>("jungle-map", tileSize, tileSize, 0, tX, tY);
		}
	}
	
	fs.close();

	// Create an entity 
	Entity tank = m_pRegistry->CreateEntity();

	// Add some components to that entity
	tank.AddComponent<TransformComponent>(glm::vec2(10., 10.), glm::vec2(3., 3.), 45.);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(40., 0.));
	tank.AddComponent<SpriteComponent>("tank-image", 32, 32,1);

	// Create an entity 
	Entity truck = m_pRegistry->CreateEntity();

	// Add some components to that entity
	truck.AddComponent<TransformComponent>(glm::vec2(50., 100.), glm::vec2(3., 3.), 0.);
	truck.AddComponent<RigidBodyComponent>(glm::vec2(0., 50.));
	truck.AddComponent<SpriteComponent>("truck-image", 32, 32,1);
}

void Game::Setup()
{
	LoadLevel(1);
}

void Game::ProcessInput()
{
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			m_isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
			{
				m_isRunning = false;
			}
			break;
		}
	}
}

void Game::Update()
{
	double timeToWait = MS_PER_FRAME - (SDL_GetTicks() - m_millisecsPreviousFrame);
	if (timeToWait > 0 && timeToWait <= m_millisecsPreviousFrame)
	{
		SDL_Delay(timeToWait);
	}
	double deltaTime = (SDL_GetTicks() - m_millisecsPreviousFrame) / 1000.0;
	m_millisecsPreviousFrame = SDL_GetTicks();

	// Ask all systems to update
	m_pRegistry->GetSystem<MovementSystem>().Update(deltaTime);

	// Update the registry to process the entities that are waiting to be created/deleted
	m_pRegistry->Update();
}

void Game::Render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 21, 21, 21, 255);
	SDL_RenderClear(m_pRenderer);
	 
	m_pRegistry->GetSystem<RenderSystem>().Update(m_pRenderer, m_pAssetStore);

	SDL_RenderPresent(m_pRenderer);
}

void Game::Destroy()
{
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}