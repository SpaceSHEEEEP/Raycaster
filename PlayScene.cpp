#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "PlayScene.hpp"

float ROTATION_SPEED = 2.0f;
float PLAYER_SPEED;

// configure these
constexpr float TILE_SIZE    = 64.0f;
constexpr int   MAP_WIDTH    = 8;
constexpr int   MAP_HEIGHT   = 8;
constexpr float PLAYER_RADIUS = 10.0f;
constexpr float VIEW_HEIGHT  = 512.0f;
constexpr float MAX_WALL_HEIGHT = 500.0f;

PlayScene::PlayScene(sf::RenderWindow & window)
    : m_window(window)
    {
        std::string line;
        // get game config (player speed, other consts)
        // these won't be constexpr so we can tweak stuff while developing
        std::ifstream config("config.txt");
        if (!config) std::cout << "no config.txt found!" << '\n';
        else if (!config.is_open()) std::cout << "can't open config.txt!" << '\n';
        else 
        {
            std::string word;
            std::string setting;
            while(config >> word)
            {
                if (word == "ROTATION_SPEED") 
                {
                    config >> word;
                    ROTATION_SPEED = std::stof(word);
                }
                else if (word == "PLAYER_SPEED") 
                {
                    config >> word;
                    PLAYER_SPEED = std::stof(word);
                }
            }
            config.close();
        }

        // generate map
        std::ifstream file("map.txt");
        if (!file) std::cout << "no map.txt found!" << '\n';
        else if (file.is_open())
        {
            int rowNum = 0;
            while (std::getline(file, line))
            {
                for (int i{0}; i < line.size(); i += 2)
                {
                    if (line[i] == '#') m_entityManager.addTile(i / 2, rowNum);
                    else if (line[i] == 'P') m_entityManager.addPlayer(i/2, rowNum);
                }
                rowNum++;
            }
            file.close();
        }
        else std::cout << "file can't be opened" << '\n';
    }

void PlayScene::update()
{
    sUserInput();
    sMovement();
    sCollisions();
    sRays();
}

void PlayScene::sUserInput()
{
    // auto & movePlayer = m_entityManager.getPlayer().getComponent<CInput>();
    auto & movePlayer = m_player.getComponent<CInput>();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) movePlayer.up = true;
    else movePlayer.up = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) movePlayer.down = true;
    else movePlayer.down = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) movePlayer.left = true;
    else movePlayer.left = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) movePlayer.right = true;
    else movePlayer.right = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Q)) movePlayer.clockwise = true;
    else movePlayer.clockwise = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::E)) movePlayer.counterClockwise = true;
    else movePlayer.counterClockwise = false;
}

void PlayScene::sMovement()
{
    auto & player = m_player.getComponent<CTransform>();
    auto & input = m_player.getComponent<CInput>();

    if (input.up) 
    {
        player.pos.x += PLAYER_SPEED * std::cos(player.angle.asRadians());
        player.pos.y += PLAYER_SPEED * std::sin(player.angle.asRadians());
    }
    if (input.down) 
    {
        player.pos.x -= PLAYER_SPEED * std::cos(player.angle.asRadians());
        player.pos.y -= PLAYER_SPEED * std::sin(player.angle.asRadians());

    }
    if (input.right) 
    {
        player.pos.x -= PLAYER_SPEED * std::sin(player.angle.asRadians());
        player.pos.y += PLAYER_SPEED * std::cos(player.angle.asRadians());
    }
    if (input.left) 
    {
        player.pos.x += PLAYER_SPEED * std::sin(player.angle.asRadians());
        player.pos.y -= PLAYER_SPEED * std::cos(player.angle.asRadians());
    }
    
    if (input.clockwise) 
    {
        player.angle -= sf::degrees(ROTATION_SPEED);
        m_player.getComponent<CShape>().shape.setFillColor(sf::Color::Yellow);
    }
    if (input.counterClockwise) 
    {
        player.angle += sf::degrees(ROTATION_SPEED);
        m_player.getComponent<CShape>().shape.setFillColor(sf::Color::Cyan);
    }
}

