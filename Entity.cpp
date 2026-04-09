#include "Entity.hpp"

Entity::Entity(std::string tag)
    : m_tag{tag}
    {
    }

std::string & Entity::getTag()
{
    return m_tag;
}
