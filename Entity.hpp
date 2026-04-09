#include <SFML/Graphics/ConvexShape.hpp>
#include "Components.cpp"
#include <string>
#include <tuple>

class Entity 
{
    std::tuple<CTransform, CShape, CRays, CCollision, CInput> m_components;
    std::string m_tag;

public:
    Entity(std::string tag = "default");
    std::string & getTag();
    
    template <typename T>
    T & getComponent();

    template <typename T>
    void addComponent(T component);

    template<typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs);
};

template <typename T>
T & Entity::getComponent()
{
    return std::get<T>(m_components);
}

template <typename T>
void Entity::addComponent(T component)
{
    std::get<T>(m_components).exists = true;
}

template<typename T, typename... TArgs>
T& Entity::addComponent(TArgs&&... mArgs) 
{
    auto & component = getComponent<T>();
    component = T(std::forward<TArgs>(mArgs)...);
    component.exists = true;
    return component;
}

