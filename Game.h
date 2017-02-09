#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <deque>
#include <memory>
#include <algorithm>

#include "Obstacle.h"
#include "Constants.h"
#include "Car.h"

class Game
{
    public:
        Game();
        void run();
    private:
        void newGame();
        static bool isGameOver(Car::Lane carLane, Car::Lane objLane, Obstacle::Type type);

        sf::RenderWindow m_window;

        sf::VertexArray m_dividers;
        std::deque<Obstacle> m_obstacles;
        Car m_leftCar,
            m_rightCar;

        sf::Font m_font;
        sf::Text m_prompt;
        sf::RectangleShape m_overlayBg;
        sf::Clock m_timer;
        int m_score;
        bool m_playing;
        float m_distance,
              m_velocity;
};

#endif // GAME_H
