#pragma once
#include "Components.hpp"


using ComponentTuple = std::tuple<
    CTransform,
    CShape,
    CCollision,
    CInput,
    CScore,
    CLifespan
>;


class Entity
{

    // EntityManager is now a friend, it can use our private constructor
    // This ensures we can't make Entity instances outside EntityManager
    friend class EntityManager;

    bool        m_active    = true;
    size_t      m_id        = 0;
    std::string m_tag       = "default";

    // the constructor of entity is private, only its friends can use it
    Entity(const size_t & id, const std::string & tag);

public:

    ComponentTuple m_components;

    // constructor and destructor
    ~Entity();

    // private member access functions
    bool isActive() const;
    const std::string & tag() const;
    const size_t & id() const;
    void destroy();


    template <typename T>
    T& get()
    {
        return std::get<T>(m_components);
    }

    template <typename T>
    bool has() const
    {
        return std::get<T>(m_components).exists;
    }

    template <typename T>
    bool exists() const
    {
        return std::get<T>(m_components).exists;
    }

    template <typename T, typename... TArgs>
    T& add(TArgs&&... args)
    {
        T& component = std::get<T>(m_components);
        component = T(std::forward<TArgs>(args)...);
        component.exists = true;
        return component;
    }
};