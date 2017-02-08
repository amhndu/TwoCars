#include "Obstacle.h"

Obstacle::Obstacle(Type type, const sf::Color& color, const sf::Vector2f& pos, int lane_width)
    : m_type(type)
{
    if (type == Circle)
    {
        m_shape = std::unique_ptr<sf::Shape>(new sf::CircleShape(lane_width / 3));
    }
    else
        m_shape = std::unique_ptr<sf::Shape>(new sf::RectangleShape({2 * lane_width / 3, 2 * lane_width / 3}));

    m_shape->setPosition(pos);
    m_shape->setFillColor(color);
    m_shape->setOrigin(m_shape->getLocalBounds().width / 2, 0);
}

sf::Shape& Obstacle::getShape()
{
    return *m_shape;
}

void Obstacle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(*m_shape, states);
}