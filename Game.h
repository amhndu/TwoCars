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

        sf::RenderWindow window;
        sf::VertexArray dividers;
        std::deque<Obstacle> obstacles;
        Car leftCar,
            rightCar;
        sf::Clock timer;
        int score = 0;
        bool playing;
        float distance,
              velocity;
};

#endif // GAME_H
