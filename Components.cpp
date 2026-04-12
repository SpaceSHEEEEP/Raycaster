#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

class Component
{
public:
    bool exists{false}; // now all components got this variable 
};

class CTransform : public Component
{
public:
    sf::Vector2f pos{0.0f, 0.0f};
    sf::Vector2f velocity{0.0f, 0.0f};
    sf::Angle angle{sf::degrees(0)};

    CTransform() = default;
    CTransform(const sf::Vector2f& p, const sf::Vector2f vel, const float a)
        : pos{p}, velocity{vel}, angle{sf::degrees(a)} 
    {
    } 
};

class CShape : public Component
{
public:
    sf::ConvexShape shape;
    int pointCount;
    float sideLength;

    CShape()
        : sideLength{10.0f} // delete this
    {
        shape.setPointCount(3);
        shape.setPoint(0, {0.0f, 0.0f});
        shape.setPoint(1, {sideLength, 0.0f});
        shape.setPoint(2, {sideLength, sideLength});
    }

    CShape(int pointCount, float sideLength)
        : pointCount{pointCount}, sideLength{sideLength}
    {
        // pointCount means nothing to me now
        shape.setPointCount(4);
        shape.setPoint(0, {0.0f, 0.0f});
        shape.setPoint(1, {sideLength, 0.0f});
        shape.setPoint(2, {sideLength, sideLength});
        shape.setPoint(3, {0.0f, sideLength});
        shape.setOrigin({sideLength / 2, sideLength / 2});
        shape.setFillColor(sf::Color::Blue);
    }
};

class CRays : public Component
{
public:
    struct Ray 
    {
        sf::Angle m_angle;
        sf::Angle m_relativeAngle;
        float m_length{1000.0f};
        sf::Vector2f m_pos;
        sf::RectangleShape m_drawable = sf::RectangleShape({m_length, 1.0f}); 

        Ray(sf::Vector2f pos, sf::Angle angle, sf::Angle relativeAngle)
            : m_pos{pos}, m_angle(angle), m_relativeAngle(relativeAngle)
        {
            m_drawable.setPosition(m_pos);
            m_drawable.setRotation(m_angle);
            m_drawable.setFillColor(sf::Color::Red);
        }
    };

    int rayCount{10};
    float FOV{90.0f};
    std::vector<Ray> raysVec;
    std::vector<sf::RectangleShape> wallSlices;

    CRays() = default;
    CRays(sf::Vector2f pos, sf::Angle angle , int rayNum, float fov)
        : rayCount{rayNum}, FOV{fov}
    {
        if (rayNum == 1) 
        {
            raysVec.push_back(Ray(pos, angle, sf::degrees(0.0f)));
            wallSlices.push_back(sf::RectangleShape({512.0f, 512.0f}));
        }
        else 
        {
            float angleBtw = FOV * 2 / (rayNum - 1);
            float sliceWidth = 640.0f / rayNum;
            for (int i{0}; i < rayNum; i++)
            {
                raysVec.push_back(Ray(pos, sf::degrees(angleBtw * i - FOV), sf::degrees(angleBtw * i - FOV)));
                wallSlices.push_back(sf::RectangleShape({sliceWidth, 512.0f}));
                wallSlices.back().setPosition({0.0f + i * sliceWidth, 0.0f});
            }
        }
    }
};

class CCollision : public Component
{
public:
    float radius{0};

    CCollision() = default;
    CCollision(float r) 
        : radius{r} 
    {
    }
};

// class CScore : public Component
// {
// public:
//     int score{0};
//
//     CScore() = default;
//     CScore(int s)
//         : score{s} 
//     {
//     }
// };

// class CLifeSpan : public Component
// {
// public:
//     int lifespan{0};
//     int remaining{0};
//
//     CLifeSpan() = default;
//     CLifeSpan(int l, int r)
//         : lifespan{l}, remaining{r}
//     {
//     }
// };

class CInput : public Component
{
public:
    bool up{false};
    bool down{false};
    bool left{false};
    bool right{false};
    bool clockwise{false};
    bool counterClockwise{false};
    bool shoot{false};
    
    CInput() = default;
};





