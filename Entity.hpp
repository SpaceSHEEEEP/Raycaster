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
    T & getC();

    template <typename T>
    void addC(T component);

    template<typename T, typename... TArgs>
    T& addC(TArgs&&... mArgs);
};

template <typename T>
T & Entity::getC()
{
    return std::get<T>(m_components);
}

template <typename T>
void Entity::addC(T component)
{
    std::get<T>(m_components).exists = true;
}

template<typename T, typename... TArgs>
T& Entity::addC(TArgs&&... mArgs) 
{
    auto & component = getC<T>();
    component = T(std::forward<TArgs>(mArgs)...);
    component.exists = true;
    return component;
}

