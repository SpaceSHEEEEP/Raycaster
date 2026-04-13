#include <SFML/Graphics/ConvexShape.hpp>

#include "Entity.hpp"

#include <memory>
#include <vector>

class EntityManager
{
    const float TILE_RAD = 64.0f;
    const float PLAYER_RAD = 20.0f;
    // const float PLAYER_SPEED = 5.0f;
    // const float PLAYER_TURN_SPEED = 9.1f;
    int MAP_WIDTH = 0;
    int MAP_HEIGHT = 0;

    std::vector<std::vector<std::shared_ptr<Entity>>> m_tiles;
    Entity m_player = Entity("player");

public: 
    void addPlayer(int a, int b);
    void addTile(int a, int b);
    void addEntity(std::string tag);
    Entity & getPlayer();
    std::vector<std::vector<std::shared_ptr<Entity>>> & getTiles();
    bool isTileOrOOB(int b, int a);
    void updateInfo();
    void printMapSize();
};
