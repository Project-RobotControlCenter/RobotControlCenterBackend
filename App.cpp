//
// Created by elejandc on 11/11/24.
//

#include "App.h"

#include <iostream>

#include "Robots/RobotManager.h"

App::App(int argc, const char *argv[])
    : _db_local_ip(argv[1]), _db_local_port(argv[2]), _db_remove_ip(argv[3]), _db_remove_port(argv[4]), _db_password(argv[5]), _frontend_websocket_port(argv[6]), _robots_websocket_port(argv[7]){
    std::cout << "DEBUG : App - CONSTRUCTOR" << std::endl;

    asio::io_context ioc{1};
    RobotManager::initInstance(ioc, std::stoi(_robots_websocket_port));
}

App::~App() {
    std::cout << "DEBUG : App - DESTRUCTOR" << std::endl;
}
