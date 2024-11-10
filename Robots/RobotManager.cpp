//
// Created by elejandc on 11/9/24.
//

#include "RobotManager.h"

#include "RobotConnectionListener.h"

std::unique_ptr<RobotManager> RobotManager::_instance = nullptr;

RobotManager::RobotManager(asio::io_context &ioc, unsigned short _robot_connection_port) {
    // Init Robot Connection Listener
    RobotConnectionListener::initInstance(ioc, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), _robot_connection_port), std::bind(&RobotManager::onNewRobotConnection, this, std::placeholders::_1));
    RobotConnectionListener::run();
}

RobotManager::~RobotManager() {

}

void RobotManager::onNewRobotConnection(websocket::stream<tcp::socket> robot_websocket) {

}
