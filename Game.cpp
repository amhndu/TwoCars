#include "Game.h"

Game::Game() :
    m_window(sf::VideoMode(LANE_WIDTH * 4, WINDOW_HEIGHT), "TwoCars", sf::Style::Close | sf::Style::Titlebar),
    m_dividers(sf::Lines, 6),
    m_leftCar (LEFT_COLOR,  sf::Vector2f{LANE_WIDTH, WINDOW_HEIGHT}),
    m_rightCar(RIGHT_COLOR, sf::Vector2f{LANE_WIDTH * 3, WINDOW_HEIGHT}),
    m_overlayBg({LANE_WIDTH * 4, WINDOW_HEIGHT}),
    m_playing(false)
{
    m_dividers[0] = sf::Vertex({LANE_WIDTH, 0}, sf::Color(180, 180, 180));
    m_dividers[1] = sf::Vertex({LANE_WIDTH, WINDOW_HEIGHT}, sf::Color(180, 180, 180));
    m_dividers[2] = sf::Vertex({LANE_WIDTH * 3, 0}, sf::Color(180, 180, 180));
    m_dividers[3] = sf::Vertex({LANE_WIDTH * 3, WINDOW_HEIGHT}, sf::Color(180, 180, 180));
    m_dividers[4] = sf::Vertex({LANE_WIDTH * 2, 0}, sf::Color(180, 180, 180));
    m_dividers[5] = sf::Vertex({LANE_WIDTH * 2, WINDOW_HEIGHT}, sf::Color(180, 180, 180));

    m_leftCar .setKey(sf::Keyboard::F);
    m_rightCar.setKey(sf::Keyboard::J);

    m_font.loadFromFile("font.ttf");
    m_prompt.setFont(m_font);
    m_prompt.setColor(sf::Color(180, 180, 180));
    m_prompt.setCharacterSize(20);
    m_prompt.setString("Take all Circles, avoid all Squares.\n"
                       "Control left car with F, right with L.\n"
                       "       Press Space to start.");
    m_prompt.setPosition((m_window.getSize().x - m_prompt.getLocalBounds().width) / 2.f,
                         (m_window.getSize().y - m_prompt.getLocalBounds().height) / 2.f);

    m_overlayBg.setFillColor(sf::Color(0, 0, 0, 150));

    //newGame();
}

void Game::newGame()
{
    m_obstacles.clear();
    m_score = 0;
    m_velocity = INITIAL_VELOCITY;
    m_distance = SPAWN_DIST;
    m_playing = true;
}

void Game::run()
{
    sf::Event event;
    while (m_window.isOpen())
    {
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                m_window.close();
            if (m_playing)
            {
                m_leftCar.handleInput(event);
                m_rightCar.handleInput(event);
            }
            else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space)
            {
                m_playing = true;
                newGame();
            }
        }

        auto dt = m_timer.restart().asSeconds();

        if (m_playing)
        {
            m_velocity += ACCELERATION * dt;
            m_distance += m_velocity * dt;

            if (m_distance > SPAWN_DIST)
            {
                //IMP: Order of insertion matters, the order is used to reference which side it belongs
                m_obstacles.emplace_front(static_cast<Obstacle::Type>(rand() % 2),
                                            LEFT_COLOR,
                                            sf::Vector2f{LANE_WIDTH / 2.f + LANE_WIDTH * (rand() % 2), 0});
                m_obstacles.emplace_front(static_cast<Obstacle::Type>(rand() % 2),
                                            RIGHT_COLOR,
                                            sf::Vector2f{LANE_WIDTH * 2.f + LANE_WIDTH / 2 + LANE_WIDTH * (rand() % 2), 0});
                m_distance -= SPAWN_DIST;
            }

            for (auto& obstacle : m_obstacles)
            {
                obstacle.getShape().move(0, m_velocity * dt);
            }

            if (m_obstacles.back().getShape().getGlobalBounds().top > WINDOW_HEIGHT - CAR_HEIGHT - OBJECT_SIZE)
            {
                auto &left = m_obstacles.back();
                Car::Lane lLane = left.getShape().getGlobalBounds().left < LANE_WIDTH ? Car::Left : Car::Right;
                Obstacle::Type lType = left.getType();

                auto &right = m_obstacles[m_obstacles.size() - 2];
                Car::Lane rLane = right.getShape().getGlobalBounds().left < 3 * LANE_WIDTH ? Car::Left : Car::Right;
                Obstacle::Type rType = right.getType();

                if (isGameOver(lLane, m_leftCar.getLane(), lType) || isGameOver(rLane, m_rightCar.getLane(), rType))
                {
                    m_playing = false;
                    m_prompt.setString(" You scored " + std::to_string(m_score) + ".\n"
                       " Press Space to restart.");
                    m_prompt.setPosition(0,
                         (m_window.getSize().y - m_prompt.getLocalBounds().height) / 2.f);
                }
                else
                    ++m_score;

                m_obstacles.pop_back();
                m_obstacles.pop_back();
            }
        }

        m_window.clear(sf::Color(10, 0, 30));
        m_window.draw(m_dividers);
        for (auto& o : m_obstacles)
            m_window.draw(o);
        m_window.draw(m_leftCar);
        m_window.draw(m_rightCar);
        if (!m_playing)
        {
            m_window.draw(m_overlayBg);
            m_window.draw(m_prompt);
        }
        m_window.display();
    }
}

bool Game::isGameOver(Car::Lane carLane, Car::Lane objLane, Obstacle::Type type)
{
    if ((carLane == objLane && type == Obstacle::Circle)
        || (carLane != objLane && type == Obstacle::Square))
            return false;
    return true;
}
