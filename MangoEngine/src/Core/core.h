#pragma once
#include <cstdint>
#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
#include <memory>

#include "Logger.h"

struct IPool
{
public:
	virtual  ~IPool() {};
};

template <typename T>
class Pool : public IPool
{
public:

	Pool(int size = 100)
	{
		m_data.resize(size);
	}

	~Pool() = default;

	bool isEmpty() const
	{
		return m_data.empty();
	}

	int GetSize() const
	{
		return m_data.size();
	}

	void Resize(int n)
	{
		m_data.resize(n);
	}

	void Clear()
	{
		m_data.clear();
	}

	void Add(T object)
	{
		m_data.push_back(object);
	}

	void Set(int index, T object)
	{
		m_data[index] = object;
	}

	T& Get(int index)
	{
		return static_cast<T&>(m_data[index]);
	}

	T& operator [](unsigned int index)
	{
		return m_data[index];
	}

private:

	std::vector<T> m_data;
};

class Entity
{
public:
	Entity(int id) : m_id(id) { p_registry = nullptr; };
	Entity(const Entity& entity) = default;
	inline int GetId() const { return m_id; };

	Entity& operator =(const Entity& other) = default;
	bool operator ==(const Entity& other)
	{
		return m_id == other.GetId();
	}

	bool operator <(const Entity& other) const
	{
		return m_id < other.GetId();
	}

	bool operator !=(const Entity& other)const
	{
		return m_id != other.GetId();
	}

	bool operator >(const Entity& other) const
	{
		return m_id > other.GetId();
	}

	template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
	template <typename TComponent> void RemoveComponent();
	template <typename TComponent> bool HasComponent() const;
	template <typename TComponent> TComponent& GetComponent() const;

public:

	class Registry* p_registry;

private:
	int m_id;
};

struct IComponent
{
protected:
	static int m_nextId;
};

template <typename T>
class Component : public IComponent
{
public:
	static int GetId()
	{
		static auto id = m_nextId++;
		return id;
	}
};

const uint32_t MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

class System
{

public:
	System() = default;
	~System() = default;

	void AddEntityToSystem(Entity entity);
	void RemoveEntityToSystem(Entity entity);
	std::vector<Entity> GetSystemEntities() const;
	const Signature& GetComponentSignature() const;

	template <typename TComponent> void RequireComponent();

private:
	Signature m_componentSignature;
	std::vector<Entity> m_entities;
};

class Registry
{
public:

	Registry() = default;

	// Entity management
	Entity CreateEntity();

	// Component management
	template<typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
	template<typename TComponent> void RemoveComponent(Entity entity);
	template<typename TComponent> void HasComponent(Entity entity);
	template<typename TComponent> TComponent& GetComponent(Entity entity) const;

	// System Management
	template<typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
	template<typename TSystem> void RemoveSystem();
	template<typename TSystem> bool HasSystem() const;
	template<typename TSystem> TSystem& GetSystem() const;

	// Checks the component signature of an entity and add the entity to the systems
	// that are interested in it
	void AddEntityToSystems(Entity entity);

	void Update();
private:

	int32_t m_numEntities = 0;

	// Vector of component pools
	// each pool contains all the data for a certain component type
	// [vector index = component id], [pool index = entity id]
	std::vector<std::shared_ptr<IPool>> m_ComponentPool; //rename

	std::vector<Signature> m_entityComponentSignature;

	std::unordered_map<std::type_index, std::shared_ptr<System>> m_systems;

	std::set<Entity> m_entitiesToBeAdded;
	std::set<Entity> m_entitiesToBeKilled;
};


template <typename TComponent, typename ...TArgs >
void Entity::AddComponent(TArgs&& ...args)
{
	p_registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template <typename TComponent>
void Entity::RemoveComponent()
{
	p_registry->RemoveComponent<TComponent>(*this);
}

template <typename TComponent>
bool Entity::HasComponent() const
{
	return p_registry->HasComponent<TComponent>(*this);
}

template <typename TComponent>
TComponent& Entity::GetComponent() const
{
	return p_registry->GetComponent<TComponent>(*this);
}

template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args)
{
	const auto componentId = Component<TComponent>::GetId();

	const auto entityId = entity.GetId();

	// if the component id is greater than the current size of the component pools, then resize the vector
	while (componentId >= m_ComponentPool.size())
	{
		if (m_ComponentPool.size() == 0)
		{
			m_ComponentPool.resize(m_ComponentPool.size() + 1, nullptr);
		}
		else
		{
			m_ComponentPool.resize(m_ComponentPool.size() * 2, nullptr);
		}
	}


	// if we still dont have a pool for that component type
	if (!m_ComponentPool[componentId])
	{
		m_ComponentPool[componentId] = std::make_shared<Pool<TComponent>>();
	}

	// get the pool of component values for that component type
	std::shared_ptr<Pool<TComponent>> p_componentPool = std::static_pointer_cast<Pool<TComponent>>(m_ComponentPool[componentId]); // aqui el man hacia un c-cast turbio (o era el constructor de copia)?

	// if the entity id is greater than the current size of the component pool, then resize the pool
	if (entityId >= p_componentPool->GetSize())
	{
		p_componentPool->Resize(m_numEntities);
	}

	// create a new component object of the type T
	TComponent newComponent(std::forward<TArgs>(args)...);

	//add the new component to the component pool list 
	p_componentPool->Set(entityId, newComponent);

	// finally change the component signature
	m_entityComponentSignature[entityId].set(componentId);

	Logger::Log("Component created with id = " + std::to_string(componentId));
}


template <typename TComponent>
void System::RequireComponent()
{
	const auto componentId = Component<TComponent>::GetId();
	m_componentSignature.set(componentId);
}

template<typename TComponent>
void Registry::RemoveComponent(Entity entity)
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	m_entityComponentSignature[componentId].set(componentId, false);
}

template<typename TComponent>
void Registry::HasComponent(Entity entity)
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	m_entityComponentSignature[componentId].test(componentId);
}

template<typename TComponent>
TComponent& Registry::GetComponent(Entity entity) const
{
	const auto componentID = Component<TComponent>::GetId();
	const auto entityID = entity.GetId();
	auto componentPool = std::static_pointer_cast<Pool<TComponent>>(m_ComponentPool[componentID]);
	return componentPool->Get(entityID);
}

template<typename TSystem, typename ...TArgs>
void Registry::AddSystem(TArgs&& ...args)
{
	std::shared_ptr<TSystem> p_newSystem(std::make_shared<TSystem>(std::forward<TArgs>(args)...));
	m_systems.insert(std::make_pair(std::type_index(typeid(TSystem)), p_newSystem));
}

template<typename TSystem>
void Registry::RemoveSystem()
{
	auto system = m_systems.find(std::type_index(typeid(TSystem)));
	m_systems.erase(system);
}

template<typename TSystem>
bool Registry::HasSystem() const
{
	return m_systems.find(std::type_index(typeid(TSystem))) != m_systems.end();
}

template<typename TSystem>
TSystem& Registry::GetSystem() const
{
	auto system = m_systems.find(std::type_index(typeid(TSystem)));
	return *(std::static_pointer_cast<TSystem>(system->second));
}
