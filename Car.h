#ifndef CAR_H
#define CAR_H
#include <SFML/Graphics.hpp>

class Car : public sf::Drawable
{
    public:
        enum Lane { Left = -1, Right = 1};
        Car(const sf::Color& color, const sf::Vector2f& pos);
        void setKey(sf::Keyboard::Key key);
        void handleInput(const sf::Event& event);
        Lane getLane();
        void applyTexture();
        static sf::Texture m_carTexture;
        void update(float dt);
        void reset();
    protected:
        void draw(sf::RenderTarget& target, const sf::RenderStates states) const;
    private:
        void setLane(Lane lane);

        int m_transition;
        int m_targetX;
        Lane m_lane;
        sf::RectangleShape m_shape;
        sf::Keyboard::Key m_key;
        sf::Vector2f m_center;
};

#endif // CAR_H
