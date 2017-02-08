#include "Car.h"
#include "Constants.h"

Car::Car(const sf::Color& color, const sf::Vector2f& pos) :
    m_shape({OBJECT_WIDTH, CAR_HEIGHT}),
    m_key(sf::Keyboard::Space),
    m_center(pos)
{
    m_shape.setFillColor(color);
    m_shape.setOrigin(m_shape.getLocalBounds().width / 2, m_shape.getLocalBounds().height);
    setLane(Left);
}

void Car::setKey(sf::Keyboard::Key key)
{
    m_key = key;
}

void Car::handleInput(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == m_key)
    {
        setLane(static_cast<Lane>(m_lane * -1));
    }
}

Car::Lane Car::getLane()
{
    return m_lane;
}

void Car::draw(sf::RenderTarget& target, const sf::RenderStates states) const
{
    target.draw(m_shape);
}

void Car::setLane(Car::Lane lane)
{
    m_lane = lane;
    m_shape.setPosition(m_center + sf::Vector2f{LANE_WIDTH / 2.f * lane, 0});
}
