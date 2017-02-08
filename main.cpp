#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <memory>
#include <algorithm>

#include "Obstacle.h"
#include "Constants.h"
#include "Car.h"

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

            if (obstacles.size() > 6)
            {
                auto& left = obstacles.back();
                auto& right = std::prev(obstacles.end(), 2);
                obstacles.pop_back();
                obstacles.pop_back();
            }
            std::cout << obstacles.size() << std::endl;
        }

        for (auto& obstacle : obstacles)
        {
            obstacle.getShape().move(0, VELOCITY * dt);
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
