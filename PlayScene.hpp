#include "EntityManager.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

class PlayScene
{
    int test;
    EntityManager m_entityManager;
    sf::RenderWindow & m_window;
    Entity & m_player = m_entityManager.getPlayer(); 

public:
    PlayScene(sf::RenderWindow & window);
    void update();
    void sUserInput();
    void sMovement();
    void sCollisions();
    void render();
    void sRays();
};
