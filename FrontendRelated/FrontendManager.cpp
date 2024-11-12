//
// Created by elejandc on 11/12/24.
//

#include "FrontendManager.h"


FrontendManager::FrontendManager(asio::io_context &ioc, unsigned short _robot_connection_port)
    : _ioc(ioc) {
    std::cout << "DEBUG : FrontendManager - CONSTRUCTOR" << std::endl;
}

FrontendManager::~FrontendManager() {
    std::cout << "DEBUG : FrontendManager - DESTRUCTOR" << std::endl;
}
