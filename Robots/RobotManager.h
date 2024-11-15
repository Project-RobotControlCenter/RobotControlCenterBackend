//
// Created by elejandc on 11/9/24.
//

#ifndef ROBOTMANAGER_H
#define ROBOTMANAGER_H
#include <memory>
#include <string>
#include <unordered_map>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <boost/asio/steady_timer.hpp>
#include <utility>

#include "../Message_processing/MessageStructs.h"
#include "Robot.h"

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = asio::ip::tcp;

class RobotManager {
public:
    RobotManager(const RobotManager&) = delete;
    RobotManager& operator=(const RobotManager&) = delete;
    ~RobotManager();

    static bool initInstance(asio::io_context &ioc, unsigned short _robot_connection_port) {
        if(!_instance) {
            _instance = std::unique_ptr<RobotManager>(new RobotManager(ioc, _robot_connection_port));
        }
        return _instance != nullptr;
    }

    static RobotManager& getInstance() {
        return *_instance;
    }

    static std::shared_ptr<Robot> getRobot(const std::string &mac_address) {return getInstance().getRobotImp(mac_address);}

    static std::vector<st_robotInfo> getAllRobotsData() {return getInstance().getAllRobotsDataImp();}

private:
    static std::unique_ptr<RobotManager> _instance;
    asio::io_context &_ioc;

    RobotManager(asio::io_context &ioc, unsigned short _robot_connection_port);

    void onNewRobotConnection(websocket::stream<tcp::socket> robot_websocket);

    std::unordered_map<std::string, std::shared_ptr<Robot>> _robots;

    std::shared_ptr<Robot> getRobotImp(const std::string &mac_address);

    std::vector<st_robotInfo> getAllRobotsDataImp();
};



#endif //ROBOTMANAGER_H
