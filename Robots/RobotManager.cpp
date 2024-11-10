//
// Created by elejandc on 11/9/24.
//

#include "RobotManager.h"

#include "RobotConnectionListener.h"

std::unique_ptr<RobotManager> RobotManager::_instance = nullptr;

RobotManager::RobotManager(unsigned short _robot_connection_port) {

}

RobotManager::~RobotManager() {

}
