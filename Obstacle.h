#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SFML/Graphics.hpp>

class Obstacle : public sf::Drawable
{
    public:
        enum Type
        {
            Circle,
            Triangle,
        };
        Obstacle(Type type, const sf::Color& color, const sf::Vector2f& pos);
        sf::Shape& getShape();
        Type getType();
        static sf::Texture m_circleTexture;
        static sf::Texture m_triangleTexture;
    protected:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        Type m_type;
        std::unique_ptr<sf::Shape> m_shape;
};
#endif // OBSTACLE_H
