#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#define MESSAGE_LIFETIME 3

#include <iostream>
#include "SFML/System.hpp"

class Message
{
    public:
    Message(std::string text,sf::Vector2f origin,std::string owner);
    std::string text;
    sf::Vector2f origin;
    std::string owner;
    bool isAlive();
    private:
    sf::Clock lifetime;
};

#endif // MESSAGE_HPP
