#include "Application.hpp"

void Application::run()
{
    std::cout<<SERVER_NAME<<" "<<SERVER_VERSION<<std::endl;
    std::cout<<SERVER_COPYRIGHT<<std::endl;
    std::cout<<"Establishing database connection ..."<<std::endl;
    if(!databaseConnection.establishConnection("localhost","root","","treasuria"))
    return;
    if(!connectionManager.initializeGameWorld("Data/World/map.txt"))
    return;
    std::cout<<"Starting server ..."<<std::endl;
    connectionManager.state=ServerState::ONLINE;
    std::cout<<"Server is running!"<<std::endl;
    std::thread listening(&ConnectionManager::startListening,&connectionManager);
    std::thread communication(&ConnectionManager::packetTransfer,&connectionManager,databaseConnection);
    while(connectionManager.state==ServerState::ONLINE)
    {
    }
    connectionManager.closeConnection();
    listening.detach();
    communication.detach();
}
