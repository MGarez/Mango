#include "core.h"

int IComponent::m_nextId = 0;


void System::AddEntityToSystem(Entity entity)
{
	m_entities.push_back(entity);
}

void System::RemoveEntityToSystem(Entity entity)
{
	m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [&entity](const Entity& other) { return entity == other; }), m_entities.end());
}

std::vector<Entity> System::GetSystemEntities() const
{
	return m_entities;
}

const Signature& System::GetComponentSignature() const
{
	return m_componentSignature;
}


Entity Registry::CreateEntity()
{

	int entityId;
	entityId = m_numEntities++;
	Entity entity(entityId);
	entity.p_registry = this;
	m_entitiesToBeAdded.insert(entity);

	if (entityId >= m_entityComponentSignature.size())
	{
		m_entityComponentSignature.resize(entityId + 1);
	}

	Logger::Log("Entity created with id = " + std::to_string(entityId));
	return entity;
}

void Registry::AddEntityToSystems(Entity entity)
{
	const auto entityId = entity.GetId();

	const auto entityComponentSignature = m_entityComponentSignature[entityId];

	// Loop all the systems
	for (const auto& system : m_systems)
	{
		const auto& systemComponentSignature = system.second->GetComponentSignature();

		if ((entityComponentSignature & systemComponentSignature) == systemComponentSignature)
		{
			system.second->AddEntityToSystem(entity);
		}
	}
}

void Registry::Update()
{
	// Add the entities that are waiting to be added
	for (auto entity : m_entitiesToBeAdded)
	{
		AddEntityToSystems(entity);
	}
	m_entitiesToBeAdded.clear();

	// #Todo
}