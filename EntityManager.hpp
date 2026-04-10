#include <SFML/Graphics/ConvexShape.hpp>

#include "Entity.hpp"

#include <vector>

class EntityManager
{
    const float TILE_RAD = 64.0f;
    const float PLAYER_RAD = 20.0f;
    const float PLAYER_SPEED = 5.0f;;
    const float PLAYER_TURN_SPEED = 0.1f;

    std::vector<std::vector<Entity>> m_tiles;
    Entity m_player = Entity("player");
    int test;

public: 
    void addPlayer(int a, int b);
    void addTile(int a, int b);
    void addEntity(std::string tag);
    Entity & getPlayer();
    std::vector<std::vector<Entity>> & getTiles();
    void updateInfo();
};
