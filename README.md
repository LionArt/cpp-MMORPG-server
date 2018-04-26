# MMORPG Server

<a href="https://i.imgur.com/T4phSiO.gif"><img src="https://i.imgur.com/T4phSiO.gif" title="Treasuria"/></a>

Server of basic MMORPG game made with learning purpose. Uses MySQL Connector to connect with database and SFML multimedia library.

To config database connection, change one line in Source/Application.cpp

"if(!databaseConnection.establishConnection("localhost","root","","treasuria"))"

establishConnection() method takes 4 parameters: host, user, password, database name.

Server is using port 30000 for connection. You can change port in Headers/ConnectionManager.hpp. If you change it for server, remember to change it for client as well.

Map size is defined in Headers/Application.hpp and world is text file located at /World. I used simple parser to parse XML file from Tiled Map Editor. 

Server is really simple, not optimalized but works. It's main purpose was to learn how MMORPG games are made using very basic multimedia library instead of working game engine.
