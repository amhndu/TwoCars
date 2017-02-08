#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <memory>
#include <algorithm>

#include "Obstacle.h"
#include "Constants.h"
#include "Car.h"

bool isGameOver(Car::Lane carLane, Car::Lane objLane, Obstacle::Type type)
{
    if ((carLane == objLane && type == Obstacle::Circle)
        || (carLane != objLane && type == Obstacle::Square))
            return false;
    return true;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(LANE_WIDTH * 4, WINDOW_HEIGHT), "TwoCars", sf::Style::Close | sf::Style::Titlebar);

    sf::VertexArray dividers(sf::Lines, 6);
    dividers[0] = sf::Vertex({LANE_WIDTH, 0}, sf::Color(180, 180, 180));
    dividers[1] = sf::Vertex({LANE_WIDTH, WINDOW_HEIGHT}, sf::Color(180, 180, 180));
    dividers[2] = sf::Vertex({LANE_WIDTH * 3, 0}, sf::Color(180, 180, 180));
    dividers[3] = sf::Vertex({LANE_WIDTH * 3, WINDOW_HEIGHT}, sf::Color(180, 180, 180));
    dividers[4] = sf::Vertex({LANE_WIDTH * 2, 0}, sf::Color(180, 180, 180));
    dividers[5] = sf::Vertex({LANE_WIDTH * 2, WINDOW_HEIGHT}, sf::Color(180, 180, 180));

    std::list<Obstacle> obstacles;
    Car leftCar (LEFT_COLOR,  sf::Vector2f{LANE_WIDTH, WINDOW_HEIGHT}),
        rightCar(RIGHT_COLOR, sf::Vector2f{LANE_WIDTH * 3, WINDOW_HEIGHT});
    leftCar .setKey(sf::Keyboard::F);
    rightCar.setKey(sf::Keyboard::J);

    sf::Event event;
    sf::Clock timer;
    int score = 0;
    float distance = SPAWN_DIST, velocity = INITIAL_VELOCITY;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            leftCar.handleInput(event);
            rightCar.handleInput(event);
        }

        auto dt = timer.restart().asSeconds();
        distance += velocity * dt;
        if (velocity > 0.1) //i.e. if velocity is non-zero
            velocity += ACCELERATION * dt;

        if (distance > SPAWN_DIST)
        {
            //IMP: Order of insertion matters, the order is used to reference which side it belongs
            obstacles.emplace_front(static_cast<Obstacle::Type>(rand() % 2),
                                          LEFT_COLOR,
                                          sf::Vector2f{LANE_WIDTH / 2.f + LANE_WIDTH * (rand() % 2), 0});
            obstacles.emplace_front(static_cast<Obstacle::Type>(rand() % 2),
                                          RIGHT_COLOR,
                                          sf::Vector2f{LANE_WIDTH * 2.f + LANE_WIDTH / 2 + LANE_WIDTH * (rand() % 2), 0});
            distance -= SPAWN_DIST;
        }

        for (auto& obstacle : obstacles)
        {
            obstacle.getShape().move(0, velocity * dt);
        }

        if (obstacles.back().getShape().getGlobalBounds().top > WINDOW_HEIGHT - CAR_HEIGHT - OBJECT_WIDTH)
        {
            auto &left = obstacles.back();
            Car::Lane lLane = left.getShape().getGlobalBounds().left < LANE_WIDTH ? Car::Left : Car::Right;
            Obstacle::Type lType = left.getType();
            obstacles.pop_back();

            auto &right = obstacles.back();
            Car::Lane rLane = right.getShape().getGlobalBounds().left < 3 * LANE_WIDTH ? Car::Left : Car::Right;
            Obstacle::Type rType = right.getType();
            obstacles.pop_back();

            if (isGameOver(lLane, leftCar.getLane(), lType) || isGameOver(rLane, rightCar.getLane(), rType))
            {
                velocity = 0;
                std::cout << "You scored " << score << " points." << std::endl;
            }
            else
                ++score;

        }

        window.clear(sf::Color(10, 0, 30));
        window.draw(dividers);
        for (auto& o : obstacles)
            window.draw(o);
        window.draw(leftCar);
        window.draw(rightCar);
        window.display();
    }

    return 0;
}
