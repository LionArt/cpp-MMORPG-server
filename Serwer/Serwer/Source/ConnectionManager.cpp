#include <ConnectionManager.hpp>

ConnectionManager::ConnectionManager()
{
    isAcceptingConnections=true;
}

ConnectionManager::~ConnectionManager()
{
    for(unsigned int i=0;i<clients.size();i++)
    delete clients[i];
    clients.clear();
    players.clear();
    messages.clear();
}

bool ConnectionManager::initializeGameWorld(std::string file)
{
    std::cout<<"Loading world map..."<<std::endl;
    std::fstream temp;
    std::string line;
    temp.open(file,std::ios::in);
    if(!temp.is_open())
    {
        std::cout<<"Failed to load the map!"<<std::endl;
        return false;
    }
    else
    {
        while(getline(temp,line))
        {
            std::stringstream linestream(line);
            std::string value;
            while(getline(linestream,value,','))
            {
                unsigned int _value=atoi(value.c_str());
                world.push_back(_value);
            }
        }
        world[0]=392;
        temp.close();
        std::cout<<"Map loaded!"<<std::endl;
        return true;
    }
}

void ConnectionManager::startListening()
{
    if(listener.Disconnected)
    listener.listen(PORT);
    while(isAcceptingConnections==true)
    {
        sf::TcpSocket *socket=new sf::TcpSocket;
        if (listener.accept(*socket) == sf::Socket::Done)
        {
            socket->setBlocking(false);
            Client *client=new Client(socket);
            clients.push_back(client);
        }
        else
        delete socket;
    }
}

void ConnectionManager::blockConnections()
{
    isAcceptingConnections=false;
}

void ConnectionManager::closeConnection()
{
    blockConnections();
    clients.clear();
    listener.close();
}

bool ConnectionManager::sendPacket(Client &client,sf::Packet packet)
{
    if(client.socket->send(packet)==sf::Socket::Done)
    return true;
    else
    return false;
}


bool ConnectionManager::receivePacket(Client &client,sf::Packet &packet)
{
    if(client.socket->receive(packet)==sf::Socket::Done)
    return true;
    else
    return false;
}

