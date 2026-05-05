#include "EntityManager.h"

void EntityManager::update()
{
    // add all the entities that we want to add
    for (auto e : m_entitiesToAdd)
    {
        m_entities.push_back(e);
        m_entityMap[e->tag()].push_back(e);
    }

    m_entitiesToAdd.clear();

    // remove dead entities from the vector of all entities
    removeDeadEntities(m_entities);

    // remove dead entities from each vector in the entitiy map 
    // C++20 way of iterating through [key,value] pairs in a map
    for (auto& [tag, entityVec] : m_entityMap)
    {
        removeDeadEntities(entityVec);
    }
}

void EntityManager::removeDeadEntities(EntityVec & vec)
{
    vec.erase(
        std::remove_if(vec.begin(), vec.end(),
            [](const std::shared_ptr<Entity>& e)
            {
                return !e->isActive();
            }),
        vec.end()
    );

}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string & tag)
{
	// creat the entity shared pointer
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

	// add it to the vector of entities that will be added on next update() call
	m_entitiesToAdd.push_back(entity);

	// return the shared pointer to the entity
	return entity;
}

const EntityVec & EntityManager::getEntities()
{
	return m_entities;
}

const EntityVec & EntityManager::getEntities(const std::string & tag)
{
	// return the vector in the map where all the entities with the same tag live
	return m_entityMap[tag];
}