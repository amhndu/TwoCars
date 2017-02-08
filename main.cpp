#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <memory>
#include <algorithm>

#include "Obstacle.h"

const int LANE_WIDTH = 80;
const int WINDOW_HEIGHT = 700;
const int SPAWN_DIST = 150;


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
    sf::Event event;
    sf::Clock timer;
    float distance = SPAWN_DIST, velocity = 100;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        auto dt = timer.restart().asSeconds();
        distance += velocity * dt;
        if (distance > SPAWN_DIST)
        {
            //IMP: Order of insertion of left and right matter
            obstacles.emplace_front(static_cast<Obstacle::Type>(rand() % 2),
                                          sf::Color::Red,
                                          sf::Vector2f{LANE_WIDTH / 2 + LANE_WIDTH * (rand() % 2), 0},
                                          LANE_WIDTH);
            obstacles.emplace_front(static_cast<Obstacle::Type>(rand() % 2),
                                          sf::Color::Blue,
                                          sf::Vector2f{LANE_WIDTH * 2 + LANE_WIDTH / 2 + LANE_WIDTH * (rand() % 2), 0},
                                          LANE_WIDTH);
            distance -= SPAWN_DIST;

            if (obstacles.size() > 8)
            {
                auto& right = obstacles.back();
                auto& left  = obstacles.back();
                obstacles.pop_back();
                obstacles.pop_back();
            }
            std::cout << obstacles.size() << std::endl;
        }

        for (auto& obstacle : obstacles)
        {
            obstacle.getShape().move(0, velocity * dt);
        }
        window.clear(sf::Color(10, 0, 30));
        window.draw(dividers);
        for (auto& o : obstacles)
            window.draw(o);
        window.display();
    }

    return 0;
}