sf::Packet ConnectionManager::serialize(DatabaseConnection dbc,Client &client, PacketHeader header)
{
    sf::Packet packet;
    packet<<static_cast<uint8_t>(header);
    switch(header)
    {
        case PacketHeader::HEARTBEAT:
        {
            break;
        }
        case PacketHeader::HANDSHAKE:
        {
            break;
        }
        case PacketHeader::LOGIN:
        {
            std::vector<std::string> names;
            names=dbc.getCharacterNames(client.accountID);
            uint8_t size=static_cast<uint8_t>(names.size());
            packet<<size;
            for(unsigned int i=0;i<names.size();i++)
            packet<<names[i];
            break;
        }
        case PacketHeader::GAME:
        {
            //31x19
            //16x10
            unsigned int px=0;
            unsigned int py=0;
            for(unsigned int i=0;i<players.size();i++)
            {
                if(players[i].id==client.characterID)
                {
                    px=players[i].position.x;
                    py=players[i].position.y;
                    break;
                }
            }
            unsigned int posx=px-14;
            unsigned int posy=py-9;
            unsigned int x=posx+((posy-1)*100);
            unsigned int y=x;
            std::vector<unsigned int> snapshot;
            for(unsigned int i=0;i<CLIENT_MAP_HEIGHT;i++)
            {
                for(unsigned int j=0;j<CLIENT_MAP_WIDTH;j++)
                {
                    if(x<0 || x>9999)
                    snapshot.push_back(0);
                    else
                    snapshot.push_back(world[x]);
                    x=x+1;
                }
                x=y+(100*(i+1));
            }
            x=posx+((posy-1)*100)+10000;
            y=x;
            for(unsigned int i=0;i<CLIENT_MAP_HEIGHT;i++)
            {
                for(unsigned int j=0;j<CLIENT_MAP_WIDTH;j++)
                {
                    if(x<10000 || x>19999)
                    snapshot.push_back(0);
                    else
                    snapshot.push_back(world[x]);
                    x=x+1;
                }
                x=y+(100*(i+1));
            }
            for(unsigned int i=0;i<snapshot.size();i++)
            packet<<static_cast<uint16_t>(snapshot[i]);
            std::vector<Player> _players;
            for(unsigned int i=0;i<players.size();i++)
            {
                if(players[i].id!=client.characterID)
                {
                    if((players[i].position.x)>=px-14 && (players[i].position.x)<=px+14)
                    if((players[i].position.y)>=py-9 && (players[i].position.y)<=py+9)
                    {
                        Player player;
                        if(players[i].position.x>=px-14)
                        player.position.x=players[i].position.x-px;
                        else
                        player.position.x=px-players[i].position.x;

                        if(players[i].position.y>=py-9)
                        player.position.y=players[i].position.y-py;
                        else
                        player.position.y=py-players[i].position.y;
                        player.position.y=int(player.position.y+9);
                        player.position.x=int(player.position.x+14);
                        player.animationCounter=players[i].animationCounter;
                        player.direction=players[i].direction;
                        player.name=players[i].name;
                        _players.push_back(player);
                    }
                }
            }
            packet<<static_cast<uint16_t>(_players.size());
            for(unsigned int i=0;i<_players.size();i++)
            packet<<_players[i].name<<_players[i].position.x<<_players[i].position.y<<static_cast<uint8_t>(_players[i].direction)<<static_cast<int8_t>(_players[i].animationCounter);
            std::vector<Message> _messages;
            for(unsigned int i=0;i<messages.size();i++)
            if(!messages[i].isAlive())
            {
                messages.erase(messages.begin()+i,messages.begin()+i+1);
            }
            for(unsigned int i=0;i<messages.size();i++)
            {
                if((messages[i].origin.x)>=px-14 && (messages[i].origin.x)<=px+14)
                if((messages[i].origin.y)>=py-9 && (messages[i].origin.y)<=py+9)
                {
                    sf::Vector2f pos;
                    if(messages[i].origin.x>=px-14)
                    pos.x=messages[i].origin.x-px;
                    else
                    pos.x=px-messages[i].origin.x;
                    if(messages[i].origin.y>=py-9)
                    pos.y=messages[i].origin.y-py;
                    else
                    pos.y=py-messages[i].origin.y;
                    pos.y=int(pos.y+9);
                    pos.x=int(pos.x+14);
                    Message message(messages[i].text,pos,messages[i].owner);
                    _messages.push_back(message);
                }
            }
            packet<<static_cast<uint16_t>(_messages.size());
            for(unsigned int i=0;i<_messages.size();i++)
            packet<<_messages[i].text<<_messages[i].origin.x<<_messages[i].origin.y<<_messages[i].owner;
            break;
        }
        case PacketHeader::DISCONNECT:
        {
            packet<<client.serverReply;
            break;
        }
        default:
        break;
    }
    return packet;
}

