#include "Message.hpp"

Message::Message(std::string _text,sf::Vector2f pos,std::string _owner)
{
    text=_text;
    origin=pos;
    owner=_owner;
    lifetime.restart();
}

bool Message::isAlive()
{
    if(lifetime.getElapsedTime().asSeconds()>MESSAGE_LIFETIME)
    return false;
    else
    return true;
}
