#ifndef CLIENT_HPP
#define CLIENT_HPP

#define CLIENT_MAP_WIDTH 29
#define CLIENT_MAP_HEIGHT 19

#include <iostream>
#include "SFML/Network.hpp"
#include "Player.hpp"

enum class ClientState{DISCONNECTED,CONNECTED,HANDSHAKED,LOGGED,DATABASE,CHARACTER_SELECTED};

class Client
{
    public:
    ~Client();
    sf::Clock heartbeat;
    sf::Clock update;
    sf::Clock lastMove;
    int32_t getLastHearbeat();
    Client(sf::TcpSocket *_socket);
    sf::TcpSocket *socket;
    ClientState state;
    std::string version="";
    uint8_t serverReply=0;
    unsigned int accountID=0;
    unsigned int characterID=0;
    void restartTimer();
    void closeConnection();
};

#endif // CLIENT_HPP
