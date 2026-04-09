#include "PlayScene.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

class SceneManager
{
    sf::RenderWindow & m_window;
    PlayScene scene;
public:
    SceneManager(sf::RenderWindow & window);
    void updateScene();
    void renderScene();
};