void PlayScene::sCollisions()
{
    // TODO : can still go through corners
    auto & tileVec = m_entityManager.getTiles();
    sf::Vector2f & playerPos = m_player.getComponent<CTransform>().pos;
    int posX = (int)playerPos.x / 64;
    int posY = (int)playerPos.y / 64;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::P)) std::cout << posX << ' ' << posY << '\n';

    if (posX <  0 || posY < 0)
    {
        std::cout << posX << ' ' << posY << '\n';
    }
    else 
    {
        float overlapX = 0.0f, overlapY = 0.0f;
        // is there a box above me?
        if (posY > 0 && tileVec[posY - 1][posX].getComponent<CCollision>().exists)
        {
             overlapY = tileVec[posY - 1][posX].getComponent<CTransform>().pos.y + 64.0f - (playerPos.y - 10.0f);
             if (overlapY > 0) playerPos.y += overlapY;
        }
        // or below me?
        if (posY < 7 && tileVec[posY + 1][posX].getComponent<CCollision>().exists) // 7 DEPENDS ON MAP SIZE
        {
             overlapY = tileVec[posY + 1][posX].getComponent<CTransform>().pos.y         - (playerPos.y + 10.0f);
             if (overlapY < 0) playerPos.y += overlapY;
        }
        // of left of me
        if (posX > 0 && tileVec[posY][posX - 1].getComponent<CCollision>().exists)
        {
             overlapX = tileVec[posY][posX - 1].getComponent<CTransform>().pos.x + 64.0f - (playerPos.x - 10.0f);
             if (overlapX > 0) playerPos.x += overlapX;
        }
        // or right of me
        if (posX < 7 && tileVec[posY][posX + 1].getComponent<CCollision>().exists)
        {
             overlapX = tileVec[posY][posX + 1].getComponent<CTransform>().pos.x         - (playerPos.x + 10.0f);
             if (overlapX < 0) playerPos.x += overlapX;
        }
    }

    std::vector<CRays::Ray> & m_rays = m_player.getComponent<CRays>().rays;
    auto & input = m_player.getComponent<CInput>();
    for (CRays::Ray & r : m_rays)
    {
        r.m_pos = playerPos;
        if (input.clockwise) r.m_angle -= sf::degrees(ROTATION_SPEED);
        if (input.counterClockwise) r.m_angle += sf::degrees(ROTATION_SPEED);
        while (r.m_angle.asDegrees() < 0) r.m_angle += sf::degrees(360);
        while (r.m_angle.asDegrees() > 360) r.m_angle -= sf::degrees(360);
    }
}

