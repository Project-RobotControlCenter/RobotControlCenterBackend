//
// Created by elejandc on 11/11/24.
//

#include "App.h"

#include <iostream>

#include "DatabaseRelated/DatabaseManager.h"
#include "FrontendRelated/FrontendManager.h"
#include "Robots/RobotManager.h"

std::unique_ptr<App> App::_instance = nullptr;

App::App(int argc, const char *argv[], asio::io_context &ioc)
    : _ioc(ioc), _db_local_ip(argv[1]), _db_local_port(argv[2]), _db_remove_ip(argv[3]), _db_remove_port(argv[4]), _db_password(argv[5]), _frontend_websocket_port(argv[6]), _robots_websocket_port(argv[7]){
    std::cout << "DEBUG : App - CONSTRUCTOR" << std::endl;

    if(DatabaseManager::initInstance(_db_local_ip, _db_local_port, _db_remove_ip, _db_remove_port, _db_password)) {
        std::cout << "INFO : App - DatabaseManager initialized" << std::endl;
    }

    if(RobotManager::initInstance(ioc, std::stoi(_robots_websocket_port))) {
        std::cout << "INFO : App - RobotManager initialized" << std::endl;
    }

    if(FrontendManager::initInstance(ioc, std::stoi(_frontend_websocket_port), std::bind(&App::onNewFrontendConnection, this, std::placeholders::_1))) {
        std::cout << "INFO : App - FrontendManager initialized" << std::endl;
    }

    ioc.run();
}

App::~App() {
    std::cout << "DEBUG : App - DESTRUCTOR" << std::endl;
}

void App::onNewFrontendConnection(websocket::stream<tcp::socket> frontend_websocket) {
    std::cout << "INFO : App - New frontend connection established" << std::endl;

    _sessions.emplace_back(std::make_shared<Session>(_session_id_count++, _ioc, std::move(frontend_websocket), std::bind(&App::onSessionEnd, this, std::placeholders::_1)));
    _sessions.back()->start();

    if(_session_id_count == UINT32_MAX - 1) {
        _session_id_count = 0;
    }
}

void App::onSessionEnd(const unsigned int session_id) {
    std::cout << "INFO : App - Session finished" << std::endl;

    // Output number of existing shared_ptr instances for the session before removal
    _sessions.erase(std::remove_if(_sessions.begin(), _sessions.end(), [session_id](const std::shared_ptr<Session> &session) {
        if (session->getSessionId() == session_id) {
            // Print the use_count before removing this session
            std::cout << "INFO: Number of shared_ptr instances for session before removal: " << session.use_count() << std::endl;
            return true; // Mark for removal
        }
        return false; // Do not remove
    }), _sessions.end());
}
