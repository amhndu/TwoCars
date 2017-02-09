#include "Obstacle.h"
#include "Constants.h"

sf::Texture Obstacle::m_circleTexture;
sf::Texture Obstacle::m_triangleTexture;

Obstacle::Obstacle(Type type, const sf::Color& color, const sf::Vector2f& pos) :
    m_type(type)
{
    if (type == Circle)
    {
        m_shape = std::unique_ptr<sf::Shape>(new sf::CircleShape(OBJECT_SIZE / 2));
        m_shape->setTexture(&m_circleTexture);
    }
    else
    {
        m_shape = std::unique_ptr<sf::Shape>(new sf::RectangleShape({OBJECT_SIZE, OBJECT_SIZE}));
        m_shape->setTexture(&m_triangleTexture);
    }

    m_shape->setPosition(pos);
    m_shape->setFillColor(color);
    m_shape->setOrigin(m_shape->getLocalBounds().width / 2, 0);
}

sf::Shape& Obstacle::getShape()
{
    return *m_shape;
}

Obstacle::Type Obstacle::getType()
{
    return m_type;
}

void Obstacle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(*m_shape, states);
}