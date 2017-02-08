#ifndef CAR_H
#define CAR_H
#include <SFML/Graphics.hpp>

class Car
{
    public:
        Car(const sf::Color& color, const sf::Vector2f& pos);
        void switchLane(int lane_width);
    private:
        enum Lane {Left, Right} m_lane;
        sf::RectangleShape m_shape;
};

#endif // CAR_H
