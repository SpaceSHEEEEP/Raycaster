#include "EntityManager.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

void EntityManager::addPlayer(int a, int b)
{
        m_player.addComponent<CTransform>(sf::Vector2f(a * TILE_RAD, b * TILE_RAD), sf::Vector2f(.0f, .0f), 0.0f);
        m_player.addComponent<CShape>(4, PLAYER_RAD);
        m_player.getComponent<CShape>().shape.setFillColor(sf::Color::Magenta);
        m_player.addComponent<CInput>();
        m_player.addComponent<CRays>(sf::Vector2f(a * TILE_RAD, b * TILE_RAD), sf::degrees(0.0f), 91, 45);
}

void EntityManager::addTile(int a, int b)
{
    while (m_tiles.size() < b + 1)
    {
        m_tiles.push_back(std::vector<Entity>());
    }
    while(m_tiles[b].size() < a)
    { 
        m_tiles[b].push_back(Entity());
    }
    m_tiles[b].push_back(Entity("tile"));
    m_tiles[b][a].addComponent<CTransform>(sf::Vector2f(a* TILE_RAD, b * TILE_RAD), sf::Vector2f(0.0f, 0.0f), 0.0f);
    m_tiles[b][a].addComponent<CShape>(4, 64.0f);
    m_tiles[b][a].getComponent<CShape>().shape.setPosition(m_tiles[b][a].getComponent<CTransform>().pos + sf::Vector2f(TILE_RAD/2, TILE_RAD/2));        
    m_tiles[b][a].getComponent<CShape>().shape.setFillColor(sf::Color::White);
    m_tiles[b][a].getComponent<CShape>().shape.setOutlineThickness(2.0f);
    m_tiles[b][a].getComponent<CShape>().shape.setOutlineColor(sf::Color::Black);
    m_tiles[b][a].addComponent<CCollision>();
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

std::vector<std::vector<Entity>> & EntityManager::getTiles()
{
    return m_tiles;
}

void EntityManager::updateInfo()
{
    auto & playerShape = this->getPlayer().getComponent<CShape>().shape;
    auto & playerPos = this->getPlayer().getComponent<CTransform>();
    playerShape.setPosition(playerPos.pos);
    playerShape.setRotation(playerPos.angle);

    for (CRays::Ray & r : this->getPlayer().getComponent<CRays>().rays)
    {
        r.m_line.setPosition(r.m_pos);
        r.m_line.setSize({r.m_length, 1.0f});
        r.m_line.setRotation(r.m_angle);
    }
}
