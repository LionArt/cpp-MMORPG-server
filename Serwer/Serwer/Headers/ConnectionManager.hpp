#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#define HEARTBEAT_TIMEOUT 5000

#include "SFML/Network.hpp"
#include <iostream>
#include <mutex>
#include <fstream>
#include <sstream>
#include "Client.hpp"
#include "Message.hpp"
#include "DatabaseConnection.hpp"
#include "Version.hpp"
#define PORT 30000
#define COLISION_UNITS 42

enum class ServerState {OFFLINE,ONLINE,PRIVATE};
enum class PacketHeader {DISCONNECT,HEARTBEAT,HANDSHAKE,LOGIN,CHARACTER_SELECT,CONFIRM_LOGIN,GAME,PLAYER_MOVE,MESSAGE};
enum class LoginResponse {ACCEPTED,LOGGED,LOGIN_ERROR};
enum class ConnectionError {DEFAULT,DENIED,TIMEOUT,WRONG_DATA,ACCOUNT_LOGGED,CHARACTER_LOGGED};


class ConnectionManager
{
    unsigned int blockable[COLISION_UNITS]={926,927,958,959,990,991,1022,1023,1019,955,987,510,542,574,606,392,356,358,420,422,324,295,296,297,327,328,329,458,452,453,454,455,456,598,403,404,435,436,397,398,429,430};
    sf::SocketSelector selector;
    sf::TcpListener listener;
    bool isAcceptingConnections;
    std::mutex m_mutex;
    std::vector<Client*> clients;
    public:
    ConnectionManager();
    ~ConnectionManager();
    bool test=false;
    std::vector<Message> messages;
    std::vector<Player> players;
    std::vector<unsigned int> world;
    ServerState state=ServerState::OFFLINE;
    sf::Packet serialize(DatabaseConnection dbc,Client &client, PacketHeader header);
    void deserialize(DatabaseConnection dbc,Client &client, sf::Packet &packet);
    bool initializeGameWorld(std::string file);
    bool sendPacket(Client &client, sf::Packet packet);
    bool receivePacket(Client &client,sf::Packet &packet);
    void startListening();
    void closeConnection();
    void blockConnections();
    void packetTransfer(DatabaseConnection databaseConnection);
};

#endif // CONNECTION_MANAGER_HPP
