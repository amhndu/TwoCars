#include "Game.h"

Game::Game() :
    window(sf::VideoMode(LANE_WIDTH * 4, WINDOW_HEIGHT), "TwoCars", sf::Style::Close | sf::Style::Titlebar),
    dividers(sf::Lines, 6),
    leftCar (LEFT_COLOR,  sf::Vector2f{LANE_WIDTH, WINDOW_HEIGHT}),
    rightCar(RIGHT_COLOR, sf::Vector2f{LANE_WIDTH * 3, WINDOW_HEIGHT}),
    score(0)
{
    dividers[0] = sf::Vertex({LANE_WIDTH, 0}, sf::Color(180, 180, 180));
    dividers[1] = sf::Vertex({LANE_WIDTH, WINDOW_HEIGHT}, sf::Color(180, 180, 180));
    dividers[2] = sf::Vertex({LANE_WIDTH * 3, 0}, sf::Color(180, 180, 180));
    dividers[3] = sf::Vertex({LANE_WIDTH * 3, WINDOW_HEIGHT}, sf::Color(180, 180, 180));
    dividers[4] = sf::Vertex({LANE_WIDTH * 2, 0}, sf::Color(180, 180, 180));
    dividers[5] = sf::Vertex({LANE_WIDTH * 2, WINDOW_HEIGHT}, sf::Color(180, 180, 180));

    leftCar .setKey(sf::Keyboard::F);
    rightCar.setKey(sf::Keyboard::J);

    newGame();
}

void Game::newGame()
{
    obstacles.clear();
    score = 0;
    velocity = INITIAL_VELOCITY;
    distance = SPAWN_DIST;
    playing = true;
}

void Game::run()
{
    sf::Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (playing)
            {
                leftCar.handleInput(event);
                rightCar.handleInput(event);
            }
        }

        auto dt = timer.restart().asSeconds();
        distance += velocity * dt;
        if (playing)
            velocity += ACCELERATION * dt;

        if (playing && distance > SPAWN_DIST)
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

        if (playing) for (auto& obstacle : obstacles)
        {
            obstacle.getShape().move(0, velocity * dt);
        }

        if (obstacles.back().getShape().getGlobalBounds().top > WINDOW_HEIGHT - CAR_HEIGHT - OBJECT_SIZE)
        {
            auto &left = obstacles.back();
            Car::Lane lLane = left.getShape().getGlobalBounds().left < LANE_WIDTH ? Car::Left : Car::Right;
            Obstacle::Type lType = left.getType();

            auto &right = obstacles[obstacles.size() - 2];
            Car::Lane rLane = right.getShape().getGlobalBounds().left < 3 * LANE_WIDTH ? Car::Left : Car::Right;
            Obstacle::Type rType = right.getType();

            if (isGameOver(lLane, leftCar.getLane(), lType) || isGameOver(rLane, rightCar.getLane(), rType))
            {
                playing = false;
                std::cout << "You scored " << score << " points." << std::endl;
            }
            else
                ++score;

            obstacles.pop_back();
            obstacles.pop_back();
        }

        window.clear(sf::Color(10, 0, 30));
        window.draw(dividers);
        for (auto& o : obstacles)
            window.draw(o);
        window.draw(leftCar);
        window.draw(rightCar);
        window.display();
    }
}

bool Game::isGameOver(Car::Lane carLane, Car::Lane objLane, Obstacle::Type type)
{
    if ((carLane == objLane && type == Obstacle::Circle)
        || (carLane != objLane && type == Obstacle::Square))
            return false;
    return true;
}
