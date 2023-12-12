#include "MovementSystem.h"
#include "Components/TransformComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Logger.h"

MovementSystem::MovementSystem()
{
	RequireComponent<TransformComponent>();
	RequireComponent<RigidBodyComponent>();
}

void MovementSystem::Update(const float& deltaTime)
{
	// Loop all the entities that the system is interested in
	for (auto entity : GetSystemEntities())
	{
		// Update entity position based on its velocity
		auto& transform = entity.GetComponent<TransformComponent>();
		const auto & rigidbody = entity.GetComponent<RigidBodyComponent>();

		transform.m_position.x += rigidbody.m_velocity.x * deltaTime;
		transform.m_position.y += rigidbody.m_velocity.y * deltaTime;
		
	}
}

