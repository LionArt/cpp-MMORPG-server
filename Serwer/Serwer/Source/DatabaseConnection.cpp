#include "DatabaseConnection.hpp"

bool DatabaseConnection::establishConnection(std::string host,std::string user,std::string password, std::string database)
{
    connection=mysql_init(0);
    connection= mysql_real_connect(connection,host.c_str(),user.c_str(),password.c_str(),database.c_str(),0,NULL,0);
    if(connection)
    {
        std::cout<<"Database connection established!"<<std::endl;
        return true;
    }
    else
    {
        std::cout<<"Can't connect to database!"<<std::endl;
        return false;
    }
}

unsigned int DatabaseConnection::checkAccount(std::string login,std::string password)
{
    std::string query="SELECT id from accounts where login='"+login+"' and password='"+password+"'";
    state=mysql_query(connection,query.c_str());
    if(!state)
    {
        result=mysql_store_result(connection);
        if(mysql_num_rows(result)==0)
        {
            mysql_free_result(result);
            return 0;
        }
        else
        {
            row=mysql_fetch_row(result);
            std::string id=row[0];
            mysql_free_result(result);
            return atoi(id.c_str());
        }
    }
    else
    return 0;
}

std::vector<std::string> DatabaseConnection::getCharacterNames(unsigned int accountID)
{
    std::vector<std::string> names;
    std::string tmp;
    sprintf((char*)tmp.c_str(), "%d", accountID);
    std::string _accountID = tmp.c_str();
    std::string query="SELECT name from players where account_id='"+_accountID+"'";
    state=mysql_query(connection,query.c_str());
    if(!state)
    {
        result=mysql_store_result(connection);
        if(mysql_num_rows(result)==0)
        {
            mysql_free_result(result);
            return names;
        }
        else
        {
            while(row=mysql_fetch_row(result))
            {
                names.push_back(row[0]);
            }
            mysql_free_result(result); //Bez tej linijki pamiec sie wyczerpywala i byl blad z brakiem pamieci
            //out of memory, needed 8164 bytes
        }
    }
    return names;
}


Player DatabaseConnection::getCharacter(unsigned int accountID,unsigned int characterID)
{
    std::string tmp;
    sprintf((char*)tmp.c_str(), "%d", accountID);
    std::string _accountID = tmp.c_str();
    std::string query="SELECT * from players where account_id="+_accountID+"";
    Player player;
    state=mysql_query(connection,query.c_str());
    if(!state)
    {
        result=mysql_store_result(connection);
        if(mysql_num_rows(result)==0 || mysql_num_rows(result)<characterID)
        {
            mysql_free_result(result);
            return player;
        }
        else
        {
            std::string id;
            std::string name;
            std::string posx;
            std::string posy;
            unsigned int row_num=0;
            while(row=mysql_fetch_row(result))
            {
                row_num++;
                if(row_num==characterID)
                {
                    id=row[0];
                    name=row[2];
                    posx=row[3];
                    posy=row[4];
                }
            }
            player.id=atoi(id.c_str());
            player.name=name;
            player.position.x=atoi(posx.c_str());
            player.position.y=atoi(posy.c_str());
            mysql_free_result(result);
            return player;
        }
    }
    else
    return player;
}

