//
// Created by elejandc on 11/8/24.
//

#include "DatabaseManager.h"

#include <iostream>

DatabaseManager::DatabaseManager(const char *local_db_ip, const char *local_db_port, const char *remote_db_ip,
    const char *remote_db_port, const char *db_password) {

    // Connect to local database
    _client_local = connect(local_db_ip, local_db_port, db_password);
    if(_client_local) {
        std::cout << "INFO : DatabaseManager - CONNECTED TO LOCAL DATABASE" << std::endl;
    } else {
        std::cerr << "ERROR : DatabaseManager - FAILED TO CONNECT TO LOCAL DATABASE" << std::endl;
    }

    // Connect to remote database
    _client_remote = connect(remote_db_ip, remote_db_port, db_password);
    if(_client_remote) {
        std::cout << "INFO : DatabaseManager - CONNECTED TO REMOTE DATABASE" << std::endl;
    } else {
        std::cerr << "ERROR : DatabaseManager - FAILED TO CONNECT TO REMOTE DATABASE" << std::endl;
    }

    if(_client_local || _client_remote) {
        std::cout << "INFO : DatabaseManager - CONNECTED TO DATABASES" << std::endl;
    }
}

DatabaseManager::~DatabaseManager() {
    std::cout << "INFO : DatabaseManager - DESTRUCTOR" << std::endl;
}

std::unique_ptr<mongocxx::client> DatabaseManager::connect(const char *db_ip, const char *db_port, const char *db_password) {
    try {
        const std::string url = "mongodb://admin:" + std::string(db_password) + "@" + std::string(db_ip) + ":" + std::string(db_port);
        return std::move(std::make_unique<mongocxx::client>(mongocxx::uri{url}));
    }catch (const std::exception& e) {
        std::cerr << "ERROR : Can't connect to database: " << e.what() << std::endl;
        return nullptr;
    };
}