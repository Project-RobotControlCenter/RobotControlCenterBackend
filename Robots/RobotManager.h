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
#include <boost/json.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = asio::ip::tcp;

class Robot;

class RobotManager {
public:
    RobotManager(const RobotManager&) = delete;
    RobotManager& operator=(const RobotManager&) = delete;

    static bool initInstance(asio::io_context &ioc, unsigned short _robot_connection_port) {
        if(!_instance) {
            _instance = std::unique_ptr<RobotManager>(new RobotManager(ioc, _robot_connection_port));
        }
        return _instance != nullptr;
    }

    static RobotManager& getInstance() {
        return *_instance;
    }

private:
    static std::unique_ptr<RobotManager> _instance;

    RobotManager(asio::io_context &ioc, unsigned short _robot_connection_port);
    ~RobotManager();

    void RobotManager::onNewRobotConnection(websocket::stream<tcp::socket> robot_websocket);

    std::unordered_map<std::string, Robot> _robots;
};



#endif //ROBOTMANAGER_H
