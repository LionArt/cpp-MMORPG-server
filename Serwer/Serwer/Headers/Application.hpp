#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#define DEFAULT_MAP_HEIGHT 100
#define DEFAULT_MAP_WIDTH 100
#include <iostream>
#include <thread>
#include "ConnectionManager.hpp"
#include "DatabaseConnection.hpp"
#include "Version.hpp"

class Application
{
    public:
    ConnectionManager connectionManager;
    DatabaseConnection databaseConnection;
    void run();
};

#endif // APPLICATION_HPP
