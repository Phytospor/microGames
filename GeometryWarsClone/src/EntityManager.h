#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include "Entity.h"

class Entity;

using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::map<std::string, EntityVec>;


class EntityManager
{
    EntityVec   m_entities;
    EntityVec   m_entitiesToAdd;
    EntityMap   m_entityMap;
    size_t      m_totalEntities = 0;

    void removeDeadEntities(EntityVec& vec);

public:
    
    EntityManager() = default;

    void update();

    std::shared_ptr<Entity> addEntity(const std::string& tag);

    const EntityVec& getEntities();

    const EntityVec& getEntities(const std::string& tag);

    const EntityMap& getEntityMap();

};