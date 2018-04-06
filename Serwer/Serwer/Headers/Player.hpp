#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "SFML/System.hpp"
class Player
{
    public:
    unsigned int id=0;
    std::string name;
    sf::Vector2f position;
    int animationCounter=-1;
    unsigned int direction=0;
    float movementSpeed=10.7;
};
#endif // PLAYER_HPP
