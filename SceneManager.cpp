#include "SceneManager.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

SceneManager::SceneManager(sf::RenderWindow & window)
    : m_window{window}, scene{PlayScene(window)}
    {
    }

void SceneManager::updateScene()
{
    scene.update();
}

void SceneManager::renderScene()
{
    scene.render();
}
