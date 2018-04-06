#include "Client.hpp"

Client::Client(sf::TcpSocket *_socket)
{
    socket=_socket;
    state=ClientState::CONNECTED;
    heartbeat.restart();
}

Client::~Client()
{
    socket->disconnect();
    delete socket;
}

int32_t Client::getLastHearbeat()
{
    return heartbeat.getElapsedTime().asMilliseconds();
}

void Client::closeConnection()
{
    socket->disconnect();
    state=ClientState::DISCONNECTED;
}

void Client::restartTimer()
{
    heartbeat.restart();
}
