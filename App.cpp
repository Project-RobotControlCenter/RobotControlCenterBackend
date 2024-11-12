//
// Created by elejandc on 11/11/24.
//

#include "App.h"

#include <iostream>

#include "DatabaseRelated/DatabaseManager.h"
#include "FrontendRelated/FrontendManager.h"
#include "Robots/RobotManager.h"

std::unique_ptr<App> App::_instance = nullptr;

App::App(int argc, const char *argv[])
    : _db_local_ip(argv[1]), _db_local_port(argv[2]), _db_remove_ip(argv[3]), _db_remove_port(argv[4]), _db_password(argv[5]), _frontend_websocket_port(argv[6]), _robots_websocket_port(argv[7]){
    std::cout << "DEBUG : App - CONSTRUCTOR" << std::endl;

    asio::io_context ioc{1};

    if(DatabaseManager::initInstance(_db_local_ip, _db_local_port, _db_remove_ip, _db_remove_port, _db_password)) {
        std::cout << "INFO : App - DatabaseManager initialized" << std::endl;
    }

    if(RobotManager::initInstance(ioc, std::stoi(_robots_websocket_port))) {
        std::cout << "INFO : App - RobotManager initialized" << std::endl;
    }

    if(FrontendManager::initInstance(ioc, std::stoi(_frontend_websocket_port))) {
        std::cout << "INFO : App - FrontendManager initialized" << std::endl;
    }

    ioc.run();
}

App::~App() {
    std::cout << "DEBUG : App - DESTRUCTOR" << std::endl;
}