void ConnectionManager::deserialize(DatabaseConnection dbc,Client &client, sf::Packet &packet)
{
    uint8_t _packetHeader;
    if(packet>>_packetHeader)
    {
        PacketHeader packetHeader=static_cast<PacketHeader>(_packetHeader);
        switch(packetHeader)
        {
            case PacketHeader::HEARTBEAT:
            {
                client.restartTimer();
                break;
            }
            case PacketHeader::HANDSHAKE:
            {
                if(client.state==ClientState::CONNECTED)
                {
                    std::string version;
                    if(packet>>version)
                    {
                       if(version==CLIENT_VERSION)
                       {
                           client.state=ClientState::HANDSHAKED;
                       }
                       else
                       {
                            client.serverReply=static_cast<uint8_t>(ConnectionError::DENIED);
                            client.state=ClientState::DISCONNECTED;
                       }
                    }
                }
                break;
            }
            case PacketHeader::LOGIN:
            {
                if(client.state==ClientState::HANDSHAKED)
                {
                    std::string login;
                    std::string password;
                    if(packet>>login>>password)
                    {
                        unsigned int tempID;
                        tempID=dbc.checkAccount(login,password);
                        if(tempID!=0)
                        {
                            bool logged=false;
                            for(unsigned int i=0;i<clients.size();i++)
                            {
                                if(clients[i]->accountID==tempID)
                                {
                                    logged=true;
                                    break;
                                }
                            }
                            if(!logged)
                            {
                                client.accountID=tempID;
                                client.state=ClientState::LOGGED;
                            }
                            else
                            {
                                client.serverReply=static_cast<uint8_t>(ConnectionError::ACCOUNT_LOGGED);
                                client.state=ClientState::DISCONNECTED;
                            }
                        }
                        else
                        {
                            client.serverReply=static_cast<uint8_t>(ConnectionError::WRONG_DATA);
                            client.state=ClientState::DISCONNECTED;
                        }
                    }
                }
                break;
            }
            case PacketHeader::CHARACTER_SELECT:
            {
                if(client.state==ClientState::LOGGED)
                {
                    uint8_t tempID;
                    if(packet>>tempID)
                    {
                        if(tempID!=0)
                        {
                            Player player=dbc.getCharacter(client.accountID,tempID);
                            if(player.id!=0)
                            {
                                bool logged=false;
                                for(unsigned int i=0;i<players.size();i++)
                                {
                                    if(players[i].id==player.id)
                                    {
                                        logged=true;
                                        break;
                                    }
                                }
                                if(!logged)
                                {
                                    client.characterID=player.id;
                                    players.push_back(player);
                                    client.state=ClientState::CHARACTER_SELECTED;
                                    client.lastMove.restart();
                                    client.update.restart();
                                }
                                else
                                {
                                    client.serverReply=static_cast<uint8_t>(ConnectionError::CHARACTER_LOGGED);
                                    client.state=ClientState::DISCONNECTED;
                                }
                            }
                        }
                    }
                }
                break;
            }
            case PacketHeader::MESSAGE:
            {
                if(client.state==ClientState::CHARACTER_SELECTED)
                {
                    std::string msg;
                    if(packet>>msg)
                    {
                        for(unsigned int i=0;i<players.size();i++)
                        {
                            if(players[i].id==client.characterID)
                            {
                                Message message(msg,players[i].position,players[i].name);
                                messages.push_back(message);
                                break;
                            }
                        }
                    }
                }
                break;
            }
            case PacketHeader::PLAYER_MOVE:
            {
                int animCounter;
                unsigned int dir;
                uint8_t _dir;
                int8_t _animCounter;
                if(client.state==ClientState::CHARACTER_SELECTED)
                {
                    if(client.lastMove.getElapsedTime().asMilliseconds()>100)
                    {
                    if(packet>>_dir>>_animCounter)
                    {
                            client.lastMove.restart();
                            dir=static_cast<unsigned int>(_dir);
                            animCounter=static_cast<int>(_animCounter);
                            if(dir<4)
                            for(unsigned int i=0;i<players.size();i++)
                            {
                                if(players[i].id==client.characterID)
                                {
                                    if(animCounter!=-1)
                                    {
                                        bool block=false;
                                        if(dir==0)
                                        {
                                            unsigned int tile=world[(int(players[i].position.y)*100)+int(players[i].position.x)+10000];
                                            for(unsigned int j=0;j<COLISION_UNITS;j++)
                                            {
                                                if(tile==blockable[j])
                                                {
                                                    block=true;
                                                    break;
                                                }
                                            }
                                            if(!block)
                                            {
                                                tile=world[((int(players[i].position.y))*100)+int(players[i].position.x)];
                                                for(unsigned int j=0;j<COLISION_UNITS;j++)
                                                {
                                                    if(tile==blockable[j])
                                                    {
                                                        block=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            if(!block)
                                            players[i].position.y+=float(players[i].movementSpeed)/32;
                                        }
                                        else
                                        if(dir==1)
                                        {
                                            unsigned int tile=world[((int(players[i].position.y)-2)*100)+int(players[i].position.x)+10000];
                                            for(unsigned int j=0;j<COLISION_UNITS;j++)
                                            {
                                                if(tile==blockable[j])
                                                {
                                                    block=true;
                                                    break;
                                                }
                                            }
                                            if(!block)
                                            {
                                                tile=world[((int(players[i].position.y)-2)*100)+int(players[i].position.x)];
                                                for(unsigned int j=0;j<COLISION_UNITS;j++)
                                                {
                                                    if(tile==blockable[j])
                                                    {
                                                        block=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            if(!block)
                                            players[i].position.y-=float(players[i].movementSpeed)/32;
                                        }
                                        else
                                        if(dir==2)
                                        {
                                            unsigned int tile=world[((int(players[i].position.y)-1)*100)+int(players[i].position.x)+10000+1];
                                            for(unsigned int j=0;j<COLISION_UNITS;j++)
                                            {
                                                if(tile==blockable[j])
                                                {
                                                    block=true;
                                                    break;
                                                }
                                            }
                                            if(!block)
                                            {
                                                tile=world[((int(players[i].position.y)-1)*100)+int(players[i].position.x)+1];
                                                for(unsigned int j=0;j<COLISION_UNITS;j++)
                                                {
                                                    if(tile==blockable[j])
                                                    {
                                                        block=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            if(!block)
                                            players[i].position.x+=float(players[i].movementSpeed)/32;
                                        }
                                        else
                                        if(dir==3)
                                        {
                                            int tile=world[((int(players[i].position.y)-1)*100)+int(players[i].position.x)+10000-1];
                                            for(unsigned int j=0;j<COLISION_UNITS;j++)
                                            {
                                                if(tile==blockable[j])
                                                {
                                                    block=true;
                                                    break;
                                                }
                                            }
                                            if(!block)
                                            {
                                                unsigned int tile=world[((int(players[i].position.y)-1)*100)+int(players[i].position.x)-1];
                                                for(unsigned int j=0;j<COLISION_UNITS;j++)
                                                {
                                                    if(tile==blockable[j])
                                                    {
                                                        block=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            if(!block)
                                            players[i].position.x-=float(players[i].movementSpeed)/32;
                                        }
                                    }
                                    players[i].animationCounter=animCounter;
                                    players[i].direction=dir;
                                    break;
                                }
                            }
                            client.lastMove.restart();
                        }
                    }
                }
                break;
            }
            case PacketHeader::DISCONNECT:
            {
                client.state=ClientState::DISCONNECTED;
                client.closeConnection();
                break;
            }
            default:
            break;
        }
    }
}

void ConnectionManager::packetTransfer(DatabaseConnection databaseConnection)
{
    while(isAcceptingConnections)
    {
        m_mutex.lock();
        for(unsigned int i=0;i<clients.size();i++)
        {
            if(clients[i]->getLastHearbeat()<HEARTBEAT_TIMEOUT)
            {
                sendPacket(*clients[i],serialize(databaseConnection,*clients[i],PacketHeader::HEARTBEAT));
                if(clients[i]->state!=ClientState::DISCONNECTED)
                {
                    sf::Packet received;
                    if(receivePacket(*clients[i],received))
                    deserialize(databaseConnection,*clients[i],received);
                }
                else
                {
                    sendPacket(*clients[i],serialize(databaseConnection,*clients[i],PacketHeader::DISCONNECT));
                }
                if(clients[i]->state==ClientState::HANDSHAKED)
                {
                    sendPacket(*clients[i],serialize(databaseConnection,*clients[i],PacketHeader::HANDSHAKE));
                }
                else if(clients[i]->state==ClientState::LOGGED)
                {
                    sendPacket(*clients[i],serialize(databaseConnection,*clients[i],PacketHeader::LOGIN));
                }
                else if(clients[i]->state==ClientState::CHARACTER_SELECTED)
                {
                    if(clients[i]->update.getElapsedTime().asMilliseconds()>1)
                    {
                        sendPacket(*clients[i],serialize(databaseConnection,*clients[i],PacketHeader::GAME));
                        clients[i]->update.restart();
                    }
                }
            }
            else
            {
                for(unsigned int j=0;j<players.size();j++)
                {
                    if(players[j].id==clients[i]->characterID)
                    {
                        players.erase(players.begin()+j,players.begin()+j+1);
                        break;
                    }
                }
                delete clients[i];
                clients.erase(clients.begin()+i,clients.begin()+i+1);
            }
        }
        m_mutex.unlock();
    }
}
