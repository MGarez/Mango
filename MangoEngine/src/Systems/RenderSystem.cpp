#include "RenderSystem.h"

#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"
#include "AssetStore/AssetStore.h"

#include <SDL.h>
#include <vector>
#include <algorithm>

RenderSystem::RenderSystem()
{
	RequireComponent<TransformComponent>();
	RequireComponent<SpriteComponent>();
}

void RenderSystem::Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore)
{
	// Create a vector with the sprite and transform components of all entities
	struct renderable
	{
		TransformComponent transform;
		SpriteComponent sprite;
	};
	
	std::vector<renderable> renderableVector;
	for (const auto & entity : GetSystemEntities())
	{
		struct renderable renderableEntity;
		renderableEntity.transform = entity.GetComponent<TransformComponent>();
		renderableEntity.sprite = entity.GetComponent<SpriteComponent>();
		renderableVector.push_back(renderableEntity);
	}

	std::sort(renderableVector.begin(), renderableVector.end(), [](const renderable& lhs, const renderable& rhs) { return lhs.sprite.zindex < rhs.sprite.zindex; });

	// Loop all the entities that the system is interested in
	for (const renderable & entity : renderableVector)
	{
		// Update entity position based on its velocity
		const TransformComponent transform = entity.transform;
		const SpriteComponent sprite = entity.sprite;
		SDL_Rect srcRect = sprite.srcRest;
		SDL_Rect dstRect =
		{
			static_cast<int>(transform.m_position.x),
			static_cast<int>(transform.m_position.y),
			static_cast<int>(sprite.width * transform.m_scale.x),
			static_cast<int>(sprite.height * transform.m_scale.y)
		};

		SDL_RenderCopyEx(renderer, assetStore->GetTexture(sprite.assetId), &srcRect, &dstRect, transform.m_rotation, nullptr ,SDL_FLIP_NONE);
	}
}
