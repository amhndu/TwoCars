#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <SFML/Graphics/Color.hpp>

const int LANE_WIDTH = 80;
const int OBJECT_SIZE = LANE_WIDTH / 2;
const int CAR_HEIGHT = 2 * LANE_WIDTH / 3;
const int WINDOW_HEIGHT = 700;
const int SPAWN_DIST = 250;
const int INITIAL_VELOCITY = 150;
const int ACCELERATION = 5;
const int OBJS_ON_SCREEN = 2 * (1 + WINDOW_HEIGHT / (SPAWN_DIST + OBJECT_SIZE));

const sf::Color LEFT_COLOR = sf::Color::Red;
const sf::Color RIGHT_COLOR = sf::Color::Blue;

#endif //CONSTANTS_H