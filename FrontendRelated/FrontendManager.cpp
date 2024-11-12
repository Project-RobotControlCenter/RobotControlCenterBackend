//
// Created by elejandc on 11/12/24.
//

#include "FrontendManager.h"

#include "FrontendConnectionListener.h"

std::unique_ptr<FrontendManager> FrontendManager::_instance = nullptr;

FrontendManager::FrontendManager(asio::io_context &ioc, unsigned short _frontend_connection_port)
    : _ioc(ioc) {
    std::cout << "DEBUG : FrontendManager - CONSTRUCTOR" << std::endl;

    if(FrontendConnectionListener::initInstance(_ioc, asio::ip::tcp::endpoint(asio::ip::address_v4::any(), _frontend_connection_port), std::bind(&FrontendManager::onNewFrontendConnection, this, std::placeholders::_1))) {
        std::cout << "INFO : FrontendManager - Listener initialized" << std::endl;
        FrontendConnectionListener::run();
    } else {
        std::cerr << "ERROR : FrontendManager - Listener initialization failed" << std::endl;
    }
}

FrontendManager::~FrontendManager() {
    std::cout << "DEBUG : FrontendManager - DESTRUCTOR" << std::endl;
}

void FrontendManager::onNewFrontendConnection(websocket::stream<tcp::socket> robot_websocket) {
    std::cout << "INFO : FrontendManager - New frontend connection established" << std::endl;
}