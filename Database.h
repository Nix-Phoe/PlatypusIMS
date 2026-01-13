#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include <libpq-fe.h>
#include <string>

class Database {
private:
    PGconn* conn; 

public:
    // Constructeur 
    Database(const std::string& conninfo);

    // Destructeur 
    ~Database();

    // Méthodes utiles
    bool isConnected();              
    PGconn* getConnection();        
    std::string getErrorMessage();   
};

#endif