#include "EntityManager.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <vector>

void EntityManager::addPlayer(int a, int b)
{
        m_player.addC<CTransform>(sf::Vector2f(a * TILE_RAD, b * TILE_RAD), sf::Vector2f(.0f, .0f), 0.0f);
        m_player.addC<CShape>(4, PLAYER_RAD);
        m_player.addC<CInput>();
        m_player.addC<CRays>(sf::Vector2f(a * TILE_RAD, b * TILE_RAD), sf::degrees(0.0f), 91, 60);
}

void EntityManager::addTile(int a, int b)
{
    while (m_tiles.size() < b + 1)
    {
        m_tiles.push_back(std::vector<std::shared_ptr<Entity>>());
    }
    while(m_tiles[b].size() < a)
    { 
        m_tiles[b].push_back(std::make_shared<Entity>("empty"));
    }
    m_tiles[b].push_back(std::make_shared<Entity>("tile"));
    m_tiles[b][a]->addC<CTransform>(sf::Vector2f(a* TILE_RAD, b * TILE_RAD), sf::Vector2f(0.0f, 0.0f), 0.0f);
    m_tiles[b][a]->addC<CShape>(4, TILE_RAD);
    m_tiles[b][a]->getC<CShape>().shape.setPosition(m_tiles[b][a]->getC<CTransform>().pos + sf::Vector2f(TILE_RAD/2, TILE_RAD/2));        
    m_tiles[b][a]->getC<CShape>().shape.setFillColor(sf::Color::White);
    m_tiles[b][a]->getC<CShape>().shape.setOutlineThickness(2.0f);
    m_tiles[b][a]->getC<CShape>().shape.setOutlineColor(sf::Color::Black);
    m_tiles[b][a]->addC<CCollision>();
}

void EntityManager::addEntity(std::string tag)
{
    if (tag == "player")
    {
        // one day
    }
}

Entity & EntityManager::getPlayer()
{
    return m_player;
}

std::vector<std::vector<std::shared_ptr<Entity>>> & EntityManager::getTiles()
{
    return m_tiles;
}

bool EntityManager::isTile(int b, int a)
{
    if (a < 0 || b < 0 || a >= 8 || b >= 8) return false;
    return m_tiles[b][a]->getTag() == "tile";
}

void EntityManager::updateInfo()
{
    auto & playerShape = this->getPlayer().getC<CShape>().shape;
    auto & playerPos = this->getPlayer().getC<CTransform>();
    playerShape.setPosition(playerPos.pos);
    playerShape.setRotation(playerPos.angle);

    for (CRays::Ray & r : this->getPlayer().getC<CRays>().raysVec)
    {
        r.m_line.setPosition(r.m_pos);
        r.m_line.setSize({r.m_length, 1.0f});
        r.m_line.setRotation(r.m_angle);
    }
}
