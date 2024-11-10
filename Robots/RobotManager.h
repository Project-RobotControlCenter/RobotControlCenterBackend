//
// Created by elejandc on 11/9/24.
//

#ifndef ROBOTMANAGER_H
#define ROBOTMANAGER_H
#include <memory>
#include <string>
#include <unordered_map>

class Robot;

class RobotManager {
public:
    RobotManager(const RobotManager&) = delete;
    RobotManager& operator=(const RobotManager&) = delete;

    static bool initInstance(unsigned short _robot_connection_port) {
        if(!_instance) {
            _instance = std::unique_ptr<RobotManager>(new RobotManager(_robot_connection_port));
        }
        return _instance != nullptr;
    }

    static RobotManager& getInstance() {
        static RobotManager _instance;
        return _instance;
    }

private:
    static std::unique_ptr<RobotManager> _instance;

    RobotManager(unsigned short _robot_connection_port);
    ~RobotManager();

    std::unordered_map<std::string, Robot> _robots;
};



#endif //ROBOTMANAGER_H
