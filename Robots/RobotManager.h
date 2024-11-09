//
// Created by elejandc on 11/9/24.
//

#ifndef ROBOTMANAGER_H
#define ROBOTMANAGER_H
#include <string>
#include <unordered_map>

class Robot;

class RobotManager {
public:
    RobotManager(const RobotManager&) = delete;
    RobotManager& operator=(const RobotManager&) = delete;

    static RobotManager& getInstance() {
        static RobotManager _instance;
        return _instance;
    }
private:
    RobotManager();
    ~RobotManager();

    std::unordered_map<std::string, Robot> _robots;
};



#endif //ROBOTMANAGER_H
