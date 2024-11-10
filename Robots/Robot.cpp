//
// Created by elejandc on 11/9/24.
//

#include "Robot.h"

#include <utility>

Robot::Robot(std::string db_id, std::string name, std::string mac_address, std::string ip, std::string port, bool isAccepted)
    : _db_id(std::move(db_id)), _name(std::move(name)), _mac_address(std::move(mac_address)), _ip(std::move(ip)), _port(std::move(port)), _isAccepted(isAccepted)
{

}

Robot::~Robot() {
}