void PlayScene::sRays()
{
    std::vector<std::vector<Entity>> & tileMap = m_entityManager.getTiles(); 
    float rayAngleRad, rayAngleDeg;
    int gridX, gridY;  
    float finalX, finalY, offsetX, offsetY, aTan, horizontalLength{FLT_MAX}, verticalLength{FLT_MAX};

    for (CRays::Ray & r : m_player.getComponent<CRays>().rays)
    {
        rayAngleRad = r.m_angle.asRadians();
        rayAngleDeg = r.m_angle.asDegrees();
        finalX = r.m_pos.x; 
        finalY = r.m_pos.y;
        gridX = (int)finalX / 64; 
        gridY = (int)finalY / 64;  
        aTan = 1 / std::tan(rayAngleRad);

        // check horizontal intercepts
        if (rayAngleDeg > 0 && rayAngleDeg < 180)
        {
            // first get the finalX and finalY and gridX and gridY and offsetX and offsetY
            offsetY = 64.0f;
            offsetX = offsetY * aTan;
            gridY++;
            finalY = gridY * 64.0f;
            finalX += (finalY - r.m_pos.y) * aTan;
            gridX = finalX / 64;
            
            // now do trigonometry
            while (true)
            {
                gridX = (int)finalX / 64;
                gridY = (int)finalY / 64;
                if (gridX < 0 || gridY < 0 || gridX > 7 || gridY > 7) 
                {
                    horizontalLength = (finalY - r.m_pos.y) / std::sin(rayAngleRad);
                    break;
                }
                if (tileMap[gridY][gridX].getTag() == "tile")
                {
                    horizontalLength = (finalY - r.m_pos.y) / std::sin(rayAngleRad);
                    break;
                }
                else 
                {
                    finalY += offsetY;
                    finalX += offsetX;
                }
            }
        }
        else if (rayAngleDeg > 180 && rayAngleDeg < 360) // looking up
        {
            // get the finalX and finalY and gridX and gridY and offsetX and offsetY
            offsetY = -64.0f;
            offsetX = offsetY * aTan;
            finalY = gridY * 64.0f;
            finalX -= (r.m_pos.y - finalY) * aTan;
            gridX = finalX / 64;
            
            // do trigonometry
            while (true)
            {
                gridX = (int)finalX / 64;
                gridY = (int)finalY / 64;
                if (gridX < 0 || gridY < 1 || gridX > 7 || gridY > 7) 
                {
                    horizontalLength = (finalY - r.m_pos.y) / std::sin(rayAngleRad);
                    break;
                }
                if (tileMap[gridY - 1][gridX].getTag() == "tile")
                {
                    horizontalLength = (finalY - r.m_pos.y) / std::sin(rayAngleRad);
                    break;
                }
                else 
                {
                    finalY += offsetY;
                    finalX += offsetX;
                }
            }
        } // if angle == 0 or 180, leave horizontalLength as FLT_MAX

        // reset
        finalX = r.m_pos.x; 
        finalY = r.m_pos.y;
        gridX = (int)finalX / 64; 
        gridY = (int)finalY / 64;  

        // now check vertical intercepts
        if ((rayAngleDeg > 0 && rayAngleDeg < 90) || (rayAngleDeg > 270 && rayAngleDeg < 360)) // looking right
        {
            // first get the finalX and finalY and gridX and gridY and offsetX and offsetY
            offsetX = 64.0f;
            offsetY = offsetX / aTan;
            gridX++;
            finalX = gridX * 64.0f;
            finalY += (finalX - r.m_pos.x) / aTan;
            gridY = finalY / 64;
            
            // now do trigonometry
            while (true)
            {
                gridY = (int)finalY / 64;
                gridX = (int)finalX / 64;
                if (gridX < 0 || gridY < 0 || gridX > 7 || gridY > 7) 
                {
                    verticalLength = (finalX - r.m_pos.x) / std::cos(rayAngleRad);
                    break;
                }
                if (tileMap[gridY][gridX].getTag() == "tile")
                {
                    verticalLength = (finalX - r.m_pos.x) / std::cos(rayAngleRad);
                    break;
                }
                else 
                {
                    finalY += offsetY;
                    finalX += offsetX;
                }
            }
        }
        else if (rayAngleDeg > 90 && rayAngleDeg < 270) // looking left
        {
            // get the finalX and finalY and gridX and gridY and offsetX and offsetY
            offsetX = -64.0f;
            offsetY = offsetX / aTan;
            finalX = gridX * 64.0f;
            finalY -= (r.m_pos.x - finalX) / aTan;
            gridY = finalY / 64;

            // do trigonometry
            while (true)
            {
                gridX = (int)finalX / 64;
                gridY = (int)finalY / 64;
                if (gridX < 1 || gridY < 0 || gridX > 7 || gridY > 7) 
                {
                    verticalLength = (finalX - r.m_pos.x) / std::cos(rayAngleRad);
                    break;
                }
                if (tileMap[gridY][gridX - 1].getTag() == "tile")
                {
                    verticalLength = (finalX - r.m_pos.x) / std::cos(rayAngleRad);
                    break;
                }
                else 
                {
                    finalY += offsetY;
                    finalX += offsetX;
                }
            }
        } // if angle == 90 or 270, dont care. prolly handled by horizontalLength
      
        r.m_length = std::min(horizontalLength, verticalLength);
        if (horizontalLength < verticalLength)
        {
            if (rayAngleDeg < 180) r.m_line.setFillColor(sf::Color(255, 0, 255, 120));
            else r.m_line.setFillColor(sf::Color(0, 0, 255, 120));
        }
        else
        {
            if (rayAngleDeg < 90 || rayAngleDeg > 270) r.m_line.setFillColor(sf::Color(255, 0, 0, 120));
            else r.m_line.setFillColor(sf::Color(255, 255, 0, 120));
            
        }
    }

    // TODO: rewrite this!!! we can't move forward if we are this messy!!
    // render the 3D STUFF!
    float rectangleWidth = 64.0f * 8 / m_player.getComponent<CRays>().rayNum;
    // for (sf::RectangleShape & r : m_entityManager.getPlayer().getComponent<CRays>().viewRectangles)
    CRays & crays = m_player.getComponent<CRays>();
    sf::Angle pAngle = m_player.getComponent<CTransform>().angle;
    for (int i = 0; i < crays.rayNum; i++)
    {
        crays.viewRectangles[i].setSize({crays.viewRectangles[i].getSize().x, std::max(0.0f,  (500.0f - crays.rays[i].m_length * std::cos( pAngle.asRadians() - crays.rays[i].m_angle.asRadians() )))});
        crays.viewRectangles[i].setPosition({crays.viewRectangles[i].getPosition().x, (512.0f - std::min(crays.viewRectangles[i].getSize().y, 500.0f)) / 2});
    }
}

void PlayScene::render() 
{
    m_window.clear();
    m_entityManager.updateInfo();

    for (std::vector<Entity> & vec : m_entityManager.getTiles())
    {
        for (Entity & t : vec)
        {
            m_window.draw(t.getComponent<CShape>().shape);
        }
    }
    m_window.draw(m_player.getComponent<CShape>().shape);

    for (CRays::Ray & r : m_player.getComponent<CRays>().rays)
    {
        m_window.draw(r.m_line);
    }
    for (sf::RectangleShape & r : m_player.getComponent<CRays>().viewRectangles)
    {
        m_window.draw(r);
    }

    m_window.display();
}

