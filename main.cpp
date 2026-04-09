#include <SFML/Graphics.hpp>

#include "SceneManager.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({64 * 16, 64 * 8}), "raycaster test");
    window.setFramerateLimit(60);
    window.setPosition({800, 450});
    SceneManager sceneManager(window);
     
    while(window.isOpen())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Escape)) window.close();

        sceneManager.updateScene();
        sceneManager.renderScene();
    }

    return 0;
}
