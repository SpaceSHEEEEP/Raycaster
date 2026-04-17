#include <SFML/Graphics.hpp>

#include "SceneManager.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({640, 480}), "raycaster test");
    window.setFramerateLimit(60);
    window.setPosition({100, 150});
    SceneManager sceneManager(window);
     
    // I need this for mac version to work
    while (const std::optional event = window.pollEvent())
    {
        // Check if the window was closed
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
    }

    while(window.isOpen())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Escape)) window.close();

        sceneManager.updateScene();
        sceneManager.renderScene();
    }

    return 0;
}
