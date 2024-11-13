//
// Created by elejandc on 11/9/24.
//

#include "Robot.h"

#include <utility>

Robot::Robot(websocket::stream<tcp::socket> &robot_websocket,std::string db_id, std::string name, std::string mac_address, std::string ip, unsigned char port, bool isAccepted)
    : _robot_websocket(std::move(robot_websocket)), _db_id(std::move(db_id)), _name(std::move(name)), _mac_address(std::move(mac_address)), _ip(std::move(ip)), _port(port), _isAccepted(isAccepted)
{

}

Robot::~Robot() {
}
