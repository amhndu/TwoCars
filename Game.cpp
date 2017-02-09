#include "Game.h"
#include <cstdlib>
#include <iterator>

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

    m_leftCar .setKey(sf::Keyboard::Z);
    m_rightCar.setKey(sf::Keyboard::M);

    //TODO What should I do if loading this fails ?
    m_font.loadFromFile("assets/font.ttf");
    m_prompt.setFont(m_font);
    m_prompt.setColor(sf::Color(180, 180, 180));
    m_prompt.setCharacterSize(20);
    m_prompt.setString("Take all Circles, avoid all Triangles.\n"
                       "Control left car with Z, right with M.\n"
                       "       Press Space to start.");
    m_prompt.setPosition((m_window.getSize().x - m_prompt.getLocalBounds().width) / 2.f,
                         (m_window.getSize().y - m_prompt.getLocalBounds().height) / 2.f);

    m_overlayBg.setFillColor(sf::Color(0, 0, 0, 100));

    //If these fail to load, simple Circles/Rectangles will be used.
    Obstacle::m_circleTexture.loadFromFile("assets/circle.png");
    Obstacle::m_triangleTexture.loadFromFile("assets/triangle.png");
    Car::m_carTexture.loadFromFile("assets/car.png");
    m_leftCar.applyTexture();
    m_rightCar.applyTexture();

}

void Game::newGame()
{
    m_obstacles.clear();
    m_score = -OBJS_ON_SCREEN / 2;
    m_velocity = INITIAL_VELOCITY;
    m_distance = SPAWN_DIST;
    m_playing = true;
    m_leftCar.reset();
    m_rightCar.reset();
}

void Game::run()
{
    srand(time(nullptr));
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
                ++m_score;
                m_obstacles.emplace_front(static_cast<Obstacle::Type>(rand() % 2),
                                            LEFT_COLOR,
                                            sf::Vector2f{LANE_WIDTH / 2.f + LANE_WIDTH * (rand() % 2), 0});
                m_obstacles.emplace_front(static_cast<Obstacle::Type>(rand() % 2),
                                            RIGHT_COLOR,
                                            sf::Vector2f{LANE_WIDTH * 2.f + LANE_WIDTH / 2 + LANE_WIDTH * (rand() % 2), 0});
                m_distance -= SPAWN_DIST;
            }

            for(auto it = m_obstacles.begin(); it != m_obstacles.end(); ++it)
            {
                it->getShape().move(0, m_velocity * dt);
                if (it->getShape().getGlobalBounds().top > WINDOW_HEIGHT - CAR_HEIGHT - OBJECT_SIZE
                    && it->getShape().getGlobalBounds().top < WINDOW_HEIGHT - OBJECT_SIZE)
                {
                    auto& car = it->getShape().getGlobalBounds().left < 2 * LANE_WIDTH ? m_leftCar : m_rightCar;
                    Car::Lane lane = static_cast<int>(it->getShape().getGlobalBounds().left / LANE_WIDTH) % 2 ? Car::Right : Car::Left;
                    if (lane == car.getLane())
                    {
                        if (it->getType() == Obstacle::Triangle)
                        {
                            gameOver();
                            break;
                        }
                        it = std::prev(m_obstacles.erase(it));
                    }
                }
                else if (it->getShape().getGlobalBounds().top > WINDOW_HEIGHT - OBJECT_SIZE)
                {
                    if (it->getType() == Obstacle::Circle)
                    {
                        gameOver();
                        break;
                    }
                    it = std::prev(m_obstacles.erase(it));
                }
            }

            m_leftCar.update(dt);
            m_rightCar.update(dt);
        }


        m_window.clear(BACKGROUND_COLOR);
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
        || (carLane != objLane && type == Obstacle::Triangle))
            return false;
    return true;
}

void Game::gameOver()
{
    m_playing = false;
    m_prompt.setString(" You scored " + std::to_string(m_score) + ".\n"
        " Press Space to restart.");
    m_prompt.setPosition(0,
            (m_window.getSize().y - m_prompt.getLocalBounds().height) / 2.f);
}
