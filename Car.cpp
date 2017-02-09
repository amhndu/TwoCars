#include "Car.h"
#include "Constants.h"

sf::Texture Car::m_carTexture;

Car::Car(const sf::Color& color, const sf::Vector2f& pos) :
    m_transition(0),
    m_lane(Left),
    m_shape({OBJECT_SIZE, CAR_HEIGHT}),
    m_key(sf::Keyboard::Space),
    m_center(pos)
{
    m_shape.setFillColor(color);
    m_shape.setOrigin(m_shape.getLocalBounds().width / 2, m_shape.getLocalBounds().height);
    m_shape.setPosition(m_center + sf::Vector2f{LANE_WIDTH / 2.f * m_lane, 0});
}

void Car::applyTexture()
{
    m_shape.setTexture(&m_carTexture);
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

void Car::reset()
{
    m_transition = 0;
    m_lane = Left;
    m_shape.setPosition(m_center + sf::Vector2f{LANE_WIDTH / 2.f * m_lane, 0});
}

void Car::update(float dt)
{
    if (m_transition > 0)
    {
        m_shape.move(CAR_TRANSITION_SPEED * dt, 0);
        if (m_shape.getPosition().x >= m_targetX)
        {
            m_transition = 0;
            m_shape.setPosition(m_targetX, m_shape.getPosition().y);
        }
    }
    else if (m_transition < 0)
    {
        m_shape.move(-CAR_TRANSITION_SPEED * dt, 0);
        if (m_shape.getPosition().x <= m_targetX)
        {
            m_transition = 0;
            m_shape.setPosition(m_targetX, m_shape.getPosition().y);
        }
    }
}

void Car::draw(sf::RenderTarget& target, const sf::RenderStates states) const
{
    target.draw(m_shape);
}

void Car::setLane(Car::Lane lane)
{
    m_targetX = m_center.x + LANE_WIDTH / 2.f * lane;
    m_transition = lane - m_lane;
    m_lane = lane;
}
