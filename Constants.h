#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <SFML/Graphics/Color.hpp>

const int LANE_WIDTH = 80;
const int OBJECT_SIZE = LANE_WIDTH / 2;
const int CAR_HEIGHT = LANE_WIDTH;
const int WINDOW_HEIGHT = 600;
const int SPAWN_DIST = 250;
const int INITIAL_VELOCITY = 220;
const int ACCELERATION = 5;
const int OBJS_ON_SCREEN = 2 * (1 + WINDOW_HEIGHT / (SPAWN_DIST + OBJECT_SIZE));

const sf::Color LEFT_COLOR = sf::Color(0xFF4758FF);
const sf::Color RIGHT_COLOR = sf::Color(0x1EBAF1FF);
const sf::Color BACKGROUND_COLOR = sf::Color(0x211357FF);
#endif //CONSTANTS_H