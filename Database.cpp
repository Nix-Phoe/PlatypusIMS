#include "database.h"
#include <iostream>

//Connexion
Database::Database(const std::string& conninfo) {
    conn = PQconnectdb(conninfo.c_str());

    if (!isConnected()) {
        std::cerr << " Echec de connexion!" << std::endl;
    }
}

//Deconnexion
Database::~Database() {
    if (conn) {
        PQfinish(conn);
        std::cout << " Connexion fermee." << std::endl;
    }
}


bool Database::isConnected() {
    return PQstatus(conn) == CONNECTION_OK;
}


PGconn* Database::getConnection() {
    return conn;
}


std::string Database::getErrorMessage() {
    return PQerrorMessage(conn);
}